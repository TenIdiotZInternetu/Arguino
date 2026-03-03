using System.Numerics;
using Avalonia;
using SkiaSharp;

namespace Gui.Utils.ExtensionMethods;

public static class GeometryExtensions {
    public static Vector2 ToVector2(this Point point) {
        return new Vector2((float)point.X, (float)point.Y);
    }

    public static Vector2 ToVector2(this SKPoint point) {
        return new Vector2(point.X, point.Y);
    }

    // ReSharper disable once InconsistentNaming
    public static SKPoint ToSKPoint(this Vector2 vector) {
        return new SKPoint(vector.X, vector.Y);
    }

    public static Point ToAvaPoint(this Vector2 vector) {
        return new Point(vector.X, vector.Y);
    }

    public static SKMatrix ToSKMatrix(this ComponentManagement.Scenes.Transform transform) {
        // TODO: Translate to coordinates origin, then translate
        var rotation = SKMatrix.CreateRotationDegrees(transform.Rotation);
        var scale = SKMatrix.CreateScale(transform.ScaleX, transform.ScaleY);
        var translation = SKMatrix.CreateTranslation(transform.PositionX, transform.PositionY);

        return SKMatrix.Concat(translation, SKMatrix.Concat(rotation, scale));
    }
}