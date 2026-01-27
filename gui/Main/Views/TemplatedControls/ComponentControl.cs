using System.Numerics;
using Avalonia;
using Avalonia.Controls;
using Avalonia.Media;
using ComponentManagement;
using Gui.Controls;

namespace Gui.Views.TemplatedControls;

public class ComponentControl : Control {
     public Component Component { get; init; } = null!;
     public Point Position { get; set; }

     public static readonly AttachedProperty<var> propertyNameProperty =
          AvaloniaProperty.RegisterAttached<ComponentControl, targetType, var>("propertyName");

     public static void SetpropertyName(targetType obj, var value) => obj.SetValue(propertyNameProperty, value);
     public static var GetpropertyName(targetType obj) => obj.GetValue(propertyNameProperty);
     
     public ComponentControl() {
          ClipToBounds = true;
          Component.SpriteChangedEvent += (_) => InvalidateVisual();
     }

     public override void Render(DrawingContext context) {
          context.Custom(new SvgDrawOp(this, Component.CurrentSprite));
     }
}