using System.Net;

namespace gui.Services;

public interface ITcpConnection {
    public bool Connect(IPAddress ip, int port);
    public bool SendMessage(string message);
    public string? ReadMessage();
}