using ComponentManagement.Graph;
using Svg;

namespace ComponentManagement.Factory;

public interface ISceneLoader {
    public void LoadFile(string scenePath);
    public List<string> GetComponentTypeNames();
    public List<Component> GetComponentsOfType(string typeName);
    public Dictionary<string, Component> InstantiateComponents();
    public List<ElectricalNode> InstantiateNodes();
}