#include "request/error.hh"

#include "request/types.hh"

namespace http::error
{
    Response bad_request()
    {
        return Response(STATUS_CODE::BAD_REQUEST);
    }

    Response forbidden(const Request& request)
    {
        return Response(request, STATUS_CODE::FORBIDDEN);
    }

    Response not_found(const Request& request)
    {
        return Response(request, STATUS_CODE::NOT_FOUND);
    }

    Response method_not_allowed(const Request& request)
    {
        return Response(request, STATUS_CODE::METHOD_NOT_ALLOWED);
    }

    Response upgrade_required(const Request& request)
    {
        return Response(request, STATUS_CODE::UPGRADE_REQUIRED);
    }

    Response internal_server_error(const Request& request)
    {
        return Response(request, STATUS_CODE::INTERNAL_SERVER_ERROR);
    }

    Response not_implemented(const Request& request)
    {
        return Response(request, STATUS_CODE::NOT_IMPLEMENTED);
    }

    Response bad_gateway(const Request& request)
    {
        return Response(request, STATUS_CODE::BAD_GATEWAY);
    }

    Response gateway_timeout(const Request& request)
    {
        return Response(request, STATUS_CODE::GATEWAY_TIMEOUT);
    }

    Response http_version_not_supported(const Request& request)
    {
        return Response(request, STATUS_CODE::HTTP_VERSION_NOT_SUPPORTED);
    }
} // namespace http::error
