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
          SKPoint skpoint = Component.Transform.Position.ToSKPoint();
          context.Custom(new SvgDrawOp(Component.CurrentSprite, skpoint));
     }
     
     protected override void OnPointerPressed(PointerPressedEventArgs e) {
          bool leftPressed = e.Properties.IsLeftButtonPressed;
          var position = e.GetCurrentPoint(this).Position;
          
          if (leftPressed && !_pressHeld) {
               _component.OnControlPress(position.ToVector2());
          }
          
          _pressHeld = e.KeyModifiers.HasFlag(KeyModifiers.Alt);
     }

     protected override void OnPointerReleased(PointerReleasedEventArgs e) {
          var position = e.GetCurrentPoint(this).Position;

          if (!_pressHeld) {
               _component.OnControlRelease(position.ToVector2());
          }
     }
     
     protected override void OnAttachedToVisualTree(VisualTreeAttachmentEventArgs e) {
          base.OnAttachedToVisualTree(e);

          var topLevel = TopLevel.GetTopLevel(this);
          if (topLevel != null) {
               topLevel.KeyUp += OnGlobalKeyUp;
          }
     }

     protected override void OnDetachedFromVisualTree(VisualTreeAttachmentEventArgs e) {
          base.OnDetachedFromVisualTree(e);

          var topLevel = TopLevel.GetTopLevel(this);
          if (topLevel != null) {
               topLevel.KeyUp -= OnGlobalKeyUp;
          }
     }

     private void OnGlobalKeyUp(object? _, KeyEventArgs e) {
          if (_pressHeld && e.PhysicalKey == PhysicalKey.AltLeft) {
               _component.OnControlRelease(new Vector2());
               _pressHeld = false;
          } 
     }

     private void InvalidateVisual(SKSvg _)  => InvalidateVisual();
}