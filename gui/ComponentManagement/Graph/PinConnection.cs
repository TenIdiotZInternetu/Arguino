using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.NetworkInformation;
using System.Threading.Tasks;

namespace ComponentManagement.Graph;

public class PinConnection {
    public float Value { get; set; }
    private List<Pin> _pins = new();

    public void AddPin(Pin pin) {
        _pins.Add(pin);
        pin.VoltageChangedEvent += Propagate;
        pin.Component.OnPinConnected(pin);
    }

    public void RemovePin(Pin pin) {
        _pins.Remove(pin);
        pin.VoltageChangedEvent -= Propagate;
        pin.Component.OnPinDisconnected(pin, this);
    }

    private void Propagate(Pin sourcePin) {
        Value = sourcePin.Voltage;

        foreach (Pin pin in _pins.FindAll(p => p != sourcePin)) {
            pin.VoltageChangedEvent -= Propagate;
            pin.SetVoltage(Value);
            pin.VoltageChangedEvent += Propagate;
        }
    }
}