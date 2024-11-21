using System;
using System.Net;
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
        var tcp = new TcpConnection(IPAddress.Parse("127.0.0.1"), 2400);
        Console.WriteLine("TcpCreated");

        while (true) {
            tcp.ReadMessage();
        }
    }
}