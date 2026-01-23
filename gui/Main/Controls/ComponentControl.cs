using Avalonia.Controls;
using Avalonia.Media;
using Avalonia.Threading;
using ComponentManagement;

namespace Gui.Controls;

public class ComponentControl : Control {
     public Component Component { get; init; }
     
     public ComponentControl(Component component) {
          ClipToBounds = true;
          Component = component;
          component.SpriteChangedEvent += (_) => InvalidateVisual();
     }

     public override void Render(DrawingContext context) {
          context.Custom(new SvgDrawOp(this, Component.CurrentSprite));
     }
}