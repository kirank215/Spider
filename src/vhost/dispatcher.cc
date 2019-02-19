#include <memory>
#include "vhost/dispatcher.hh"
#include "events/watcher.hh"


std::optional<shared_con> http::Dispatcher::create_connection
                                            (const& EventRequest er)
{
    auto& r = er.req;
    for (auto& h : r.headers_)
    {
        if(h.first == "host")
        {
            for(auto& h1 : hosts_)      // hosts_ ->  set of vhosts
            {
                if(h1.server_name == h.second)
                {
                    Connection c(h1, er.s, h1.port);
                    auto cshared = make_shared<Connection>(c);
                    connections_.insert(cshared);
                    return cshared;
                }
            }
        }
    }
    return std::nullopt;
}

