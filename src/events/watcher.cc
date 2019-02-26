#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include "events/register.hh"
#include "socket/socket.hh"
#include "events/watcher.hh"
#include "vhost/dispatcher.hh"
#include "ev.h"

namespace http
{

    EventRequest::EventRequest(shared_socket sock)
    :EventWatcher{sock->fd_get()->fd_, EV_READ}, sock_{sock}
    {}

    EventResponse::EventResponse(shared_socket sock, const Response& r)
    :EventWatcher(sock->fd_get()->fd_, EV_WRITE), sock_(sock), res_(r)
    {}

    const Request& EventRequest::get_request() const
    {
        return req_;
    }

    const shared_socket& EventRequest::get_sock() const
    {
        return sock_;
    }
    void EventRequest::operator()()
    {
        req_ = Request(sock_);
        //add to the register of sockets
        auto c = dispatcher.create_connection(*this);
        if(!c)
            throw std::logic_error("Unable to form connection");
        dispatcher.respond(req_, *c);

        // FIXME use connection to send response
        event_register.unregister_ew((EventWatcher *) this);
 //       event_register.register_ew<EventResponse>(sock_, req_);
        // call response
    }

    static void  build_statusline(std::string& s, Response& r)
    {
        s += "HTTP/1.1 "; // version with SP
        auto status = statusCode(r.status_);   // pair of CODE, string
        s += std::to_string(status.first);
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
            resp_str += "\r\n";
        }
    }

    void EventResponse::operator()()
    {
        std::string resp_str;
        build_statusline(resp_str, res_);
        add_headers(resp_str, res_.headers_);
        resp_str += res_.msg_body_;
        sock_->send(resp_str.c_str(), resp_str.length());
        event_register.unregister_ew((EventWatcher *) this);
//        event_register.register_ew<EventRequest>(sock_);

    }
}
