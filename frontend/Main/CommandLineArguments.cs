using CommandLine;
using Logger;

namespace Gui;

public enum IpcType { None, Tcp, Shmem }

public class CommandLineArguments {
    [Option('s', "scene", Default = "./scene.yaml", HelpText = "Path to the .yaml scene defintion")]
    public string ScenePath { get; set; } = "./scene.yaml";

    [Option('c', "components", Default = "./ComponentManagement/Components", HelpText = "Path to the directory of components definitions")]
    public string ComponentsPath { get; set; } = "./ComponentManagement/Components";

    [Option('i', "ipc", Default = IpcType.None, HelpText = "Which interprocess communication technology should be used")]
    public IpcType IpcType { get; set; } = IpcType.None;
    
    [Option('p', "port", Default = 8888, HelpText = "Port of the core's TCP server (ipc=Tcp only)")]
    public int TcpPort { get; set; }
    
    [Option("shmem-name", Default = "", HelpText = "Named of the memory mapped file (ipc=Shmem only)")]
    public string ShmemName { get; set; }

    [Option("shmem-size", Default = 1, HelpText = "Size of the memory mapped region per buffer in pages (ipc=Shmem only)")]
    public int ShmemSize { get; set; } = 1;

    [Option("log-ipc", Default = "./frontend_tcp.log", HelpText = "Path to the log file for interprocess messages")]
    public string IpcLogFile { get; set; } = "./frontend_ipc.log";

    [Option("log-circuit", Default = "./frontend.log", HelpText = "Path to the log file for general circuitry events")]
    public string CircuitLogFile { get; set; } = "./frontend.log";

    [Option('v', "verbosity", Default = LogLevel.Info, HelpText = "Verbosity of the log files")]
    public LogLevel Verbosity { get; set; } = LogLevel.Info;
}