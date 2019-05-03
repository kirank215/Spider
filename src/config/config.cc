#include "config/config.hh"
#include <exception>
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
            throw std::logic_error("File does not exist\n");
    }

    bool VHostConfig::operator==(const VHostConfig& rhs) const
    {
        return this->server_name == rhs.server_name;
    }
    ServerConfig parse_configuration(const std::string& path, bool dry)
    {
        json j;
        j = create_json(path);
        json elt = j;
        struct ServerConfig SC;
        SC.default_vhost_found = false;
        if(!dry)
        {
            std::vector<VHostConfig> list_vhost;
            for(unsigned i = 0; i < elt.size(); i++)
            {
                struct VHostConfig v;
                if(elt["vhosts"][i]["ip"].is_string() &&
                        elt["vhosts"][i]["port"].is_number() &&
                        elt["vhosts"][i]["server_name"].is_string())
                {
                    v.ip = elt["vhosts"][i]["ip"];
                    v.server_name = elt["vhosts"][i]["server_name"];
                    v.port = elt["vhosts"][i]["port"];
                    if(elt["vhosts"][i]["proxy_pass"] != nullptr)
                    {
                        if ((elt["vhosts"][i]["proxy_pass"]["ip"].is_string()) &&
                                (elt["vhosts"][i]["proxy_pass"]["port"].is_number()))
                        {
                            struct Proxy_pass proxy_pass;
                            proxy_pass.ip = elt["vhosts"][i]["proxy_pass"]["ip"];
                            proxy_pass.port = elt["vhosts"][i]["proxy_pass"]["port"];

                            if((elt["vhosts"][i]["proxy_pass"]["proxy_set_header"]) != nullptr)
                            {
                                proxy_pass.proxy_set_header =
                                    elt["vhosts"][i]["proxy_pass"]["proxy_set_header"].get<std::map<std::string, std::string>>();
                            }
                            if((elt["vhosts"][i]["proxy_pass"]["proxy_remove_header"]) != nullptr)
                            {
                                for(auto a : elt["vhosts"][i]["proxy_pass"]["proxy_remove_header"])
                                {
                                    proxy_pass.proxy_remove_header.push_back(a);
                                }
                            }
                            if((elt["vhosts"][i]["proxy_pass"]["set_header"]) != nullptr)
                            {
                                proxy_pass.set_header =
                                    elt["vhosts"][i]["proxy_pass"]["set_header"].get<std::map<std::string, std::string>>();
                            }
                            if((elt["vhosts"][i]["proxy_pass"]["remove_header"]) != nullptr)
                            {
                                for(auto a : elt["vhosts"][i]["proxy_pass"]["remove_header"])
                                {
                                    proxy_pass.remove_header.push_back(a);
                                }
                            }
                            v.proxy_pass = proxy_pass;
                        }
                    }
                    else
                    {
                        v.root = elt["vhosts"][i]["root"];
                        if(elt["vhosts"][i]["default_file"].is_string())
                            v.default_file = elt["vhosts"][i]["default_file"];
                        else
                            v.default_file = "index.html";
                    }
                    if(elt["vhosts"][i]["ssl_cert"] != nullptr && elt["vhosts"][i]["ssl_cert"].is_string()
                            && elt["vhosts"][i]["ssl_key"]  != nullptr && elt["vhosts"][i]["ssl_key"].is_string())
                    {
                        v.ssl_cert = elt["vhosts"][i]["ssl_cert"];
                        v.ssl_key = elt["vhosts"][i]["ssl_key"];
                    }
                    if(elt["vhosts"][i]["auth_basic_user"] != nullptr
                            && elt["vhosts"][i]["auth_basic"] != nullptr && elt["vhosts"][i]["auth_basic"].is_string())
                    {
                        v.auth_basic = elt["vhosts"][i]["auth_basic"];
                        //list of user for loop to add every strings...
                    }
                    if(elt["vhosts"][i]["health_endpoint"] != nullptr)
                        v.health_endpoint = elt["vhosts"][i]["health_endpoint"];
                    if(elt["vhosts"][i]["default_vhost"] != nullptr) // equal to true
                    {
                        if(!SC.default_vhost_found)
                        {
                            SC.default_vhost_found = true;
                            //need to set this vhost as the default one
                        }
                    }
                    list_vhost.push_back(v);
                }
            }
            SC.list_vhost = list_vhost;
        }
        return SC;
    }
}
