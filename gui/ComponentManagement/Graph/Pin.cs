using Logger;

namespace ComponentManagement.Graph;

public class Pin {
    public uint Id { get; init; }
    public string? Name { get; init; }

    public bool IsDriving { get; private set; }
    public DigitalState State => (IsDriving || (_node?.IsHigh ?? false)) ?
                                 DigitalState.High : DigitalState.Low;

    public bool IsHigh => State == DigitalState.High;
    public bool IsLow => State == DigitalState.Low;

    public bool IsWriteOnly => _mode == Mode.WriteOnly;
    public bool IsReadOnly => _mode == Mode.ReadOnly;

    public bool IsConnected => _node != null;
    
    public event Action<Pin, bool>? DrivingChangedEvent;
    public event Action<Pin, DigitalState>? StateChangedEvent;

    public event Action<Pin>? PinConnectedEvent;
    public event Action<Pin>? PinDisconnectedEvent;
    
    private Component _component;
    private ElectricalNode? _node;
    
    public enum Mode { General, ReadOnly, WriteOnly }
    private Mode _mode = Mode.General;

    public Pin(Component component, uint id, string? name = null) {
        _component = component;
        Id = id;
        Name = name;
    }
    
    public void MakeReadOnly() => _mode = Mode.ReadOnly;
    public void MakeWriteOnly() => _mode = Mode.WriteOnly;
    public void MakeGeneral() => _mode = Mode.General;
    
    public void SetHigh() => SetValue(true);
    public void SetLow() => SetValue(false);

    public void SetValue(DigitalState value) => SetValue(value == DigitalState.High);
    
    public void SetValue(bool value) {
        if (_mode == Mode.ReadOnly) {
            ComponentManager.Logger?.Log(new WarningMessage($"Attempting to set value to a ReadOnly pin {this}"));
            return;
        }
        
        bool wasDriving = IsDriving;
        IsDriving = value;
        if (wasDriving == IsDriving) return;

        ComponentManager.Logger?.Log(new DebugMessage($"Changed the driving value of pin {this} to {value}"));
        DrivingChangedEvent?.Invoke(this, value);
    }

    public void SetMode(Mode mode) {
        if (mode == _mode) return;
        _mode = mode;
        ComponentManager.Logger?.Log(new DebugMessage($"Changed the access mode of pin {this} to {mode}"));

        if (_node == null) return;
        _node.StateChangedEvent -= NotifyStateChange;

        if (!IsWriteOnly) {
            _node.StateChangedEvent += NotifyStateChange;
        }
    }

    public void ConnectToNode(ElectricalNode node) {
        Disconnect();
        _node = node;
        ComponentManager.Logger?.Log(new DebugMessage($"Connected pin {this} to node {_node}"));

        if (!IsWriteOnly) {
            _node.StateChangedEvent += NotifyStateChange;
        }
        
        NotifyStateChange(_node, _node.State);
        PinConnectedEvent?.Invoke(this);
    }

    public void Disconnect() {
        if (_node == null) return;

        _node.StateChangedEvent -= NotifyStateChange;
        ComponentManager.Logger?.Log(new DebugMessage($"Disconnected pin {this} from node {_node}"));
        _node = null;
        NotifyStateChange(null, State);
        PinDisconnectedEvent?.Invoke(this);
    }


    public void NotifyStateChange(ElectricalNode? _, DigitalState nodeState) {
        if (_mode == Mode.WriteOnly) {
            ComponentManager.Logger?.Log(new WarningMessage($"Attempting to read value from a WriteOnly pin {this}"));
            return;
        }

        if (IsDriving) return;
        ComponentManager.Logger?.Log(new DebugMessage($"Reading value of the pin {this} changed to {nodeState}"));
        StateChangedEvent?.Invoke(this, State);
    }

    public override string ToString() => _component.Name + "." + (Name ?? Id.ToString());
}