#pragma once
#include "events.hh"

using namespace http;

class EventRequest
{
    void operator()() override;
    private :
    request req;
};

class EventResponse
{
    void operator()() override;
    private :
    response res;
};
