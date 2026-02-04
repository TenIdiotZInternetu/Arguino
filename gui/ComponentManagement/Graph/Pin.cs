namespace ComponentManagement.Graph;

public class Pin {
    public const float GND_VOLTAGE = 0;
    private const float EPSILON = 1e-7f;

    public uint Id { get; init; }
    public string? Name { get; init; }

    public float Voltage => _node?.Voltage ?? float.NaN;
    public bool IsConnected => _node != null;
    public bool UnderVoltage => !float.IsNaN(Voltage);
    
    public event Action<Pin, float>? VoltageSetEvent;
    public event Action<Pin, float>? VoltageChangedEvent;

    public event Action<Pin>? PinConnectedEvent;
    public event Action<Pin>? PinDisconnectedEvent;
    
    private Component _component;
    private ElectricalNode? _node;
    
    private enum PinMode { General, ReadOnly, WriteOnly }
    private PinMode _mode = PinMode.General;
    
    public void MakeReadOnly() => _mode = PinMode.ReadOnly;
    public void MakeWriteOnly() => _mode = PinMode.WriteOnly;
    public void MakeGeneral() => _mode = PinMode.General;

    public Pin(Component component, uint id, string? name = null) {
        _component = component;
        Id = id;
        Name = name;
    }

    public void SetVoltage(float value) {
        bool voltagesAlmostEqual = Math.Abs(value - Voltage) < EPSILON;
        if (voltagesAlmostEqual) {
            return;
        }

        if (_mode == PinMode.ReadOnly) {
            // TODO: Log misuse
            return;
        }
        
        VoltageSetEvent?.Invoke(this, value);
    }

    public void NotifyVoltageChange(float value) {
        if (_mode == PinMode.WriteOnly) {
            // TODO: Log misuse
            return;
        }

        VoltageChangedEvent?.Invoke(this, value);
    }

    public void ConnectToNode(ElectricalNode node) {
        _node = node;
        PinConnectedEvent?.Invoke(this);
    }

    public void Disconnect() {
        _node = null;
        PinDisconnectedEvent?.Invoke(this);
    }
}