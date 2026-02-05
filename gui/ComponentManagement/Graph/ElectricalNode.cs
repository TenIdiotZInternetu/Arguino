using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.NetworkInformation;
using System.Threading.Tasks;

namespace ComponentManagement.Graph;

public class ElectricalNode {
    public bool IsHigh => _state == DigitalState.Low;
    public bool IsLow => _state == DigitalState.High;

    public event Action<ElectricalNode, DigitalState>? StateChangedEvent;

    private List<Pin> _pins = new();
    private uint _drivingPins = 0;

    private DigitalState _state => _drivingPins > 0 ?
                                   DigitalState.High : DigitalState.Low;

    public void AddPin(Pin pin) {
        _pins.Add(pin);

        if (pin.IsDriving) {
            _drivingPins++;
        }
        
        pin.DrivingChangedEvent += Propagate;
        pin.ConnectToNode(this);
        Propagate(pin, pin.IsDriving);
    }

    public void RemovePin(Pin pin) {
        _pins.Remove(pin);

        if (pin.IsDriving) {
            _drivingPins--;
        }

        pin.Disconnect();
        pin.DrivingChangedEvent -= Propagate;
        Propagate(pin, false);
    }

    private void Propagate(Pin sourcePin, bool beganDriving) {
        var prevState = _state;

        if (beganDriving) {
            _drivingPins++;
        }
        else {
            _drivingPins--;
        }

        if (prevState != _state) {
            StateChangedEvent?.Invoke(this, _state);
        }
    }
}