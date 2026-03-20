#include "Simulator.hpp"

namespace arguino::simulator {

Simulator* Simulator::s_instance;

void Simulator::init()
{
    s_instance = new Simulator();
}

void Simulator::handle_event(Event event)
{
    auto& queue = s_instance->_eventQueue;
    queue.enqueue_local(event);

    do {
        queue.execute_next_event();
    } while (event.id() != queue.last_executed_event().id());

    s_instance->f_ipcPostEvent(event);
}


};  // namespace arguino::simulator