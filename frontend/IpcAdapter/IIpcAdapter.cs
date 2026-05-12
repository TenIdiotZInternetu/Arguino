using System;
using IpcAdapter.Events;

namespace IpcAdapter;

public interface IIpcAdapter {
    private const int RECONNECT_DELAY = 3000;
    public bool IsConnected { get; }
    public event Action<Event> ReceivedEventEvent;
    
    public void SendEvent(Event @event);
    
    public Task<bool> TryConnectAsync();
    public void Disconnect();

    public async Task ConnectAsync() {
        while (!await TryConnectAsync()) {
            await Task.Delay(RECONNECT_DELAY);
        }
    }
    
    public void Reconnect() {
        Disconnect();
        Task.Run(ConnectAsync);
    }
}