using System.Diagnostics;

namespace Logger;

public interface ILogger {
    public LogLevel Verbosity { get; set; }
    
    public void Log(string message);
    public void Log(IMessage message);

    public void LogDebug(string messageText) =>
        Log(new DebugMessage(messageText));

    public void LogInfo(string messageText) =>
        Log(new InfoMessage(messageText));

    public void LogWarning(string messageText) =>
        Log(new WarningMessage(messageText));

    public void LogError(string messageText) =>
        Log(new ErrorMessage(messageText));
}