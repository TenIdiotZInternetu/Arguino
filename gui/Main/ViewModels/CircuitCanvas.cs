using System.Collections.ObjectModel;
using CommunityToolkit.Mvvm.ComponentModel;
using ComponentManagement;

namespace Gui.ViewModels;

public class CircuitCanvas : ObservableObject {
    public ObservableCollection<Component> Components { get; } = new();
}