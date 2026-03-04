using ComponentManagement.Graph;

namespace ComponentManagement.Factory;

public interface ISceneLoader {
    public void LoadScene(string scenePath);
    public List<string> GetComponentTypeNames();
    public Dictionary<string, Component> InstantiateComponents();
    public List<ElectricalNode> InstantiateNodes();
}