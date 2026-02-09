using System.Diagnostics;

namespace Logger;

public class FileLogger : ILogger, IDisposable, IAsyncDisposable {
    public Stopwatch? Timer { get; set; }
    StreamWriter _file;
    
    public FileLogger(string path) {
        _file = new StreamWriter(path);
    }
    
    public void Log(string message) {
        _file.WriteLine($"{GetTimestamp()} {message}");
    }
    
    public void Log(IMessage message) {
        _file.WriteLine($"{GetTimestamp()}[{message.Type()}] {message.AsString()}");
    }

    public void Dispose() {
        _file.Dispose();
    }

    public async ValueTask DisposeAsync() {
        await _file.DisposeAsync();
    }

    private string GetTimestamp() {
        if (Timer == null) {
            return "";
        }
        
        return $"[{Timer.Elapsed:mm\\:ss\\.ffffff}]";
    }
}