#include "response.hh"

#include <chrono>
#include <ctime>
#include <string>

http::Response::Response(const STATUS_CODE& st)
    : status_{st}
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
    std::string t = std::ctime(&time);
    headers_.emplace("Date", t.erase(t.size() - 1 ));     // removes trailing \n
    headers_.emplace("Content-Length", "0");
}

void http::Response::build_get(std::string& body)
{
    msg_body_ = body;
    auto time =
        std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::string t = std::ctime(&time);
    headers_.emplace("Date", t.erase(t.size() - 1 ));     // removes trailing \n
    headers_.emplace("Content-Length", std::to_string(body.length()));
}

// same as get for the moment
void http::Response::build_post()
{
    msg_body_ = "";
    auto time =
        std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::string t = std::ctime(&time);
    headers_.emplace("Date", t.erase(t.size() - 1 ));     // removes trailing \n
    headers_.emplace("Content-Length", "0");
}

static void known_methods(std::string& body)
{
    body = "The known methods include ";
    for( auto& x : http::methods)
    {
        if(x.second != http::KNOWN)
            body += x.first + " ";
    }
    body += ".";
}

/*
static http::STATUS_CODE incoming_error(const std::string& err)
{
    if(err.compare("uri") == 0)
        return http::URI_TOO_LONG;
    else if(err.compare("payload") == 0)
        return http::PAYLOAD_TOO_LARGE;
    else if(err.compare("headers") == 0)
        return http::HEADER_FIELDS_TOO_LARGE;
    else
        return http::SHOULD_NOT_HAPPEN;
}
*/

http::Response::Response(const Request& request, const STATUS_CODE& st,
                            std::string body)
{
    status_ = st;
    if (request.incoming_error_ != OK)
    {
        status_ = request.incoming_error_;
    }
    else if (request.m_ == BAD) // Got a bad method.
        status_ = BAD_REQUEST;
    else if (!check_headers())
        status_ = BAD_REQUEST;
    else if(request.m_ == KNOWN)
    {
        status_ = METHOD_NOT_ALLOWED;
        known_methods(msg_body_);
    }
    else if(request.version_ != "HTTP/1.1")
    {
        status_ = UPGRADE_REQUIRED;
        msg_body_ = "This service requires an " + request.version_ + " protocol.";
    }
    else
    {
        headers_ = request.headers_;
//        msg_body_ = request.msg_body_;

        if (request.m_ == HEAD)
            build_head();
        if (request.m_ == GET)
            build_get(body);
        if (request.m_ == POST)
            build_post();
    }
}

http::Response::Response(const Request& request, VHostConfig& v, const STATUS_CODE& st,
                            std::string body)
    : Response(request, st, body)
{
    this->vc = v;
}
