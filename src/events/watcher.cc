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
    req = request();
    EventResponse er = EventResponse(s, req);
    //add to the list register thing
    sockaddr addr;
    r = bind(s->fd_get()->fd_, addr);
    socklen_t len = sizeof(addr);
    auto new_socket = sock_->accept(&addr, &len);
    event_register.register_ew(s);
    event_register.register_ew(new_socket);
}
