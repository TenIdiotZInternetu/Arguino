using Avalonia.Controls;

namespace Gui.Views.UserControls;

public partial class Timer : UserControl {
    public Timer() {
        InitializeComponent();
        DataContext = new ViewModels.Timer();
    }
}