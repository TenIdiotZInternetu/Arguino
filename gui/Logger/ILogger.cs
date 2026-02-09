namespace Logger;

public interface ILogger {
    public void Log(string message);
    public void Log(IMessage message);
}