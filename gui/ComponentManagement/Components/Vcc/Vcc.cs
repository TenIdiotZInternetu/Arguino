using ComponentManagement.Graph;

namespace ComponentManagement.Components;

public class Vcc : Component {

    // TODO: Make voltage adjustable parameter in config/in main window
    public const float VOLTS = 5;

    private Pin _pin;

    public Vcc(string definitionPath) : base(definitionPath) {
        _pin = GetPin(0)!;
        _pin.MakeWriteOnly();
        _pin.SetHigh();
    }
}