using Avalonia;
using Avalonia.Controls;
using Avalonia.Input;
using Avalonia.Media;
using ComponentManagement.Circuitry;
using ComponentManagement.Scenes;
using Gui.Utils.ExtensionMethods;
using Svg.Skia;

namespace Gui.Views.TemplatedControls;

public class ComponentControl : Control {
     private Component _component = null!;
     private bool _pressHeld = false;

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
          context.Custom(new SvgDrawOp(Component.CurrentSprite, Component.Transform.ToSKMatrix()));
     }
     
     protected override void OnPointerPressed(PointerPressedEventArgs e) {
          bool leftPressed = e.Properties.IsLeftButtonPressed;
          bool altPressed = e.KeyModifiers.HasFlag(KeyModifiers.Alt);
          var position = e.GetCurrentPoint(this).Position;
          
          if (leftPressed) {
               if (!_pressHeld) {
                    _component.OnControlPress(position.ToVector2());
               }
               
               _pressHeld = !_pressHeld && altPressed;
          }
     }

     protected override void OnPointerReleased(PointerReleasedEventArgs e) {
          if (!_pressHeld) {
               _component.OnControlRelease();
          }
     }

     private void InvalidateVisual(SKSvg _)  => InvalidateVisual();
}