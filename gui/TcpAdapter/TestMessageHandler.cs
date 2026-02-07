using System.Text;

namespace TcpAdapter;

public class TestMessageHandler : IMessageHandler<TestMessageHandler> {
    public const char WRITE_FLAG = 'W';

    public char Delimeter => ';';
    
    public event Action<string>? ReadEvent;
    public event Action<ArduinoState>? StateChangedEvent;

    private StateEncoder _stateEncoder = new();
    private TcpClient<TestMessageHandler> _client;

    public static TestMessageHandler Create(TcpClient<TestMessageHandler> client) {
        return new TestMessageHandler {
            _client = client
        };
    }

    public void OnReadMessage(string message) {
        ArduinoState state = _stateEncoder.Decode(message);
        ReadEvent?.Invoke(message);
        StateChangedEvent?.Invoke(state);
    }

    public void SendReadMessage() {
        _ = _client.SendMessageAsync("R");
    }
    
    public void SendWriteMessage(ArduinoState state) {
        StringBuilder messageBuilder = new StringBuilder()
            .Append(WRITE_FLAG)
            .Append(_stateEncoder.Encode(state))
            .Append(Delimeter);
        
        _ = _client.SendMessageAsync(messageBuilder.ToString());
    }
}
