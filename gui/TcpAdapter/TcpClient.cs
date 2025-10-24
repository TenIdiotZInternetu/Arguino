using System.Net;
using System.Net.Sockets;
using System.Text;

namespace TcpAdapter;

public class TcpClient {
    public bool Connected => _client.Connected;
    public readonly IPEndPoint Endpoint;
    
    private const string LOCAL_HOST = "127.0.0.1";
    private readonly System.Net.Sockets.TcpClient _client = new();

    public TcpClient(int port) {
        Endpoint = new IPEndPoint(IPAddress.Parse(LOCAL_HOST), port);
    }

    public TcpClient(string ip, int port) {
        Endpoint = new IPEndPoint(IPAddress.Parse(ip), port);
    }

    public bool Connect() {
        try {
            _client.Connect(Endpoint);
            return true;
        }
        catch (SocketException e) {
            return false;
        }
    }
    
    public string ReadMessage(string messageDelimeter) {
        if (!Connected) {
            throw new Exception("Client is not connected to the endpoint");
        }

        var stream = _client.GetStream();
        var data = new StringBuilder();

        while (true) {
            int b = stream.ReadByte();
            if (b == -1) continue;             // EOF

            data.Append((char)b);
            if (FoundDelimeter(data, messageDelimeter)) break;
        }

        return data.ToString(0, data.Length - messageDelimeter.Length);
    }

    private bool FoundDelimeter(StringBuilder data, string delimeter) {
        if (data.Length < delimeter.Length) {
            return false;
        }
        
        int delimIndex = data.Length - delimeter.Length;
        string lastChars = data.ToString(delimIndex, delimeter.Length);

        return delimeter == lastChars;
    }
    
    public void WriteMessage(string message) {
        var stream = _client.GetStream();
        byte[] bytes = Encoding.ASCII.GetBytes(message);
        
        stream.Write(bytes, 0, message.Length);
    }
}