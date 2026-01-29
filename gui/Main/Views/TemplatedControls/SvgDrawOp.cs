using System;
using Avalonia;
using Avalonia.Media;
using Avalonia.Platform;
using Avalonia.Rendering.SceneGraph;
using Avalonia.Skia;
using SkiaSharp;
using Svg.Skia;

namespace Gui.Views.TemplatedControls;

public class SvgDrawOp(SKSvg Svg, SKPoint Translation) : ICustomDrawOperation {
          
    public Rect Bounds => new(new Point(0,0), new Point(100,100));

    public void Dispose() { }
    public bool HitTest(Point p) => false;
    public bool Equals(ICustomDrawOperation? other) => false;
          
    public void Render(ImmediateDrawingContext context) {
        var leaseFeature = context.TryGetFeature<ISkiaSharpApiLeaseFeature>();
        if (leaseFeature == null) {
            throw new Exception("LEASE");
        }

        using var lease = leaseFeature.Lease();
        var canvas = lease.SkCanvas;
        canvas.Save();
 
        canvas.Translate(Translation);
        canvas.DrawPicture(Svg.Picture);
        canvas.Restore();
    }
}