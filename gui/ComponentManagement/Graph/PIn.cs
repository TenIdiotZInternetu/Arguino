namespace ComponentManagement;

public class Pin {
    public Component Component { get; init; }
    public uint Id { get; init; }
    public string? Name { get; init; }
    
    public float Voltage { get; private set; }
    public float Current { get; private set; }
    
    private enum PinMode { General, ReadOnly, WriteOnly }
    private PinMode _mode = PinMode.General;
    
    public void MakeReadOnly() => _mode = PinMode.ReadOnly;
    public void MakeWriteOnly() => _mode = PinMode.WriteOnly;
    public void MakeGeneral() => _mode = PinMode.General;

    public Pin(Component component, uint id, string? name = null) {
        Component = component;
        Id = id;
        Name = name;
    }

    public void SetVoltage(float value) {
        if (_mode == PinMode.ReadOnly) {
            // TODO: Log misuse
            return;
        }
        
        Voltage = value;
    }

    public void SetCurrent(float value) {
        if (_mode == PinMode.ReadOnly) {
            // TODO: Log misuse
            return;
        }
        
        Current = value;
    }
}