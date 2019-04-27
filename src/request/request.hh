/**
 * \file request/request.hh
 * \brief Request declaration.
 */

#pragma once
#include <algorithm>
#include <iostream>
#include <map>
#include <string>

#include "misc/sys-wrapper.hh"
#include "socket/socket.hh"
#include "types.hh"

namespace http
{
    /**
     * \struct Request
     * \brief Value object representing a request.
     */
    enum method
    {
        GET,
        POST,
        HEAD,
        BAD,
        KNOWN
    };
    inline std::map<std::string, method> methods =
        {
            { "GET" , GET },
            { "POST" , POST },
            { "HEAD" , HEAD },
            { "PUT" , KNOWN },
            { "DELETE" , KNOWN },
            { "CONNECT" , KNOWN },
            { "OPTIONS" , KNOWN },
            { "TRACE" , KNOWN }
        };
    struct Request
    {
        Request() = default;
        Request(shared_socket);
        Request(const Request&) = default;
        Request& operator=(const Request&) = default;
        Request(Request&&) = default;
        Request& operator=(Request&&) = default;
        ~Request() = default;
        // Add members to store the information relative to a request.
        method m_;
        std::string request_uri_;
        std::string version_;
        std::map<std::string, std::string> headers_;
        int msg_body_len_;
        std::string msg_body_;
        std::string incoming_error_;
    };
} // namespace http
