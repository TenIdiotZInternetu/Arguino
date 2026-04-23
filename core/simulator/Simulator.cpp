#include "Simulator.hpp"

namespace arguino::simulator {

Simulator* Simulator::s_instance;

void Simulator::init(std::function<void(const Event&)> eventCallback)
{
    s_instance = new Simulator();
    s_instance->f_eventCallback = eventCallback;
}

void Simulator::handle_event(Event event)
{
    auto& queue = s_instance->_eventQueue;
    queue.enqueue_local(event);

    do {
        queue.execute_next_event();
    } while (event.id() != queue.last_executed_event().id());

    s_instance->f_eventCallback(event);
}

void Simulator::handle_events()
{
    auto& queue = s_instance->_eventQueue;
    if (queue.is_empty()) return;

    auto lastLvt = queue.last_lvt();

    do {
        queue.execute_next_event();
    } while (lastLvt <= queue.last_executed_event().localVirtualTime);
}


};  // namespace arguino::simulator