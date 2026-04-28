namespace Logger;

public enum LogLevel { Debug, Info, Warning, Error }

public interface IMessage {
    public LogLevel LogLevel { get; }
    public string AsString();
    public string Type();
}