using ComponentManagement.Scenes;

namespace ComponentManagement.Factory;

public interface IConfigurationLoader {
    public ComponentConfiguration? LoadConfig(string configPath);
}