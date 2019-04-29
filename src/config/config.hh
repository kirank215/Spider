/**
 * \file config/config.hh
 * \brief Declaration of ServerConfig and VHostConfig.
 */

#pragma once
#include <map>
#include <vector>
#include <string>
#include <openssl/ssl.h>
#include <openssl/err.h>

namespace http
{
    /**
     * \struct VHostConfig
     * \brief Value object storing a virtual host configuration.
     *
     * Since each virtual host of the server has its own configuration, a
     * dedicated structure is required to store the information related to
     * each one of them.
     */
    struct Proxy_pass
    {
        std::string ip;
        uint16_t port;
        std::map<std::string, std::string> proxy_set_header;//nn
        std::vector<std::string> proxy_remove_header;//nn
        std::map<std::string, std::string> set_header;//nn
        std::vector<std::string> remove_header;//nn
    };
    struct VHostConfig
    {
        VHostConfig() = default;
        VHostConfig(const VHostConfig&) = default;
        VHostConfig& operator=(const VHostConfig&) = default;
        VHostConfig(VHostConfig&&) = default;
        VHostConfig& operator=(VHostConfig&&) = default;
        bool operator==(const VHostConfig&) const;

        ~VHostConfig() = default;
        std::string ip;
        std::string server_name;
        uint16_t port;
        std::string root;                           // mutual exclusive proxy
        std::string default_file;                   //not allowed if proxy
        std::string ssl_cert;                       //both
        std::string ssl_key;                        //or none here
        Proxy_pass proxy_pass;
        std::string auth_basic;                     //both or none
        std::vector <std::string> auth_basic_users; //here
        std::string health_endpoint; //nn
        bool auto_index; //nn, not allowed if proxy
        bool default_vhost; //nn, needs to be unique
    };

    int setKeyCert(SSL_CTX* );     // possibly for every host

    /**
     * \struct ServerConfig
     * \brief Value object storing the server configuration.
     *
     * To avoid opening the configuration file each time we need to access the
     * server configuration, a dedicated structure is required to store it.
     */
    struct ServerConfig
    {
        ServerConfig() = default;
        ServerConfig(const ServerConfig&) = default;
        ServerConfig& operator=(const ServerConfig&) = default;
        ServerConfig(ServerConfig&&) = default;
        ServerConfig& operator=(ServerConfig&&) = default;

        ~ServerConfig() = default;
        size_t header_max_size;
        size_t uri_max_size;
        size_t payload_max_size;
        bool default_vhost_found;
        std::vector<VHostConfig> list_vhost;
        VHostConfig default_vhost;
        SSL_CTX *ctx = NULL;
    };

    /**
     * \brief Parse the server configuration file.
     *
     * \param path string containing the path to the server configuration
     * file.
     * \return The server configuration.
     */
    ServerConfig parse_configuration(const std::string& path, bool dry);


    /**
     * \brief Initialise SSL-server for the given server.
     *
     * \param sc The server for which ssl-server has to be initialised
     *
     */
    SSL_CTX* InitServerCTX();

} // namespace http
