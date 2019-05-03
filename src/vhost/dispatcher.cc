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
                if(h.first == "Host")
                {
                    size_t colon = h.second.find(":");  // h.second = host:port
                    std::string port = "";        // in case of nc
                    std::string host = h.second.substr(1);
                    if (colon != std::string::npos)        // with curl
                    {
                        host = h.second.substr(1, colon-1);
                        port = h.second.substr(colon+1);
                    }
                    for(auto& conf : hosts_.list_vhost)      // hosts_ ->  list of vhostsconfigs
                    {
                        std::string dport = std::to_string(conf.port);  // port of vhost from serverconfig
                        if((conf.server_name == host || conf.ip == host) &&
                                (dport == port || port == ""))
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

    void Dispatcher::insert_staticfile(shared_vhost vs)
    {
        vstatic_.emplace_back(vs);
    }

    void Dispatcher::respond(const Request& req, const Connection& c)
    {
        auto vc = c.vc_;
        for(auto& vs : vstatic_)
        {
            if(vc == vs->conf_get())
                vs->respond(req, c, 0, 0); // FIXME remaining iterator for respond
        }
    }
    // add response function here that finds the static file of a vhost config and calls the respond function
}
