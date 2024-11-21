using System;
using System.Net;
using System.Net.Sockets;
using System.Text;

namespace gui.Controllers;

public class TcpConnection : ITcpConnection {
    private const int BUFFER_SIZE = 64;
    
    private readonly TcpListener _listener;
    
    private readonly byte[] _dataBuffer = new byte[BUFFER_SIZE];

    public TcpConnection(IPAddress ip, int port) {
        _listener = new TcpListener(ip, port);
        _listener.Start();
    }
    
    public bool Connect(IPAddress ip, int port) {
        throw new System.NotImplementedException();
    }

    public bool SendMessage(string message) {
        throw new System.NotImplementedException();
    }

    public string? ReadMessage() {
        using TcpClient client = _listener.AcceptTcpClient();
        var stream = client.GetStream();

        var data = new StringBuilder();

        while (ReadToBuffer(stream, out int byteCount)) {
            data.Append(Encoding.ASCII.GetString(_dataBuffer, 0, byteCount));
            Console.WriteLine("Received: {0}", data);
        }

        return data.ToString();
    }

    private bool ReadToBuffer(NetworkStream stream, out int byteCount) {
        byteCount = stream.Read(_dataBuffer, 0, _dataBuffer.Length);
        return byteCount != 0;
    }
}