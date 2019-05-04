/**
 * \file vhost/dispatcher.hh
 * \brief Dispatcher declaration.
 */

#pragma once
#include <optional>
#include "vhost/connection.hh"
#include "events/watcher.hh"
#include "request/request.hh"
#include "vhost/vhost-static-file.hh"



namespace http
{
    /**
     * \class Dispatcher
     * \brief Instance in charge of dispatching requests between vhosts.
     */


//    int APM_add(std::map<std::string, int>, std::string);
//    int APM_sub(std::map<std::string, int>, std::string);

    class Dispatcher
    {
        public:
            Dispatcher() = default;
            Dispatcher(const Dispatcher&) = delete;
            Dispatcher& operator=(const Dispatcher&) = delete;
            Dispatcher(Dispatcher&&) = delete;
            Dispatcher& operator=(Dispatcher&&) = delete;

            std::optional<Connection> create_connection(const EventRequest&);
            void set_hosts(ServerConfig&);
            ServerConfig get_hosts();
            void insert_staticfile(shared_vhost vs);
            void respond(const Request&, const Connection& c);




        private:
            //** Dispatcher.
            // change to map of config and static file
            ServerConfig hosts_;
            std::vector<shared_vhost> vstatic_;
            //std::vector<Connection> connections_;

    };

    /**
     * \brief Service object.
     */
    extern Dispatcher dispatcher;
} // namespace http
