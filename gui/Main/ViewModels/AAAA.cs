using System;
using CommunityToolkit.Mvvm.ComponentModel;
using TcpAdapter;

namespace gui.ViewModels;

public partial class TempLed : ObservableObject {
    private const string LED_ON_CSS = "#ON: { visibility: visible; }";
    private const string LED_OFF_CSS = "#ON: { visibility: hidden; }";
    [ObservableProperty] private string _ledCss;

    public TempLed() {
        _ledCss = LED_OFF_CSS;
        // MainController.Adapter.StateChangedEvent += OnStateChanged;
    }

    private void OnStateChanged(ArduinoState state) {
        Console.WriteLine(state.GetDigital(3));
    }
}