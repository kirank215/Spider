#include "listener.hh"

#include <ev.h>
#include <iostream>
#include <netinet/in.h>

#include "events/register.hh"
#include "events/watcher.hh"
#include "misc/socket.hh"
#include "socket/socket.hh"

http::ListenerEW::ListenerEW(shared_socket socket)
    : EventWatcher(socket->fd_get()->fd_, EV_READ)
{
    sock_ = socket;
    struct sockaddr* addr = new sockaddr();     // FIXME leak
    socklen_t len = sizeof(addr);
    if (getsockname(socket->fd_get()->fd_, addr, &len) == -1)
        throw std::logic_error("Listener constructor: trying to intialize with bad socket");
    delete addr;
}

void http::ListenerEW::operator()()
{
    sockaddr addr;
    socklen_t len = sizeof(addr);
    try
    {
        auto new_socket = sock_->accept(&addr, &len);
        http::event_register.register_ew<EventRequest>(new_socket);
    }
    catch(std::exception& e)
    {
        std::cerr << e.what();
        return;
    }
}

