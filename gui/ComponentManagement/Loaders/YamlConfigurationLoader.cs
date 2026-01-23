using YamlDotNet.RepresentationModel;
using YamlDotNet.Serialization;

namespace ComponentManagement.Loaders;

public static class YamlConfigurationLoader {
    public static ComponentConfiguration LoadYaml(string path) {
        using var file = File.OpenRead(path);
        var fileStream = new StreamReader(file);
        
        var deserializer = new Deserializer();
        var config = deserializer.Deserialize<ComponentConfiguration>(fileStream);
        return config;
    }
}