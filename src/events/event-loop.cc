#include <iostream>
#include "event-loop.hh"

http::EventLoop::EventLoop()
{
    loop = EV_DEFAULT;
}

// Is this necessary ?
http::EventLoop::~EventLoop()
{
    ev_loop_destroy(loop);
}

// ADD ERROR HANDLING ?
void http::EventLoop::register_watcher(EventWatcher* event)
{
    ev_io_start(loop, &event->watcher_get());
}

void http::EventLoop::unregister_watcher(EventWatcher* event)
{
    ev_io_stop(loop, &event->watcher_get());
}

void http::EventLoop::register_sigint_watcher(ev_signal* sig) const
{
    ev_signal_start(loop, sig);
}

static void signal_callback(struct ev_loop* loop, ev_signal* , int)
{
    std::cerr << "\nSIGINT captured\n";
    ev_break(loop, EVBREAK_ALL);
}

void http::EventLoop::operator()() const
{
    ev_signal signal;
    ev_signal_init (&signal, signal_callback, SIGINT);
    this->register_sigint_watcher(&signal);
    ev_run(loop, 0);
}
