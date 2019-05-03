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
        std::string path = c.vc_.root + req.request_uri_ ;
        std::string out;
        std::string line;
        std::ifstream f(path);
        if(!f.is_open())
            st = NOT_FOUND;
        if(req.request_uri_ == c.vc_.health_endpoint)   // get on healthpoint returns the metrics
        {
            nlohmann::json j_map(http::APM);
            out = j_map.dump();
            out += "\n";
            nlohmann::json apm_local = c.vc_.APM_local;
            out += apm_local.dump();
            st = OK;
            // POST same as GET for now. Change for later stages
        }
        else if(req.m_ == GET || req.m_ == POST)
        {
            while(std::getline(f, line))
                out += line;
        }
        else
        {
            st = OK;
        }
        Response resp(req, st, out);
        auto resp_sock = c.s_;
        event_register.register_ew<EventResponse>(resp_sock, resp);
    }
}
