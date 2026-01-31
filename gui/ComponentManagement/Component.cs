using System.Numerics;
using System.Reflection;
using ComponentManagement.Graph;
using ComponentManagement.Loaders;
using Svg.Skia;

namespace ComponentManagement;

public abstract class Component {
    
    // TODO: Replace ComponentConfiguration by an Interface
    
    public string Name => _configuration.Name;
    public string Description => _configuration.Description;

    public Transform Transform { get; set; } = new();
    
    public Dictionary<string, SKSvg> Sprites { get; private set; }
    public SKSvg CurrentSprite { get; private set; }
    public event Action<SKSvg>? SpriteChangedEvent;
    
    private ComponentConfiguration _configuration;
    private List<Pin> _pins = new();
    private ComponentNode _thisNode;
    
    public Component(string definitionPath) {
        _thisNode = new ComponentNode(this);
        _configuration = YamlConfigurationLoader.LoadYaml(definitionPath);

        InitPins();
        Sprites = SkiaSvgLoader.LoadSvgs(definitionPath);
        CurrentSprite = Sprites.First().Value;
    }
    
    public virtual void OnInitialization() {}
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

    private void InitPins() {
        uint pinId = 0;
        foreach (var pinName in _configuration.PinNames) {
            _pins.Add(new Pin(this, pinId, pinName));
            pinId++;
        }

        for (; pinId < _pins.Count; pinId++) {
            _pins.Add(new Pin(this, pinId));
        }
    }
}
