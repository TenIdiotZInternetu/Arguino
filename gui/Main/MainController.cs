using System;
using System.Diagnostics;
using System.Threading;
using System.Threading.Tasks;
using TcpAdapter;

namespace gui;

public static class MainController {
    public static IMessageHandler Adapter { get; private set; } = null!;
    public static Stopwatch GlobalTimer { get; private set; } = null!;

    public static event Action AppInitializedEvent;
    
    public static async Task InitApp()
    {
        Console.WriteLine("I'm alive");
        GlobalTimer = Stopwatch.StartNew();
        
        var tcpClient = new TcpClient<TestMessageHandler>(8888);
        await tcpClient.ConnectAsync();
        Console.WriteLine("Connected!");
        Adapter = tcpClient.Handler;
        
        AppInitializedEvent?.Invoke();

        while (true) {
            await tcpClient.SendMessageAsync("R");
            await Task.Delay(10);
        }
    }
}