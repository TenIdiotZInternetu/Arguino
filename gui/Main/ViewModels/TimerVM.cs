using System;
using System.Diagnostics;
using Avalonia.Controls;
using Avalonia.Media;
using Avalonia.Threading;
using CommunityToolkit.Mvvm.ComponentModel;

namespace Gui.ViewModels;

public partial class TimerVM : ObservableObject {
    private const int REFRESH_RATE = 60;
    
    private readonly Stopwatch _stopwatch = Stopwatch.StartNew();
    private readonly DispatcherTimer _timer = new();
    
    [ObservableProperty] private string _elapsedTimeText = "00:00.000";

    public TimerVM() {
        _timer.Interval = TimeSpan.FromMilliseconds(1000.0 / REFRESH_RATE);
        _timer.Tick += UpdateText;
        _timer.Start();
    }

    public void UpdateText(object? sender, EventArgs e) {
        ElapsedTimeText = _stopwatch.Elapsed.ToString(@"mm\:ss\.fff");
    }
}