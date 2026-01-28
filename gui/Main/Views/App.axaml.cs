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
        }

        base.OnFrameworkInitializationCompleted();
        MainController.InitApp(mainWindow);
    }
}