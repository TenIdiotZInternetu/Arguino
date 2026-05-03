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

    public bool IsEmpty => ConsumerOffset == ProducerOffset;
    
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
    }

    public bool Write(ReadOnlySpan<byte> data) {
        if (BytesAvailable <= data.Length) {
            return false;
        }
        
        long bytesUntilBufferEnd = BufferSize - _nextProducerOffset;

        if (data.Length < bytesUntilBufferEnd) {
            // TODO: Potentially slow?
            _mappedRegion.WriteArray(BUFFER_LOCATION + ProducerOffset, data.ToArray(), 0, data.Length);
        }
        else {
            var firstBytes = data.Slice(0, (int)bytesUntilBufferEnd);
            var lastBytes = data.Slice((int)bytesUntilBufferEnd, data.Length);
            
            _mappedRegion.WriteArray(BUFFER_LOCATION + ProducerOffset, firstBytes.ToArray(), 0, firstBytes.Length);
            _mappedRegion.WriteArray(BUFFER_LOCATION, lastBytes.ToArray(), 0, lastBytes.Length);
        }

        ProducerOffset = _nextProducerOffset;
        _nextProducerOffset = ShiftOffset(_nextProducerOffset, data.Length);
        return true;
    }

    public bool Write(string data) {
        return Write(Encoding.ASCII.GetBytes(data));
    }


    public byte[]? ConsumeUntil(byte delimeter) {
        if (!FindDelimeter(delimeter, out long offset)) {
            return null;
        }

        bool wrapsAround = offset < ConsumerOffset;
        long dataLength = wrapsAround ? 
            offset + BufferSize - ConsumerOffset :
            offset - ConsumerOffset;

        dataLength++; // 0-indexed offset
        
        byte[] result = new byte[dataLength];

        if (!wrapsAround) {
            _mappedRegion.ReadArray(BUFFER_LOCATION + ConsumerOffset, result, 0, result.Length);
        }
        else {
            long firstBytesCount = BufferSize - ConsumerOffset;
            long lastBytesCount = offset;

            _mappedRegion.ReadArray(BUFFER_LOCATION + ConsumerOffset, result, 0, (int)firstBytesCount);
            _mappedRegion.ReadArray(BUFFER_LOCATION, result, (int)firstBytesCount, (int)lastBytesCount);
        }
        
        ConsumerOffset = ShiftOffset(ConsumerOffset, dataLength);
        return result;
    }

    private bool FindDelimeter(byte delimeter, out long foundAtOffset) {
        long offset = ConsumerOffset;
        byte data;

        do {
            foundAtOffset = offset;
            
            if (offset == ProducerOffset) { // Delimeter not found in the present data
                return false;
            }
            
            if (offset == BufferSize) {
                offset = 0;
            }
            
            _mappedRegion.Read(BUFFER_LOCATION + offset, out data);
            offset++;
        } while (data != delimeter);

        return true;
    }

    private long ShiftOffset(long offset, long shift) {
        return (offset + shift) % BufferSize;
    }
}