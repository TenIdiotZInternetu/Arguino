using ComponentManagement.Graph;
using TcpAdapter;

namespace ComponentManagement.Components;

public class Arduino : Component {
    public const int POLLING_RATE_MS = 10;
    
    public int TcpPort { get; set; }

    private ArduinoState _state = new();
    
    // TODO: Replace by interface
    private bool _clientRunning;
    private TcpClient<TestMessageHandler>? _tcpClient;
    private TestMessageHandler? _tcpHandler => _tcpClient?.Handler;

    public Arduino(string definitionPath) : base(definitionPath) { }

    public override void OnPinStateChanged(Pin pin) {
        uint idx = GetPinIndex(pin.Name!);
        _state.DigitalPins[idx] = pin.IsHigh;

        if (_clientRunning) {
            _tcpHandler!.SendWriteMessage(_state);
        }
    }

    public void ConnectToSimulator(TcpClient<TestMessageHandler> tcpClient) {
        _tcpClient = tcpClient;
        _tcpHandler!.StateChangedEvent += UpdateCircuit;
        
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
            bool pinValue = newState.DigitalPins[i];
            Pin pin = GetDigitalPin(i);
            pin.SetValue(pinValue);
        }
    }

    private Pin GetDigitalPin(int index) => GetPin("D" + index);
    private Pin GetAnalogPin(int index) => GetPin("A" + index);
    private uint GetPinIndex(string pinName) => uint.Parse(pinName[1..]);
}