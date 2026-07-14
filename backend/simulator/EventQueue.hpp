#ifndef ARGUINO_EVENTQUEUE_HPP
#define ARGUINO_EVENTQUEUE_HPP

#include <queue>

#include "Events.hpp"

namespace arguino::simulator {

class EventQueue {
   public:
    void enqueue_local(Event& event);
    void enqueue_remote(Event event);
    void execute_next_event();

    Event& last_executed_event() { return _lastExecuteEvent; }
    bool is_empty() const { return _localEvents.empty() && _remoteEvents.empty(); }
    int64_t next_lvt() const { return _nextLvt; }

   private:
    std::queue<Event> _localEvents;
    std::queue<Event> _remoteEvents;
    Event _lastExecuteEvent;

    int64_t _nextLvt;

    void clear();
    size_t earliest_queue_lvt(const std::queue<Event>& queue);
};

}  // namespace arguino::simulator

#endif