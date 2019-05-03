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
        struct ServerConfig SC;
        SC.default_vhost_found = false;
        if(!dry)
        {
            std::vector<VHostConfig> list_vhost;
            for(auto elt : j)
            {
                for(unsigned int i = 0; i < elt.size(); i++)
                {
                    struct VHostConfig v;
                    if(elt[i]["ip"].is_string() &&
                            elt[i]["port"].is_number() &&
                            elt[i]["server_name"].is_string())
                    {
                        v.ip = elt[i]["ip"];
                        v.server_name = elt[i]["server_name"];
                        v.port = elt[i]["port"];
                        if(elt[i]["proxy_pass"])
                        {
                            if ((elt[i]["proxy_pass"]["ip"].is_string()) &&
                                (elt[i]["proxy_pass"]["port"].is_number()))
                                {
                                    struct Proxy_pass proxy_pass;
                                    proxy_pass.ip = elt[i]["proxy_pass"]["ip"];
                                    proxy_pass.port = elt[i]["proxy_pass"]["port"];

                                    if((elt[i]["proxy_pass"]["proxy_set_header"]))
                                    {
                                        for(auto it = elt[i]["proxy_pass"]["proxy_set_header"].begin();
                                        it != elt[i]["proxy_pass"]["proxy_set_header"].end(); it++)
                                        {
                                            proxy_pass.proxy_set_header.insert(it->first, it->second);
                                        }
                                    }
                                    if((elt[i]["proxy_pass"]["proxy_remove_header"]))
                                    {
                                        for(int k = 0; elt[i]["proxy_pass"]["proxy_remove_header"][k]; k++)
                                        {
                                            proxy_pass.proxy_remove_header.push_back(elt[i]["proxy_pass"]["proxy_remove_header"][k]);
                                        }
                                    }
                                    if((elt[i]["proxy_pass"]["set_header"]))
                                    {
                                        for(auto it = elt[i]["proxy_pass"]["set_header"].begin();
                                        it != elt[i]["proxy_pass"]["set_header"].end(); it++)
                                        {
                                            proxy_pass.set_header.insert(it->first, it->second);
                                        }
                                    }
                                    if((elt[i]["proxy_pass"]["remove_header"]))
                                    {
                                        for(int k = 0; elt[i]["proxy_pass"]["remove_header"][k]; k++)
                                        {
                                            proxy_pass.remove_header.push_back(elt[i]["proxy_pass"]["remove_header"][k]);
                                        }
                                    }
                                    v.proxy_pass = proxy_pass;
                                }
                        }
                        else
                        {
                            v.root = elt[i]["root"];
                            if(elt[i]["default_file"].is_string())
                                v.default_file = elt[i]["default_file"];
                            else
                                v.default_file = "index.html";
                        }
                        if(elt[i]["ssl_cert"] && elt[i]["ssl_cert"].is_string()
                            && elt[i]["ssl_key"] && elt[i]["ssl_key"].is_string())
                        {
                            v.ssl_cert = elt[i]["ssl_cert"];
                            v.ssl_key = elt[i]["ssl_key"];
                        }
                        if(elt[i]["auth_basic_user"]
                            && elt[i]["auth_basic"] && elt[i]["auth_basic"].is_string())
                        {
                            v.auth_basic = elt[i]["auth_basic"];
                            //list of user for loop to add every strings...
                        }
                        if(elt[i]["health_endpoint"])
                            v.health_endpoint = elt[i]["health_endpoint"];
                        if(elt[i]["default_vhost"]) // equal to true
                        {
                            if(!SC.default_vhost_found)
                            {
                                SC.default_vhost_found = true;
                                //need to set this vhost as the default one
                            }
                        }
                        list_vhost.push_back(v);
                    }
                    else
                        throw std::logic_error("Vhost incomplete\n");
                }
            }
            SC.list_vhost = list_vhost;
        }
        return SC;
    }
}
