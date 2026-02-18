using System.Numerics;
using ComponentManagement.Graph;
using ComponentManagement.Loaders;
using ComponentManagement.Scenes;
using Logger;
using Svg.Skia;

namespace ComponentManagement;

public abstract class Component {
    public required string Name { get; set; }
    public string TypeName { get; init; }

    public ComponentConfiguration Configuration => _configs[TypeName];
    public Transform Transform { get; set; } = new();
    public SKSvg CurrentSprite { get; private set; }
    
    public event Action<SKSvg>? SpriteChangedEvent;

    private readonly List<Pin> _pins = [];
    private static readonly Dictionary<string, ComponentConfiguration> _configs = [];
    
    internal Component(string typeName) {
        TypeName = typeName;
        
        InitPins();
        CurrentSprite = Configuration.Sprites.First().Value;
    }
    
    public virtual void OnInitialized() {}
    public virtual void OnPinConnected(Pin pin) {}
    public virtual void OnPinDisconnected(Pin pin) {}
    public virtual void OnPinStateChanged(Pin pin) {}
    public virtual void OnControlPress(Vector2 cursorPosition) {}
    public virtual void OnControlRelease() {}
    // TODO: OnInspect()

    public Pin? GetPin(string name) {
        try {
            return _pins.First(pin => pin.Name == name);
        }
        catch (InvalidOperationException) {
            ComponentManager.LogError($"Accessing unknown pin '{name}' of the component {this}");
            return null;
        }
    }

    public Pin? GetPin(uint id) {
        try {
            return _pins.First(pin => pin.Id == id);
        }
        catch (InvalidOperationException) {
            ComponentManager.LogError($"Accessing unknown pin with '{id}' of the component {this}");
            return null;
        }
    }

    internal static void AddConfiguration(ComponentConfiguration config) {
        _configs.TryAdd(config.Name, config);
    }

    protected void UpdateSprite(SKSvg sprite) {
        if (CurrentSprite == sprite) return;
        
        CurrentSprite = sprite;
        SpriteChangedEvent?.Invoke(sprite);
    }

    protected void UpdateSprite(string spriteName) {
        try {
            UpdateSprite(Configuration.Sprites[spriteName]);
        }
        catch (IndexOutOfRangeException) {
            ComponentManager.LogError($"Accessing unknown sprite '{spriteName}' of the component {this}");
        }
    }

    private void InitPins() {
        foreach (Pin pin in _pins) {
            pin.StateChangedEvent += OnPinStateChanged;
            pin.PinConnectedEvent += OnPinConnected;
            pin.PinDisconnectedEvent += OnPinDisconnected;
        }
    }

    private void OnPinStateChanged(Pin pin, DigitalState _) => OnPinStateChanged(pin);

    public override string ToString() => Name;
}
