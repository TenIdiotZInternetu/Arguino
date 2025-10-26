using System;
using System.IO;
using Avalonia;
using Avalonia.Controls;
using Avalonia.Media;
using Avalonia.Platform;
using Avalonia.Rendering.SceneGraph;
using Avalonia.Skia;
using Avalonia.Threading;
using gui;
using SkiaSharp;
using Svg;
using Svg.Skia;
using TcpAdapter;

namespace Gui.Controls;

public class Svg : Control {
     private SKSvg? _svg;
     private SvgDocument _doc;
     
     
     public Svg() {
          LoadSvg();
          ClipToBounds = true;

          MainController.AppInitializedEvent += () => {
               MainController.Adapter.StateChangedEvent += OnStateChanged;
          };
     }
     
     static Svg()
     {
          AffectsRender<Svg>();
     }

     private class DrawOp : ICustomDrawOperation {
          private readonly SKSvg _svg;
          
          public Rect Bounds => new Rect(new Point(0,0), new Point(100,100));

          public DrawOp(SKSvg svg) {
               _svg = svg;
          }

          public void Dispose() { }

          public bool HitTest(Point p) => false;

          public bool Equals(ICustomDrawOperation? other) => false;
          
          public void Render(ImmediateDrawingContext context) {
               if (_svg.Picture == null) {
                    throw new Exception("PICTURE");
               }
               
               var leaseFeature = context.TryGetFeature<ISkiaSharpApiLeaseFeature>();
               if (leaseFeature == null) {
                    throw new Exception("LEASE");
               }

               using var lease = leaseFeature.Lease();
               var canvas = lease.SkCanvas;
               canvas.Save();
               canvas.Clear(SKColors.Empty);
               
               SKPicture pic =  _svg.Picture;
               var svgRect = pic.CullRect;
               var scale = (float)Math.Min(Bounds.Width / svgRect.Width, Bounds.Height / svgRect.Height);
               canvas.Translate((float)Bounds.X, (float)Bounds.Y);
               // canvas.Scale(scale);
               
               canvas.DrawPicture(_svg.Picture);
               canvas.Restore();
          }
     }
     
     private void LoadSvg()
     {
          using var stream = File.OpenRead("/home/touster/Kodiky/arguino/gui/Components/Led/Led.svg");
          _doc = SvgDocument.Open<SvgDocument>(stream);
          _svg = SKSvg.CreateFromSvgDocument(_doc);
     }

     private void OnStateChanged(ArduinoState state) {
          var elem = _doc.GetElementById("ON");
          elem.Visibility = state.GetDigital(3) ? "visible" : "hidden";
          _svg!.FromSvgDocument(_doc);
     }

     public override void Render(DrawingContext context) {
          if (_svg == null) {
               throw new Exception("SVG");
          }
          context.Custom(new DrawOp(_svg));
          Dispatcher.UIThread.InvokeAsync(InvalidateVisual, DispatcherPriority.Background);
     }
}