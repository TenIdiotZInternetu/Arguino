using System.Numerics;
using Svg;
using Svg.Skia;

namespace ComponentManagement.Factory;

public interface ISvgLoader {
    public void LoadDocuments(string componentPath);
    public void ResizeDocuments(Vector2 desiredSize);
    public Dictionary<string, SvgDocument> GetDocuments();
    public Dictionary<string, SKSvg> CompileSvgs();
}