#include "Simulator.hpp"

namespace arguino::simulator {

Simulator* Simulator::s_instance;

void Simulator::init(std::function<void(const Event&)> eventCallback, logger_ptr logger)
{
    s_instance = new Simulator();
    s_instance->f_eventCallback = eventCallback;
    s_instance->_logger = logger == nullptr  //
                            ? std::make_shared<logger::DummyLogger>()
                            : logger;
}

void Simulator::handle_event(Event event)
{
    auto& queue = s_instance->_eventQueue;
    queue.enqueue_local(event);

    handle_events();

    s_instance->f_eventCallback(event);
}

void Simulator::handle_events()
{
    auto& queue = s_instance->_eventQueue;
    if (queue.is_empty()) return;

    do {
        queue.execute_next_event();
    } while (queue.next_lvt() < queue.last_executed_event().localVirtualTime);
}


};  // namespace arguino::simulator