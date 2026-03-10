using System.Numerics;
using Svg;
using Svg.Skia;

namespace ComponentManagement.Factory.Loaders;

public class SkiaSvgLoader : ISvgLoader {
    private readonly Dictionary<string, SvgDocument> _loadedDocuments = [];

    public void LoadDocuments(string componentPath) {
        string[] svgFileNames = Directory.GetFiles(componentPath, "*.svg");

        foreach (var fileName in svgFileNames) {
            using var stream = File.OpenRead(fileName);

            var svgDocument = SvgDocument.Open<SvgDocument>(stream);
            string svgName = Path.GetFileNameWithoutExtension(fileName);
            _loadedDocuments.Add(svgName, svgDocument);
        }
    }
    
    public Dictionary<string, SvgDocument> GetDocuments() => _loadedDocuments;

    public Dictionary<string, SKSvg> CompileSvgs() {
        Dictionary<string, SKSvg> compiledSvgs = [];

        foreach (var (svgName, svgDoc) in _loadedDocuments) {
            var svg = SKSvg.CreateFromSvgDocument(svgDoc);
            compiledSvgs.Add(svgName, svg);
        }

        return compiledSvgs;
    }

    public void ResizeDocuments(Vector2 desiredSize) {
        foreach (SvgDocument doc in _loadedDocuments.Values) {
            if (desiredSize.X != 0 && desiredSize.Y != 0) {
                doc.AspectRatio = new(SvgPreserveAspectRatio.none);
            }
            else {
                doc.AspectRatio = new(SvgPreserveAspectRatio.xMinYMin);
            }

            if (desiredSize.X != 0) {
                doc.Width = new(SvgUnitType.Pixel, desiredSize.X);
            }
            if (desiredSize.Y != 0) {
                doc.Height = new(SvgUnitType.Pixel, desiredSize.Y);
            }
        }
    }
}