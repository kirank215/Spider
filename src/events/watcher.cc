#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include "events/register.hh"
#include "socket/socket.hh"
#include "events/watcher.hh"
#include "vhost/dispatcher.hh"
#include "ev.h"
#include "vhost/apm.hh"

namespace http
{

    EventRequest::EventRequest(shared_socket sock)
    :EventWatcher{sock->fd_get()->fd_, EV_READ}, sock_{sock}
    {}

    EventResponse::EventResponse(shared_socket sock, const Response& r)
    :EventWatcher(sock->fd_get()->fd_, EV_WRITE), sock_(sock), res_(r)
    {
    }

    const Request& EventRequest::get_request() const
    {
        return req_;
    }

    const shared_socket& EventRequest::get_sock() const
    {
        return sock_;
    }

    void update_APM(VHostConfig& vc, STATUS_CODE& st)
    {

        if(st == 200)
        {
            apm.APM_add(apm.APM_, "global_requests_2xx");
            apm.APM_add(vc.APM_local, "requests_2xx");
        }
        else if(st >= 400 && st < 500)
        {
            apm.APM_add(apm.APM_, "global_requests_4xx");
            apm.APM_add(vc.APM_local, "requests_4xx");
        }
        else if(st >= 500 && st < SHOULD_NOT_HAPPEN)
        {
            apm.APM_add(apm.APM_, "global_requests_5xx");
            apm.APM_add(vc.APM_local, "requests_5xx");
        }
        apm.APM_add(apm.APM_, "global_requests_nb");
        apm.APM_add(vc.APM_local, "requests_nb");

    }

    void EventRequest::operator()()
    {
        apm.APM_add(apm.APM_, "global_connections_active");
        req_ = Request(sock_);
        //add to the register of sockets
        auto c = dispatcher.create_connection(*this);
        if(!c)
            throw "Unable to form connection.";

        dispatcher.respond(req_, *c);

        event_register.unregister_ew((EventWatcher *) this);
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
        resp_str += "\r\n";
    }

    void EventResponse::operator()()
    {
        std::string resp_str;
        build_statusline(resp_str, res_);
        add_headers(resp_str, res_.headers_);
        resp_str += res_.msg_body_;
        resp_str += "\r\n";
        sock_->send(resp_str.c_str(), resp_str.length());

// reusing connections
        auto it = res_.headers_.find("Connection");
        if (it != res_.headers_.end())
        {
            if(it->second == "alive" && res_.status_ == OK)
            {
                apm.APM_add(apm.APM_, "global_connections_active");
                event_register.register_ew<EventRequest>(sock_);
            }
        }
        apm.APM_sub(apm.APM_, "global_connections_active");
        update_APM(res_.vc, res_.status_);
        event_register.unregister_ew((EventWatcher *) this);
    }
}
