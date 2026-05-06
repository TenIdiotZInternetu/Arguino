using System.Numerics;
using Avalonia;

namespace Gui.Utils.ExtensionMethods;

public static class AvaloniaPointExtensions {
    public static Vector2 ToVector2(this Point point) {
        return new Vector2((float)point.X, (float)point.Y);
    }
}