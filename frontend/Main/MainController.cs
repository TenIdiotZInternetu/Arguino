using System;
using System.Diagnostics;
using System.IO.MemoryMappedFiles;
using System.Linq;
using System.Threading.Tasks;
using Avalonia.Controls.ApplicationLifetimes;
using CommandLine;
using ComponentManagement;
using ComponentManagement.Components;
using ComponentManagement.Loaders;
using ComponentManagement.Scenes;
using Gui.ViewModels;
using Gui.Views;
using IpcAdapter;
using IpcAdapter.Encoders;
using Logger;
using SharedMemoryAdapter;
using TcpAdapter;

namespace Gui;

public static class MainController {
    public static CommandLineArguments Arguments { get; private set; } = null!;
    public static MainWindow MainWindow { get; private set; } = null!;
    public static Scene Scene { get; private set; } = null!;
    public static TcpMessageHandler Adapter { get; private set; } = null!;
    public static Stopwatch GlobalTimer { get; private set; } = null!;
    
    public static ILogger Logger { get; private set; } = null!;

    public static event Action? AppInitializedEvent;

    private static bool _isInitialized = false;
    
    public static void InitApp(IClassicDesktopStyleApplicationLifetime desktop) {
        if (_isInitialized) {
            throw new InvalidOperationException("Repeated call of InitApp");
        }
        
        if (desktop.MainWindow is MainWindow mainWindow) {
            MainWindow = mainWindow;
        } else {
            throw new UnreachableException("Only main window of type MainWindow is supported");
        }

        Arguments = Parser.Default.ParseArguments<CommandLineArguments>(desktop.Args).Value;
        GlobalTimer = Stopwatch.StartNew();
        InitLogger();
        Logger.LogInfo("Starting App initialization.");

        InitScene();
        InitArduino();
        
        _isInitialized = true;
        AppInitializedEvent?.Invoke();
        Logger.LogInfo("App initialization complete.");
    }

    private static void InitLogger() {
        var fileLogger = new FileLogger(Arguments.CircuitLogFile);
        fileLogger.Timer = GlobalTimer;
        fileLogger.Verbosity = Arguments.Verbosity;
        Logger = new CompositeLogger(fileLogger);
        ComponentManager.Logger = Logger;
    }

    private static void InitScene() {
        Scene = YamlSceneLoader.LoadScene(
            Arguments.ScenePath,
            Arguments.ComponentsPath
        );

        var canvas = new CircuitCanvas();
        foreach (var comp in Scene.ComponentsMap.Values) {
            canvas.Components.Add(comp);
            comp.OnInitialized();
        }
        
        MainWindow.CircuitCanvas.DataContext = canvas;
    }

    private static void InitArduino() {
        Arduino? arduino;
        
        if (Arguments.IpcType == IpcType.None) {
            Logger.LogInfo("IPC type not specified. Skipping connection to simulator..");
            return;
        }
        
        try {
            // TODO: This is probably a stupid way to do this
            arduino = Scene.ComponentsMap.First(c => c.Value.TypeName == nameof(Arduino)).Value as Arduino;
        }
        catch (InvalidOperationException) {
            Logger.LogWarning("Arduino not found in the scene. Skipping connection to simulator.");
            return;
        }

        
        var fileLogger = new FileLogger(Arguments.IpcLogFile, Arguments.Verbosity);
        fileLogger.Timer = GlobalTimer;
        var logger = new CompositeLogger(fileLogger);
        
        IEncoder encoder = new TextEncoder();
        IIpcAdapter? ipc = null;

        if (Arguments.IpcType == IpcType.Tcp) {
            var tcpClient = new TcpClient(Arguments.TcpPort);
            ipc = new TcpMessageHandler(tcpClient, encoder, logger);
        }
        if (Arguments.IpcType == IpcType.Shmem) {
            ipc = new TwoWayBuffer(Arguments.ShmemName, Arguments.ShmemSize, encoder, logger);
        }

        arduino?.ConnectToSimulator(ipc!);
    }
}