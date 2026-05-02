using System.IO.MemoryMappedFiles;
using IpcAdapter;
using IpcAdapter.Events;

namespace SharedMemoryAdapter;

public class TwoWayBuffer : IIpcAdapter {
    public bool IsConnected { get; }
    public event Action<Event>? ReceivedEventEvent;

    private MemoryMappedFile _mappedMemory;
    private CircularBuffer _producer;
    private CircularBuffer _consumer;

    public TwoWayBuffer(string path) {
        _mappedMemory = MemoryMappedFile.CreateFromFile(path, FileMode.Open);
        int shmemSize = 2 * Environment.SystemPageSize;
        
    }
    
    public void SendEvent(Event @event) {
        throw new NotImplementedException();
    }
}