using System;
using System.Threading;
using System.Threading.Tasks;
using TcpAdapter;

namespace gui;

public static class MainController
{
    public static async Task InitApp()
    {
        Console.WriteLine("I'm alive");
        
        var tcpClient = new TcpClient<TestMessageHandler>(8888);
        await tcpClient.ConnectAsync();
        Console.WriteLine("Connected!");
        tcpClient.Handler.StateChangedEvent += TestRead;

        while (true) {
            await tcpClient.SendMessageAsync("R");
            await Task.Delay(15);
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