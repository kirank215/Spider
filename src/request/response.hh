/**
 * \file request/response.hh
 * \brief Response declaration.
 */

#pragma once

#include "request.hh"
#include "types.hh"
#include <map>
namespace http
{
    std::list<std::string> General = {
        "Cache-control",     "Connection", "Date", "Pragma", "Trailer",
        "Transfer-encoding", "Upgrade",    "Via",  "Warning"};

    std::list<std::string> Entity = {
        "Allow",          "Content_encoding", "Content_language",
        "Content_length", "Content_location", "Content_md5",
        "Content_range",  "Content_type",     "Expires",
        "Last_modified",  "Extension_header"};

    std::list<std::string> Request = {"Accept",
                                      "Accept-Charset",
                                      "Accept-Encoding",
                                      "Accept-Language",
                                      "Authorization",
                                      "Expect",
                                      "From",
                                      "Host",
                                      "If-Match",
                                      "If-Modified-Since",
                                      "If-None-Match",
                                      "If-Range",
                                      "If-Unmodified-Since",
                                      "Max-Forwards",
                                      "Proxy-Authorization",
                                      "Range",
                                      "Referer",
                                      "TE",
                                      "User-Agent"};

    std::list<std::string> Response = {
        "Accept-Ranges",      "Age",         "ETag",   "Location",
        "Proxy-Authenticate", "Retry-After", "Server", "Vary",
        "WWW-Authenticate"
    };

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
