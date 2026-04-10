namespace IpcAdapter.Events;

public enum EventType { Unknown, Write, PinMode }

public struct Event {
    public const int MAX_ARGS = 3;
    
    public EventType Type;
    public long TimestampMicros;
    public long LocalVirtualTime;
    public int[] Args = new int[MAX_ARGS];

    public Event() { }
}