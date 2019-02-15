#include "socket/socket.hh"

namespace http
{
    Socket::Socket(const misc::shared_fd& fd)
        : fd_{fd}
    {}

    ssize_t Socket::recv(void* dst, size_t len)
    {
        return recv(fd_, dest, len, 0);
    }
    ssize_t send(const void* src, size_t len)
    {
        return send(fd_, src, len, 0);
    }
    ssize_t sendfile(misc::shared_fd& dst, off_t& off, size_t count)
    {
        return sendfile(fd_, dst.fd_get()->fd_, off, count);
    }
    void bind(const sockaddr* addr, socklen_t addrlen)
    {
        bind(fd_, addr, addrlen);
    }
    void listen(int backlog)
    {
        listen(fd_, backlog)
    }
    void setsockopt(int level, int optname, int optval)
    {
        setsockopt(fd_, level, optname, optval, sizeof(optval));
    }
    std::shared_ptr<Socket> accept(sockaddr* addr,
            socklen_t* addrlen)
    {
        return accept(fd_, addr, addrlen);
    }
    void connect(const sockaddr*, socklen_t addrlen)
    {
        connect(fd_, sockaddr, addrlen);
    }
} // namespace http
