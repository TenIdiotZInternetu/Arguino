using Logger;

namespace ComponentManagement;

public static class ComponentManager {
    public static ILogger? Logger;

    internal static void LogDebug(string messageText) =>
        Logger?.Log(new DebugMessage(messageText));

    internal static void LogInfo(string messageText) =>
        Logger?.Log(new InfoMessage(messageText));

    internal static void LogWarning(string messageText) =>
        Logger?.Log(new WarningMessage(messageText));

    internal static void LogError(string messageText) =>
        Logger?.Log(new ErrorMessage(messageText));
}