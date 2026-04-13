using ComponentManagement.Graph;
using IpcAdapter.Events;

namespace ComponentManagement.Components;

public partial class Arduino {
    private EventQueue _queue = new();

    internal Event CreateWriteEvent(int pinId, DigitalState value) =>
        new() {
            Type = EventType.Write,
            Args = [ pinId, (int)value ],
            Action = () => GetDigitalPin(pinId).SetValue(value)
        };

    private void InvokeWriteEvent(int pinId, DigitalState value) => 
        HandleEvent(CreateWriteEvent(pinId, value));

    internal Event CreatePinModeEvent(int pinId, PinMode mode) =>
        new() {
            Type = EventType.Write,
            Args = [ pinId, (int)mode ],
            Action = () => GetDigitalPin(pinId).SetMode(mode switch {
                PinMode.In => Pin.Mode.ReadOnly,
                PinMode.Out => Pin.Mode.WriteOnly,
            })
        };

    private void InvokePinModeEvent(int pinId, PinMode mode) =>
        HandleEvent(CreatePinModeEvent(pinId, mode));

    private void ProcessRemoteEvent() {
        
    }
    
    private void HandleEvent(Event @event) {
        
    }
}