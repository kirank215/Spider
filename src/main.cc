#include "error/not-implemented.hh"
#include "config/config.hh"
#include "misc/socket.hh"
#include "misc/fd.hh"
#include "socket/default-socket.hh"
#include <iostream>
#include <cstring>
using namespace http;
using namespace misc;

int main(int argc, char *argv[])
{
    bool dry = 0;
    if(argc >3 || argc <= 1)
        throw std::logic_error("Invalid number of arguments");
    if(strcmp(argv[1],"-t") == 0)
        dry = 1;
    ServerConfig lol;
    try
    {
        lol = parse_configuration(argv[dry+1], dry);
    }
    catch(std::exception& e)
    {
        std::cerr << e.what();
        exit(1);
    }
    auto def_sock = DefaultSocket(AF_INET, SOCK_STREAM, 0);


    /*TESTING PARSING
    for(auto it = lol.list_vhost.begin(); it != lol.list_vhost.end(); it++)
    {
        VHostConfig foo = *it;
        std::cout << "IP : " << foo.ip << "\n";
        std::cout << "port : " << (unsigned)foo.port<< "\n";
        std::cout << "server_name : " << foo.server_name<< "\n";
        std::cout << "root : " << foo.root<< "\n";
        std::cout << "default_file : " << foo.default_file<< "\n\n";
    }*/
}
