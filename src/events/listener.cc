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
        std::cerr
            << "Listener constructor: trying to intialize with bad socket";
    struct sockaddr_in* addr_in = reinterpret_cast<sockaddr_in*>(addr);
    std::cout << addr_in->sin_port;
}

void http::ListenerEW::operator()()
{
    sockaddr addr;
    socklen_t len = sizeof(addr);
    auto new_socket = sock_->accept(&addr, &len);
    http::event_register.register_ew<EventRequest>(new_socket);
}
