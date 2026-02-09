using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading.Channels;
using Logger;

namespace TcpAdapter;

public class TcpClient<THandler> 
    where  THandler : IMessageHandler<THandler> {
    public THandler Handler { get; init; }
    
    public bool Connected => _client.Connected;
    public readonly IPEndPoint Endpoint;
    
    private const string LOCAL_HOST = "127.0.0.1";
    private const uint BUFFER_SIZE = 256;
    
    private readonly System.Net.Sockets.TcpClient _client = new();
    private NetworkStream? _stream = null;
    private byte[] _buffer = new byte[BUFFER_SIZE];
    private Channel<string> _sendQueue = Channel.CreateUnbounded<string>();
    
    private CancellationTokenSource _cts = new();

    private ILogger? _logger;

    public TcpClient(int port) {
        Endpoint = new IPEndPoint(IPAddress.Parse(LOCAL_HOST), port);
        Handler = THandler.Create(this);
    }

    public TcpClient(string ip, int port) {
        Endpoint = new IPEndPoint(IPAddress.Parse(ip), port);
        Handler = THandler.Create(this);
    }

    public async Task ConnectAsync() {
        try {
            await _client.ConnectAsync(Endpoint);
            _stream = _client.GetStream();
        }
        catch (Exception e) {
            _logger?.Log(new LogMessage.Error($"Error while connecting to server: {e.Message}"));
            return;
        }

        _logger?.Log(new LogMessage.Info("Connected to server."));
        
        _ = Task.Run(LoopReadAsync);
        _logger?.Log(new LogMessage.Info("Reading loop began."));
        _ = Task.Run(LoopWriteAsync);
        _logger?.Log(new LogMessage.Info("Writing loop began."));
    }

    public async Task SendMessageAsync(string message) {
        await _sendQueue.Writer.WriteAsync(message);
    }

    public TcpClient<THandler> SetLogger(ILogger logger) {
        _logger = logger;
        return this;
    }

    private async Task LoopReadAsync() {
        StringBuilder chunkBuilder = new();
        StringBuilder messageBuilder = new();
        List<string> messages = new();

        while (!_cts.IsCancellationRequested) {
            int bytesRead = await _stream!.ReadAsync(_buffer,  0, _buffer.Length, _cts.Token);
            if (bytesRead == 0) break; // Disconnected
            
            _logger?.Log(new LogMessage.Info($"Reading {bytesRead} bytes..."));
            
            var chunk = Encoding.UTF8.GetString(_buffer, 0, bytesRead);
            chunkBuilder.Append(chunk);
            messageBuilder.Clear();

            foreach (char c in chunkBuilder.ToString()) {
                if (c == Handler.Delimeter) {
                    messages.Add(messageBuilder.ToString());
                    chunkBuilder.Remove(0, messageBuilder.Length + 1);
                    messageBuilder.Clear();
                }
                else {
                    messageBuilder.Append(c);
                }
            }

            foreach (var message in messages) {
                Handler.OnReadMessage(message);
            }
            
            messages.Clear();
        }
    }

    private async Task LoopWriteAsync() {
        while (await _sendQueue.Reader.WaitToReadAsync()) {
            while (_sendQueue.Reader.TryRead(out var message)) {
                var data = Encoding.UTF8.GetBytes(message + Handler.Delimeter);
                await _stream!.WriteAsync(data, 0, data.Length);
                _logger?.Log(new LogMessage.Info($"Sent {data.Length} bytes..."));
            }
        }
    }
}