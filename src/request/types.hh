/**
 * \file request/types.hh
 * \brief STATUS_CODE declaration.
 */

#pragma once

#include <stdexcept>
#include <stdint.h>
#include <utility>

namespace http
{
    constexpr auto http_crlf = "\r\n";

    enum STATUS_CODE : uint16_t
    {
        OK = 200,
        BAD_REQUEST = 400,
        FORBIDDEN = 403,
        NOT_FOUND = 404,
        METHOD_NOT_ALLOWED = 405,
        UPGRADE_REQUIRED = 426,
        INTERNAL_SERVER_ERROR = 500,
        NOT_IMPLEMENTED = 501,
        BAD_GATEWAY = 502,
        GATEWAY_TIMEOUT = 504,
        HTTP_VERSION_NOT_SUPPORTED = 505,
        SHOULD_NOT_HAPPEN
    };

    /**
     * \brief Associate a status code to its message.
     *
     * \param code STATUS_CODE http status code.
     * \return pair associating a status code to its message.
     */
    inline constexpr std::pair<STATUS_CODE, decltype(http_crlf)>
    statusCode(STATUS_CODE code)
    {
        switch (code)
        {
        case OK:
            return {OK, "OK"};
            break;
        case BAD_REQUEST:
            return {BAD_REQUEST, "Bad Request"};
        case FORBIDDEN:
            return {FORBIDDEN, "Forbidden"};
        case NOT_FOUND:
            return {NOT_FOUND, "Not Found"};
        case METHOD_NOT_ALLOWED:
            return {METHOD_NOT_ALLOWED, "Method Not Allowed"};
        case UPGRADE_REQUIRED:
            return {UPGRADE_REQUIRED, "Upgrade Required"};
        case INTERNAL_SERVER_ERROR:
            return {INTERNAL_SERVER_ERROR, "Internal Server Error"};
        case NOT_IMPLEMENTED:
            return {NOT_IMPLEMENTED, "Not Implemented"};
        case BAD_GATEWAY:
            return {BAD_GATEWAY, "Bad Gateway"};
        case GATEWAY_TIMEOUT:
            return {GATEWAY_TIMEOUT, "Gateway Timeout"};
        case HTTP_VERSION_NOT_SUPPORTED:
            return {HTTP_VERSION_NOT_SUPPORTED, "HTTP Version Not Supported"};
        case SHOULD_NOT_HAPPEN:
            throw std::logic_error("UNSET STATUS CODE");
        default:
            throw std::logic_error("unknown status_code");
        }
    }
} // namespace http
