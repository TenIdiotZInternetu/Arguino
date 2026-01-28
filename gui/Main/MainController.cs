using System;
using System.Diagnostics;
using System.Threading.Tasks;
using TcpAdapter;

namespace Gui;

public static class MainController {
    public static TestMessageHandler Adapter { get; private set; } = null!;
    public static Stopwatch GlobalTimer { get; private set; } = null!;

    public static event Action? AppInitializedEvent;
    
    public static async Task InitApp()
    {
        Console.WriteLine("I'm alive");
        GlobalTimer = Stopwatch.StartNew();
        
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