namespace ComponentManagement.Components;

public class Arduino : Component  {
    public int TcpPort { get; set; }
    
    public Arduino(string definitionPath) : base(definitionPath) { }
}