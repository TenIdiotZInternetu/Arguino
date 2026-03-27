using System;
using IpcAdapter.Events;

namespace IpcAdapter;

public interface IIpcAdapter {
    public bool IsConnected { get; protected set; }
    public event Action<Event> ReceivedEventEvent;

    public void SendEvent(Event @event);
}