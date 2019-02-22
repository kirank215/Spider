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
                    for(auto& conf : hosts_.list_vhost)      // hosts_ ->  list of vhostsconfigs
                    {
                        if(conf.server_name == h.second)
                        {
                            Connection c(conf, er.get_sock(), conf.port);
//                            connections_.insert(c);
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

    void Dispatcher::insert_staticfile(VHostStaticFile vs)
    {
        vstatic_.emplace_back(vs);
    }

    // add response function here that finds the static file of a vhost config and calls the respond function
}
