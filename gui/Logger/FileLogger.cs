namespace Logger;

public class FileLogger : ILogger, IDisposable, IAsyncDisposable {
    StreamWriter _file;
    
    public FileLogger(string path) {
        _file = new StreamWriter(path);
    }
    
    public void Log(string message) {
        _file.WriteLine(message);
    }
    
    public void Log(IMessage message) {
        _file.WriteLine($"[{message.Type()}] {message.ToString()}");
    }

    public void Dispose() {
        _file.Dispose();
    }

    public async ValueTask DisposeAsync() {
        await _file.DisposeAsync();
    }
}