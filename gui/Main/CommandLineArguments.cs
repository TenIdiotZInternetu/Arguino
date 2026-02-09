using CommandLine;

namespace Gui;

public class CommandLineArguments {
    [Option('s', "scene", Default = "./scene.yaml", HelpText = "Path to the .yaml scene defintion")]
    public string ScenePath { get; set; } = "./scene.yaml";

    [Option('c', "components", Default = "./ComponentManagement/Components", HelpText = "Path to the directory of components definitions")]
    public string ComponentsPath { get; set; } = "./ComponentManagement/Components";

    [Option('p', "--port", Default = 8888, HelpText = "Port of the core's TCP server")]
    public int TcpPort { get; set; }

    [Option("no-tcp", Default = false, HelpText = "Turn off connection to TCP server at initialization")]
    public bool NoTcp { get; set; }
    
    [Option("log-tcp", Default = "./frontend_tcp.log", HelpText = "Path to the log file for TCP messages")]
    public string TcpLogFile { get; set; } =  "./frontend_tcp.log";
    
    [Option("log-circuit", Default = "./frontend.log", HelpText = "Path to the log file for general circuitry events")]
    public string CircuitLogFile { get; set; } =  "./frontend.log";
}
        