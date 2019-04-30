
#include "socket/ssl-socket.hh"
#include "misc/openssl/ssl.hh"



namespace http
{
    SSLSocket::SSLSocket(int domain, int type, int protocol, SSL_CTX* ssl_ctx)
        : Socket(std::make_shared<misc::FileDescriptor>(
                    sys::socket(domain, type, protocol))),
                    ssl_(SSL_new(ssl_ctx), SSL_free)
    {}

    SSLSocket::SSLSocket(const misc::shared_fd& fd, SSL_CTX* ssl_ctx)
        : Socket(fd), ssl_(SSL_new(ssl_ctx), SSL_free)
    {
        SSL_set_fd(ssl_.get(), *(this->fd_));
        int accept = SSL_accept(ssl_.get());
        if(accept < 0)
        {
            ERR_print_errors_fp(stderr);
            int ret = SSL_get_error(ssl_.get(), accept);
            std::cout << ret;
        }
    }

    ssize_t SSLSocket::recv(void* dst, size_t len)
    {
        return ssl::read(ssl_.get(), dst, len);
    }

    ssize_t SSLSocket::send(const void* src, size_t len)
    {
        return ssl::write(ssl_.get(), src, len);
    }

    ssize_t SSLSocket::sendfile(misc::shared_fd& dst, off_t& off, size_t count)
    {
        return sys::sendfile(*fd_, *dst, &off, count);
    }

    void SSLSocket::bind(const sockaddr* addr, socklen_t addrlen)
    {
        sys::bind(*fd_, addr, addrlen);
    }

    void SSLSocket::listen(int backlog)
    {
        sys::listen(*fd_, backlog);
    }
    void SSLSocket::setsockopt(int level, int optname, int optval)
    {
        socklen_t len = sizeof(optval);
        sys::setsockopt(*fd_, level, optname,(char*)&optval, len);
    }
    std::shared_ptr<Socket> SSLSocket::accept(sockaddr* addr,
            socklen_t* addrlen)
    {
        //SSL_CTX* ctx = SSL_CTX_new(SSLv23_client_method());
        auto resfd = sys::accept(*fd_, addr, addrlen);
        auto sharedfd = std::make_shared<misc::FileDescriptor>(std::move(resfd));
        auto result = std::make_shared<SSLSocket>(sharedfd,
                                        SSL_get_SSL_CTX(ssl_.get()));
        return result;
    }

    void SSLSocket::connect(const sockaddr* addr, socklen_t addrlen)
    {
        // possibly check validity of cerificates before or at some point
        sys::connect(*fd_, addr, addrlen);
    }

    int SSLSocket::ssl_set_fd(int fd)
    {
        return SSL_set_fd(ssl_.get(), fd);
    }


}


