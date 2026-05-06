#include "EventQueue.hpp"

#include "Simulator.hpp"

namespace arguino::simulator {

void EventQueue::enqueue_local(Event& event)
{
    event.localVirtualTime = _nextLvt;
    ++_nextLvt;
    _localEvents.push(event);
    _lastLvt = std::max(_lastLvt, event.localVirtualTime);

    Simulator::log_debug("Enqueued local event " + event.to_string());
}

void EventQueue::enqueue_remote(Event event)
{
    _remoteEvents.push(event);
    _lastLvt = std::max(_lastLvt, event.localVirtualTime);
    Simulator::log_debug("Enqueued remote event " + event.to_string());
}

void EventQueue::execute_next_event()
{
    auto& queueAhead = (earliest_queue_lvt(_localEvents) < earliest_queue_lvt(_remoteEvents))
        ? _localEvents
        : _remoteEvents;

    Event processedEvent = queueAhead.front();
    queueAhead.pop();

    processedEvent.action();
    _nextLvt = std::max(_nextLvt, processedEvent.localVirtualTime + 1);
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

size_t EventQueue::earliest_queue_lvt(const std::queue<Event>& queue)
{
    if (queue.empty()) {
        return SIZE_MAX;
    }

    return queue.front().localVirtualTime;
}

}  // namespace arguino::simulator
