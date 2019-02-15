#include "socket/socket.hh"
#include <sys/socket.h>

namespace http
{
    Socket::Socket(const misc::shared_fd& fd)
        : fd_{fd}
    {}

    ssize_t Socket::recv(void* dst, size_t len)
    {
        return recv(fd_, dst, len, 0);
    }
    ssize_t Socket::send(const void* src, size_t len)
    {
        return send(fd_, src, len, 0);
    }
    ssize_t Socket::sendfile(misc::shared_fd& dst, off_t& off, size_t count)
    {
        return sendfile(fd_, dst, off, count);
    }
    void Socket::bind(const sockaddr* addr, socklen_t addrlen)
    {
        bind(fd_, addr, addrlen);
    }
    void Socket::listen(int backlog)
    {
        listen(fd_, backlog)
    }
    void Socket::setsockopt(int level, int optname, int optval)
    {
        setsockopt(fd_, level, optname, optval, sizeof(optval));
    }
    std::shared_ptr<Socket> Socket::accept(sockaddr* addr,
            socklen_t* addrlen)
    {
        return accept(fd_, addr, addrlen);
    }
    void Socket::connect(const sockaddr*, socklen_t addrlen)
    {
        connect(fd_, sockaddr, addrlen);
    }
} // namespace http
