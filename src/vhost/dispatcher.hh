/**
 * \file vhost/dispatcher.hh
 * \brief Dispatcher declaration.
 */

#pragma once
#include <optional>
#include "vhost/connection.hh"
#include "events/watcher.hh"
#include "request/request.hh"



namespace http
{
    /**
     * \class Dispatcher
     * \brief Instance in charge of dispatching requests between vhosts.
     */
    class Dispatcher
    {
        public:
            Dispatcher() = default;
            Dispatcher(const Dispatcher&) = delete;
            Dispatcher& operator=(const Dispatcher&) = delete;
            Dispatcher(Dispatcher&&) = delete;
            Dispatcher& operator=(Dispatcher&&) = delete;

            std::optional<shared_con> create_connection(const EventRequest&);


        private:
            //** Dispatcher.
            ServerConfig hosts_;
            std::vector<shared_con> connections_;

    };

    /**
     * \brief Service object.
     */
    extern Dispatcher dispatcher;
} // namespace http
