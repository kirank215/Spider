#pragma once
#include "events.hh"
#include "request/request.hh"
#include "request/response.hh"
#include "socket/socket.hh"
#include "config/config.hh"

namespace http
{
class EventRequest : public EventWatcher
    {
        public:
            explicit EventRequest(shared_socket s);
            ~EventRequest() = default;
            void operator()() final;
            const Request& get_request() const;
            const shared_socket& get_sock() const;
        private :
            shared_socket sock_;
            Request req_;
    };

    class EventResponse : public EventWatcher
    {
        public:
            explicit EventResponse(shared_socket s, const Response& r);
            ~EventResponse() = default;
            void operator()() final;
        private :
            shared_socket sock_;
            Response res_;
    };
    void update_APM(VHostConfig& vc, STATUS_CODE& st);
}
