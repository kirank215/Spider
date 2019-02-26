#include <fstream>
#include "request/response.hh"
#include "events/register.hh"

#include "events/watcher.hh"
#include "vhost/vhost-static-file.hh"
#include "vhost/connection.hh"

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
        else if(req.m_ == GET)
        {
            while(std::getline(f, line))
                out += line;
            st = OK;
        }
        else if(req.m_ == POST)
        {
            st = OK;
            // For later stages
        }

        else if(req.m_ == HEAD)
        {
            st = OK;
        }
        Response resp(req, st, out);
        auto resp_sock = c.s_;
        event_register.register_ew<EventResponse>(resp_sock, resp);
    }
}
