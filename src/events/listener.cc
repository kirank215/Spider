#include "listener.hh"

#include <ev.h>
#include <iostream>

#include "events/register.hh"
#include "misc/socket.hh"
#include "socket/socket.hh"

http::ListenerEW::ListenerEW(shared_socket socket)
    : EventWatcher(socket->fd_get(), EV_READ)
{
    sock_(socket) struct sockaddr_in* addr;
    socklen_t len = sizeof(addr);
    // TO FIX : Remove cast
    if (getsockname(socket->fd_get()->fd_, (struct sockaddr*)addr, &len) == -1)
        std::cerr
            << "Listener constructor: trying to intialize with bad socket";
    port_ = addr->sin_port;
}

void http::ListenerEW::operator()()
{
    struct sockaddr* addr;
    socklen_t len = sizeof(addr);
    auto new_socket = sock_->accept(addr, &len);

    http::event_register.register_ew(new_socket);
    // Add to loop
}
