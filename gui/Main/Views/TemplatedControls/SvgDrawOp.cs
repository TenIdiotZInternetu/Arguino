using System;
using Avalonia;
using Avalonia.Controls;
using Avalonia.Media;
using Avalonia.Platform;
using Avalonia.Rendering.SceneGraph;
using Avalonia.Skia;
using Svg.Skia;

namespace Gui.Controls;

public class SvgDrawOp(Control Parentcontrol, SKSvg Svg) : ICustomDrawOperation {
          
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
               
        canvas.Translate((float)Bounds.X, (float)Bounds.Y);
        canvas.DrawPicture(Svg.Picture);
        canvas.Restore();
    }
}