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
#include <fstream>
#include <iostream>
#include <json.hpp>
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

    VHostConfig::VHostConfig()
    {
         APM_local.insert(std::pair<std::string,int>("requests_2xx",  0));
         APM_local.insert(std::pair<std::string,int>("requests_4xx",  0));
         APM_local.insert(std::pair<std::string,int>("requests_5xx",  0));
         APM_local.insert(std::pair<std::string,int>("requests_nb" , 0));

    }

    VHostConfig::~VHostConfig()
    {
      if(ctx != NULL)
        SSL_CTX_free(ctx);
    }

    int setKeyCert(VHostConfig& vc)
    {

        if(vc.ssl_cert == "")
            return 1;

        vc.ctx = SSL_CTX_new(SSLv23_server_method());
        //XXX check on how cert and key are generated
        if (SSL_CTX_use_certificate_file(vc.ctx, vc.ssl_cert.c_str(), SSL_FILETYPE_PEM) <= 0) {
            ERR_print_errors_fp(stderr);
            return -1;
        }

        if (SSL_CTX_use_PrivateKey_file(vc.ctx, vc.ssl_key.c_str(), SSL_FILETYPE_PEM) <= 0 ) {
            ERR_print_errors_fp(stderr);
            return -1;
        }

        // check validity of key
        if ( !SSL_CTX_check_private_key(vc.ctx) )
        {
            std::cerr << "Private key does not match the public certificate\n";
            return -1;
        }
        return 1;
    }


    ServerConfig parse_configuration(const std::string& path, bool dry)
    {
        json elt;
        elt = create_json(path);
        struct ServerConfig SC;
        SC.default_vhost_found = false;
        if(!dry)
        {
            std::vector<VHostConfig> list_vhost;
            size_t size = elt["vhosts"].size();
            for(unsigned i = 0; i < size; i++)
            {
                struct VHostConfig v;
                if(elt["vhosts"][i]["ip"].is_string() &&
                        elt["vhosts"][i]["port"].is_number() &&
                        elt["vhosts"][i]["server_name"].is_string())
                {
                    v.ip = elt["vhosts"][i]["ip"];
                    v.server_name = elt["vhosts"][i]["server_name"];
                    v.port = elt["vhosts"][i]["port"];

                    if(elt["vhosts"][i]["auto_index"] != nullptr
                                && elt["vhosts"][i]["auto_index"])
                        v.auto_index = true;
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
                            v.default_file = "index.txt";
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

    void InitServerCTX()
    {
        // setup
        OpenSSL_add_all_algorithms();
        SSL_load_error_strings();

    }

}
