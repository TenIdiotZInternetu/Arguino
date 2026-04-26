using Logger;

namespace ComponentManagement;

public static class ComponentManager {
    public static ILogger? Logger;
    
    internal static void LogDebug(string messageText) =>
        Logger?.LogDebug(messageText);

    internal static void LogInfo(string messageText) =>
        Logger?.LogInfo(messageText);

    internal static void LogWarning(string messageText) =>
        Logger?.LogWarning(messageText);

    internal static void LogError(string messageText) =>
        Logger?.LogError(messageText);
}