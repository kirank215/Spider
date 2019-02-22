#include <memory>
#include "vhost/dispatcher.hh"
#include "events/watcher.hh"

namespace http
{

    std::optional<Connection> Dispatcher::create_connection
        (const EventRequest& er)
        {
            const Request& r = er.get_request();
            for (auto& h : r.headers_)
            {
                if(h.first == "host")
                {
                    for(auto& h1 : hosts_.list_vhost)      // hosts_ ->  set of vhosts
                    {
                        if(h1.server_name == h.second)
                        {
                            Connection c(h1, er.get_sock(), h1.port);
                        //    connections_.insert(c);
                            return c;
                        }
                    }
                }
            }
            return std::nullopt;
        }

    void Dispatcher::set_hosts(ServerConfig& hosts)
    {
        hosts_ = hosts;
    }
}
