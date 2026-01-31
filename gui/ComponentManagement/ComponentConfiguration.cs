namespace ComponentManagement;

public record ComponentConfiguration {
    public required string Name;
    public string Description = "";
    public uint Pins = 0;
    public List<string> PinNames = new();
    // TODO: Pin positions (probably through svg element ids)
    // TODO: Child components
}