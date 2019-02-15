#include "request.hh"


namespace http
{
    static method method_type(std::string str)
    {
        if(str.compare("GET") == 0)
            return GET;
        else if(str.compare("POST") == 0)
            return POST;
        else if(str.compare("HEAD") == 0)
            return HEAD;
        else
            return BAD;
    }

    static std::string get_token(int& pos, std::string delim,
                                    const std::string buffer)
    {
        int count = 0;
        auto found = buffer.find(delim, pos);
        if(found != std::string::npos)
        {
            count = found - pos;
            std::string res = buffer.substr(pos, count);
            pos = found + 1;
            return res;
        }
        else
            throw std::logic_error("Parsing error in Request");
    }

    Request::Request(shared_socket new_socket)
    {
        char buffer[4096]; //Ususally buffer lenght is 8kb
        auto check = recv(new_socket->fd_get()->fd_, buffer, 4096, 0); // 0 is a flag
        int max = 0;
        if (check < 0)
            std::cout << "Error reading the request";
        else
        {
            std::string type = "";
            std::string body = "";
            int i = 0;
            int len = sizeof(buffer);
            m_ = method_type(get_token(i, " ", buffer));
            request_uri_ = get_token(i, " ", buffer);
            version_ = get_token(i, http_crlf, buffer);
            msg_body_len_ = 0;
            while(1)
            {
                i++;
                type = get_token(i, ":", buffer);
                body = get_token(i, http_crlf, buffer);
                if(type.compare("Content-Length") == 0)
                {
                    msg_body_len_ = stoi(body);
                }
                headers_.emplace(type, body);
                if(i+2 < len)
                {
                    if(buffer[i+1] == '\r' && buffer[i+2] == '\n')
                    {
                        i = i+3;
                        break;
                    }
                }
                else
                    break;
            }
            if(headers_.find("Host") == headers_.end())
            {
                throw std::logic_error("No Host provided");
            }
            auto it = headers_.find("Connection");  // Remove this block
            it->second = "close";                    // in later parts
            max = i + msg_body_len_;
            while(i <= max && i <= len)
            {
                msg_body_.push_back(buffer[i]);
                i++;
            }
        }
    }
}
