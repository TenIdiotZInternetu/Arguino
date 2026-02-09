using System.Diagnostics;

namespace Logger;

public class FileLogger : ILogger, IDisposable, IAsyncDisposable {
    public Stopwatch? Timer { get; set; }
    StreamWriter _writer;
    
    public FileLogger(string path) {
        _writer = new StreamWriter(path) {
            AutoFlush = true
        };
    }
    
    public void Log(string message) {
        _writer.WriteLine($"{GetTimestamp()} {message}");
    }
    
    public void Log(IMessage message) {
        _writer.WriteLine($"{GetTimestamp()}[{message.Type()}] {message.AsString()}");
    }

    public void Dispose() {
        _writer.Dispose();
    }

    public async ValueTask DisposeAsync() {
        await _writer.DisposeAsync();
    }

    private string GetTimestamp() {
        if (Timer == null) {
            return "";
        }
        
        return $"[{Timer.Elapsed:mm\\:ss\\.ffffff}]";
    }
}