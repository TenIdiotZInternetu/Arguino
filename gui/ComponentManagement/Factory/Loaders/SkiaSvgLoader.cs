using System.Numerics;
using ComponentManagement.Scenes;
using SkiaSharp;
using Svg;
using Svg.Skia;

namespace ComponentManagement.Factory.Loaders;

public static class SkiaSvgLoader {
    public static Dictionary<string, SKSvg> LoadSvgs(string path, ComponentConfiguration component) {
        var loadedSvgs = new Dictionary<string, SKSvg>();
        string[] svgFileNames = Directory.GetFiles(path, "*.svg");

        // TODO: Catch file exceptions
        foreach (var fileName in svgFileNames) {
            using var stream = File.OpenRead(fileName);

            var svgDocument = SvgDocument.Open<SvgDocument>(stream);
            ScaleToDesiredSize(svgDocument, component.ImageSize);
            var svg = SKSvg.CreateFromSvgDocument(svgDocument);

            string svgName = Path.GetFileNameWithoutExtension(fileName);
            loadedSvgs.Add(svgName, svg);
            ComponentManager.LogInfo($"Loaded SVG {svgName} of component type {component.Name}.");
        }
        
        return loadedSvgs;
    }

    private static void ScaleToDesiredSize(SvgDocument svgDocument, Vector2 desiredSize) {
        if (desiredSize.X != 0 && desiredSize.Y != 0) {
            svgDocument.AspectRatio = new(SvgPreserveAspectRatio.none);
        }
        else {
            svgDocument.AspectRatio = new(SvgPreserveAspectRatio.xMinYMin);
        }

        if (desiredSize.X != 0) {
            svgDocument.Width = new(SvgUnitType.Pixel, desiredSize.X);
        }
        if (desiredSize.Y != 0) {
            svgDocument.Height = new(SvgUnitType.Pixel, desiredSize.Y);
        }
    }
}