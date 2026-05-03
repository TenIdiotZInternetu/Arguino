using System.IO.MemoryMappedFiles;
using System.Text;
using IpcAdapter;
using IpcAdapter.Encoders;
using IpcAdapter.Events;
using Logger;

namespace SharedMemoryAdapter;

public class TwoWayBuffer : IIpcAdapter {
    public const byte MESSAGE_DELIMETER = (byte)';';
    
    public bool IsConnected { get; private set; }
    public event Action<Event>? ReceivedEventEvent;

    private MemoryMappedFile? _mappedMemory;
    private CircularBuffer _consumingBuffer;
    private CircularBuffer _producingBuffer;

    private IEncoder _encoder;
    private ILogger? _logger;

    public TwoWayBuffer(string path, int pages, IEncoder encoder, ILogger? logger = null) {
        _encoder = encoder;
        _logger = logger;
        
        // TODO: Don't use absolute path, just a filename
        // DIfferentiate Windows and Linux path
        _mappedMemory = MemoryMappedFile.CreateFromFile(path, FileMode.Open);
        int shmemSize = 2 * pages * Environment.SystemPageSize;
        _consumingBuffer = new CircularBuffer(_mappedMemory, 0, shmemSize / 2);
        _producingBuffer = new CircularBuffer(_mappedMemory, shmemSize / 2, shmemSize / 2);

        IsConnected = true;
        _ = Task.Run(ListenForRemoteEvents);
        _logger?.LogInfo("Successfuly connected to simulator's shared memory buffer.");
        // TODO: Log failure
    }
    
    public void SendEvent(Event @event) {
        string message = _encoder.EncodeEvent(@event);
        if (!_producingBuffer.Write(message + MESSAGE_DELIMETER)) {
            _logger?.LogError("Could not write to shared memory buffer, not enough available space.");
            // TODO: Pause producer
        }
    }

    public void Disconnect() {
        _mappedMemory = null;
        IsConnected = false;
        _logger?.LogWarning("Disconnected from shared memory.");
    }

    private void ListenForRemoteEvents() {
        try {
            // TODO: Use IPC semaphore
            while (IsConnected) {
                if (_consumingBuffer.IsEmpty) continue;

                byte[]? data = _consumingBuffer.ConsumeUntil(MESSAGE_DELIMETER);
                if (data == null) {
                    _logger?.LogError("A span of incoming message could not be determined, skipping.");
                    continue;
                }

                string message = Encoding.ASCII.GetString(data)[..^1]; // strip delimeter at the end
                
                if (!_encoder.TryDecodeEvent(message, out Event @event)) {
                    _logger?.LogError("Failed to decode message: " + message);
                    continue;
                }

                ReceivedEventEvent?.Invoke(@event);
            }
        }
        catch (Exception e) {
            _logger?.LogError("Consuming messages from shared memory buffer failed: " + e.Message);
        }
    }
}