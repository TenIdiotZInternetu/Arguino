using ComponentManagement.Graph;
using IpcAdapter.Events;

namespace ComponentManagement.Components;

public partial class Arduino {
    private long _lastSeenLvt;

    private void ProcessRemoteEvent(Event @event) {
        switch (@event.Type) {
            case EventType.Write: 
                WriteToPin(@event.Args[0], (DigitalState)@event.Args[1]); break;
            case EventType.PinMode:
                SetPinMode(@event.Args[0], (PinMode)@event.Args[1]); break;
            case EventType.Unknown:
                ComponentManager.LogError("Encountered unknown event type");
                return;
        }

        _lastSeenLvt = @event.LocalVirtualTime;
    }
    
    private Event CreateWriteEvent(int pinId, DigitalState value) =>
        new() {
            Type = EventType.Write,
            Args = [ pinId, (int)value ]
        };

    private Event CreatePinModeEvent(int pinId, PinMode mode) =>
        new() {
            Type = EventType.PinMode,
            Args = [ pinId, (int)mode ]
        };

    private void InvokeWriteEvent(int pinId, DigitalState value) => 
        SendLocalEvent(CreateWriteEvent(pinId, value));
    
    private void InvokePinModeEvent(int pinId, PinMode mode) =>
        SendLocalEvent(CreatePinModeEvent(pinId, mode));
    
    private void SendLocalEvent(Event @event) {
        @event.LocalVirtualTime = _lastSeenLvt;
        _ipc?.SendEvent(@event);
    }
}