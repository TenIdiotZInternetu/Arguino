using System.Text;
using Logger;

namespace TcpAdapter;

public class MessageHandler {
    public const char WRITE_FLAG = 'W';
    public const char READ_FLAG = 'R';

    public char Delimeter => ';';
    
    public event Action<string>? ReadEvent;

    private TcpClient _client;
    
    private ILogger? _logger;

    public MessageHandler(TcpClient client) {
        _client = client;
    }

    public void HandleMessage(string message) {
        _logger?.Log(new LogMessage.Read(message));
        ReadEvent?.Invoke(message);
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
