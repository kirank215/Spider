/**
 * \file request/response.hh
 * \brief Response declaration.
 */

#pragma once

#include "request/request.hh"
#include "request/types.hh"
#include <map>
namespace http
{
    /**
     * \struct Response
     * \brief Value object representing a response.
     */
    struct Response
    {
        explicit Response(const STATUS_CODE&);
        Response(const Request&, const STATUS_CODE& = STATUS_CODE::OK);

        Response() = default;
        Response(const Response&) = default;
        Response& operator=(const Response&) = default;
        Response(Response&&) = default;
        Response& operator=(Response&&) = default;
        ~Response() = default;
        // Add members to store the information relative to a response.
        STATUS_CODE status;
        std::map<std::string, std::string> headers;
        std::string msg_body;
    };
} // namespace http
