using ComponentManagement.Graph;
using ComponentManagement.Scenes;
using ComponentManagement.Utils;
using YamlDotNet.Core;
using YamlDotNet.Serialization;

namespace ComponentManagement.Loaders;

public static class YamlComponentLoader {
    public record struct ConfigDto() {
        public required string Name;
        public string Description = "";
        public string ImageSize = "";
        public uint Pins = 0;
        public List<string> PinNames = [];
        // TODO: Pin positions (probably through svg element ids)
        // TODO: Child components
    }

    public static ComponentConfiguration? LoadYaml(string componentPath) {
        var fileName = Path.GetFileName(componentPath) + ".yaml";     // find file with the same name as the directory

        try {
            using var file = File.OpenRead(componentPath + "/" + fileName);
            using var fileStream = new StreamReader(file);

            var deserializer = new DeserializerBuilder()
                .WithCaseInsensitivePropertyMatching()
                .Build();
                
            var configDto = deserializer.Deserialize<ConfigDto>(fileStream);
            return CreateConfig(configDto);
        }
        catch (FileNotFoundException e) {
            ComponentManager.LogError($"Component Configuration: Could not find .yaml file at {componentPath}. Exception: {e.Message}");
            return null;
        }
        catch (YamlException e) {
            ComponentManager.LogError($"Component Configuration: Could not deserialize {fileName}. Exception: {e.Message}");
            return null;
        }
        catch (Exception e) {
            ComponentManager.LogError($"Component Configuration: Could not deserialize {fileName}. Exception: {e.Message}");
            return null;
        }
    }

    private static ComponentConfiguration CreateConfig(ConfigDto configDto) {
        return new ComponentConfiguration {
            Name = configDto.Name,
            Description = configDto.Description,
            ImageSize = YamlUtils.StringToVector2(configDto.ImageSize) ?? ComponentConfiguration.DEFAULT_IMAGE_SIZE,
            Pins = CreatePins(configDto)
        };
    }

    private static List<PinPrototype> CreatePins(ConfigDto configDto) {
        List<PinPrototype> pins = [];

        uint pinId = 0;
        for (; pinId < configDto.Pins; pinId++) {
            pins.Add(new PinPrototype(pinId));
        }

        foreach (var pinName in configDto.PinNames) {
            pins.Add(new PinPrototype(pinId, pinName));
            pinId++;
        }

        return pins;
    }
}