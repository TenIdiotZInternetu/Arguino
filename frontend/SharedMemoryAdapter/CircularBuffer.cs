using System.IO.MemoryMappedFiles;
using System.Text;

namespace SharedMemoryAdapter;

public class CircularBuffer {
    public const long PRODUCER_PTR_LOCATION = 0;
    public const long CONSUMER_PTR_LOCATION = 8;
    public const long BUFFER_LOCATION = 16;
    
    private MemoryMappedViewAccessor _mappedRegion;
    private long _nextProducerOffset;

    public long BufferSize => _mappedRegion.Capacity - BUFFER_LOCATION;
    public long BytesFilled => Math.Abs(ProducerOffset - ConsumerOffset);
    public long BytesAvailable => BufferSize - BytesFilled;
    
    public long ProducerOffset {
        get {
            _mappedRegion.Read(PRODUCER_PTR_LOCATION, out long value);
            return value;
        }
        set => _mappedRegion.Write(PRODUCER_PTR_LOCATION, value);
    }
    
    public long ConsumerOffset {
        get {
            _mappedRegion.Read(CONSUMER_PTR_LOCATION, out long value);
            return value;
        }
        set => _mappedRegion.Write(CONSUMER_PTR_LOCATION, value);
    }

    public CircularBuffer(MemoryMappedFile mappedMemory, long offset, long size) {
        _mappedRegion = mappedMemory.CreateViewAccessor(offset, size);
        _mappedRegion.Write(PRODUCER_PTR_LOCATION, 0);
        _mappedRegion.Write(CONSUMER_PTR_LOCATION, 0);
    }

    public bool Write(ReadOnlySpan<byte> data) {
        if (BytesAvailable <= data.Length) {
            return false;
        }
        
        long bytesUntilBufferEnd = BufferSize - _nextProducerOffset;

        if (data.Length < bytesUntilBufferEnd) {
            // TODO: Potentially slow?
            _mappedRegion.WriteArray(ProducerOffset, data.ToArray(), 0, data.Length);
        }
        else {
            var firstBytes = data.Slice(0, (int)bytesUntilBufferEnd);
            var lastBytes = data.Slice((int)bytesUntilBufferEnd, data.Length);
            
            _mappedRegion.WriteArray(ProducerOffset, firstBytes.ToArray(), 0, firstBytes.Length);
            _mappedRegion.WriteArray(BUFFER_LOCATION, lastBytes.ToArray(), 0, lastBytes.Length);
        }

        ProducerOffset = _nextProducerOffset;
        _nextProducerOffset = GetNextProducerOffset(data.Length);
        return true;
    }

    public bool Write(string data) {
        return Write(Encoding.ASCII.GetBytes(data));
    }


    // public bool Consume<T>(out T data) {
    //     return false;
    // }

    private long GetNextProducerOffset(long shift) {
        return (ProducerOffset + shift) % BufferSize;
    }
}