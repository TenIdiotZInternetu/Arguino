using System.Collections.Generic;
using Avalonia;
using Avalonia.Controls;
using Avalonia.Controls.Primitives;
using Avalonia.Data;
using Avalonia.Input;
using Avalonia.Media;
using ComponentManagement.Graph;
using Gui.Utils.ExtensionMethods;
using ShimSkiaSharp;
using Svg.Skia;

namespace Gui.Views.TemplatedControls;

public class ComponentControl : TemplatedControl {
     public const string SELECTED_PSEUDOCLASS = ":selected";

     private Component _component = null!;
     private Matrix _transfromMatrix;
     private bool _pressHeld = false;

     public ComponentControl() {
          MainController.AppInitializedEvent += InvalidateVisual;
     }

     private void InvalidateVisual(SKSvg _)  => InvalidateVisual();

     #region Ava Properties
          public static readonly DirectProperty<ComponentControl, Component> ComponentProperty = AvaloniaProperty.RegisterDirect<ComponentControl, Component>(
               nameof(Component), o => o.Component, (o, v) => o.Component = v);

          public Component Component {
               get => _component;
               set {
                    SetAndRaise(ComponentProperty, ref _component, value);
                    TransformMatrix = _component?.Transform.ToAvaMatrix() ?? Matrix.Identity;
               }
          }

          public static readonly DirectProperty<ComponentControl, Matrix> TransformMatrixProperty =
              AvaloniaProperty.RegisterDirect<ComponentControl, Matrix>(nameof(TransformMatrix),
                   o => o.TransformMatrix,
                   (o, v) => o.TransformMatrix = v);
          
          public Matrix TransformMatrix {
             get => _transfromMatrix;
             set => SetAndRaise(TransformMatrixProperty, ref _transfromMatrix, value);
          }

          public static readonly StyledProperty<bool> IsSelectedProperty =
              AvaloniaProperty.Register<ComponentControl, bool>(nameof(IsSelected));

          public bool IsSelected {
             get => GetValue(IsSelectedProperty);
             set => SetValue(IsSelectedProperty, value);
          }

     #endregion

     #region Ava Props Handlers
          private void HandleComponentChanged(AvaloniaPropertyChangedEventArgs change) {
               if (change.OldValue is Component oldComponent) {
                    oldComponent.SpriteChangedEvent -= InvalidateVisual;
               }
               if (change.NewValue is Component newComponent) {
                    newComponent.SpriteChangedEvent += InvalidateVisual;
               }
          }

          private void HandleIsSelectedChanged(AvaloniaPropertyChangedEventArgs change) {
               PseudoClasses.Set(SELECTED_PSEUDOCLASS, IsSelected);
          }
     #endregion

     #region Events
          private void OnGlobalKeyUp(object? _, KeyEventArgs e) {
               if (_pressHeld && e.PhysicalKey == PhysicalKey.AltLeft) {
                    _component.OnControlRelease();
                    _pressHeld = false;
               } 
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
               if (!_pressHeld) {
                    _component.OnControlRelease();
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

          protected override void OnPropertyChanged(AvaloniaPropertyChangedEventArgs change) {
               base.OnPropertyChanged(change);
               if (change.Property == ComponentProperty) {
                    HandleComponentChanged(change);
               }
               if (change.Property == IsSelectedProperty) {
                    HandleIsSelectedChanged(change);
               }
          }
     #endregion

}