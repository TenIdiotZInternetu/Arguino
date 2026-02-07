using ComponentManagement.Graph;

namespace ComponentManagement.Scenes;

public class Scene {
    public Dictionary<string, Component> ComponentsMap { get; init; } = [];
    public List<ElectricalNode> Nodes { get; init; } = [];
}
        