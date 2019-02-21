#include <fstream>
#include "request/response.hh"

#include "events/watcher.hh"
#include "vhost/vhost-static-file.hh"
#include "vhost/connection.hh"

http::VHostStaticFile::VHostStaticFile(const VHostConfig& v):VHost(v)
{}

void http::VHostStaticFile::respond(const Request& req, Connection c,
        remaining_iterator rm1, remaining_iterator rm2)
{
    STATUS_CODE st;
    std::string path = req.request_uri_ + c.vc_.root;
    std::string out;
    std::string line;
    std::ifstream f(path);
    if(!f.is_open())
        st = NOT_FOUND;
    else
    {
        while(std::getline(f, line))
            out += line;
        st = OK;
    }
    Response resp(req, st, out);
    EventResponse er(c.s_, resp);
}
