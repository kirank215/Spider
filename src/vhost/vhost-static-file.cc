#include <fstream>
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


namespace http
{
    VHostStaticFile::VHostStaticFile(const VHostConfig& v)
    :VHost{v}
    {}

    void VHostStaticFile::respond(const Request& req, Connection c,
            remaining_iterator, remaining_iterator)
    {
        STATUS_CODE st = SHOULD_NOT_HAPPEN;
        std::string file = req.request_uri_;
        file += file == "/" ? c.vc_.default_file : "";
        std::string path = c.vc_.root + file ;
        std::string out;
        std::string line;
        std::ifstream f(path);
        if(!f.is_open())
            st = NOT_FOUND;
        if(req.request_uri_ == c.vc_.health_endpoint)   // get on healthpoint returns the metrics
        {
            apm.APM_.insert(c.vc_.APM_local.begin(), c.vc_.APM_local.end());
            nlohmann::json j_map(apm.APM_);
            out = j_map.dump(4);
            out += "\n";
            st = OK;
            // POST same as GET for now. Change for later stages
        }
        else
        {
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
        auto resp_sock = c.s_;
        event_register.register_ew<EventResponse>(resp_sock, resp);
    }

}
