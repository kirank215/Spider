#include <iostream>
#include <cstring>
#include <fcntl.h>
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

size_t http::Request::header_max_size = 0;
size_t http::Request::uri_max_size = 0;
int http::Request::payload_max_size = 0;

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

    AddrInfoHint hints;
    hints.family(AF_UNSPEC);
    hints.socktype(SOCK_STREAM);

    //server socket creation and binding
    for(VHostConfig vc : sc.list_vhost)
    {
        auto vstatic = VHostFactory::Create(vc);

        dispatcher.insert_staticfile(vstatic);
        AddrInfo addrinfo = misc::getaddrinfo(vc.ip.c_str(),
                std::to_string(vc.port).c_str(), hints);
        /*
           for(auto &i : addrinfo)
           {
           if(i.ai_family == AF_INET)
           {
           struct sockaddr_in *ipv4 = (struct sockaddr_in *)i.ai_addr;
           i.ai_addr->sockaddr = ipv4;
           }
           else
           {
           struct sockaddr_in *ipv6 = (struct sockaddr_in6 *)i.ai_addr;
           i.ai_addr = ipv6;
           }
           }
         */
        std::shared_ptr<DefaultSocket> sha_sock;
        if((*addrinfo.begin()).ai_family == AF_INET)
            sha_sock = std::make_shared<DefaultSocket>
                (AF_INET , SOCK_STREAM, 0);
        else
        {
            sha_sock = std::make_shared<DefaultSocket>
                (AF_INET6 , SOCK_STREAM, 0);
            sha_sock->ipv6_set(true);
        }
        sha_sock->setsockopt(SOL_SOCKET, (SO_REUSEPORT | SO_REUSEADDR), 1);
        sys::fcntl_set((*(sha_sock)->fd_get()), O_NONBLOCK);

        for(auto &i : addrinfo)
        {
            try
            {
                sha_sock->bind(i.ai_addr, i.ai_addrlen);
            }
            catch(std::system_error&)
            {
                continue;
            }
        }
        sha_sock->listen(10);

        http::Request::header_max_size = 8000;  // @Salome add this VHostCofig
        http::Request::uri_max_size = 2000;     // values here after finishing
        http::Request::payload_max_size = 2000;  // that part FIXME

        //calling listener
        /*EventWatcherRegistry event_register();*/
        event_register.register_ew<ListenerEW>(sha_sock);
    }
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
