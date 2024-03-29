/**
 * \file socket/ssl-socket.hh
 * \brief SSLSocket declaration.
 */

#pragma once

#include <functional>
#include <memory>
#include <openssl/err.h>
#include <openssl/ssl.h>

#include "misc/socket.hh"
#include "socket/socket.hh"
#include "config/config.hh"
#include "misc/addrinfo/addrinfo.hh"

namespace http
{
    /**
     * \struct SSLSocket
     * \brief Implementation of SSL sockets
     */
    struct SSLSocket : public Socket
    {
        SSLSocket(int domain, int type, int protocol, SSL_CTX* ssl_ctx);

        SSLSocket(const misc::shared_fd& fd, SSL_CTX* ssl_ctx);

        ssize_t recv(void* dst, size_t len) final;

        ssize_t send(const void* src, size_t len) final;

        ssize_t sendfile(misc::shared_fd& src, off_t& offset,
                         size_t count) final;

        void bind(const sockaddr* addr, socklen_t addrlen) final;

        void listen(int backlog) final;

        void setsockopt(int level, int optname, int optval) final;

        shared_socket accept(sockaddr* addr, socklen_t* addrlen) final;
        void connect(const sockaddr*, socklen_t) final;

        void set_host_name(const std::string&);


        int ssl_set_fd(int fd);


    private:
        /**
         * \brief Socket's SSL connection.
         *
         * From ssl(3):
         *
         * SSL (SSL Connection)
         *   This is the main SSL/TLS structure which is created by a server or
         *   client per established connection. This actually is the core
         *   structure in the SSL API. At run-time the application usually deals
         *   with this structure which has links to mostly all other structures.
         *
         * Warning: with this unique_ptr syntax, you'll need to instanciate the
         * pointer with both a value and a Deleter function.
         */
        std::unique_ptr<SSL, decltype(SSL_free)*> ssl_;
    };
    int ServerNameCallback(SSL *ssl, int *ad, void *arg);

} // namespace http
    std::shared_ptr<http::Socket>  create_SSLSocket(
                const misc::AddrInfo&, const http::VHostConfig&);
