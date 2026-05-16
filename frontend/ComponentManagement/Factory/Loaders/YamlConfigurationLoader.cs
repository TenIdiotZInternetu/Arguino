using ComponentManagement.Graph;
using ComponentManagement.Scenes;
using ComponentManagement.Utils;
using YamlDotNet.Core;
using YamlDotNet.Serialization;

namespace ComponentManagement.Factory.Loaders;

public class YamlConfigurationLoader : IConfigurationLoader {
    public record struct ConfigDto() {
        public required string Name;
        public string Description = "";
        public string ImageSize = "";
        public uint Pins = 0;
        public List<string> PinNames = [];
        // TODO: Pin positions (probably through svg element ids)
        // TODO: Child components
    }

    public ComponentConfiguration? LoadConfig(string configPath) {
        try {
            using var file = File.OpenRead(configPath);
            using var fileStream = new StreamReader(file);

            var deserializer = new DeserializerBuilder()
                .WithCaseInsensitivePropertyMatching()
                .Build();
                
            var configDto = deserializer.Deserialize<ConfigDto>(fileStream);
            return CreateConfig(configDto, configPath);
        }
        catch (YamlException e) {
            ComponentManager.LogError($"Component Configuration: Could not deserialize {configPath}. Exception: {e.Message}");
            return null;
        }
        catch (Exception e) {
            ComponentManager.LogError($"Component Configuration: Could not deserialize {configPath}. Exception: {e.Message}");
            return null;
        }
    }

    private ComponentConfiguration CreateConfig(ConfigDto configDto, string configPath) {
        return new ComponentConfiguration {
            Name = configDto.Name,
            ComponentPath = Path.GetDirectoryName(configPath)!,
            Description = configDto.Description,
            ImageSize = YamlUtils.StringToVector2(configDto.ImageSize) ?? ComponentConfiguration.DEFAULT_IMAGE_SIZE,
            Pins = CreatePins(configDto)
        };
    }

    private List<PinPrototype> CreatePins(ConfigDto configDto) {
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