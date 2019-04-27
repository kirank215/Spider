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

    int setKeyCert(SSL_CTX* ctx)
    {
        SSL_CTX_set_ecdh_auto(ctx, 1);      // ??

        //XXX check on how cert and key are generated
        if (SSL_CTX_use_certificate_file(ctx, "cert.pem", SSL_FILETYPE_PEM) <= 0) {
            ERR_print_errors_fp(stderr);
            return -1;
        }

        if (SSL_CTX_use_PrivateKey_file(ctx, "key.pem", SSL_FILETYPE_PEM) <= 0 ) {
            ERR_print_errors_fp(stderr);
            return -1;
        }

        // check validity of key
        if ( !SSL_CTX_check_private_key(ctx) )
        {
            std::cerr << "Private key does not match the public certificate\n";
            return -1;
        }
        return 1;
    }

    ~ServerConfig()
    {
        if(this.sc != NULL)
            SSL_CTX_free(ctx);
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

            if (elt["vhosts"] == nullptr)
                throw("No vhosts found in config file");

            // Filling server config struct
            if (elt["payload_max_size"] != nullptr)
            {
                SC.payload_max_size = elt["payload_max_size"];
            }
            if (elt["uri_max_size"] != nullptr)
            {
                SC.uri_max_size = elt["uri_max_size"];
            }
            if (elt["header_max_size"] != nullptr)
            {
                SC.header_max_size = elt["header_max_size"];
            }
            int size = elt["vhosts"].size();
            size = size;
            for (unsigned i = 0; i < elt["vhosts"].size(); i++)
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
                        if (elt["vhosts"][i]["root"] != nullptr
                            || elt["vhosts"][i]["default_file"] != nullptr
                            || elt["vhosts"][i]["auto_index"] != nullptr)
                            throw("Invalid config file");

                        if ((elt["vhosts"][i]["proxy_pass"]["ip"].is_string()) &&
                                (elt["vhosts"][i]["proxy_pass"]["port"].is_number()))
                        {
                            if (elt["vhosts"][i]["proxy_pass"]["ip"] == nullptr
                                || elt["vhosts"][i]["proxy_pass"]["port"]
                                    == nullptr)
                                throw("Invalid config file");

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
                        if (elt["vhosts"][i]["root"] == nullptr)
                            throw("Invalid config file, no root found");
                        v.root = elt["vhosts"][i]["root"];
                        if(elt["vhosts"][i]["default_file"].is_string())
                            v.default_file = elt["vhosts"][i]["default_file"];
                        else
                            v.default_file = "index.html";
                    }

                    // Proxy pass / root set after this point.
                    if ((elt["vhosts"][i]["ssl_cert"] != nullptr
                         && elt["vhosts"][i]["ssl_key"] == nullptr)
                        || (elt["vhosts"][i]["ssl_cert"] == nullptr
                            && elt["vhosts"][i]["ssl_key"] != nullptr))
                        throw("Invalid config file");

                    if ((elt["vhosts"][i]["auth_basic"] != nullptr
                         && elt["vhosts"][i]["auth_basic_users"] == nullptr)
                        || (elt["vhosts"][i]["auth_basic"] == nullptr
                            && elt["vhosts"][i]["auth_basic_users"] != nullptr))
                        throw new std::invalid_argument(
                            "Invalid config file, need both auth_basic \
                        and auth_basic_user or none.");

                    if(elt["vhosts"][i]["ssl_cert"] != nullptr && elt["vhosts"][i]["ssl_cert"].is_string()
                            && elt["vhosts"][i]["ssl_key"]  != nullptr && elt["vhosts"][i]["ssl_key"].is_string())
                    {
                        v.ssl_cert = elt["vhosts"][i]["ssl_cert"];
                        v.ssl_key = elt["vhosts"][i]["ssl_key"];
                    }
                    if (elt["vhosts"][i]["auth_basic_users"] != nullptr
                        && elt["vhosts"][i]["auth_basic"] != nullptr
                        && elt["vhosts"][i]["auth_basic"].is_string())
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
                            SC.default_vhost = v;
                        } else
                        {
                            throw("Invalid config file");
                        }
                    }
                    list_vhost.push_back(v);
                } else
                {
                    throw("Invalid config file");
                }
            }
            SC.list_vhost = list_vhost;
        }
        return SC;
    }

    void InitServerCTX(ServerConfig sc)
    {
        // setup
        OpenSSl_add_all_algorithms();
        SSL_load_error_strings();

        const SSL_METHOD* method = SSLv23_server_method;
        sc.ctx = SSL_CTX_new(method);
        if(sc.ctx == NULL)
             ERR_print_errors_fp(stderr);
    }
}
