using ComponentManagement.Graph;
using TcpAdapter;

namespace ComponentManagement.Components;

public class Arduino : Component {
    public const int POLLING_RATE_MS = 10;
    
    private ArduinoState _state = new();
    
    // TODO: Replace by interface
    private bool _clientRunning;
    private TcpClient? _tcpClient;
    private MessageHandler? _tcpHandler => _tcpClient?.Handler;
    
    private readonly SynchronizationContext _uiContext;

    public Arduino(string definitionPath) : base(definitionPath) {
        _uiContext = SynchronizationContext.Current!;
    }

    public override void OnPinStateChanged(Pin pin) {
        uint idx = GetPinIndex(pin.Name!);
        _state.DigitalPins[idx] = pin.IsHigh;

        if (_clientRunning) {
            _tcpHandler!.SendWriteMessage(_state);
        }
    }

    public void ConnectToSimulator(TcpClient tcpClient) {
        _tcpClient = tcpClient;
        
        _tcpHandler!.StateChangedEvent += state => {
            _uiContext.Post(_ => UpdateCircuit(state), null);    
        };
        
        Task.Run(async () => {
            await _tcpClient!.ConnectAsync();
            _clientRunning = true;
        
            while (_clientRunning) {
                _tcpHandler!.SendReadMessage();
                await Task.Delay(POLLING_RATE_MS);
            }
        });
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

    private Pin GetDigitalPin(int index) => GetPin("D" + index);
    private Pin GetAnalogPin(int index) => GetPin("A" + index);
    private uint GetPinIndex(string pinName) => uint.Parse(pinName[1..]);
}