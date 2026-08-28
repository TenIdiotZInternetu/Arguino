using System.Numerics;
using System.Globalization;

namespace ComponentManagement.Factory.Loaders;

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
        success = success && float.TryParse(values[0], CultureInfo.GetCultureInfo("en-US"), out x);
        success = success && float.TryParse(values[1], CultureInfo.GetCultureInfo("en-US"), out y);

        if (!success) {
            ComponentManager.LogError($"Incorrect vector2 definition '{valuePair}'; Float parsing failed.");
            return null;
        }
        
        return new Vector2(x, y);
    }
}