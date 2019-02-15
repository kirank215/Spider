#include "watcher.hh"

#include "ev.h"
#include <iostream>
#include <netinet/in.h>

#include "events/register.hh"
#include "socket/socket.hh"

using namespace http;

EventRequest::EventRequest(shared_socket sock)
{
    EventWatcher::EventWatcher(s->fd_get()->fd_, EV_READ);
    s = sock;
}

EventResponse::EventResponse(shared_socket sock, Request request)
{
    EventWatcher::EventWatcher(s->fd_get(), EV_WRITE);
    s = sock;
    Response r(request, request.status);
    res = r;
}

EventRequest::operator()()
{
    req = Request(s);
    //add to the register of sockets

    event_register.register_ew<EventResponse>(s, req);
}

void  build_statusline(std::string& s, Response& r)
{
    s += r.version_; // version with SP
    auto status = statusCode(status);   // pair of CODE, string
    s += status.first;
    s += " ";
    s += status.second;
    s += "\r\n";
}


void add_headers(std::string& resp_str, std::map<std::string,std::string>& headers)
{
    for(auto &x : headers)
    {
        resp_str += x.first;
        resp_str += ": ";
        resp_str += x.second;
    }
}

EventResponse::operator()()
{
    std::string resp_string;
    build_statusline(resp_str, res);
    add_headers(resp_str, headers);
    ssize_t s = send(s->fd_ge()->fd_, resp_string, 100, 0);

}
