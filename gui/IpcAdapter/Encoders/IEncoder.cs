using IpcAdapter.Events;

namespace IpcAdapter.Encoders;

public interface IEncoder {
    public string EncodeEvent(Event @event);
    public Event DecodeEvent(string message);
}