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

    public override void OnPinStateChanged(Pin pin) => UpdatePins();

    public override void OnControlPress(Vector2 cursorPosition) {
        _isPushed = true;
        UpdateSprite(SPRITE_BUTTON_DOWN);
        UpdatePins();
    }

    public override void OnControlRelease() {
        _isPushed = false;
        UpdateSprite(SPRITE_BUTTON_UP);
        UpdatePins();
    }

    private void UpdatePins() {
        UpdatePin(GetPin(0)!);
        UpdatePin(GetPin(1)!);
    }

    private void UpdatePin(Pin pin) {
        Pin otherPin = GetPin(1 - pin.Id)!;

        if (otherPin.IsHigh && _isPushed) {
            pin.SetHigh();
        }
        else {
            pin.SetLow();
        }
    }
}