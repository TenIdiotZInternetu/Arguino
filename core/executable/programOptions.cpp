#include <boost/program_options.hpp>

namespace po = boost::program_options;

struct ProgramOptions {
    std::string SimulatorLogPath;

    // TCP implementation
    std::string TcpLogPath;
    int TcpPort;
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

ProgramOptions parse_arguments(int argc, char** argv)
{
    po::options_description description("Command line options");
    description.add(create_common_options());

    // #if TCP
    description.add(create_tcp_options());

    po::variables_map variables;
    po::store(po::parse_command_line(argc, argv, description), variables);
    po::notify(variables);

    if (variables.contains("help")) {
        std::cout << description << std::endl;
        exit(1);
    }

    ProgramOptions options;
    options.SimulatorLogPath = variables["log-simulator"].as<std::string>();

    // #if TCP
    options.TcpPort = variables["port"].as<int>();
    options.TcpLogPath = variables["log-tcp"].as<std::string>();

    return options;
}