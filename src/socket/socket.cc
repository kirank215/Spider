#include "socket/socket.hh"
#include "default-socket.hh"

namespace http
{
    Socket::Socket(const misc::shared_fd& fd)
        : fd_{fd}
    {}

    ssize_t Socket::recv(void* dst, size_t len)
    {
         return sys::recv(*fd_, dst, len, 0);
    }
    ssize_t Socket::send(const void* src, size_t len)
    {
        return sys::send(*fd_, src, len, 0);
    }
    ssize_t Socket::sendfile(misc::shared_fd& dst, off_t& off, size_t count)
    {
        return sys::sendfile(*fd_, *dst, &off, count);
    }
    void Socket::bind(const sockaddr* addr, socklen_t addrlen)
    {
        sys::bind(*fd_, addr, addrlen);
    }
    void Socket::listen(int backlog)
    {
        sys::listen(*fd_, backlog);
    }
    void Socket::setsockopt(int level, int optname, int optval)
    {
        socklen_t len = sizeof(optval);
        sys::setsockopt(*fd_, level, optname,(char*)&optval, len);
    }
    std::shared_ptr<Socket> Socket::accept(sockaddr* addr,
            socklen_t* addrlen)
    {
        auto res = sys::accept(*fd_, addr, addrlen);
        auto res1 = std::make_shared<misc::FileDescriptor>(std::move(res));
        auto res2 = DefaultSocket(res1);
        auto result = std::make_shared<DefaultSocket>(res2);
        return result;
    }
    void Socket::connect(const sockaddr* addr, socklen_t addrlen)
    {
        sys::connect(*fd_, addr, addrlen);
    }
} // namespace http
