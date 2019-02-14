/**
 * \file request/request.hh
 * \brief Request declaration.
 */

#pragma once

#include "types.hh"
#include "../misc/sys-wrapper.hh"
#include <string>
#include <algorithm>
#include <map>
namespace http
{
    /**
     * \struct Request
     * \brief Value object representing a request.
     */
    enum method { GET, POST, HEAD };
    struct Request
    {
        Request() = default;
        Request(const std::string buffer); //Handle scoket initialization before
        Request(const Request&) = default;
        Request& operator=(const Request&) = default;
        Request(Request&&) = default;
        Request& operator=(Request&&) = default;
        ~Request() = default;
        // Add members to store the information relative to a request.
        method m_;
        STATUS_CODE status;
        std::string request_uri_;
        std::string version_;
        std::map<std::string, std::string> headers_;
        int msg_body_len_;
        std::string msg_body_;
    };
} // namespace http
