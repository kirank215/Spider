#include "socket/default-socket.hh"

#include "misc/socket.hh"

namespace http
{
    DefaultSocket::DefaultSocket(int domain, int type, int protocol)
        : Socket{std::make_shared<misc::FileDescriptor>(
              sys::socket(domain, type, protocol))}
    {}

    void DefaultSocket::listen(int backlog)
    {
        sys::listen(*fd_, backlog);
    }
} // namespace http
