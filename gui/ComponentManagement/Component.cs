using System.Numerics;
using System.Reflection;
using ComponentManagement.Graph;
using ComponentManagement.Loaders;
using Svg.Skia;

namespace ComponentManagement;

public abstract class Component {
    public string Name { get; set; }
    public string TypeName => Configuration.Name;
    public string Description => Configuration.Description;

    public Transform Transform { get; set; } = new();
    
    public Dictionary<string, SKSvg> Sprites { get; private set; }
    public SKSvg CurrentSprite { get; private set; }
    public event Action<SKSvg>? SpriteChangedEvent;
    
    // TODO: Replace ComponentConfiguration by an Interface
    protected ComponentConfiguration Configuration;
    protected List<Pin> Pins = [];
    
    public Component(string definitionPath) {
        Configuration = YamlConfigurationLoader.LoadYaml(definitionPath);

        InitPins();
        Sprites = SkiaSvgLoader.LoadSvgs(definitionPath);
        CurrentSprite = Sprites.First().Value;
    }
    
    public virtual void OnPinConnected(Pin pin) {}
    public virtual void OnPinDisconnected(Pin pin) {}
    public virtual void OnInputChange(Pin pin, float voltage) {}
    public virtual void OnControlPress(Vector2 cursorPosition) {}
    public virtual void OnControlRelease() {}
    // TODO: OnInspect()

    // TODO: Log getting unknown Pins and Sprites
    public Pin GetPin(string name) {
        return Pins.First(pin => pin.Name == name);
    }

    public Pin GetPin(uint id) {
        return Pins.First(pin => pin.Id == id);
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
        foreach (var pinName in Configuration.PinNames) {
            Pins.Add(new Pin(this, pinId, pinName));
            pinId++;
        }

        for (; pinId < Configuration.Pins; pinId++) {
            Pins.Add(new Pin(this, pinId));
        }

        foreach (Pin pin in Pins) {
            pin.VoltageChangedEvent += OnInputChange;
            pin.PinConnectedEvent += OnPinConnected;
            pin.PinDisconnectedEvent += OnPinDisconnected;
        }
    }
}
