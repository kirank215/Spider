#include "connection.hh"
#include "vhost/vhost.hh"

http::Connection::Connection(VHostConfig& v, shared_socket s, uint16_t p)
{
    vc = v;
    sock = s;
    port = p;
}
