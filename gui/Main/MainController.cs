using System;
using System.Diagnostics;
using System.Numerics;
using System.Threading.Tasks;
using ComponentManagement.Components;
using Gui.ViewModels;
using Gui.Views;
using TcpAdapter;

namespace Gui;

public static class MainController {
    public static TestMessageHandler Adapter { get; private set; } = null!;
    public static Stopwatch GlobalTimer { get; private set; } = null!;

    public static event Action? AppInitializedEvent;
    
    public static async Task InitApp(MainWindow mainWindow)
    {
        Console.WriteLine("I'm alive");
        GlobalTimer = Stopwatch.StartNew();

        var canvas = new CircuitCanvas();
        string ledPath = "/home/touster/Kodiky/arguino/gui/ComponentManagement/Components/Led";
        var led1 = new Led(ledPath);
        var led2 = new Led(ledPath);
        led2.Transform.Position = 200 * Vector2.One;
        var led3 = new Led(ledPath);
        led3.Transform.PositionX = 400;
        
        canvas.Components.Add(led1);
        canvas.Components.Add(led2);
        canvas.Components.Add(led3);
        mainWindow.CircuitCanvas.DataContext = canvas; 
        
        
        
        var tcpClient = new TcpClient<TestMessageHandler>(8888);
        await tcpClient.ConnectAsync();
        Console.WriteLine("Connected!");
        Adapter = tcpClient.Handler;
        Adapter.ReadEvent += ReadTest;
        
        

        AppInitializedEvent?.Invoke();

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