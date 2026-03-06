#ifndef ARGUINO_EVENTQUEUE_HPP
#define ARGUINO_EVENTQUEUE_HPP

#include <queue>

#include "Events.hpp"

namespace arguino::simulator {

class EventQueue {
   public:
   private:
    std::queue<Event> _localEvents;
    std::queue<Event> _remoteEvents;
    std::queue<Event> _processedEvents;
};

}  // namespace arguino::simulator

#endif