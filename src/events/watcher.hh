#pragma once
#include "events.hh"

using namespace http;

class EventRequest : public EventWatcher
{
    EventRequest(shared_socket s);
    ~EventRequest();
    void operator()() override;
    private :
    shared_socket s;
    request req;
};

class EventResponse : public EventWatcher
{
    EventResponse(shared_socket s, request req);
    ~EventResponse();
    void operator()() override;
    private :
    shared_socket s;
    response res;
};
