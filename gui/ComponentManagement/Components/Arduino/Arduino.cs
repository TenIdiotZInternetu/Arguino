using ComponentManagement.Graph;
using TcpAdapter;
using IpcAdapter;
using IpcAdapter.Events;

namespace ComponentManagement.Components;

public partial class Arduino : Component {
    private IIpcAdapter? _ipc;
    private readonly SynchronizationContext _uiContext;

    public Arduino(string definitionPath) : base(definitionPath) {
        _uiContext = SynchronizationContext.Current!;
    }

    public override void OnPinStateChanged(Pin pin) {
        int idx = GetPinIndex(pin.Name!);
        
        if (_ipc?.IsConnected ?? false) {
            InvokeWriteEvent(idx, pin.State);
        }
    }

    public void ConnectToSimulator(IIpcAdapter ipc) {
        _ipc = ipc;
        
        _ipc.ReceivedEventEvent += e => {
            _uiContext.Post(_ => ProcessRemoteEvent(e), null);    
        };
    }
    
    private void WriteToPin(int pinId, DigitalState value) {
        GetDigitalPin(pinId).SetValue(value);
    }
    
    private void SetPinMode(int pinId, PinMode mode) {
        GetDigitalPin(pinId).SetMode(mode switch {
            PinMode.In => Pin.Mode.ReadOnly,
            PinMode.Out => Pin.Mode.WriteOnly,
        });
    }

    private Pin GetDigitalPin(int index) => GetPin("D" + index)!;
    private Pin GetAnalogPin(int index) => GetPin("A" + index)!;
    private int GetPinIndex(string pinName) => int.Parse(pinName[1..]);
}