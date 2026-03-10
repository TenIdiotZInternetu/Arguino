using System.Numerics;
using System.Reflection;
using ComponentManagement.Graph;
using ComponentManagement.Scenes;
using ComponentManagement.Utils;
using YamlDotNet.Serialization;

namespace ComponentManagement.Factory.Loaders;

// TODO: Too many responsibilities

public class YamlSceneLoader : ISceneLoader {
    public class ComponentsMap : Dictionary<string, SceneDto.ComponentDto>;
    public class TypesMap : Dictionary<string, ComponentsMap>;
    public class NodesList : List<List<string>>;

    public record SceneDto {
        public required TypesMap Components { get; init; } = [];
        public required NodesList Nodes { get; init; } = [];

        public record ComponentDto {
            public string Position = "0 0";
            public float Rotation = 0;
            public string Scale = "1 1";
            
            // TODO: Flatten extra properties
            public Dictionary<string, object>? ExtraProps { get; set; } 
        }
    }

    private SceneDto? _sceneDto;
    private readonly Dictionary<string, Type> _typesByName = [];
    private readonly Dictionary<string, Component> _instancesByName = [];
    private readonly Dictionary<string, List<Component>> _instancesByType = [];

    public Dictionary<string, Component> InstantiateComponents() {
        if (_sceneDto?.Nodes == null) {
            throw new InvalidOperationException("Instantiating components before loading them from file");
        }

        foreach (var (typeName, componentDtos) in _sceneDto.Components) {
            Type? type;

            if (!_typesByName.TryGetValue(typeName, out type)) {
                type = MapComponentType(typeName);
                if (type == null) continue;
            }

            foreach (var (compName, compDto) in componentDtos) {
                CreateComponentInstance(typeName, compName, compDto);
            }
        }

        return _instancesByName;
    }

    private Type? MapComponentType(string typeName) {
        // TODO: Change assembly for custom added scripts

        try {
            Type type = Assembly.GetExecutingAssembly()
                .GetTypes()
                .First(t => t.Name == typeName);

            _typesByName.Add(typeName, type);
            _instancesByType.Add(typeName, []);
            return type;
        }
        catch (InvalidOperationException) {
            ComponentManager.LogError($"Component of type {typeName} is not defined.");
            return null;
        }
    }

    private Transform ParseTransform(SceneDto.ComponentDto compDto) {
        return new Transform {
            Position = YamlUtils.StringToVector2(compDto.Position) ?? Vector2.Zero,
            Rotation = compDto.Rotation,
            Scale = YamlUtils.StringToVector2(compDto.Scale) ?? Vector2.One
        };
    }

    private void CreateComponentInstance(string typeName, string componentName, SceneDto.ComponentDto componentDto) {
        try {
            Type type = _typesByName[typeName];
            Component compInstance = (Activator.CreateInstance(type, typeName) as Component)!;

            compInstance.Name = componentName;
            compInstance.Transform = ParseTransform(componentDto);
            HandleExtraProps(compInstance, componentDto.ExtraProps);

            _instancesByType[typeName].Add(compInstance);
            _instancesByName[componentName] = compInstance;
            ComponentManager.LogInfo($"Instantiated component {componentName} of type {typeName}");
        }
        catch {
            ComponentManager.LogError($"Failed to create instance {componentName} of type {typeName}.");
        }
    }

    private void HandleExtraProps(Component component, Dictionary<string, object>? properties) {
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

    public List<ElectricalNode> InstantiateNodes() {
        if (_sceneDto?.Nodes == null) {
            throw new InvalidOperationException("Instantiating nodes before loading them from file");
        }

        List<ElectricalNode> nodes = [];

        foreach (List<string> nodeDto in _sceneDto.Nodes) {
            ElectricalNode node = new();

            foreach (string pinDto in nodeDto) {
                string[] pinDtoElements = pinDto.Split('.');
                string compName = pinDtoElements[0];
                string compPinName = pinDtoElements[1];

                if (!_instancesByName.TryGetValue(compName, out Component? component)) {
                    ComponentManager.LogError($"Component instance {compName} in node definition {nodeDto} could not be found.");
                    continue;
                }
                
                Pin? pin = uint.TryParse(compPinName, out uint pinId) ?
                    component.GetPin(pinId) :
                    component.GetPin(compPinName);

                if (pin != null) {
                    node.AddPin(pin);
                }
                else {
                    ComponentManager.LogError($"Failed to add a pin {pin} to the node '{node}'");
                }
                
                // TODO: Transitivity through nodes
            }

            nodes.Add(node);
            ComponentManager.LogInfo($"Instantiated node '{node}'");
        }

        return nodes;
    }

    public void LoadFile(string scenePath) {
        using var fileStream = new StreamReader(scenePath);

        var deserializer = new DeserializerBuilder()
            .WithCaseInsensitivePropertyMatching()
            .Build();
            
        _sceneDto = deserializer.Deserialize<SceneDto>(fileStream);
    }

    public List<string> GetComponentTypeNames() {
        if (_sceneDto?.Components == null) {
            throw new InvalidOperationException("Getting component types before loading them from file");
        }

        return [.. _sceneDto.Components.Keys];
    }

    public List<Component> GetComponentsOfType(string typeName) {
        if (_sceneDto?.Components == null) {
            throw new InvalidOperationException("Getting component instances before loading them from file");
        }

        return _instancesByType[typeName];
    }
}