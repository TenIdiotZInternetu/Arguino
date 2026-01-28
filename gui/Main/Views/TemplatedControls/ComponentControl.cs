using System.Numerics;
using Avalonia;
using Avalonia.Controls;
using Avalonia.Media;
using ComponentManagement;
using Gui.Controls;
using Svg.Skia;

namespace Gui.Views.TemplatedControls;

public class ComponentControl : Control {
     private Component _component;

     public static readonly DirectProperty<ComponentControl, Component> ComponentProperty = AvaloniaProperty.RegisterDirect<ComponentControl, Component>(
          nameof(Component), o => o.Component, (o, v) => o.Component = v);

     public Component Component {
          get => _component;
          set => SetAndRaise(ComponentProperty, ref _component, value);
     }

     public static readonly StyledProperty<Point> PositionProperty = AvaloniaProperty.Register<ComponentControl, Point>(
          nameof(Position));

     public Point Position {
          get => GetValue(PositionProperty);
          set => SetValue(PositionProperty, value);
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

     private void InvalidateVisual(SKSvg _)  => InvalidateVisual();

     public override void Render(DrawingContext context) {
          context.Custom(new SvgDrawOp(Component.CurrentSprite));
     }
}