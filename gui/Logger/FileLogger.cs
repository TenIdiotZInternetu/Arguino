using System.Diagnostics;

namespace Logger;

public class FileLogger : ILogger, IDisposable {
    private const float FLUSH_INTERVAL = 2f;
    
    public Stopwatch? Timer { get; set; }
    
    StreamWriter _writer;
    PeriodicTimer _flushTimer;
    Task _flushTask;
    
    public FileLogger(string path) {
        _writer = new StreamWriter(path);
        _flushTimer = new PeriodicTimer(TimeSpan.FromSeconds(FLUSH_INTERVAL));
        _flushTask = BackgroundFlushAsync();
    }
    
    public void Log(string message) {
        _writer.WriteLine($"{GetTimestamp()} {message}");
    }
    
    public void Log(IMessage message) {
        _writer.WriteLine($"{GetTimestamp()}[{message.Type()}] {message.AsString()}");
    }

    public void Dispose() {
        _writer.Flush();
        _writer.Dispose();
        _flushTimer.Dispose();
        _flushTask.Dispose();
    }

    private string GetTimestamp() {
        if (Timer == null) {
            return "";
        }
        
        return $"[{Timer.Elapsed:mm\\:ss\\.ffffff}]";
    }
    
    private async Task BackgroundFlushAsync() {
        while (await _flushTimer.WaitForNextTickAsync()) {
            await _writer.FlushAsync();
        }
    }
}