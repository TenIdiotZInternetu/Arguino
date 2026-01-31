using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Numerics;
using System.Reflection;
using System.Threading.Tasks;
using YamlDotNet.Serialization;

namespace ComponentManagement.Loaders;

public class YamlSceneLoader {
    public record SceneDto {
        public record ComponentDto {
            public required string Name;
            public string Position = "0 0";
            public float Rotation = 0;
            public string Scale = "1 1";
        }

        // ReSharper disable once CollectionNeverUpdated.Global
        public required List<ComponentDto> Components { get; init; } = new();
    }

    public static Scene LoadScene(string scenePath, string componentsPath) {
        var sceneDto = Deserialize(scenePath);
        var scene = new Scene();

        foreach (var compDto in sceneDto.Components) {
            var componentDir = Path.Combine(componentsPath, compDto.Name);
            var component = CreateComponent(compDto, componentDir);
            scene.Components.Add(component);
        }

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

    private static Component CreateComponent(SceneDto.ComponentDto compDto, string componentsPath) {

        // TODO: Cache already defined components
        // TODO: Change assembly for custom added scripts
        // TODO: Log unknown component names

        Type? compType = Assembly.GetExecutingAssembly()
            .GetTypes()
            .First(type => type.Name == compDto.Name);

        if (compType == null) {
            throw new Exception($"Component {compDto.Name} does not exist");
        }
        
        var component = Activator.CreateInstance(compType, componentsPath) as Component;
        component!.Transform = new Transform{
            Position = StringToVector2(compDto.Position) ?? Vector2.Zero,
            Rotation = compDto.Rotation,
            Scale = StringToVector2(compDto.Scale) ?? Vector2.One
        };
        
        return component;
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