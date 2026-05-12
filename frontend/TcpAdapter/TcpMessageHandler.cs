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
        _client.ReachedEof += (this as IIpcAdapter).Reconnect;
    }

    public void SendEvent(Event @event) {
        string message = _encoder.EncodeEvent(@event) + Delimeter;        
        
        Task.Run(async () => {
            try {
                await _client.SendMessageAsync(message);
                _logger?.LogDebug($"TCP Message sent: '{message}'");
            }
            catch (Exception e) {
                _logger?.LogError($"Error while sending message: {e.Message}");
            }
        });
    }
    
    public async Task<bool> TryConnectAsync() => await _client.TryConnectAsync();
    public void Disconnect() => _client.Disconnect();

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
        _logger?.LogDebug($"TCP Message received: '{message}'");

        if (_encoder.TryDecodeEvent(message, out Event @event)) {
            ReceivedEventEvent?.Invoke(@event);
        };
    }
}
