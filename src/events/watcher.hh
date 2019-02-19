#pragma once
#include "events.hh"
#include "request/request.hh"
#include "request/response.hh"
#include "socket/socket.hh"

using namespace http;

class EventRequest : public EventWatcher
{
    public:
    EventRequest(shared_socket s);
    ~EventRequest();
    void operator()() override;
    private :
    shared_socket s;
    Request req;
};

class EventResponse : public EventWatcher
{
    public:
    EventResponse(shared_socket s, Request req);
    ~EventResponse();
    void operator()() override;
    private :
    shared_socket sock_;
    Response res_;
};
