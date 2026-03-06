#include "EventQueue.hpp"

namespace arguino::simulator {

void EventQueue::enqueue_local(Event event)
{
    event.localVirtualTime = _nextLvt;
    ++_nextLvt;
    _localEvents.push(event);
}

void EventQueue::enqueue_remote(Event event)
{
    _remoteEvents.push(event);
}

void EventQueue::execute_next_event()
{
    auto queueAhead = std::min(_localEvents, _remoteEvents, [](auto lq, auto rq) {
        return lq.front().localVirtualTime < rq.front().localVirtualTime;
    });

    Event processedEvent = queueAhead.front();
    queueAhead.pop();

    processedEvent.action();
    _nextLvt = std::min(_nextLvt, processedEvent.localVirtualTime);
    _processedEvents.push(processedEvent);
}

void EventQueue::clear()
{
    _localEvents = std::queue<Event>();
    _remoteEvents = std::queue<Event>();
    _processedEvents = std::queue<Event>();
    _nextLvt = 0;
}

void EventQueue::clear_events_before(size_t lvt)
{
    // TODO
}

void EventQueue::clear_events_after(size_t lvt) {}

void EventQueue::rollback_to(size_t lvt) {}

}  // namespace arguino::simulator
