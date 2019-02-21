#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include "events/register.hh"
#include "socket/socket.hh"
#include "events/watcher.hh"
#include "ev.h"

namespace http
{

    EventRequest::EventRequest(shared_socket sock)
    :EventWatcher{sock->fd_get()->fd_, EV_READ}, sock_{sock}
    {}

    EventResponse::EventResponse(shared_socket sock, const Response& r)
    :EventWatcher(sock->fd_get()->fd_, EV_WRITE), sock_(sock), res_(r)
    {
    }

    void EventRequest::operator()()
    {
        req_ = Request(sock_);
        //add to the register of sockets

        event_register.register_ew<EventResponse>(sock_, req_);
        // call response
    }

    static void  build_statusline(std::string& s, Response& r)
    {
        s += "HTTP/1.1 "; // version with SP
        auto status = statusCode(r.status_);   // pair of CODE, string
        s += status.first;
        s += " ";
        s += status.second;
        s += "\r\n";
    }


   static void add_headers(std::string& resp_str, std::map<std::string,std::string>& headers)
    {
        for(auto &x : headers)
        {
            resp_str += x.first;
            resp_str += ": ";
            resp_str += x.second;
        }
    }

    void EventResponse::operator()()
    {
        std::string resp_str;
        build_statusline(resp_str, res_);
        add_headers(resp_str, res_.headers_);
        resp_str += res_.msg_body_;
        send(sock_->fd_get()->fd_, resp_str.c_str(), resp_str.length(), 0);

    }
}
