using ComponentManagement.Scenes;
using YamlDotNet.RepresentationModel;
using YamlDotNet.Serialization;
using YamlDotNet.Serialization.NamingConventions;

namespace ComponentManagement.Loaders;

public static class YamlConfigurationLoader {
    public static ComponentConfiguration LoadYaml(string componentPath) {
        var fileName = Path.GetFileName(componentPath) + ".yaml";           // find file with the same name as the directory
        using var file = File.OpenRead(componentPath + "/" + fileName);
        var fileStream = new StreamReader(file);

        var deserializer = new DeserializerBuilder()
            .WithCaseInsensitivePropertyMatching()
            .Build();
            
        var config = deserializer.Deserialize<ComponentConfiguration>(fileStream);
        return config;
    }
}