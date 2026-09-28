namespace Logger;

public class CompositeLogger : ILogger {
    public LogLevel Verbosity { get; set; }
    
    private readonly List<ILogger> _loggers = [];

    public CompositeLogger(params ILogger[] loggers) {
        _loggers.AddRange(loggers);
        Verbosity = loggers.Min(l => l.Verbosity);
    }
    
    public void Log(string message) {
        _loggers.ForEach(l => l.Log(message));
    }
    
    public void Log(IMessage message) {
        _loggers.ForEach(l => l.Log(message));
    }
    
    public void AddLogger(ILogger logger) => _loggers.Add(logger);
    public void RemoveLogger(ILogger logger) => _loggers.Remove(logger);
}