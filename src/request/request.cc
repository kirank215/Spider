#include "request.hh"


namespace http
{
    static method method_type(std::string str)
    {
    /*
        if(str.compare("GET") == 0)
            return GET;
        else if(str.compare("POST") == 0)
            return POST;
        else if(str.compare("HEAD") == 0)
            return HEAD;
        else
            return BAD;
    */
        auto m = methods.find(str);
        if(m != methods.end())
            return m->second;
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
        long unsigned int header_max_size = 8000; //FIXME this is default value,
        long unsigned int uri_max_size = 2000;    //calculate from actual value
        int payload_max_size = 20;              //from config file later;

        char buffer[4096]; //Ususally buffer lenght is 8kb

        long unsigned int header_size = 0;

        auto check = new_socket->recv(buffer, 4096);
        int max = 0;
        if (check < 0)
            std::cout << "Error reading the request";
        else
        {
            std::string met = "";
            std::string type = "";
            std::string body = "";
            incoming_error_ = "";
            int i = 0;
            int len = sizeof(buffer);
            met = get_token(i, " ", buffer);
            m_ = method_type(met);
            request_uri_ = get_token(i, " ", buffer);
            if(request_uri_.size() > uri_max_size)
                incoming_error_ = "uri";
            version_ = get_token(i, http_crlf, buffer);
            header_size = met.size() + request_uri_.size()+ version_.size();
            msg_body_len_ = 0;
            while(1)
            {
                i++;
                type = get_token(i, ":", buffer);
                body = get_token(i, http_crlf, buffer);
                header_size += type.size() + body.size();
                if(type.compare("Content-Length") == 0)
                {
                    msg_body_len_ = stoi(body);
                }
                headers_.emplace(type, body);
                if(header_size > header_max_size)
                {
                    incoming_error_ = "headers";
                    return;
                }
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
            //            auto it = headers_.find("Connection");  // Remove this block
            //            if(it != headers_.end())
            //                it->second = "close";                    // in later parts
            max = i + msg_body_len_;
            while(i < max && i < len)
            {
                msg_body_.push_back(buffer[i]);
                i++;
                if(i > payload_max_size)
                {
                    if(incoming_error_.compare("") == 0)
                        incoming_error_ = "payload";
                    return;
                }
            }
        }
    }
}
