using System.Numerics;
using Avalonia;
using Avalonia.Controls;
using Avalonia.Input;
using Avalonia.Media;
using ComponentManagement;
using Gui.Utils.ExtensionMethods;
using SkiaSharp;
using Svg.Skia;

namespace Gui.Views.TemplatedControls;

public class ComponentControl : Control {
     private Component _component = null!;

     public static readonly DirectProperty<ComponentControl, Component> ComponentProperty = AvaloniaProperty.RegisterDirect<ComponentControl, Component>(
          nameof(Component), o => o.Component, (o, v) => o.Component = v);

     public Component Component {
          get => _component;
          set => SetAndRaise(ComponentProperty, ref _component, value);
     }

     public ComponentControl() {
          MainController.AppInitializedEvent += InvalidateVisual;
     }

     protected override void OnPropertyChanged(AvaloniaPropertyChangedEventArgs change) {
          base.OnPropertyChanged(change);
          if (change.Property == ComponentProperty) {
               HandleComponentChanged(change);
          }
     }

     private void HandleComponentChanged(AvaloniaPropertyChangedEventArgs change) {
          if (change.OldValue is Component oldComponent) {
               oldComponent.SpriteChangedEvent -= InvalidateVisual;
          }
          if (change.NewValue is Component newComponent) {
               newComponent.SpriteChangedEvent += InvalidateVisual;
          }
     }

     public override void Render(DrawingContext context) {
          SKPoint skpoint = Component.Transform.Position.ToSKPoint();
          context.Custom(new SvgDrawOp(Component.CurrentSprite, skpoint));
     }
     
     protected override void OnPointerPressed(PointerPressedEventArgs e) {
          bool leftPressed = e.Properties.IsLeftButtonPressed;
          var position = e.GetCurrentPoint(this).Position;
          
          if (leftPressed) {
               _component.OnControlPress(position.ToVector2());
          }
     }

     protected override void OnPointerReleased(PointerReleasedEventArgs e) {
          var position = e.GetCurrentPoint(this).Position;
          _component.OnControlRelease(position.ToVector2());
     }
     
     private void InvalidateVisual(SKSvg _)  => InvalidateVisual();
}