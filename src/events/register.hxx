#pragma once

#include "register.hh"

namespace http
{
    template <typename EventWatcher, typename... Args>
    std::shared_ptr<EventWatcher>
    EventWatcherRegistry::register_ew(Args&&... args)
    {
        auto ew = std::make_shared<EventWatcher>(std::forward<Args>(args)...);
        events_[ew.get()] = ew;
        loop_.register_watcher(ew.get());
        return ew;
    }
} // namespace http
