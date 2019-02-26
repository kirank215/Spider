#pragma once

#include <optional>

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

    inline bool EventWatcherRegistry::unregister_ew(EventWatcher* ew)
    {
        auto to_remove = events_.find(ew);
        if (to_remove == events_.end())
            return false;
        events_.erase(to_remove);
        loop_.unregister_watcher(ew);
        return true;
    }

    inline std::optional<std::shared_ptr<EventWatcher>>
    EventWatcherRegistry::at(EventWatcher* ew)
    {
        auto res = events_.find(ew);
        if (res != events_.end())
            return res->second;
        return std::nullopt;
    }
} // namespace http
