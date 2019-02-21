/**
 * \file vhost/connection.hh
 * \brief Connection declaration.
 */

#pragma once

#include <memory>
#include "vhost/vhost.hh"
#include "socket/socket.hh"
#include "config/config.hh"

namespace http
{
    /**
     * \struct Connection
     * \brief Value object representing a connection.
     *
     * We need to keep track of the state of each request while it has not
     * been fully processed.
     */
    struct Connection
    {
        Connection() = default;
        Connection(const Connection&) = default;
        Connection(VHostConfig& v, shared_socket s, uint16_t p);
        Connection& operator=(const Connection&) = default;
        Connection(Connection&&) = default;
        Connection& operator=(Connection&&) = default;
        ~Connection() = default;

        // contains socket of client(accepted in listener) and the vhost
        // used by dispacther
        /* FIXME: Add members to store the information relative to the
        ** connection. When implementing requests requiring multiple responses
        ** will need a boolean to know the state of the request.
        */
        VHostConfig vc_;
        shared_socket s_;
        uint16_t p_;
    };
    using shared_con = std::shared_ptr<Connection>;
} // namespace http
