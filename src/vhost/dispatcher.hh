/**
 * \file vhost/dispatcher.hh
 * \brief Dispatcher declaration.
 */

#pragma once
#include "connection.hh"


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

        void create_connection(Const& Request);


    private:
        //** Dispatcher.
        ServerConfig hosts_;
        std::vector<Connection> connections_;

    };

    /**
     * \brief Service object.
     */
    extern Dispatcher dispatcher;
} // namespace http
