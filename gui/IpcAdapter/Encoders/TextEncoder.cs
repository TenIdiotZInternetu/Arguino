using IpcAdapter.Events;

namespace IpcAdapter.Encoders;

public class TextEncoder : IEncoder {
    public string EncodeEvent(Event @event) {
        return "encoded";
    }
    
    public Event DecodeEvent(string message) {
        return new Event();
    }
}