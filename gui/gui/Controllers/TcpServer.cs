using System;
using System.Net;
using System.Net.Sockets;
using System.Text;

namespace gui.Controllers;

public class TcpServer {
    public readonly IPEndPoint Endpoint;
    
    private const string LOCAL_HOST = "127.0.0.1"; 
    private readonly TcpListener _listener;
    private TcpClient _client;

    public TcpServer(int port) {
        Endpoint = new IPEndPoint(IPAddress.Parse(LOCAL_HOST), port);
        _listener = new TcpListener(Endpoint);
        _listener.Start();
        _client = _listener.AcceptTcpClient();
    }

    public void WriteMessage(string message) {
        if (!_client.Connected) {
            _client = _listener.AcceptTcpClient();
        }
        
        var stream = _client.GetStream();
        byte[] bytes = Encoding.ASCII.GetBytes(message);
        
        stream.Write(bytes, 0, message.Length);
    }
}