using System.Diagnostics;
using ComponentManagement.Circuitry;
using ComponentManagement.Scenes;

namespace ComponentManagement.Components;

public class Led : Component {
    private const string SPRITE_LED_ON = "ledOn";
    private const string SPRITE_LED_OFF = "ledOff";

    private const string PIN_CATHODE = "cathode";
    private const string PIN_ANODE = "anode";

    private const int INTEGRATOR_INTERVAL_MS = 30;
    private const int INTEGRATOR_TRESHOLD_MS = 4;
    private Timer _integratorTimer;
    
    private Pin _cathode = null!;
    private Pin _anode = null!;

    private readonly SynchronizationContext _uiContext;
    private readonly Stopwatch _stopwatch = new();

    private TimeSpan _lastMeasuredTime;
    private TimeSpan _integratedTime = TimeSpan.Zero;
    private bool _turnedOn;

    public Led(string typeName) : base(typeName) {
        _uiContext = SynchronizationContext.Current!;
    }

    internal override void OnInitialized() {
        UpdateSprite(SPRITE_LED_OFF);
        _cathode = GetPin(PIN_CATHODE)!;
        _cathode.MakeReadOnly();
        _anode = GetPin(PIN_ANODE)!;

        _integratorTimer = new(OnIntegratorTimer, null, INTEGRATOR_INTERVAL_MS, INTEGRATOR_INTERVAL_MS);
        _stopwatch.Start();
        _lastMeasuredTime = _stopwatch.Elapsed;
    }

    public override void OnPinStateChanged(Pin pin) {
        if (pin != _cathode) return;
        
        if (pin.IsHigh) {
            _lastMeasuredTime = _stopwatch.Elapsed;
        }
        else {
            _integratedTime += _stopwatch.Elapsed - _lastMeasuredTime;
        }
    }

    private void OnIntegratorTimer(object? _) {
        if (_cathode.IsHigh) {
            _integratedTime += _stopwatch.Elapsed - _lastMeasuredTime;
        }
        
        var shouldBeOn = _integratedTime.TotalMilliseconds >= INTEGRATOR_TRESHOLD_MS;
        _lastMeasuredTime = _stopwatch.Elapsed;
        _integratedTime = TimeSpan.Zero;
        
        if (shouldBeOn == _turnedOn) {
            return;
        }

        _turnedOn = shouldBeOn;
        
        _uiContext.Post(_ => {
            UpdateSprite(_turnedOn ? SPRITE_LED_ON : SPRITE_LED_OFF);
        }, null);
    }
}