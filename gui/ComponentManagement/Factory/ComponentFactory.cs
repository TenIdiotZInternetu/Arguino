using ComponentManagement.Factory.Loaders;
using ComponentManagement.Graph;
using ComponentManagement.Scenes;

namespace ComponentManagement.Factory;

public class SceneFactory {
    public Scene Scene { get; private set; }

    private string _scenePath;
    private string _componentsDirPath;

    public SceneFactory(string scenePath, string componentsDirPath){
        _scenePath = scenePath;
        _componentsDirPath = componentsDirPath;
    }

    public void LoadResources() {
        // TODO: Determine for different file formats
        ISceneLoader sceneLoader = new YamlSceneLoader();

        sceneLoader.LoadFile(_scenePath);
        var componentTypeNames = sceneLoader.GetComponentTypeNames();
        var componentInstances = sceneLoader.InstantiateComponents(); // TODO: I hate this

        foreach (string typeName in componentTypeNames) {
            CompleteComponentType(typeName);
            
            foreach (Component instance in sceneLoader.GetComponentsOfType(typeName)) {
                CompleteComponentInstance(instance);
            }
        }

        Scene = new Scene {
            ComponentsMap = componentInstances,
            Nodes = sceneLoader.InstantiateNodes()
        };
    }

    private void CompleteComponentType(string typeName) {
        var componentConfig = CreateConfig(typeName);
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
        Component.AddConfiguration(componentConfig);
    }

    private void CompleteComponentInstance(Component instance) {
        instance.Transform.BaseSize = instance.Configuration.ImageSize;
        instance.InitPins();
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
            ComponentManager.LogError($"A file with path {yamlPath} was expected but not found. Skipping definition of component type {typeName}");
            return null;
        }
    }

    private string GetComponentTypePath(string typeName) => $"{_componentsDirPath}/{typeName}";
}