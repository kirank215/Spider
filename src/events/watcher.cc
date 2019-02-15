#include "watcher.hh"

#include "ev.h"
#include <iostream>
#include <netinet/in.h>

#include "events/register.hh"
#include "misc/socket.hh"

using namespace http;

EventRequest::EventRequest(shared_socket sock)
{
    EventWatcher::EventWatcher(s->fd_get()->fd_, EV_READ);
    s = sock;
}

EventResponse::EventResponse(shared_socket sock, request req)
{
    EventWatcher::EventWatcher(s->fd_get(), EV_WRITE);
    s = sock;
    res = new response;
}

EventRequest::operator()()
{
    req = request(s);
    EventResponse er = EventResponse(s, req);
    //add to the register of sockets

    event_register.register_ew(s);
}
