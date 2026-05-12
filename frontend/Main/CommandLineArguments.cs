using System;
using CommandLine;
using CommandLine.Text;
using Logger;

namespace Gui;

public enum IpcType { None, Tcp, Shmem }

public class CommandLineArguments
{
    [Option('s', "scene", Default = "./scene.yaml", HelpText = "Path to the .yaml scene defintion")]
    public string ScenePath { get; set; } = "./scene.yaml";

    [Option('c', "components", Default = "./ComponentManagement/Components", HelpText = "Path to the directory of components definitions")]
    public string ComponentsPath { get; set; } = "./ComponentManagement/Components";

    [Option('i', "ipc", Default = IpcType.Shmem, HelpText = "Interprocess communication technology used for connection with the simulator")]
    public IpcType IpcType { get; set; } = IpcType.Shmem;

    [Option('p', "port", Default = 8888, HelpText = "Port of the core's TCP server (ipc=Tcp only)")]
    public int TcpPort { get; set; }

    [Option("shmem-name", Default = "Arguino-ipc", HelpText = "Name of the memory mapped file (ipc=Shmem only)")]
    public string ShmemName { get; set; } = "Arguino-ipc";

    [Option("shmem-size", Default = 1, HelpText = "Size of the memory mapped region per buffer in pages (ipc=Shmem only)")]
    public int ShmemSize { get; set; } = 1;

    [Option("log-ipc", Default = "./frontend_ipc.log", HelpText = "Path to the log file for interprocess messages")]
    public string IpcLogFile { get; set; } = "./frontend_ipc.log";

    [Option("log-circuit", Default = "./frontend.log", HelpText = "Path to the log file for general circuitry events")]
    public string CircuitLogFile { get; set; } = "./frontend.log";

    [Option('v', "verbosity", Default = LogLevel.Info, HelpText = "Verbosity of the log files")]
    public LogLevel Verbosity { get; set; } = LogLevel.Info;

    // Solution taken from Simon at https://stackoverflow.com/questions/42010512/how-to-view-commandlineparser-parsing-errors-in-visual-studio-2015
    public static CommandLineArguments? ParseArguments(string[]? args)
    {
        CommandLineArguments? options = null;

        var result = Parser.Default.ParseArguments<CommandLineArguments>(args);
        result
            .WithParsed(o => options = o)
            .WithNotParsed(_ =>
            {
                var helpText = HelpText.AutoBuild(result,
                    h => HelpText.DefaultParsingErrorsHandler(result, h),
                    e => e);
                Console.WriteLine(helpText);
            });

        return options;
    }
}