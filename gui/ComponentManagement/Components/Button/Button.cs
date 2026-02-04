using System.Numerics;
using ComponentManagement.Graph;

namespace ComponentManagement.Components.Button;

public class Button : Component {
    private const string SPRITE_BUTTON_UP =  "buttonUp";
    private const string SPRITE_BUTTON_DOWN =  "buttonDown";
    
    private bool _isPushed = false;

    public Button(string definitionPath) : base(definitionPath) {
        UpdateSprite(SPRITE_BUTTON_UP);
    }

    public override void OnInputChange(Pin pin, float voltage) => UpdateVoltages();
    public override void OnPinConnected(Pin pin) => UpdateVoltages();
    public override void OnPinDisconnected(Pin pin) => UpdateVoltages();

    public override void OnControlPress(Vector2 cursorPosition) {
        _isPushed = true;
        UpdateSprite(SPRITE_BUTTON_DOWN);
        UpdateVoltages();
    }

    public override void OnControlRelease() {
        _isPushed = false;
        UpdateSprite(SPRITE_BUTTON_UP);
        UpdateVoltages();
    }

    private void UpdateVoltages() {
        Pin higherVoltagePin = Pins.MaxBy(p => p.Voltage)!;
        Pin lowerVoltagePin = GetPin(1 - higherVoltagePin.Id);

        if (_isPushed) {
            lowerVoltagePin.SetVoltage(higherVoltagePin.Voltage);
        }
        else {
            // TODO: Blatantly wrong
            lowerVoltagePin.SetVoltage(Pin.GND_VOLTAGE);
        }
    }
}