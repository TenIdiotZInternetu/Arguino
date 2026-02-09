using Logger;

namespace TcpAdapter;

public struct LogMessage {
    public record struct Read(string TcpMessage) : IMessage {
        public string AsString() => $"Received message from the server: {TcpMessage}";
        public string Type() => "TCP::READ";
    }
    
    public record struct Send(string TcpMessage) : IMessage {
        public string AsString() => $"Sent message to the server: {TcpMessage}";
        public string Type() => "TCP::SEND";
    }
}

