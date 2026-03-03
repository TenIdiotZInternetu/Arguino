using System;
using Avalonia;
using Avalonia.Media;
using Avalonia.Platform;
using Avalonia.Rendering.SceneGraph;
using Avalonia.Skia;
using Gui.Utils.ExtensionMethods;
using Svg.Skia;
using SkiaSharp;

namespace Gui.Views.TemplatedControls;

using Transform = ComponentManagement.Scenes.Transform;

public class SvgDrawOp(SKSvg Svg, Transform Transform) : ICustomDrawOperation {
    
    // TODO: Change based on actual window resolution
    public SKPoint Translation => Transform.Position.ToSKPoint();
    public float Rotation => Transform.Rotation;

    public Rect Bounds => new(
        Translation.X,
        Translation.Y,
        Svg.Picture!.CullRect.Width,
        Svg.Picture.CullRect.Height
    );

    public void Dispose() { }
    public bool HitTest(Point p) => Bounds.Contains(p);
    public bool Equals(ICustomDrawOperation? other) => false;
          
    public void Render(ImmediateDrawingContext context) {
        var leaseFeature = context.TryGetFeature<ISkiaSharpApiLeaseFeature>();
        if (leaseFeature == null) {
            throw new Exception("LEASE");
        }

        using var lease = leaseFeature.Lease();
        var canvas = lease.SkCanvas;
        canvas.Save();
 
        // TODO: Scale and Rotation
        canvas.Translate(Translation);
        canvas.DrawPicture(Svg.Picture);
        canvas.Restore();
    }
}