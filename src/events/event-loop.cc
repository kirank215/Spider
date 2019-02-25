#include <iostream>
#include "event-loop.hh"

http::EventLoop::EventLoop()
{
    loop = EV_DEFAULT;
}

// Is this necessary ?
http::EventLoop::~EventLoop()
{
//    ev_loop_destroy(loop);
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
    exit(1);
}

static void timer_callback(struct ev_loop* loop, ev_timer* , int)
{
    std::cout << " TIMEOUT \n";
    ev_break(loop, EVBREAK_ONE);
}

void http::EventLoop::operator()() const
{
    // Timer
    ev_timer time;
    ev_timer_init (&time, timer_callback, 10., 0.);  // change 0. to 'x' to repeat loop after 'x' seconds
    ev_timer_start (loop, &time);

    ev_signal signal;
    ev_signal_init (&signal, signal_callback, SIGINT);
    this->register_sigint_watcher(&signal);
    ev_run(loop, 0);
}
