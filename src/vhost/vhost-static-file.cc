#include <fstream>
#include <filesystem>
#include "request/response.hh"
#include "events/register.hh"

#include "events/watcher.hh"
#include "vhost/vhost-static-file.hh"
#include "vhost/connection.hh"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wswitch-enum"
#pragma GCC diagnostic ignored "-Wswitch-default"
#pragma GCC diagnostic ignored "-Winline"
#ifdef __clang__
#pragma GCC diagnostic ignored "-Wlogical-op-parentheses"
#endif
#include <json.hpp>
#include "apm.hh"


namespace fs = std::filesystem;

namespace http
{

    void build_html(std::string& output_string, std::string root)
    {
        output_string = "<!DOCTYPE html>\n<html>\n<head>";
        output_string += "\n<metacharset=utf-8>\n";
        output_string += "<title>Index of" + root + "</title>" + "\n"
                            + "</head>" + "\n" + "<body>" + "\n";
        output_string += "<ul>\n";

        for (const auto & dir : fs::directory_iterator(root))
        {
            std::string path = dir.path();
            output_string += "<li><a href=\"";
            output_string += path.substr(2);
            output_string += "\">";
            output_string += path.substr(2);
            output_string += "</a></li>\n";
        }

        output_string += "</ul>\n</body>\n</html>";
    }

    VHostStaticFile::VHostStaticFile(const VHostConfig& v)
    :VHost{v}
    {}

    void VHostStaticFile::respond(const Request& req, Connection c,
            remaining_iterator, remaining_iterator)
    {
        STATUS_CODE st = SHOULD_NOT_HAPPEN;
        std::string path = c.vc_.root + req.request_uri_ ;
        std::string out;
        std::error_code e;
        std::string content_type = "text/plain";
        if(req.request_uri_ == c.vc_.health_endpoint)   // get on healthpoint returns the metrics
        {
            apm.APM_.insert(c.vc_.APM_local.begin(), c.vc_.APM_local.end());
            nlohmann::json j_map(apm.APM_);
            out = j_map.dump(4);
            out += "\n";
            st = OK;
        }
        else if(fs::is_directory(req.request_uri_, e) && c.vc_.auto_index
                                        && c.vc_.default_file == "index.txt")
        {
            build_html(out, path);
            st = e ? OK : NOT_FOUND;
            content_type = "text/html";
        }
        else
        {
            path += req.request_uri_ == "/" ? c.vc_.default_file : "";
            std::string line;
            std::ifstream f(path);
            if(!f.is_open())
                st = NOT_FOUND;
            else if(req.m_ == GET || req.m_ == POST)
            {
                while(std::getline(f, line))
                    out += line + "\n";
                st = OK;
                // POST same as GET for now. Change for later stages
            }
            else if(req.m_ == HEAD)
            {
                st = OK;
            }
        }
        Response resp(req, c.vc_,  st, out);
        resp.content_type = content_type;
        auto resp_sock = c.s_;
        event_register.register_ew<EventResponse>(resp_sock, resp);
    }

}
