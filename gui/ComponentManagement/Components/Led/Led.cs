namespace ComponentManagement.Components;

public class Led : Component {
    private const string SPRITE_LED_ON = "ledOn";
    private const string SPRITE_LED_OFF = "ledOff";

    private const string PIN_ANODE = "anode";
    private const string PIN_CATHODE = "cathode";
    
    public Led(string definitionPath) : base(definitionPath) {}

    public override void OnRisingEdge(Pin pin) {
        UpdateSprite(SPRITE_LED_ON);
        PassCurrent(pin);
    }

    public override void OnFallingEdge(Pin pin) {
        UpdateSprite(SPRITE_LED_OFF);
        PassCurrent(pin);
    }

    private void PassCurrent(Pin inputPin) {
        Pin outputPin = inputPin.Name == PIN_ANODE ?
            GetPin(PIN_CATHODE) :
            GetPin(PIN_ANODE);
        
        outputPin.SetCurrent(inputPin.Current);
        outputPin.SetVoltage(inputPin.Voltage);
    }
}