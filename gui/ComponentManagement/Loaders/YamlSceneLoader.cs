using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Diagnostics;
using System.Linq;
using System.Numerics;
using System.Reflection;
using System.Threading.Tasks;
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
        }
    }

    private static Dictionary<string, Type> _typeNamesMap = [];

    public static Scene LoadScene(string scenePath, string componentsPath) {
        var sceneDto = Deserialize(scenePath);
        var scene = new Scene();

        scene.Components.AddRange(
            HandleComponents(sceneDto.Components, componentsPath)
        );

        return scene;
    }

    private static SceneDto Deserialize(string scenePath) {
        using var file = File.OpenRead(scenePath);
        var fileStream = new StreamReader(file);

        var deserializer = new DeserializerBuilder()
            .WithCaseInsensitivePropertyMatching()
            .Build();
            
        return deserializer.Deserialize<SceneDto>(fileStream);
    }

    private static List<Component> HandleComponents(ComponentsMap componentDtos, string componentsPath) {
        List<Component> handledComponents = [];
        
        foreach (var (name, dto) in componentDtos) {
            string typeName = dto.Type;
            Type? type;

            if (!_typeNamesMap.TryGetValue(typeName, out type)) {
                type = MapComponentType(typeName);
            }

            if (Activator.CreateInstance(type, componentsPath) is Component compInstance) {
                compInstance.Name = name;
                compInstance.Transform = ParseTransform(dto);
                handledComponents.Add(compInstance);
            } 
            else {
                throw new UnreachableException($"{typeName} instance could not be created.");
            }
        }

        return handledComponents;
    }

    private static Type MapComponentType(string typeName) {
        // TODO: Change assembly for custom added scripts
        // TODO: Log unknown component types
        
        Type? compType = Assembly.GetExecutingAssembly()
            .GetTypes()
            .First(type => type.Name == typeName);

        if (compType == null) {
            throw new Exception($"Component of type {typeName} does not exist");
        }

        _typeNamesMap.Add(typeName, compType);
        return compType;
    }

    private static Transform ParseTransform(SceneDto.ComponentDto compDto) {
        return new Transform {
            Position = StringToVector2(compDto.Position) ?? Vector2.Zero,
            Rotation = compDto.Rotation,
            Scale = StringToVector2(compDto.Scale) ?? Vector2.One
        };
    }

    private static Vector2? StringToVector2(string valuePair) {
        // TODO: Log incorrect use of value pair
        
        string[] values = valuePair.Split();
        if (values.Length != 2) {
            return null;
        }

        float x = 0, y = 0;
        bool success = true;
        success = success && float.TryParse(values[0], out x);
        success = success && float.TryParse(values[1], out y);

        if (!success) {
            return null;
        }
        
        return new Vector2(x, y);
    }
}