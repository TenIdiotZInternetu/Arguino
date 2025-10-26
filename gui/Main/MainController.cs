using System;
using System.Threading;
using System.Threading.Tasks;
using TcpAdapter;

namespace gui;

public static class MainController {
    public static IMessageHandler Adapter { get; private set; } = null!;

    public static event Action AppInitializedEvent;
    
    public static async Task InitApp()
    {
        Console.WriteLine("I'm alive");
        
        var tcpClient = new TcpClient<TestMessageHandler>(8888);
        await tcpClient.ConnectAsync();
        Console.WriteLine("Connected!");
        Adapter = tcpClient.Handler;
        // Adapter.StateChangedEvent += TestRead;
        
        AppInitializedEvent?.Invoke();

        while (true) {
            await tcpClient.SendMessageAsync("R");
            await Task.Delay(1000);
        }
    }

    private static void TestRead(ArduinoState state) {
        Console.WriteLine($"Pin 3: {state.GetDigital(3)}");
    }
    
    private static void TestClient() {
        var client = new TestTcpClient(8888);
        
        while (!client.Connect()) {
            Console.WriteLine("Waiting for connection");
            Thread.Sleep(3000);
        }
        
        Console.WriteLine("Connected!");

        while (true) {
            client.WriteMessage("R>::<");
            string message = client.ReadMessage(">::<");
            Console.WriteLine(message);
            Thread.Sleep(1000);
        }
    }
}