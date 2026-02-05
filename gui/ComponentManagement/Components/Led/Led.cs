using System.Runtime.Serialization;
using ComponentManagement.Graph;

namespace ComponentManagement.Components;

public class Led : Component {
    private const float FORWARD_VOLTAGE = 3.3f;

    private const string SPRITE_LED_ON = "ledOn";
    private const string SPRITE_LED_OFF = "ledOff";

    private const string PIN_CATHODE = "cathode";
    private const string PIN_ANODE = "anode";

    private Pin _cathode;
    private Pin _anode;

    public Led(string definitionPath) : base(definitionPath) {
        UpdateSprite(SPRITE_LED_OFF);
        _cathode = GetPin(PIN_CATHODE);
        _cathode.MakeReadOnly();
        _anode = GetPin(PIN_ANODE);
    }

    public override void OnPinStateChanged(Pin pin) {
        UpdateSprite(_cathode.IsHigh ? SPRITE_LED_ON : SPRITE_LED_OFF);
    }
}