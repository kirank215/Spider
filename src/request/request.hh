/**
 * \file request/request.hh
 * \brief Request declaration.
 */

#pragma once
#include "socket/socket.hh"
#include "types.hh"
#include "misc/sys-wrapper.hh"
#include <string>
#include <algorithm>
#include <map>
#include <iostream>
namespace http
{
    /**
     * \struct Request
     * \brief Value object representing a request.
     */
    enum method { GET, POST, HEAD, BAD};
    struct Request
    {
        Request() = default;
        Request(shared_socket new_socket);
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
        STATUS_CODE status;
    };
} // namespace http
