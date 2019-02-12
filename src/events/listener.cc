#include "listener.hh"

#include "misc/socket.hh"

http::ListenerEW::ListenerEW(shared_socket socket)
    : sock_(socket)
{
    struct sockaddr_in* addr;
    if (getsockname(socket.fd_get(), addr, sizeof(addr)) == -1)
        std::cerr
            << "Listener constructor: trying to intialize with bad socket";
    port_ = addr.sin_port;
}

void http::ListenerEW::operator()()
{
    struct sockaddr_in addr*;
    auto new_socket = accept(addr,sizeof(addr);

    event_register.register_ew();
    // Add to loop
}
