/**
 * \file request/request.hh
 * \brief Request declaration.
 */

#pragma once

#include "request/types.hh"
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
        Request(const Request&) = default;
        Request& operator=(const Request&) = default;
        Request(Request&&) = default;
        Request& operator=(Request&&) = default;
        ~Request() = default;
        // Add members to store the information relative to a request.
        method m;
        STATUS_CODE status;
        std::string request_uri;
        std::map<std::string, std::string> headers;
        std::string msg_body;
    };
} // namespace http
