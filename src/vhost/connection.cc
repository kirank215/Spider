#include "connection.hh"
#include "vhost/vhost.hh"

namespace http
{
    Connection(VhostConfig& v, shared_socket s, port p)
    {
        vhost = v;
        sock = s;
        port = p;
    }
}
