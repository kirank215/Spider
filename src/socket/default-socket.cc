#include "socket/default-socket.hh"

#include "misc/socket.hh"

namespace http
{
    DefaultSocket::DefaultSocket(int domain, int type, int protocol)
        : Socket{std::make_shared<misc::FileDescriptor>(
              sys::socket(domain, type, protocol))}
    {}

    DefaultSocket::DefaultSocket(const misc::shared_fd& fd)
        : Socket{fd}
    {}

    ssize_t DefaultSocket::recv(void* dst, size_t len)
    {
        return sys::recv(*fd_, dst, len, 0);
    }
    ssize_t DefaultSocket::send(const void* src, size_t len)
    {
        return sys::send(*fd_, src, len, 0);
    }
    ssize_t DefaultSocket::sendfile(misc::shared_fd& dst, off_t& off, size_t count)
    {
        return sys::sendfile(*fd_, *dst, &off, count);
    }
    void DefaultSocket::bind(const sockaddr* addr, socklen_t addrlen)
    {
        sys::bind(*fd_, addr, addrlen);
    }
    void DefaultSocket::listen(int backlog)
    {
        sys::listen(*fd_, backlog);
    }
    void DefaultSocket::setsockopt(int level, int optname, int optval)
    {
        socklen_t len = sizeof(optval);
        sys::setsockopt(*fd_, level, optname, (char*)&optval, len);
    }
    std::shared_ptr<Socket> DefaultSocket::accept(sockaddr* addr,
            socklen_t* addrlen)
    {
        auto res = sys::accept(*fd_, addr, addrlen);
        auto res1 = std::make_shared<misc::FileDescriptor>(std::move(res));
        auto result = std::make_shared<DefaultSocket>(res1);
        return result;
    }
    void DefaultSocket::connect(const sockaddr* addr, socklen_t addrlen)
    {
        sys::connect(*fd_, addr, addrlen);
    }


} // namespace http
