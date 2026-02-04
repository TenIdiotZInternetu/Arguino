using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using ComponentManagement.Graph;

namespace ComponentManagement;

public class Scene {
    public Dictionary<string, Component> ComponentsMap { get; init; } = [];
    public List<ElectricalNode> Nodes { get; init; } = [];
}
        