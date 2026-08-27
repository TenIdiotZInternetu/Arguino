using ComponentManagement.Circuitry;
using ComponentManagement.Scenes;

namespace ComponentManagement.Components;

public class Led : Component {
    private const string SPRITE_LED_ON = "ledOn";
    private const string SPRITE_LED_OFF = "ledOff";

    private const string PIN_CATHODE = "cathode";
    private const string PIN_ANODE = "anode";

    private const int TURNOFF_DELAY_MS = 15;
    private Timer _turnOffTimer;
    
    private Pin _cathode = null!;
    private Pin _anode = null!;

    private readonly SynchronizationContext _uiContext;

    public Led(string typeName) : base(typeName) {
        _uiContext = SynchronizationContext.Current!;
    }

    internal override void OnInitialized() {
        UpdateSprite(SPRITE_LED_OFF);
        _cathode = GetPin(PIN_CATHODE)!;
        _cathode.MakeReadOnly();
        _anode = GetPin(PIN_ANODE)!;

        _turnOffTimer = new(OnTurnOffTimer, null, TURNOFF_DELAY_MS, Timeout.Infinite);
    }

    public override void OnPinStateChanged(Pin pin) {
        if (pin != _cathode) return;
        
        if (pin.IsLow) {
            StartTimer();
        }
        else {
            StopTimer();
            UpdateSprite(SPRITE_LED_ON);
        }
    }

    private void OnTurnOffTimer(object? _) {
        _uiContext.Post(_ => {
            if (_cathode.IsLow) {
                UpdateSprite(SPRITE_LED_OFF);
            }
        }, null);
    }

    private void StartTimer() =>
        _turnOffTimer.Change(TURNOFF_DELAY_MS, Timeout.Infinite);

    private void StopTimer() =>
        _turnOffTimer.Change(Timeout.Infinite, Timeout.Infinite);
}