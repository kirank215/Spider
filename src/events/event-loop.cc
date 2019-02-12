#include "event-loop.hh"

http::EventLoop::Eventloop()
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
    ev_io_start(loop, event);
}

void http::EventLoop::unregister_watcher(EventWatcher* event)
{
    ev_io_stop(loop, event);
}
