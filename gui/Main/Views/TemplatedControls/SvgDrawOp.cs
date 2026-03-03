using System;
using Avalonia;
using Avalonia.Media;
using Avalonia.Platform;
using Avalonia.Rendering.SceneGraph;
using Avalonia.Skia;
using Svg.Skia;
using SkiaSharp;

namespace Gui.Views.TemplatedControls;

public class SvgDrawOp(SKSvg Svg, SKMatrix Transform) : ICustomDrawOperation {

    public Rect Bounds => Transform.MapRect(Svg.Picture!.CullRect).ToAvaloniaRect();

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
 
        canvas.Concat(ref Transform);
        canvas.DrawPicture(Svg.Picture);
        canvas.Restore();
    }
}