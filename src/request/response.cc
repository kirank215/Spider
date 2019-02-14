#include "response.hh"

http::Response::Response(const STATUS_CODE& st)
    : status(st)
{
    headers = std::map<std::string, std::string>();
    msg_body = "";
}

int check_list(std::string header, std::list<std::string> list)
{
    for (auto it = list.begin(); it != list.end(); ++it)
    {
        if (*it == header)
            return 1;
    }
    return 0;
}
int check_headers(std::map<std::string, std::string> headers)
{
    for (auto it = map.begin(); it != map.end(); ++it)
    {
        if (check_list(it->first, General))
        {
            continue;
        }
        if (check_list(it->first, Request))
        {
            continue;
        }
        if (check_list(it->first, Entity))
        {
            continue;
        }
        return 0;
    }
    return 1;
}
http::Response::Response(const Request& request,
                         const STATUS_CODE& st = STATUS_CODE::OK)
    : status(st)
{
    if (request.method != "GET" || request.method != "POST"
        || request.method != "HEAD")
        status = 400;
    else
    {
        if (!check_headers)
        {
        }
        headers = request.headers;
        msg_body = request.msg_body;
    }
}
