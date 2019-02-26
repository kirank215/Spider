#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include "config/config.hh"
#include "misc/socket.hh"
#include "misc/fd.hh"
#include "socket/default-socket.hh"
#include "misc/addrinfo/addrinfo.hh"
#include "events/listener.hh"
#include "events/register.hh"
#include "error/not-implemented.hh"
#include "vhost/dispatcher.hh"
#include "vhost/vhost-static-file.hh"
#include "vhost/vhost-factory.hh"

using namespace http;
using namespace misc;

EventWatcherRegistry http::event_register;
Dispatcher http::dispatcher;

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
        std::cerr << "\n" << e.what();
        return 1;
    }

        dispatcher.set_hosts(sc);   // add list of hosts in dispatcher

        /*Doing only ipv4 now*/
        auto sha_sock = std::make_shared<DefaultSocket>(AF_INET, SOCK_STREAM, 0);

        sha_sock->setsockopt(SOL_SOCKET, (SO_REUSEPORT | SO_REUSEADDR), 1);

        AddrInfoHint hints;
        hints.family(AF_INET);
        hints.socktype(SOCK_STREAM);

        //server socket creation and binding
        VHostConfig vc = sc.list_vhost[0];
        auto vstatic = VHostFactory::Create(vc);

        dispatcher.insert_staticfile(vstatic);
        const char *port = std::to_string(vc.port).c_str();
        const char *ip = vc.ip.c_str();
        AddrInfo addrinfo = misc::getaddrinfo(ip, port, hints);
        sha_sock->bind(addrinfo.begin()->ai_addr, addrinfo.begin()->ai_addrlen);
        sha_sock->listen(10);


        //calling listener
        /*EventWatcherRegistry event_register();*/
        event_register.register_ew<ListenerEW>(sha_sock);
        event_register.loop_get()();


    return 0;

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
