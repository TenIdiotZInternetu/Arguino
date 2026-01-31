using System.Numerics;
using Avalonia;
using SkiaSharp;

namespace Gui.Utils.ExtensionMethods;

public static class Vector2Extensions {
    // ReSharper disable once InconsistentNaming
    public static SKPoint ToSKPoint(this Vector2 vector) {
        return new SKPoint(vector.X, vector.Y);
    }

    public static Point ToAvaPoint(this Vector2 vector) {
        return new Point(vector.X, vector.Y);
    }
}