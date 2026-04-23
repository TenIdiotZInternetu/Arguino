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

    Event& last_executed_event() { return _processedEvents.back(); }
    bool is_empty() const { return _localEvents.empty() && _remoteEvents.empty(); }
    int64_t next_lvt() const { return _nextLvt; }
    int64_t last_lvt() const { return _lastLvt; }

   private:
    std::queue<Event> _localEvents;
    std::queue<Event> _remoteEvents;
    std::queue<Event> _processedEvents;

    int64_t _nextLvt;
    int64_t _lastLvt;
    int64_t _maxGvtFallback;

    void clear();
    void clear_events_before(size_t lvt);
    void clear_events_after(size_t lvt);
    void rollback_to(size_t lvt);


    size_t earliest_queue_lvt(const std::queue<Event>& queue);
};

}  // namespace arguino::simulator

#endif