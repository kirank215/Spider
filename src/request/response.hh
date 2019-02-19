/**
 * \file request/response.hh
 * \brief Response declaration.
 */

#pragma once

#include <list>
#include <map>

#include "request/request.hh"
#include "request/types.hh"
namespace http
{
    std::list<std::string> General_H = {
        "Cache-control",     "Connection", "Date", "Pragma", "Trailer",
        "Transfer-encoding", "Upgrade",    "Via",  "Warning"};

    std::list<std::string> Entity_H = {
        "Allow",          "Content_encoding", "Content_language",
        "Content_length", "Content_location", "Content_md5",
        "Content_range",  "Content_type",     "Expires",
        "Last_modified",  "Extension_header"};

    std::list<std::string> Request_H = {"Accept",
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

    std::list<std::string> Response_H = {
        "Accept-Ranges",      "Age",         "ETag",   "Location",
        "Proxy-Authenticate", "Retry-After", "Server", "Vary",
        "WWW-Authenticate"};

    /**
     * \struct Response
     * \brief Value object representing a response.
     */
    struct Response
    {
        explicit Response(const STATUS_CODE&);
        Response(const Request&, const STATUS_CODE& = STATUS_CODE::OK,
                    std::string body = "");

        Response() = default;
        Response(const Response&) = default;
        Response& operator=(const Response&) = default;
        Response(Response&&) = default;
        Response& operator=(Response&&) = default;
        ~Response() = default;

        void build_head();
        void build_get(std::string& body);
        void build_post();
        int check_headers() const;
        int check_list(const std::string& header,
                       const std::list<std::string>& list) const;

        // Add members to store the information relative to a response.
        STATUS_CODE status_;
        std::map<std::string, std::string> headers_;
        std::string msg_body_;
    };
} // namespace http
