using System.Collections.ObjectModel;
using CommunityToolkit.Mvvm.ComponentModel;
using ComponentManagement.Graph;

namespace Gui.ViewModels;

public class CircuitCanvas : ObservableObject {
    public ObservableCollection<Component> Components { get; } = new();
}