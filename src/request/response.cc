#include "response.hh"

http::Response::Response(const STATUS_CODE& st)
    : status(st)
{
    headers = std::map<std::string, std::string>();
    msg_body = "";
}

int http::check_list(std::string header, std::list<std::string> list)
{
    for (auto it = list.begin(); it != list.end(); ++it)
    {
        if (*it == header)
            return 1;
    }
    return 0;
}
int http::check_headers(std::map<std::string, std::string> map)
{
    for (auto it = map.begin(); it != map.end(); ++it)
    {
        if (check_list(it->first, http::General_H))
        {
            continue;
        }
        if (check_list(it->first, http::Request_H))
        {
            continue;
        }
        if (check_list(it->first, http::Entity_H))
        {
            continue;
        }
        return 0;
    }
    return 1;
}
http::Response::Response(const Request& request, const STATUS_CODE& st)
    : status(st)
{
    if (request.m == BAD) // Got a bad method.
        status = BAD_REQUEST;
    else
    {
        if (!check_headers)
        {
        }
        headers = request.headers;
        msg_body = request.msg_body;
    }
}
