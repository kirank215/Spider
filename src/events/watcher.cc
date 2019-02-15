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

EventResponse::EventResponse(shared_socket sock, Request req)
{
    EventWatcher::EventWatcher(s->fd_get(), EV_WRITE);
    s = sock;
}

EventRequest::operator()()
{
    req = Request(s);
    EventResponse er = EventResponse(s, req);
    //add to the register of sockets

    event_register.register_ew(s);
}
