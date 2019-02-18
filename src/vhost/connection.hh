/**
 * \file vhost/connection.hh
 * \brief Connection declaration.
 */

#pragma once

#include <memory>
#include "vhost.hh"
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
        Connection& operator=(const Connection&) = default;
        Connection(Connection&&) = default;
        Connection& operator=(Connection&&) = default;
        ~Connection() = default;

        // contains socket of client(accepted in listener) and the vhost
        // used by dispacther
        /* FIX: Add members to store the information relative to the
        ** connection.
        */
        shared_vhost vhost;
        shared_socket s;
        port p;
    };
} // namespace http
