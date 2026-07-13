using System;
using System.Diagnostics;
using System.Linq;
using System.Threading.Tasks;
using Avalonia.Controls.ApplicationLifetimes;
using ComponentManagement;
using ComponentManagement.Components;
using ComponentManagement.Factory;
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
    public static TcpMessageHandler Adapter { get; private set; } = null!; // TODO - Remove
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

        if (CommandLineArguments.ParseArguments(desktop.Args) is CommandLineArguments args) {
            Arguments = args;
        }
        else {
            throw new ArgumentException("Command line errors");
        }
        
        GlobalTimer = Stopwatch.StartNew();
        Logger = InitLogger(Arguments.CircuitLogFile, Arguments.Verbosity);
        ComponentManager.Logger = Logger;
        Logger.LogInfo("Starting App initialization.");

        InitScene();
        InitArduino();
        
        _isInitialized = true;
        AppInitializedEvent?.Invoke();
        Logger.LogInfo("App initialization complete.");
    }

    private static ILogger InitLogger(string path, LogLevel verbosity) {
        var fileLogger = new FileLogger(path, verbosity);
        fileLogger.Timer = GlobalTimer;
        return new CompositeLogger(fileLogger);
    }

    private static void InitScene() {
        var SceneFactory = new SceneFactory(Arguments.ScenePath, Arguments.ComponentsPath);
        Scene = SceneFactory.LoadResources();

        var canvas = new CircuitCanvas();
        foreach (var comp in Scene.ComponentsMap.Values) {
            canvas.Components.Add(comp);
        }
        
        MainWindow.CircuitCanvas.DataContext = canvas;
    }

    // TODO - move to Arduino component
    // TODO - IPC arguments through ExtraProps in scene
    private static void InitArduino() {
        Arduino? arduino;
        
        if (Arguments.IpcType == IpcType.None) {
            Logger.LogInfo("IPC type not specified. Skipping connection to simulator..");
            return;
        }
        
        try {
            arduino = Scene.ComponentsMap.First(c => c.Value.TypeName == nameof(Arduino)).Value as Arduino;
        }
        catch (InvalidOperationException) {
            Logger.LogWarning("Arduino not found in the scene. Skipping connection to simulator.");
            return;
        }
        
        ILogger ipcLogger = InitLogger(Arguments.IpcLogFile, Arguments.Verbosity);
        IEncoder encoder = new TextEncoder();
        IIpcAdapter? ipc = null;

        try {
            if (Arguments.IpcType == IpcType.Tcp) {
                var tcpClient = new TcpClient(Arguments.TcpPort);
                ipc = new TcpMessageHandler(tcpClient, encoder, ipcLogger);
            }

            if (Arguments.IpcType == IpcType.Shmem) {
                ipc = new TwoWayBuffer(Arguments.ShmemName, Arguments.ShmemSize, encoder, ipcLogger);
            }

            Task.Run(ipc!.ConnectAsync);
            arduino?.ConnectToSimulator(ipc);
        }
        catch (Exception e) {
            ipcLogger.LogError("Error during IPC initialization: " + e.Message);
        }
    }
}