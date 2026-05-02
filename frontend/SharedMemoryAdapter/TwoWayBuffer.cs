using System.IO.MemoryMappedFiles;
using IpcAdapter;
using IpcAdapter.Encoders;
using IpcAdapter.Events;
using Logger;

namespace SharedMemoryAdapter;

public class TwoWayBuffer : IIpcAdapter {
    public bool IsConnected { get; private set; }
    public event Action<Event>? ReceivedEventEvent;

    private MemoryMappedFile? _mappedMemory;
    private CircularBuffer _consumer;
    private CircularBuffer _producer;

    private IEncoder _encoder;
    private ILogger? _logger;

    public TwoWayBuffer(string path, int pages, IEncoder encoder, ILogger? logger = null) {
        _mappedMemory = MemoryMappedFile.CreateFromFile(path, FileMode.Open);
        int shmemSize = 2 * pages * Environment.SystemPageSize;
        _consumer = new CircularBuffer(_mappedMemory, 0, shmemSize / 2);
        _producer = new CircularBuffer(_mappedMemory, shmemSize / 2, shmemSize / 2);

        _encoder = encoder;
        _logger = logger;
        IsConnected = true;
    }
    
    public void SendEvent(Event @event) {
        string message = _encoder.EncodeEvent(@event);
        _producer.Write(message + ';');
    }

    public void Disconnect() {
        _mappedMemory = null;
        IsConnected = false;
    }
}