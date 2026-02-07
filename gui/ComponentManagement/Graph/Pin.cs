namespace ComponentManagement.Graph;

public class Pin {
    public uint Id { get; init; }
    public string? Name { get; init; }

    public bool IsDriving { get; private set; }
    public DigitalState State => (IsDriving || (_node?.IsHigh ?? false)) ?
                                 DigitalState.High : DigitalState.Low;

    public bool IsHigh => State == DigitalState.High;
    public bool IsLow => State == DigitalState.Low;

    public bool IsWriteOnly => _mode == PinMode.WriteOnly;
    public bool IsReadOnly => _mode == PinMode.ReadOnly;

    public bool IsConnected => _node != null;
    
    public event Action<Pin, bool>? DrivingChangedEvent;
    public event Action<Pin, DigitalState>? StateChangedEvent;

    public event Action<Pin>? PinConnectedEvent;
    public event Action<Pin>? PinDisconnectedEvent;
    
    private Component _component;
    private ElectricalNode? _node;
    
    private enum PinMode { General, ReadOnly, WriteOnly }
    private PinMode _mode = PinMode.General;

    
    public void MakeReadOnly() => _mode = PinMode.ReadOnly;
    public void MakeWriteOnly() => _mode = PinMode.WriteOnly;
    public void MakeGeneral() => _mode = PinMode.General;

    public void SetHigh() => SetValue(true);
    public void SetLow() => SetValue(false);
    
    public void SetValue(bool value) {
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

    public Pin(Component component, uint id, string? name = null) {
        _component = component;
        Id = id;
        Name = name;
    }

    public void ConnectToNode(ElectricalNode node) {
        Disconnect();
        _node = node;

        if (!IsWriteOnly) {
            _node.StateChangedEvent += NotifyStateChange;
        }
        
        NotifyStateChange(_node, _node.State);
        PinConnectedEvent?.Invoke(this);
    }

    public void Disconnect() {
        if (_node == null) return;

        _node.StateChangedEvent -= NotifyStateChange;
        _node = null;
        NotifyStateChange(null, State);
        PinDisconnectedEvent?.Invoke(this);
    }


    private void NotifyStateChange(ElectricalNode? _, DigitalState nodeState) {
        if (_mode == PinMode.WriteOnly) {
            // TODO: Log misuse
            return;
        }

        if (IsDriving) return;
        StateChangedEvent?.Invoke(this, State);
    }

    public override string ToString() => _component.Name + "." + (Name ?? Id.ToString());
}