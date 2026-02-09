using System;
using System.Diagnostics;
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
using Logger;
using TcpAdapter;

namespace Gui;

public static class MainController {
    public static CommandLineArguments Arguments { get; private set; } = null!;
    public static MainWindow MainWindow { get; private set; } = null!;
    public static Scene Scene { get; private set; } = null!;
    public static MessageHandler Adapter { get; private set; } = null!;
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
        Logger.Log(new InfoMessage("Starting App initialization."));

        InitScene();
        InitArduino();
        
        _isInitialized = true;
        AppInitializedEvent?.Invoke();
        Logger.Log(new InfoMessage("App initialization complete."));
    }

    private static void InitLogger() {
        var fileLogger = new FileLogger(Arguments.CircuitLogFile);
        fileLogger.Timer = GlobalTimer;
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
        
        try {
            // TODO: This is probably a stupid way to do this
            arduino = Scene.ComponentsMap.First(c => c.Value.TypeName == nameof(Arduino)).Value as Arduino;
        }
        catch (InvalidOperationException) {
            Logger.Log(new WarningMessage("Arduino not found in the scene. Skipping TCP initialization."));
            return;
        }

        if (Arguments.NoTcp) {
            Logger.Log(new InfoMessage("--no-tcp flag is set. Skipping TCP initialization."));
            return;
        }
        
        var fileLogger = new FileLogger(Arguments.TcpLogFile);
        fileLogger.Timer = GlobalTimer;

        var tcpClient = new TcpClient(Arguments.TcpPort)
            .SetLogger(new CompositeLogger(fileLogger));
        
        arduino?.ConnectToSimulator(tcpClient);
    }

    private static async void TempTcpTest() {
        var tcpClient = new TcpClient(Arguments.TcpPort);
        await tcpClient.ConnectAsync();
        Console.WriteLine("Connected!");
        Adapter = tcpClient.Handler;
        Adapter.ReadEvent += ReadTest;


        _ = Task.Run(TestWrite);

        while (true) {
            await tcpClient.SendMessageAsync("R");
            await Task.Delay(10);
        }
    }

    private static async Task TestWrite() {
        ArduinoState state = new ArduinoState();
        while (true) {
            Adapter.SendWriteMessage(state);
            // state.SetDigital(8, !state.GetDigital(8));
            await Task.Delay(3000);
        }
    }

    private static void ReadTest(string message) {
        // Console.WriteLine(message);
    }
}