using System;
using System.Net;
using System.Threading;
using System.Threading.Tasks;
using Avalonia;
using Avalonia.Controls.ApplicationLifetimes;
using Avalonia.Markup.Xaml;
using gui.Controllers;

namespace gui.Views;

public partial class App : Application
{
    public override void Initialize() {
        AvaloniaXamlLoader.Load(this);
    }

    public override void OnFrameworkInitializationCompleted() {
        Console.WriteLine("I'm alive");
        Task.Run(TestClient);
        Task.Run(TestServer);
        
        if (ApplicationLifetime is IClassicDesktopStyleApplicationLifetime desktop) {
            desktop.MainWindow = new MainWindow();
        }

        base.OnFrameworkInitializationCompleted();
    }

    private void TestClient() {
        var client = new TcpVendee( 2400);
        
        while (!client.Connect()) {
            Console.WriteLine("Waiting for connection");
            Thread.Sleep(3000);
        }
        
        Console.WriteLine("Listening");

        while (true) {
            string message = client.ReadMessage(":<>:");
            Console.WriteLine(message);
        }
    }

    private void TestServer() {
        var server = new TcpServer(2401);

        while (true) {
            server.WriteMessage("gui Running :<>:");
            Thread.Sleep(2000);
        }
    }
}