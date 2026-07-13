using ComponentManagement.Circuitry;
using ComponentManagement.Scenes;

namespace ComponentManagement.Factory;

public interface ISceneLoader {
    // TODO - move instantiations to LoadFile, and only expose Type/Component/Node getters

    public void LoadFile(string scenePath);
    public List<string> GetComponentTypeNames();
    public List<Component> GetComponentsOfType(string typeName);
    public Dictionary<string, Component> InstantiateComponents();
    public List<ElectricalNode> InstantiateNodes();
}