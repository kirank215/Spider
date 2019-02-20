#include "error/not-implemented.hh"
#include "config/config.hh"
#include "misc/socket.hh"
#include "misc/fd.hh"
#include "socket/default-socket.hh"
#include "misc/addrinfo/addrinfo.hh"
#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
using namespace http;
using namespace misc;

int main(int argc, char *argv[])
{
    bool dry = 0;
    if(argc >3 || argc <= 1)
        throw std::logic_error("Invalid number of arguments");
    if(strcmp(argv[1],"-t") == 0)
        dry = 1;
    ServerConfig sc;
    try
    {
        sc = parse_configuration(argv[dry+1], dry);
    }
    catch(std::exception& e)
    {
        std::cerr << e.what();
        exit(1);
    }
    /* Dont knwo whether to be done here or listener*/
    auto def_sock = DefaultSocket(AF_UNSPEC, SOCK_STREAM, 0);

    AddrInfoHint hints;
    hints.family(AF_UNSPEC);
    hints.socktype(SOCK_STREAM);
    hints.addrlen(32);
    // hints.flags(AI_PASSIVE);

    //server
    VHostConfig vc = sc.list_vhost[0];
    const char *port = std::to_string(vc.port).c_str();
    AddrInfo adrin = misc::getaddrinfo(NULL, port, hints);

    //def_sock.bind()
    //calling listener


    /*TESTING PARSING
      for(auto it = sc.list_vhost.begin(); it != sc.list_vhost.end(); it++)
      {
      VHostConfig foo = *it;
      std::cout << "IP : " << foo.ip << "\n";
      std::cout << "port : " << (unsigned)foo.port<< "\n";
      std::cout << "server_name : " << foo.server_name<< "\n";
      std::cout << "root : " << foo.root<< "\n";
      std::cout << "default_file : " << foo.default_file<< "\n\n";
      }*/
}
