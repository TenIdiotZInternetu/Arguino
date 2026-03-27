using ComponentManagement.Graph;
using TcpAdapter;
using IpcAdapter;

namespace ComponentManagement.Components;

public class Arduino : Component {
    private ArduinoState _state = new();
    
    // TODO: Replace by interface
    private IIpcAdapter? _ipc;
    
    private readonly SynchronizationContext _uiContext;

    public Arduino(string definitionPath) : base(definitionPath) {
        _uiContext = SynchronizationContext.Current!;
    }

    public override void OnPinStateChanged(Pin pin) {
        uint idx = GetPinIndex(pin.Name!);
        _state.DigitalPins[idx] = pin.IsHigh;

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

    private void UpdateCircuit(ArduinoState newState) {
        for (int i = 0; i < ArduinoState.DIGITAL_PIN_COUNT; i++) {
            Pin pin = GetDigitalPin(i);
            bool pinValue = newState.DigitalPins[i];
            var pinMode = newState.PinModes[i];

            if (pinMode == ArduinoState.PinMode.In) {
                pin.MakeReadOnly();
            }

            if (pinMode == ArduinoState.PinMode.Out) {
                pin.MakeWriteOnly();
                pin.SetValue(pinValue);
            }
        }
    }

    private Pin GetDigitalPin(int index) => GetPin("D" + index)!;
    private Pin GetAnalogPin(int index) => GetPin("A" + index)!;
    private uint GetPinIndex(string pinName) => uint.Parse(pinName[1..]);
}