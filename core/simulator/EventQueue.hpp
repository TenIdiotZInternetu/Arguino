#ifndef ARGUINO_EVENTQUEUE_HPP
#define ARGUINO_EVENTQUEUE_HPP

#include <queue>

#include "Events.hpp"

namespace arguino::simulator {

class EventQueue {
   public:
    void enqueue_local(Event event);
    void enqueue_remote(Event event);
    void execute_next_event();

   private:
    std::queue<Event> _localEvents;
    std::queue<Event> _remoteEvents;
    std::queue<Event> _processedEvents;

    size_t _nextLvt;
    size_t _maxGvtFallback;

    void clear();
    void clear_events_before(size_t lvt);
    void clear_events_after(size_t lvt);
    void rollback_to(size_t lvt);
};

}  // namespace arguino::simulator

#endif