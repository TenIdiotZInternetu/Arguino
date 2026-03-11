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

    public Rect Bounds => Transform.MapRect(_hitBox).ToAvaloniaRect();

    private SKRect _hitBox => Svg.Picture!.CullRect;

    public void Dispose() { }
    public bool Equals(ICustomDrawOperation? other) => false;

    public bool HitTest(Point p) {
        SKPoint point = p.ToSKPoint();
        SKPoint localSpacePoint = Transform.Invert().MapPoint(point);
        return _hitBox.Contains(localSpacePoint);
    }
          
    public void Render(ImmediateDrawingContext context) {
        var leaseFeature = context.TryGetFeature<ISkiaSharpApiLeaseFeature>();
        if (leaseFeature == null) {
            throw new Exception("LEASE");
        }

        using var lease = leaseFeature.Lease();
        var canvas = lease.SkCanvas;
        canvas.Save();
 
        // canvas.Concat(ref Transform);
        canvas.DrawPicture(Svg.Picture);
        canvas.Restore();
    }
}