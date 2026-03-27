using IpcAdapter.Events;

namespace IpcAdapter.Encoders;

public class TextEncoder : IEncoder{
    public string EncodeEvent(Event @event) {
        throw new NotImplementedException();
    }
    
    public Event DecodeEvent(string message) {
        throw new NotImplementedException();
    }
}