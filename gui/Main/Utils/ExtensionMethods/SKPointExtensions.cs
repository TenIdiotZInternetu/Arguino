using System.Numerics;
using ShimSkiaSharp;

namespace Gui.Utils.ExtensionMethods;

// ReSharper disable once InconsistentNaming
public static class SKPointExtensions {
    public static Vector2 ToVector2(this SKPoint point) {
        return new Vector2(point.X, point.Y);
    }
}