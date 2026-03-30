using System.Text;
using IpcAdapter;
using IpcAdapter.Encoders;
using IpcAdapter.Events;
using Logger;

namespace TcpAdapter;

public class TcpMessageHandler : IIpcAdapter {
    public char Delimeter => ';';
    public bool IsConnected => _client.Connected;
    
    public event Action<Event>? ReceivedEventEvent;
    
    private TcpClient _client;
    private IEncoder _encoder;
    private ILogger? _logger;
    
    private StringBuilder _messageBuilder = new();

    public TcpMessageHandler(TcpClient client, IEncoder encoder, ILogger? logger = null) {
        _client = client;
        _encoder = encoder;
        _logger = logger;

        _client.SetLogger(logger);
        _client.ReadBytesEvent += OnBytesRead;
        Task.Run(_client.ConnectAsync);
    }

    public void SendEvent(Event @event) {
        string message = _encoder.EncodeEvent(@event) + Delimeter;        
        
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

    private void OnBytesRead(string bytechunk) {
        foreach (char c in bytechunk) {
            if (c == Delimeter) {
                HandleMessage(_messageBuilder.ToString());
                _messageBuilder.Clear();
            }
            else {
                _messageBuilder.Append(c);
            }
        }
    }
    
    private void HandleMessage(string message) {
        _logger?.Log(new LogMessage.Read(message));
        Event @event = _encoder.DecodeEvent(message);
        ReceivedEventEvent?.Invoke(@event);
    }
}
