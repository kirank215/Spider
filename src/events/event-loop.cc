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

void http::EventLoop::operator()() const
{
    ev_run(loop, 0);
}
