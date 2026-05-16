using ComponentManagement.Factory.Loaders;
using ComponentManagement.Graph;
using ComponentManagement.Scenes;
using Svg.Skia;

namespace ComponentManagement.Factory;

public class SceneFactory {
    public Scene Scene { get; private set; } = new();

    private string _scenePath;
    private string _componentsDirPath;

    public SceneFactory(string scenePath, string componentsDirPath){
        _scenePath = scenePath;
        _componentsDirPath = componentsDirPath;
    }

    public Scene LoadResources() {
        // TODO: Determine for different file formats
        ISceneLoader sceneLoader = new YamlSceneLoader();

        sceneLoader.LoadFile(_scenePath);
        var componentTypeNames = sceneLoader.GetComponentTypeNames();
        var componentInstances = sceneLoader.InstantiateComponents();

        foreach (string typeName in componentTypeNames) {
            bool typeSuccess = CompleteComponentType(typeName);
            if (!typeSuccess) continue;
            
            foreach (Component instance in sceneLoader.GetComponentsOfType(typeName)) {
                CompleteComponentInstance(instance);
            }
        }

        Scene = new Scene {
            ComponentsMap = componentInstances,
            Nodes = sceneLoader.InstantiateNodes()
        };

        return Scene;
    }

    private bool CompleteComponentType(string typeName) {
        var componentConfig = CreateConfig(typeName);
        if (componentConfig == null) {
            ComponentManager.LogWarning($"Configuration of component type {typeName} could not be created. Skipping instances of this type.");
            return false;
        }

        componentConfig.ComponentPath = GetComponentTypePath(typeName);

        ISvgLoader svgLoader = new SkiaSvgLoader();
        svgLoader.LoadDocuments(componentConfig.ComponentPath);
        svgLoader.ResizeDocuments(componentConfig.ImageSize);
        
        // TODO: Create proxy object to add SVG documents before compilation
        // foreach (var (spriteName, svgDoc) in svgLoader.GetDocuments()) {
        //     Component firstInstance = _componentInstances.First(entry => entry.Key == typeName).Value;
        //     firstInstance.OnLoadSvgDocument(spriteName, svgDoc);
        // }

        componentConfig.Sprites = svgLoader.CompileSvgs();

        if (componentConfig.Sprites.Count == 0) {
            ComponentManager.LogWarning($"No SVGs for type {typeName} were found. Components will be present but not rendered.");
        }

        Component.AddConfiguration(componentConfig);
        ComponentManager.LogInfo($"Defined the component type {typeName}");
        return true;
    }

    private void CompleteComponentInstance(Component instance) {
        instance.Transform.BaseSize = instance.Configuration.ImageSize;
        instance.InitPins();

        if (instance.Configuration.Sprites.Count != 0) {
            instance.CurrentSprite = instance.Configuration.Sprites.First().Value;
        }
        else {
            instance.CurrentSprite = new SKSvg();
        }

        instance.OnInitialized();
    }

    private ComponentConfiguration? CreateConfig(string typeName) {
        IConfigurationLoader configLoader;

        var typePath = GetComponentTypePath(typeName);
        var configFileName = Path.GetFileName(typePath);     // find file with the same name as the directory
        var configPath = $"{typePath}/{configFileName}";

        var yamlPath = configPath + ".yaml";
        
        if (File.Exists(yamlPath)) {
            configLoader = new YamlConfigurationLoader();
            return configLoader.LoadConfig(yamlPath);
        }
        else {
            ComponentManager.LogError($"A file at path {yamlPath} was expected but not found. Skipping definition of component type {typeName}");
            return null;
        }
    }

    private string GetComponentTypePath(string typeName) => $"{_componentsDirPath}/{typeName}";
}