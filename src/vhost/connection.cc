#include "connection.hh"

http::Connection::Connection(VHostConfig& vh, shared_socket sock, uint16_t port)
{
    vc_ = vh;
    s_ = sock;
    p_ = port;
}
