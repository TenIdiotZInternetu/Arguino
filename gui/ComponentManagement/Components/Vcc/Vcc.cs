using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection.Metadata;
using System.Threading.Tasks;

namespace ComponentManagement.Components;

public class Vcc : Component
{
    // TODO: Adjustable parameter in config/in main window
    public const float VOLTS = 5;

    public Vcc(string definitionPath) : base(definitionPath) {}
}