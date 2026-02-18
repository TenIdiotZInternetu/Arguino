using System;
using System.Collections.Generic;
using System.Linq;
using System.Numerics;
using System.Threading.Tasks;
using Logger;

namespace ComponentManagement.Utils;

public static class YamlUtils {
    public static Vector2? StringToVector2(string valuePair) {
        if (valuePair == "") {
            ComponentManager.LogDebug("Encountered empty string when expected vector string; Using defualt value instead.");
            return null;
        }

        string[] values = valuePair.Split();
        if (values.Length != 2) {
            ComponentManager.LogError($"Incorrect vector2 definition '{valuePair}'; Must be 2 float values separated by a space.");
            return null;
        }

        float x = 0, y = 0;
        bool success = true;
        success = success && float.TryParse(values[0], out x);
        success = success && float.TryParse(values[1], out y);

        if (!success) {
            ComponentManager.LogError($"Incorrect vector2 definition '{valuePair}'; Float parsing failed.");
            return null;
        }
        
        return new Vector2(x, y);
    }
}