using System.Collections.ObjectModel;
using System.ComponentModel;
using CommunityToolkit.Mvvm.ComponentModel;
using Gui.Controls;
using Gui.Views.TemplatedControls;

namespace Gui.ViewModels;

public class CircuitCanvas : ObservableObject {
    public ObservableCollection<Component> Components { get; } = new();
}