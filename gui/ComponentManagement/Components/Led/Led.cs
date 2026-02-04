using System.Runtime.Serialization;
using ComponentManagement.Graph;

namespace ComponentManagement.Components;

public class Led : Component {
    private const float FORWARD_VOLTAGE = 3.3f;

    private const string SPRITE_LED_ON = "ledOn";
    private const string SPRITE_LED_OFF = "ledOff";

    private const string PIN_ANODE = "anode";
    private const string PIN_CATHODE = "cathode";

    private Pin _anode;
    private Pin _cathode;

    public Led(string definitionPath) : base(definitionPath) {
        UpdateSprite(SPRITE_LED_OFF);
        _anode = GetPin(PIN_ANODE);
        _cathode = GetPin(PIN_CATHODE);
    }

    public override void OnInputChange(Pin pin, float voltage) {
        float anodeVoltage = _anode.IsConnected ?
            _anode.Voltage :
            Pin.GND_VOLTAGE;

        bool conducts = _cathode.Voltage - anodeVoltage > FORWARD_VOLTAGE;

        if (conducts) {
            _anode.SetVoltage(voltage);
            UpdateSprite(SPRITE_LED_ON);
        }
        else {
            UpdateSprite(SPRITE_LED_OFF);
        }
    }
}