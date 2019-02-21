#pragma once
#include "events.hh"
#include "request/request.hh"
#include "request/response.hh"
#include "socket/socket.hh"

namespace http
{

    class EventRequest : public EventWatcher
    {
        public:
            explicit EventRequest(shared_socket s);
            ~EventRequest() = default;
            void operator()() final;
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

}
