using Avalonia;
using Avalonia.Controls;
using Avalonia.Markup.Xaml;
using Gui.ViewModels;

namespace Gui.Views.UserControls;

public partial class Timer : UserControl {
    public Timer() {
        InitializeComponent();
        DataContext = new TimerVM();
    }
}