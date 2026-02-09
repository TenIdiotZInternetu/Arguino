using System.Text;
using Logger;

namespace TcpAdapter;

public class MessageHandler {
    public const char WRITE_FLAG = 'W';
    public const char READ_FLAG = 'R';

    public char Delimeter => ';';
    
    public event Action<string>? ReadEvent;
    public event Action<ArduinoState>? StateChangedEvent;

    private StateEncoder _stateEncoder = new();
    private TcpClient _client;
    
    private ILogger? _logger;

    public MessageHandler(TcpClient client) {
        _client = client;
    }

    public void OnReadMessage(string message) {
        _logger?.Log(new LogMessage.Read(message));
        ArduinoState state = _stateEncoder.Decode(message);
        ReadEvent?.Invoke(message);
        StateChangedEvent?.Invoke(state);
    }

    public void SendReadMessage() {
        SendMessage(READ_FLAG.ToString());
    }
    
    public void SendWriteMessage(ArduinoState state) {
        StringBuilder messageBuilder = new StringBuilder()
            .Append(WRITE_FLAG)
            .Append(_stateEncoder.Encode(state));

        SendMessage(messageBuilder.ToString());
    }

    public void SetLogger(ILogger logger) {
        _logger = logger;
    }

    private void SendMessage(string message) {
        Task.Run(async () => {
            try {
                await _client.SendMessageAsync(message);
                _logger?.Log(new LogMessage.Send(message));
            }
            catch (Exception e) {
                _logger?.Log(new ErrorMessage($"Error while sending message: {e.Message}"));
            }
        });
    }
}
