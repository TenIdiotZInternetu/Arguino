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
        uint idx = GetPinIndex(pin.Name!);
        
        if (_ipc?.IsConnected ?? false) {
            // TODO: _ipc.SendEvent();
        }
    }

    public void ConnectToSimulator(IIpcAdapter ipc) {
        _ipc = ipc;
        
        _ipc.ReceivedEventEvent += e => {
            // TODO: _uiContext.Post(_ => , null);    
        };
    }

    private Pin GetDigitalPin(int index) => GetPin("D" + index)!;
    private Pin GetAnalogPin(int index) => GetPin("A" + index)!;
    private uint GetPinIndex(string pinName) => uint.Parse(pinName[1..]);
}