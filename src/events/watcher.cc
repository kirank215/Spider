#include "watcher.hh"

#include "ev.h"
#include <iostream>
#include <netinet/in.h>

#include "events/register.hh"
#include "misc/socket.hh"

using namespace http;

EventRequest::EventRequest(shared_socket sock)
{
    EventWatcher::EventWatcher(s->fd_get(), EV_READ);
    s = sock;
    req = new request;
}


EventResponse::EventResponse(shared_socket sock, request req)
{
    EventWatcher::EventWatcher(s->fd_get(), EV_WRITE);
    s = sock;
    res = new response;
}

EventRequest::operator()()
{
    ssize_t c;
    while(c != -1)
    {

        c = read(fd);
    }
}

EventResponse::operator()()
{

}
