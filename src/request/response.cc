#include "response.hh"

#include <chrono>
#include <ctime>
#include <string>

http::Response::Response(const STATUS_CODE& st)
    : status_(st)
{
    headers_ = std::map<std::string, std::string>();
    msg_body_ = "";
}

int http::Response::check_list(const std::string& header,
                               const std::list<std::string>& list) const
{
    for (auto it = list.begin(); it != list.end(); ++it)
    {
        if (*it == header)
            return 1;
    }
    return 0;
}

int http::Response::check_headers() const
{
    for (auto it = headers_.begin(); it != headers_.end(); ++it)
    {
        if (check_list(it->first, http::General_H))
            continue;
        if (check_list(it->first, http::Request_H))
            continue;
        if (check_list(it->first, http::Entity_H))
            continue;
        return 0;
    }
    return 1;
}

void http::Response::build_head()
{
    msg_body_ = "";
    auto time =
        std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    headers_.emplace("Date", std::ctime(&time));
    headers_.emplace("Content-Length", "0");
}

void http::Response::build_get(std::string& body)
{
    msg_body_ = body;
    auto time =
        std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    headers_.emplace("Date", std::ctime(&time));
    headers_.emplace("Content-Length", std::to_string(body.length()));
}

void http::Response::build_post()
{
    msg_body_ = "";
    auto time =
        std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    headers_.emplace("Date", std::ctime(&time));
    headers_.emplace("Content-Length", "0");
}

http::Response::Response(const Request& request, const STATUS_CODE& st,
                            std::string body)
    : status_(st)
{
    if (request.m_ == BAD) // Got a bad method.
        status_ = BAD_REQUEST;
    if (!check_headers())
        status_ = BAD_REQUEST;
    else
    {
        if (request.m_ == HEAD)
            build_head();
        if (request.m_ == GET)
            build_get(body);
        if (request.m_ == POST)
            build_post();
        headers_ = request.headers_;
        msg_body_ = request.msg_body_;
    }
}
