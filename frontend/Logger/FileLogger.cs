using System.Diagnostics;

namespace Logger;

public class FileLogger : ILogger, IDisposable {
    public LogLevel Verbosity { get; set; }
    public Stopwatch? Timer { get; set; }
    
    private readonly StreamWriter _writer;
    private readonly object _lock = new();
    
    public FileLogger(string path, LogLevel verbosity = LogLevel.Info) {
        _writer = new StreamWriter(path) { AutoFlush = true };
        Verbosity = verbosity;
    }
    
    public void Log(string message) {
        lock (_lock) {
            _writer.WriteLine($"{GetTimestamp()} {message}");
        }
    }
    
    public void Log(IMessage message) {
        if (message.LogLevel < Verbosity) return;
        
        lock (_lock) {
            _writer.WriteLine($"{GetTimestamp()}[{message.Type()}] {message.AsString()}");
        }
    }

    public void Dispose() {
        lock (_lock) {
            _writer.Dispose();
        }
    }

    private string GetTimestamp() {
        if (Timer == null) {
            return "";
        }
        
        return $"[{Timer.Elapsed:mm\\:ss\\.ffffff}]";
    }
}