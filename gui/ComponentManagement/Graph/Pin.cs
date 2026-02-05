namespace ComponentManagement.Graph;

public class Pin {
    public uint Id { get; init; }
    public string? Name { get; init; }

    public bool IsDriving { get; private set; }
    public bool IsHigh => _state == DigitalState.High;
    public bool IsLow => _state == DigitalState.Low;

    public bool IsConnected => _node != null;
    
    public event Action<Pin, bool>? DrivingChangedEvent;
    public event Action<Pin, DigitalState>? StateChangedEvent;

    public event Action<Pin>? PinConnectedEvent;
    public event Action<Pin>? PinDisconnectedEvent;
    
    private Component _component;
    private ElectricalNode? _node;
    
    private enum PinMode { General, ReadOnly, WriteOnly }
    private PinMode _mode = PinMode.General;

    private DigitalState _state => (IsDriving || (_node?.IsHigh ?? false)) ?
                                   DigitalState.High : DigitalState.Low;
    
    public void MakeReadOnly() => _mode = PinMode.ReadOnly;
    public void MakeWriteOnly() => _mode = PinMode.WriteOnly;
    public void MakeGeneral() => _mode = PinMode.General;

    public void SetHigh() => SetDriving(true);
    public void SetLow() => SetDriving(false);

    public Pin(Component component, uint id, string? name = null) {
        _component = component;
        Id = id;
        Name = name;
    }

    public void NotifyStateChange() {
        if (_mode == PinMode.WriteOnly) {
            // TODO: Log misuse
            return;
        }

        StateChangedEvent?.Invoke(this, _state);
    }

    public void ConnectToNode(ElectricalNode node) {
        _node = node;
        PinConnectedEvent?.Invoke(this);
    }

    public void Disconnect() {
        _node = null;
        PinDisconnectedEvent?.Invoke(this);
    }

    private void SetDriving(bool value) {
        if (_mode == PinMode.ReadOnly) {
            // TODO: Log misuse
            return;
        }
        
        bool wasDriving = IsDriving;
        IsDriving = value;

        if (wasDriving != IsDriving) {
            DrivingChangedEvent?.Invoke(this, value);
        }
    }
}