using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection.Metadata;
using System.Threading.Tasks;
using ComponentManagement.Graph;

namespace ComponentManagement.Components;

public class Vcc : Component {

    // TODO: Make voltage adjustable parameter in config/in main window
    public const float VOLTS = 5;
    private const uint PIN = 0;

    public Vcc(string definitionPath) : base(definitionPath) {
        GetPin(PIN).MakeWriteOnly();
    }

    public override void OnPinConnected(Pin pin) {
        pin.SetVoltage(VOLTS);
    }

    public override void OnPinDisconnected(Pin pin) {
        pin.SetVoltage(Pin.GND_VOLTAGE);
    }
}