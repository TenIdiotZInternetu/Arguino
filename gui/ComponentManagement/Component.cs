using System.Numerics;
using System.Reflection;
using Svg.Skia;

namespace ComponentManagement;

public abstract class Component {
    public event Action<SKSvg>? SpriteChangedEvent;
    
    protected Dictionary<string, SKSvg> Sprites;
    protected SKSvg CurrentSprite;
    
    private List<Pin> _pins;
    private ComponentNode _thisNode;
    
    public Component(string definitionPath) {
        _thisNode = new ComponentNode(this);
        Sprites = SkiaSvgLoader.LoadSvgs(definitionPath);
        CurrentSprite = Sprites.First().Value;

        foreach (var pin in _pins) {
            pin.ValuesChangedEvent += _thisNode.UpdateConnectedNodes;
        }
    }
    
    public virtual void OnInitializationAsync() {}
    public virtual void OnRisingEdge(Pin pin) {}
    public virtual void OnFallingEdge(Pin pin) {}
    public virtual void OnInputChange(Pin pin) {}
    public virtual void OnControlPress(Vector2 cursorPosition) {}
    public virtual void OnControlRelease(Vector2 cursorPosition) {}
    // TODO: OnInspect()

    public Pin GetPin(string name) {
        return _pins.First(pin => pin.Name == name);
    }

    public Pin GetPin(uint id) {
        return _pins.First(pin => pin.Id == id);
    }

    protected void UpdateSprite(SKSvg sprite) {
        if (CurrentSprite == sprite) return;
        
        CurrentSprite = sprite;
        SpriteChangedEvent?.Invoke(sprite);
    }

    protected void UpdateSprite(string spriteName) {
        UpdateSprite(Sprites[spriteName]);
    }
}