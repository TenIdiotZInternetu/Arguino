using System.Diagnostics;
using System.Numerics;
using System.Reflection;
using ComponentManagement.Graph;
using ComponentManagement.Scenes;
using ComponentManagement.Utils;
using Logger;
using YamlDotNet.Serialization;

namespace ComponentManagement.Loaders;

using ComponentsMap = Dictionary<string, YamlSceneLoader.SceneDto.ComponentDto>;
using NodesList = List<List<string>>;

public static class YamlSceneLoader {
    public record SceneDto {
        public required ComponentsMap Components { get; init; } = [];
        public required NodesList Nodes { get; init; } = [];

        public record ComponentDto {
            public required string Type;
            public string Position = "0 0";
            public float Rotation = 0;
            public string Scale = "1 1";
            
            // TODO: Flatten extra properties
            public Dictionary<string, object>? ExtraProps { get; set; } 
        }
    }

    private static readonly Dictionary<string, Type> TYPE_NAMES_MAP = [];

    public static Scene LoadScene(string scenePath, string componentsPath) {
        var sceneDto = DeserializeYaml(scenePath);

        var instantiatedComponents = InstantiateComponents(sceneDto.Components, componentsPath);
        var instantiatedNodes = InstantiatedNodes(sceneDto.Nodes, instantiatedComponents);

        return new Scene {
            ComponentsMap = instantiatedComponents,
            Nodes = instantiatedNodes
        };
    }

    private static SceneDto DeserializeYaml(string scenePath) {
        using var file = File.OpenRead(scenePath);
        var fileStream = new StreamReader(file);

        var deserializer = new DeserializerBuilder()
            .WithCaseInsensitivePropertyMatching()
            .Build();
            
        return deserializer.Deserialize<SceneDto>(fileStream);
    }

    private static Dictionary<string, Component> InstantiateComponents(ComponentsMap componentDtos, string componentsPath) {
        Dictionary<string, Component> components = [];
        
        foreach (var (name, dto) in componentDtos) {
            string typeName = dto.Type;
            string componentDir = componentsPath + "/" + typeName;
            Type? type;


            if (!TYPE_NAMES_MAP.TryGetValue(typeName, out type)) {
                type = MapComponentType(typeName);
                if (type == null) continue;
            }


            if (Activator.CreateInstance(type, componentDir) is Component compInstance) {
                compInstance.Name = name;
                compInstance.Transform = ParseTransform(dto);
                HandleExtraProps(compInstance, dto.ExtraProps);
                components.Add(name, compInstance);
                ComponentManager.LogInfo($"Instantiated component {name} of type {typeName}");
            }
            else {
                ComponentManager.LogError($"{typeName} instance could not be created.");
            }
        }

        return components;
    }

    private static Type? MapComponentType(string typeName) {
        // TODO: Change assembly for custom added scripts

        Type type;

        try {
            type = Assembly.GetExecutingAssembly()
                .GetTypes()
                .First(t => t.Name == typeName);
        }
        catch (InvalidOperationException) {
            ComponentManager.LogError($"Component of type {typeName} is not defined.");
            return null;
        }

        TYPE_NAMES_MAP.Add(typeName, type);
        ComponentManager.LogInfo($"Defined the component type {typeName}");
        return type;
    }

    private static Transform ParseTransform(SceneDto.ComponentDto compDto) {
        return new Transform {
            Position = YamlUtils.StringToVector2(compDto.Position) ?? Vector2.Zero,
            Rotation = compDto.Rotation,
            Scale = YamlUtils.StringToVector2(compDto.Scale) ?? Vector2.One
        };
    }

    private static void HandleExtraProps(Component component, Dictionary<string, object>? properties) {
        if (properties == null) return;        
        
        foreach (var (key, value) in properties) {
            Type type = component.GetType();
            var property = type.GetProperty(key);

            if (property == null) {
                ComponentManager.LogWarning($"Unknown extra property '{key}' of {component} found in the scene's YAML");
                continue;
            }
            
            var convertedValue = Convert.ChangeType(value, property.PropertyType);
            property.SetValue(component, convertedValue);
            ComponentManager.LogDebug($"Added extra property '{key}' to {component}");
        }
    }

    private static List<ElectricalNode> InstantiatedNodes(NodesList nodesDto, Dictionary<string, Component> components) {
        List<ElectricalNode> nodes = [];

        foreach (List<string> nodeDto in nodesDto) {
            ElectricalNode node = new();

            foreach (string pinDto in nodeDto) {
                string[] pinDtoElements = pinDto.Split('.');
                string compName = pinDtoElements[0];
                string compPinName = pinDtoElements[1];

                Component component = components[compName];
                Pin? pin = uint.TryParse(compPinName, out uint pinId) ?
                    component.GetPin(pinId) :
                    component.GetPin(compPinName);

                if (pin != null) {
                    node.AddPin(pin);
                }
                else {
                    ComponentManager.LogError($"Failed to add a pin to the node '{node}'");
                }
                
                // TODO: Transitivity through nodes
            }

            nodes.Add(node);
            ComponentManager.LogInfo($"Instantiated node '{node}'");
        }

        return nodes;
    }
}