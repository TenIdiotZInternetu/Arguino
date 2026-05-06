using System;
using Avalonia;
using Avalonia.Controls.ApplicationLifetimes;
using Avalonia.Markup.Xaml;

namespace Gui.Views;

public partial class App : Application
{
    public override void Initialize() {
        AvaloniaXamlLoader.Load(this);
    }
    
    public override void OnFrameworkInitializationCompleted() {
        MainWindow mainWindow = new MainWindow();
        
        if (ApplicationLifetime is IClassicDesktopStyleApplicationLifetime desktop) {
            desktop.MainWindow = mainWindow;
            MainController.InitApp(desktop);
        } else {
            throw new NotImplementedException("Only Desktop style applications are supported");
        }

        base.OnFrameworkInitializationCompleted();
    }
}