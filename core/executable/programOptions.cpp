#include <boost/program_options.hpp>
#include <iostream>

namespace po = boost::program_options;

struct ProgramOptions {
    std::string SimulatorLogPath;

    // TCP implementation
    std::string TcpLogPath;
    int TcpPort;

    // Shared memory implementation
    std::string ShmemName;
    int ShmemSizePages;
};

static po::options_description create_common_options()
{
    po::options_description description("Common options");

    description.add_options()                                       //
        ("help,h",                                                  //
            "Print this help message")                              //
        ("log-simulator",                                           //
            po::value<std::string>()->default_value("./core.log"),  //
            "Path to the log file for simulator events");           //

    return description;
}

static po::options_description create_tcp_options()
{
    po::options_description description("TCP options");

    description.add_options()                                           //
        ("port,p",                                                      //
            po::value<int>()->default_value(8888),                      //
            "Port on which to launch TCP server")                       //
        ("log-tcp",                                                     //
            po::value<std::string>()->default_value("./core_tcp.log"),  //
            "Path to the log file for TCP events");                     //

    return description;
}

static po::options_description create_shmem_options()
{
    po::options_description description("Shared memory options");

    description.add_options()                                                       //
        ("shmem-name,n",                                                            //
            po::value<std::string>()->default_value("Arguino-ipc"),                 //
            "Name of the shared memory segment")                                    //
        ("shmem-size,s",                                                            //
            po::value<int>()->default_value(1024),                                  //
            "Size of each of the write and read shared memory segments in pages");  //

    return description;
}

ProgramOptions parse_arguments(int argc, char** argv)
{
    po::options_description description("Command line options");
    description.add(create_common_options());

#ifdef ARGUINO_TCP
    description.add(create_tcp_options());
#endif
#ifdef ARGUINO_SHARED_MEMORY
    description.add(create_shmem_options());
#endif

    po::variables_map variables;
    po::store(po::parse_command_line(argc, argv, description), variables);
    po::notify(variables);

    if (variables.count("help")) {
        std::cout << description << std::endl;
        exit(1);
    }

    ProgramOptions options;
    options.SimulatorLogPath = variables["log-simulator"].as<std::string>();

#ifdef ARGUINO_TCP
    options.TcpPort = variables["port"].as<int>();
    options.TcpLogPath = variables["log-tcp"].as<std::string>();
#endif
#ifdef ARGUINO_SHARED_MEMORY
    options.ShmemName = variables["shmem-name"].as<std::string>();
    options.ShmemSizePages = variables["shmem-size"].as<int>();
#endif

    return options;
}