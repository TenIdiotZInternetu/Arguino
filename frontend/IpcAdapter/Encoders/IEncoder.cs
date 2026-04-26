using IpcAdapter.Events;

namespace IpcAdapter.Encoders;

public interface IEncoder {
    public string EncodeEvent(Event @event);
    public bool TryDecodeEvent(string message, out Event @event);
}