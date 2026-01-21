namespace ComponentManagement.Components;

public class Led : Component {
    private const string SPRITE_LED_ON = "ledOn";
    private const string SPRITE_LED_OFF = "ledOff";

    private const string PIN_ANODE = "anode";
    private const string PIN_CATHODE = "cathode";
    
    public Led(string definitionPath) : base(definitionPath) {}

    public override void OnRisingEdge(Pin pin) {
        UpdateSprite(SPRITE_LED_ON);
    }

    public override void OnFallingEdge(Pin pin) {
        UpdateSprite(SPRITE_LED_OFF);
    }

    private void PassCurrent(Pin pin) {
        
    }
}