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

    public static Avalonia.Point ToAvaPoint(this Vector2 vector) {
        return new Point(vector.X, vector.Y);
    }

    public static SKMatrix ToSKMatrix(this ComponentManagement.Scenes.Transform transform) {
        var originShift = SKMatrix.CreateTranslation(-transform.BaseSize.X / 2, -transform.BaseSize.Y / 2);
        var inverseShift = SKMatrix.CreateTranslation(transform.BaseSize.X / 2, transform.BaseSize.Y / 2);

        var rotation = SKMatrix.CreateRotationDegrees(transform.Rotation);
        var scale = SKMatrix.CreateScale(transform.ScaleX, transform.ScaleY);
        var translation = SKMatrix.CreateTranslation(transform.PositionX, transform.PositionY);

        return originShift
            .MulitplyLeft(rotation)
            .MulitplyLeft(scale)
            // .MulitplyLeft(translation)
            .MulitplyLeft(inverseShift);
    }

    public static Avalonia.Matrix ToAvaMatrix(this ComponentManagement.Scenes.Transform transform) {
        
        var originShift = Matrix.CreateTranslation(-transform.BaseSize.X / 2, -transform.BaseSize.Y / 2);
        var inverseShift = Matrix.CreateTranslation(transform.BaseSize.X / 2, transform.BaseSize.Y / 2);

        var rotation = Matrix.CreateRotation(Matrix.ToRadians(transform.Rotation));
        var scale = Matrix.CreateScale(transform.ScaleX, transform.ScaleY);
        var translation = Matrix.CreateTranslation(transform.PositionX, transform.PositionY);

        // return originShift * rotation * scale * translation * inverseShift;
        return originShift;
    }

    public static SKMatrix MulitplyLeft(this SKMatrix self, SKMatrix other) {
        return SKMatrix.Concat(other, self);
    }
}