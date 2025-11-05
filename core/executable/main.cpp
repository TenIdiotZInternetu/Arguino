#include <iostream>
#include <thread>

#include "upp_shrmem.hpp"

#include "ArguinoConnectionHandler.hpp"
#include "StateEncoder.hpp"
#include "TcpServer.hpp"

#include "sketch.cpp"

using encoder_t = StateEncoder;
using connection_handler_t = ArguinoConnectionHandler<encoder_t>;

void tcp_thread_func() 
{
    arguino::tcp::TcpServer<connection_handler_t> server(8888);
    server.launch();
}

void simulator_thread_func() 
{
    G_ARDUINO_STATE_PTR->init_timer();
    setup();
    while (true) {
        loop();
    }
}

void exec_thread_func(const std::string& cmd) 
{
    std::system(cmd.c_str());
}

template< std::convertible_to<std::string> ... SL>
std::thread start_exec_thread(SL &&... vl)
{
    std::string cmd;
    ((cmd += std::forward<SL>(vl), cmd += " "), ...);
    return std::thread(exec_thread_func, std::move(cmd));
}

ulibpp::shared_memory sm;
ulibpp::shared_memory_mapping smm;
std::unique_ptr<ArduinoState> my_ptr;

void map_shared_memory() 
{
    std::size_t size = sizeof(ArduinoState);
    smm.map(sm, 0, size);
    void* addr = smm.get_addr();
    G_ARDUINO_STATE_PTR = new (addr) ArduinoState();
}

void create_shared_memory(const std::string& shm_name) 
{
    std::size_t size = sizeof(ArduinoState);
    try {
        sm.open(shm_name, size, ulibpp::shared_memory::read | ulibpp::shared_memory::write);
    } 
    catch (...) 
    {
        sm.create(shm_name, size, ulibpp::shared_memory::read | ulibpp::shared_memory::write);
    }
    map_shared_memory();
}

void open_shared_memory(const std::string& shm_name)
{
    std::size_t size = sizeof(ArduinoState);
    sm.open(shm_name, size, ulibpp::shared_memory::read | ulibpp::shared_memory::write);
    map_shared_memory();
}

void map_my_memory()
{
	my_ptr = std::make_unique<ArduinoState>();
    G_ARDUINO_STATE_PTR = &*my_ptr;
}

std::string only_tcp_flag = "--only-tcp";
std::string only_simulator_flag = "--only-simulator";
std::string start_tcp_flag = "--start_tcp";
std::string single_process_flag = "--single-process";
std::string default_shrmem_name = "arguino_shm";

int main(int argc, char * * argv) 
{
    if (argc > 1 && argv[1] == single_process_flag)
    {
		map_my_memory();
        std::thread tcp_thread(tcp_thread_func);
        simulator_thread_func();
        tcp_thread.join();
    }
    else if (argc > 2 && argv[1] == only_tcp_flag)
    {
		std::string shrmem_name = argv[2];
		open_shared_memory(shrmem_name);
        tcp_thread_func();
	}
    else if (argc > 2 && argv[1] == only_simulator_flag)
    {
        std::string shrmem_name = argv[2];
        open_shared_memory(shrmem_name);
        simulator_thread_func();
    }
    else if (argc > 1 && argv[1] == start_tcp_flag)
    {
        std::string shrmem_name = default_shrmem_name;
        create_shared_memory(shrmem_name);
        std::thread simulator_thread = start_exec_thread(argv[0], only_simulator_flag, shrmem_name);
		tcp_thread_func();
        simulator_thread.join();
    }
    else
    {
        std::string shrmem_name = default_shrmem_name;
        create_shared_memory(shrmem_name);
        std::thread tcp_thread = start_exec_thread(argv[0], only_tcp_flag, shrmem_name);
        simulator_thread_func();
        tcp_thread.join();
    }
	return 0;
}
