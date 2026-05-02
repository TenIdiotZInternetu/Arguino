using System.IO.MemoryMappedFiles;
using IpcAdapter;
using IpcAdapter.Encoders;
using IpcAdapter.Events;
using Logger;

namespace SharedMemoryAdapter;

public class TwoWayBuffer : IIpcAdapter {
    public bool IsConnected { get; }
    public event Action<Event>? ReceivedEventEvent;

    private CircularBuffer _consumer;
    private CircularBuffer _producer;

    private IEncoder _encoder;
    private ILogger _logger;

    public TwoWayBuffer(string path, IEncoder encoder, ILogger logger) {
        var mappedMemory = MemoryMappedFile.CreateFromFile(path, FileMode.Open);
        int shmemSize = 2 * Environment.SystemPageSize;
        _consumer = new CircularBuffer(mappedMemory, 0, shmemSize / 2);
        _producer = new CircularBuffer(mappedMemory, shmemSize / 2, shmemSize / 2);

        _encoder = encoder;
        _logger = logger;
    }
    
    public void SendEvent(Event @event) {
        string message = _encoder.EncodeEvent(@event);
        _producer.Write(message);
    }
}