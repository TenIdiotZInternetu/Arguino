using ComponentManagement.Factory.Loaders;
using ComponentManagement.Graph;
using ComponentManagement.Scenes;

namespace ComponentManagement.Factory;

public class SceneFactory {
    public Scene Scene { get; private set; }

    private string _scenePath;
    private string _componentsDirPath;

    private Dictionary<string, Component> _componentInstances; 

    public SceneFactory(string scenePath, string componentsDirPath){
        _scenePath = scenePath;
        _componentsDirPath = componentsDirPath;
    }

    public void LoadResources() {
        // TODO: Determine for different file formats
        ISceneLoader sceneLoader = new YamlSceneLoader();
        ISvgLoader svgLoader = new SkiaSvgLoader();

        sceneLoader.LoadScene(_scenePath);
        var componentTypeNames = sceneLoader.GetComponentTypeNames();
        _componentInstances = sceneLoader.InstantiateComponents();

        foreach (string typeName in componentTypeNames) {
            var componentConfig = CreateConfig(typeName);
            if (componentConfig == null) continue;
            componentConfig.ComponentPath = GetComponentTypePath(typeName);
            Component.AddConfiguration(componentConfig);

            svgLoader.LoadDocuments(componentConfig.ComponentPath);
            svgLoader.ResizeDocuments(componentConfig.ImageSize);
            
            // TODO: Create proxy object to add SVG documents before compilation
            // foreach (var (spriteName, svgDoc) in svgLoader.GetDocuments()) {
            //     Component firstInstance = _componentInstances.First(entry => entry.Key == typeName).Value;
            //     firstInstance.OnLoadSvgDocument(spriteName, svgDoc);
            // }
            
            foreach (var (_, instance) in _componentInstances) {
                instance.Transform.BaseSize = componentConfig.ImageSize;
            }
        }
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