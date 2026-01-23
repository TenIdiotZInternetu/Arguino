namespace ComponentManagement.Graph;

public class ComponentNode {
    private Component _component;
    private HashSet<ComponentNode> _connectedNodes = new();

    public ComponentNode(Component component) {
        _component = component;
    }

    // TODO: Pin or Connection abstraction?
    public void ConnectNode(Pin myPin, Pin theirPin) {}
    
    public void UpdateConnectedNodes(Pin myPIn) {} 
}