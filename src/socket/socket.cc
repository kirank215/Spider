#include "socket/socket.hh"

namespace http
{
    Socket::Socket(const misc::shared_fd& fd)
        : fd_{fd}
    {}

    ssize_t Socket::recv(void* dst, size_t len)
    {
         return sys::recv(fd_, dst, len, 0);
    }
    ssize_t Socket::send(const void* src, size_t len)
    {
        return sys::send(fd_, src, len, 0);
    }
    ssize_t Socket::sendfile(misc::shared_fd& dst, off_t& off, size_t count)
    {
        return sys::sendfile(fd_, dst, off, count);
    }
    void Socket::bind(const sockaddr* addr, socklen_t addrlen)
    {
        sys::bind(fd_, addr, addrlen);
    }
    void Socket::listen(int backlog)
    {
        sys::listen(fd_, backlog);
    }
    void Socket::setsockopt(int level, int optname, int optval)
    {
        sys::setsockopt(fd_, level, optname, optval, sizeof(optval));
    }
    std::shared_ptr<Socket> Socket::accept(sockaddr* addr,
            socklen_t* addrlen)
    {
        const auto res = sys::accept(fd_, addr, addrlen); //FIXME
        make_shared<misc::FileDescriptor>(res);
        return Socket(res);
    }
    void Socket::connect(const sockaddr*, socklen_t addrlen)
    {
        sys::connect(fd_, sockaddr, addrlen);
    }
} // namespace http
