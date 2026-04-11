namespace IpcAdapter.Events;

public class EventQueue {
    private readonly Queue<Event> _localEvents = new();
    private readonly Queue<Event> _remoteEvents = new();
    private readonly Queue<Event> _processedEvents = new();

    private long _nextLvt;

    public void EnqueueLocal(ref Event @event) {
        @event.LocalVirtualTime = _nextLvt;
        _nextLvt++;
        _localEvents.Enqueue(@event);
    }

    public void EnqueueRemote(Event @event) {
        _remoteEvents.Enqueue(@event);
    }

    public void ExecuteNextEvent() {
        Queue<Event> targetQueue = (EarliestQueueLvt(_localEvents) < EarliestQueueLvt(_remoteEvents)) 
            ? _localEvents 
            : _remoteEvents;

        Event processedEvent = targetQueue.Dequeue();
        processedEvent.Action?.Invoke();

        _nextLvt = Math.Max(_nextLvt, processedEvent.LocalVirtualTime + 1);
        _processedEvents.Enqueue(processedEvent);
    }

    public void Clear() {
        _localEvents.Clear();
        _remoteEvents.Clear();
        _processedEvents.Clear();
        _nextLvt = 0;
    }

    private long EarliestQueueLvt(Queue<Event> queue) {
        if (queue.Count > 0) {
            return queue.Peek().LocalVirtualTime;
        }
        return long.MaxValue;
    }
}