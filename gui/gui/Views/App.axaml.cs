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
        Task.Run(Test);
        
        if (ApplicationLifetime is IClassicDesktopStyleApplicationLifetime desktop) {
            desktop.MainWindow = new MainWindow();
        }

        base.OnFrameworkInitializationCompleted();
    }

    private void Test() {
        var tcp = new TcpVendee( 2400);
        
        while (!tcp.Connect()) {
            Console.WriteLine("Waiting for connection");
            Thread.Sleep(3000);
        }
        
        Console.WriteLine("Listening");

        while (true) {
            string message = tcp.ReadMessage(":<>:");
            Console.WriteLine(message);
        }
    }
}