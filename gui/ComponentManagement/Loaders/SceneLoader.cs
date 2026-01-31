using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Numerics;
using System.Threading.Tasks;
using YamlDotNet.Serialization;

namespace ComponentManagement.Loaders;

public class SceneLoader {
    public record SceneDto {
        public record ComponentDto {
            public required string Name;
            public string Position = "0 0";
            public float Rotation = 0;
            public string Scale = "1 1";
        }

        public required List<ComponentDto> Components;
    }

    public static void LoadScene(string scenePath, string componentsPath) {
        var sceneDto = Deserialize(scenePath);
        var scene = new Scene();

        foreach (var compDto in sceneDto.Components) {
            var component = CreateComponent(compDto, componentsPath);
            scene.Components.Add(new )
        }
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
        var transform = new Transform{
            Position = StringToVector2(compDto.Position),
            Rotation = compDto.Rotation,
            Scale = StringToVector2(compDto.Scale)
        };

        var component = new Component()
    }

    private static Vector2 StringToVector2(string valuePair) {

    }
}