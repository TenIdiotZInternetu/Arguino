namespace TcpAdapter;

public class TestMessageHandler : IMessageHandler {
    public char Delimeter => ';';
    
    public event Action<string>? ReadEvent;
    public event Action<ArduinoState>? StateChangedEvent;

    private StateEncoder _stateEncoder = new();
    
    public void HandleRead(string message) {
        ArduinoState state = _stateEncoder.Decode(message);
        ReadEvent?.Invoke(message);
        StateChangedEvent?.Invoke(state);
    }

}