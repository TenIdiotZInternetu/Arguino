using System;
using System.Diagnostics;
using System.Linq;
using System.Numerics;
using System.Threading.Tasks;
using Avalonia.Controls.ApplicationLifetimes;
using CommandLine;
using ComponentManagement.Components;
using ComponentManagement.Loaders;
using Gui.ViewModels;
using Gui.Views;
using TcpAdapter;

namespace Gui;

public static class MainController {
    public static CommandLineArguments Arguments { get; private set; } = null!;
    public static MainWindow MainWindow { get; private set; } = null!;
    public static TestMessageHandler Adapter { get; private set; } = null!;
    public static Stopwatch GlobalTimer { get; private set; } = null!;

    public static event Action? AppInitializedEvent;

    private static bool _isInitialized = false;
    
    public static void InitApp(IClassicDesktopStyleApplicationLifetime desktop)
    {
        if (_isInitialized) {
            throw new InvalidOperationException("Repeated call of InitApp");
        }

        Console.WriteLine("I'm alive");

        if (desktop.MainWindow is MainWindow mainWindow) {
            MainWindow = mainWindow;
        } else {
            throw new UnreachableException("Only main window of type MainWindow is supported");
        }

        Arguments = Parser.Default.ParseArguments<CommandLineArguments>(desktop.Args).Value;

        InitCanvas();
        
        if (!Arguments.NoTcp) {
            Task.Run(TempTcpTest);
        }
        
        GlobalTimer = Stopwatch.StartNew();
        _isInitialized = true;
        AppInitializedEvent?.Invoke();
    }

    private static void InitCanvas() {
        var scene = YamlSceneLoader.LoadScene(
            Arguments.ScenePath,
            Arguments.ComponentsPath
        );

        var canvas = new CircuitCanvas();
        foreach (var comp in scene.Components) {
            canvas.Components.Add(comp);
        }
        
        MainWindow.CircuitCanvas.DataContext = canvas;
    }

    private static async void TempTcpTest() {
        var tcpClient = new TcpClient<TestMessageHandler>(Arguments.TcpPort);
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
            state.SetDigital(8, !state.GetDigital(8));
            await Task.Delay(3000);
        }
    }

    private static void ReadTest(string message) {
        // Console.WriteLine(message);
    }
}