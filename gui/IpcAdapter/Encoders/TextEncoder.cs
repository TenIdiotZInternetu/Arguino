using IpcAdapter.Events;

namespace IpcAdapter.Encoders;

public class TextEncoder : IEncoder {
    private const int COMMON_PARTS = 3;
    private const char PART_DELIMETER = ':';
    private const string COMMON_PART_FORMAT = "{0:D12}:{1:D7}";
    private const string WRITE_FORMAT = "{0:D12}:{1:D7}";
    private const string PINMODE_FORMAT = "{0:D12}:{1:D7}";
    
    public static string EncodeWrite(Event e)
    {
        return string.Format(WRITE_FORMAT, e.Args[0], e.Args[1]);
    }

    public static string EncodePinMode(Event e)
    {
        return string.Format(PINMODE_FORMAT, e.Args[0], (char)e.Args[1]);
    }
    
    public string EncodeEvent(Event @event) {
        string commonPart = string.Format(COMMON_PART_FORMAT, @event.TimestampMicros, @event.LocalVirtualTime);
        return @event.Type switch {
            EventType.Write => commonPart + EncodeWrite(@event),
            EventType.PinMode => commonPart + EncodePinMode(@event),
            _ => throw new ArgumentOutOfRangeException("Sending Unknown event")
        };
    }
    
    public bool TryDecodeEvent(string message, out Event @event) {
        @event = new Event();
        string[] parts = message.Split(PART_DELIMETER);
        
        if (!long.TryParse(parts[0], out @event.TimestampMicros) ||
            !long.TryParse(parts[1], out @event.LocalVirtualTime)) {
            return false;
        };

        @event.Type = parts[2] switch {
            "W" => EventType.Write,
            "P" => EventType.PinMode,
            _ => EventType.Unknown
        };

        if (parts.Length > COMMON_PARTS) {
            @event.Args = parts.Skip(COMMON_PARTS)
                .Take(Event.MAX_ARGS)
                .Select(int.Parse).ToArray();
        }
        
        return true;
    }
}