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
     private SvgDocument _svgDoc;
     private SKSvg _svgOn;
     private SKSvg _svgOff;

     public bool IsOn { get; private set; }
     
     public Svg() {
          LoadSvg();
          ClipToBounds = true;

          MainController.AppInitializedEvent += () => {
               MainController.Adapter.StateChangedEvent += OnStateChanged;
          };
     }

     private class DrawOp : ICustomDrawOperation {
          private readonly Svg _parentControl;
          
          public Rect Bounds => new Rect(new Point(0,0), new Point(100,100));

          public DrawOp(Svg parentControl) {
               _parentControl = parentControl;
          }

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
               
               SKSvg svgToRender = _parentControl.IsOn ? 
                         _parentControl._svgOn : _parentControl._svgOff;
               
               canvas.DrawPicture(svgToRender.Picture);
               canvas.Restore();
          }
     }
     
     private void LoadSvg()
     {
          using var stream = File.OpenRead("/home/touster/Kodiky/arguino/gui/Components/Led/Led.svg");
          _svgDoc = SvgDocument.Open<SvgDocument>(stream);
          _svgOn = SKSvg.CreateFromSvgDocument(_svgDoc);
          _svgDoc.GetElementById("ON").Visibility = "hidden";
          _svgOff = SKSvg.CreateFromSvgDocument(_svgDoc);
     }

     private void OnStateChanged(ArduinoState state) {
          IsOn = state.GetDigital(3);
     }

     public override void Render(DrawingContext context) {
          if (_svgOn == null) {
               throw new Exception("SVG");
          }
          context.Custom(new DrawOp(this));
          Dispatcher.UIThread.InvokeAsync(InvalidateVisual, DispatcherPriority.Background);
     }
}