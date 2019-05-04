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
#include "socket/ssl-socket.hh"
#include "misc/addrinfo/addrinfo.hh"
#include "events/listener.hh"
#include "events/register.hh"
#include "error/not-implemented.hh"
#include "vhost/dispatcher.hh"
#include "vhost/vhost-static-file.hh"
#include "vhost/vhost-factory.hh"
#include "vhost/apm.hh"

using namespace http;
using namespace misc;

EventWatcherRegistry http::event_register;
Dispatcher http::dispatcher;
APM http::apm;


std::shared_ptr<Socket> create_SSLSocket(
        const AddrInfo& addrinfo, const VHostConfig& vc)
{
    std::shared_ptr<SSLSocket> sha_sock;
    if((*addrinfo.begin()).ai_family == AF_INET)
        sha_sock = std::make_shared<SSLSocket>
            (AF_INET , SOCK_STREAM, 0, vc.ctx);
    else
    {
        sha_sock = std::make_shared<SSLSocket>
            (AF_INET6 , SOCK_STREAM, 0, vc.ctx);
        sha_sock->ipv6_set(true);
    }
    sha_sock->setsockopt(SOL_SOCKET, (SO_REUSEPORT | SO_REUSEADDR), 1);
    sys::fcntl_set((*(sha_sock)->fd_get()), O_NONBLOCK);

    sha_sock->ssl_set_fd(*(sha_sock)->fd_get());

    return sha_sock;
}


std::shared_ptr<Socket> create_DefaultSocket(
        const AddrInfo& addrinfo)
{
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

    return sha_sock;
}

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

    InitServerCTX();


    dispatcher.set_hosts(sc);   // add list of hosts in dispatcher

    AddrInfoHint hints;
    hints.family(AF_UNSPEC);
    hints.socktype(SOCK_STREAM);

    //server socket creation and binding
    for (auto vc : sc.list_vhost)
    {
        auto vstatic = VHostFactory::Create(vc);

        if(setKeyCert(vc) == -1)    // set key and certificate of host
            return -1;                     // add SNI for flexibilty with more vhosts

        dispatcher.insert_staticfile(vstatic);
        AddrInfo addrinfo = misc::getaddrinfo(vc.ip.c_str(),
                std::to_string(vc.port).c_str(), hints);

        //     removed dead code here


        std::shared_ptr<Socket> sha_sock;
        if(vc.ctx == NULL)
            sha_sock = create_DefaultSocket(addrinfo);
        else
        {
            sha_sock = create_SSLSocket(addrinfo, vc);
            SSL_CTX_set_tlsext_servername_callback(vc.ctx, ServerNameCallback);
        }

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

        event_register.register_ew<ListenerEW>(sha_sock);
    }
    event_register.loop_get()();


    return 0;
}

