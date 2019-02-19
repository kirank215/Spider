#include "config/config.hh"

#include "error/not-implemented.hh"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wswitch-enum"
#pragma GCC diagnostic ignored "-Wswitch-default"
#pragma GCC diagnostic ignored "-Winline"
#ifdef __clang__
#pragma GCC diagnostic ignored "-Wlogical-op-parentheses"
#endif
#include <json.hpp>
#include <fstream>
#include <iostream>
#pragma GCC diagnostic pop

using json = nlohmann::json;

namespace http
{

    static json create_json(const std::string path)
    {
        std::ifstream file(path);
        if(file.is_open())
        {

            json j = json::parse(file);
            return j;
        }
        else
            throw std::logic_error("File does not exist");//FIXME, should return int
    }

    ServerConfig parse_configuration(const std::string& path)
    {
        struct ServerConfig SC;
        json j = create_json(path); // FIXME, should return int
        std::vector<VHostConfig> list_vhost;
        for(auto elt : j)
        {
            for(unsigned int i = 0; i < elt.size(); i++)
            {
                struct VHostConfig v;
                if(elt[i]["ip"].is_string() &&
                    elt[i]["port"].is_number() &&
                    elt[i]["server_name"].is_string() &&
                    elt[i]["root"].is_string())
                {
                    v.ip = elt[i]["ip"];
                    v.server_name = elt[i]["server_name"];
                    v.port = elt[i]["port"];
                    v.root = elt[i]["root"];
                    if(elt[i]["default_file"].is_string())
                        v.default_file = elt[i]["default_file"];
                    else
                        v.default_file = "index.html";
                    list_vhost.push_back(v);
                }
            }
            SC.list_vhost = list_vhost;
        }
        return SC;
    }
}
