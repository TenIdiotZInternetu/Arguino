using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.NetworkInformation;
using System.Threading.Tasks;

namespace ComponentManagement.Graph;

public class ElectricalNode {
    public float Voltage { get; set; }
    private List<Pin> _pins = new();

    public void AddPin(Pin pin) {
        _pins.Add(pin);
        pin.VoltageSetEvent += Propagate;
        pin.ConnectToNode(this);
    }

    public void RemovePin(Pin pin) {
        _pins.Remove(pin);
        pin.Disconnect();
        pin.VoltageSetEvent -= Propagate;
    }

    private void Propagate(Pin sourcePin, float voltage) {
        Voltage = voltage;

        foreach (Pin pin in _pins.FindAll(p => p != sourcePin)) {
            pin.NotifyVoltageChange(Voltage);
        }
    }
}