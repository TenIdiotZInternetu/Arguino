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
    
    // TODO: Change based on actual window resolution
    public Rect Bounds => new(new Point(0,0), new Point(1920,1080));

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