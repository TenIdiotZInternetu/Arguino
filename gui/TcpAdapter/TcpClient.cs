using System.Diagnostics;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading.Channels;
using Logger;

namespace TcpAdapter;

public class TcpClient {
    private const int RECONNECT_DELAY = 3000;
    
    public bool Connected => _client?.Connected ?? false;
    public readonly IPEndPoint Endpoint;
    
    private const string LOCAL_HOST = "127.0.0.1";
    private const uint BUFFER_SIZE = 4096;
    
    private System.Net.Sockets.TcpClient? _client = new();
    private NetworkStream? _stream = null;
    private byte[] _buffer = new byte[BUFFER_SIZE];
    private Channel<string> _sendQueue = Channel.CreateUnbounded<string>();
    
    private CancellationTokenSource _cts;

    public event Action<string> ReadBytesEvent;

    private ILogger? _logger;

    public TcpClient(int port) {
        Endpoint = new IPEndPoint(IPAddress.Parse(LOCAL_HOST), port);
    }

    public TcpClient(string ip, int port) {
        Endpoint = new IPEndPoint(IPAddress.Parse(ip), port);
    }

    public async Task ConnectAsync() {
        while (!await TryConnectAsync()) {
            await Task.Delay(RECONNECT_DELAY);
        }
    }

    public async Task<bool> TryConnectAsync() {
        try {
            _cts = new();
            _client = new System.Net.Sockets.TcpClient();
            await _client.ConnectAsync(Endpoint);
            _stream = _client.GetStream();
            _logger?.LogInfo($"Connected to server at {Endpoint}.");
            
            _ = Task.Run(LoopReadAsync);
            _logger?.LogInfo("Reading loop initiated.");
            
            _ = Task.Run(LoopWriteAsync);
            _logger?.LogInfo("Writing loop initiated.");
        }
        catch (Exception e) {
            _logger?.LogError($"Failed to initiate TCP Client: {e.Message}");
            return false;
        }
        
        _logger?.LogInfo("TCP Client successfully initiated.");
        return true;
    }

    public void Disconnect() {
        _cts.Cancel();
        _stream?.Dispose();
        _client?.Close();
        _logger?.LogWarning("Server disconnected.");
    }

    public void Reconnect() {
        Disconnect();
        _logger?.LogInfo($"Attempting to reconnect at {Endpoint}.");
        Task.Run(ConnectAsync);
    }

    public async Task SendMessageAsync(string message) {
        await _sendQueue.Writer.WriteAsync(message);
    }

    public TcpClient SetLogger(ILogger? logger) {
        _logger = logger;
        return this;
    }

    private async Task LoopReadAsync() {
        try {
            while (!_cts.IsCancellationRequested) {
                int bytesRead = await _stream!.ReadAsync(_buffer, 0, _buffer.Length, _cts.Token);
                if (bytesRead == 0) {
                    // Disconnected
                    Reconnect();
                    return;
                }

                _logger?.LogDebug($"Reading {bytesRead} bytes...");

                var byteChunk = Encoding.UTF8.GetString(_buffer, 0, bytesRead);
                ReadBytesEvent?.Invoke(byteChunk);
            }
        }
        catch (Exception e) {
            _logger?.LogError($"TCP Reading loop failed: {e.Message}");
        }
    }

    private async Task LoopWriteAsync() {
        try {
            while (await _sendQueue.Reader.WaitToReadAsync()) {
                while (_sendQueue.Reader.TryRead(out var message)) {
                    var data = Encoding.UTF8.GetBytes(message);
                    await _stream!.WriteAsync(data, 0, data.Length);
                    _logger?.LogDebug($"Sent {data.Length} bytes...");
                }
            }
        }
        catch (Exception e) {
            _logger?.LogError($"TCP Writing loop failed: {e.Message}");
        }
    }
}