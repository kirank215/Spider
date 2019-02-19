#include "error/not-implemented.hh"
#include "config/config.hh"
#include <iostream>

using namespace http;

int main(int argc, char *argv[])
{
    using namespace http;
    int i = 1;
    for(; i < argc; i++)
    {
        if(argv[i][0] != '-')
            break;
    }
    struct ServerConfig serveurconfig = http::parse_configuration(argv[i]);
    return 1;
    /*   TESTING PARSE CONFIG
    const std::string path= "configure_file.json";
    ServerConfig lol = parse_configuration(path);
    for(auto it = lol.list_vhost.begin(); it != lol.list_vhost.end(); it++)
    {
        VHostConfig foo = *it;
        std::cout << "IP : " << foo.ip << "\n";
        std::cout << "port : " << (unsigned)foo.port<< "\n";
        std::cout << "server_name : " << foo.server_name<< "\n";
        std::cout << "root : " << foo.root<< "\n";
        std::cout << "default_file : " << foo.default_file<< "\n\n";
    }
    throw http::NotImplemented();
*/
}
