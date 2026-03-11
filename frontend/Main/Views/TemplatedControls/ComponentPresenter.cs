
using Avalonia;
using Avalonia.Controls;
using Avalonia.Media;
using Avalonia.Skia;
using ComponentManagement.Graph;
using Gui.Utils.ExtensionMethods;
using SkiaSharp;

namespace Gui.Views.TemplatedControls;

class ComponentPresenter : Control {

    private SKMatrix _transformMatrix => Component.Transform.ToSKMatrix();

    public override void Render(DrawingContext context) {
        context.Custom(new SvgDrawOp(Component.CurrentSprite, _transformMatrix));
    }

    protected override Size MeasureOverride(Size availableSize) {
        var rect = Component.CurrentSprite?.Picture!.CullRect;
        Rect bounds = _transformMatrix.MapRect(rect.Value).ToAvaloniaRect();
        return new Size(1000, 1000);
    }

    # region Ava Properties

        public static readonly StyledProperty<Component> ComponentProperty =
            AvaloniaProperty.Register<ComponentPresenter, Component>(nameof(Component));
        
        public Component Component
        {
            get => this.GetValue(ComponentProperty);
            set => SetValue(ComponentProperty, value);
        }

    #endregion
}