using System.Numerics;
using ComponentManagement.Graph;
using Svg.Skia;

namespace ComponentManagement.Scenes;

public record struct ComponentConfiguration() {
    public static readonly Vector2 DEFAULT_IMAGE_SIZE = new(50, 50); 

    public required string Name;
    public string Description = "";
    public Vector2 ImageSize = DEFAULT_IMAGE_SIZE;
    public List<PinPrototype> Pins = [];
    public Dictionary<string, SKSvg> Sprites = [];
    // TODO: Pin positions (probably through svg element ids)
    // TODO: Child components
}