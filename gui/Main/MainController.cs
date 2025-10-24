using System;
using System.Threading;
using System.Threading.Tasks;
using TcpAdapter;

namespace gui;

public static class MainController
{
    public static void InitApp()
    {
        Console.WriteLine("I'm alive");
        Task.Run(TestClient);
    }
    
    private static void TestClient() {
        var client = new TcpClient(8888);
        
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