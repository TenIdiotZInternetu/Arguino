using Svg;
using Svg.Skia;

namespace ComponentManagement;

public static class SkiaSvgLoader {
    public static Dictionary<string, SKSvg> LoadSvgs(string path) {
        var loadedSvgs = new Dictionary<string, SKSvg>();
        string[] svgFileNames = Directory.GetFiles(path, "*.svg");

        foreach (var fileName in svgFileNames) {
            using var stream = File.OpenRead(fileName);
            var svgDocument =  SvgDocument.Open<SvgDocument>(stream);
            var svg = SKSvg.CreateFromSvgDocument(svgDocument);
            
            string svgName = Path.GetFileNameWithoutExtension(fileName);
            loadedSvgs.Add(svgName, svg);
        }
        
        return loadedSvgs;
    }
}