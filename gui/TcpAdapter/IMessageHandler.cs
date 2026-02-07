namespace TcpAdapter;

public interface IMessageHandler<TSelf> 
    where TSelf :  IMessageHandler<TSelf>
{
    public static abstract TSelf Create(TcpClient<TSelf> client);
    
    public void OnReadMessage(string message);
    public void SendWriteMessage(ArduinoState state);
    public char Delimeter { get; }

    public event Action<string> ReadEvent;
    public event Action<ArduinoState> StateChangedEvent;
}