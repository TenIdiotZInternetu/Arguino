using System.Text;

namespace ComponentManagement.Graph;

public class ElectricalNode {
    public uint DrivingPins { get; private set; } = 0;
    public DigitalState State => DrivingPins > 0 ?
                                 DigitalState.High : DigitalState.Low;

    public bool IsHigh => State == DigitalState.High;
    public bool IsLow => State == DigitalState.Low;

    public event Action<ElectricalNode, DigitalState>? StateChangedEvent;

    private List<Pin> _pins = new();


    public void AddPin(Pin pin) {
        _pins.Add(pin);

        if (pin.IsDriving) {
            DrivingPins++;
        }
        
        pin.ConnectToNode(this);
        pin.DrivingChangedEvent += Propagate;
    }

    public void RemovePin(Pin pin) {
        _pins.Remove(pin);

        if (pin.IsDriving) {
            DrivingPins--;
        }

        pin.DrivingChangedEvent -= Propagate;
        pin.Disconnect();
        Propagate(pin, false);
    }

    private void Propagate(Pin sourcePin, bool beganDriving) {
        var prevState = State;

        if (beganDriving) {
            DrivingPins++;
        }
        else {
            DrivingPins--;
        }

        if (prevState != State) {
            StateChangedEvent?.Invoke(this, State);
        }
    }

    public override string ToString() {
        StringBuilder sb = new();
        sb.Append("[ ");
        sb.AppendJoin(", ", _pins);
        sb.Append(" ]");
        return sb.ToString();
    }
}