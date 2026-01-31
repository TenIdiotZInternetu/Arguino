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

    public override void OnInputChange(Pin pin) {
        if (!_isPushed) {
            return;
        }
        
        Pin otherPin = GetPin(1 - pin.Id);
        otherPin.SetCurrent(pin.Current);
    }

    public override void OnControlPress(Vector2 cursorPosition) {
        _isPushed = true;
        UpdateSprite(SPRITE_BUTTON_DOWN);
    }

    public override void OnControlRelease() {
        _isPushed = false;
        UpdateSprite(SPRITE_BUTTON_UP);
    }
}