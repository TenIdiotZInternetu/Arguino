namespace TcpAdapter;

public interface IMessageHandler
{
    public void HandleRead(string message);
    public char Delimeter { get; }
    
    public event Action<string> ReadEvent;
    public event Action<ArduinoState> StateChangedEvent;
}