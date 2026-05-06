namespace Logger;

public readonly record struct DebugMessage(string Text) : IMessage {
    public LogLevel LogLevel => LogLevel.Debug;
    public string AsString() => Text;
    public string Type() => "DEBUG";
}

public readonly record struct InfoMessage(string Text) : IMessage {
    public LogLevel LogLevel => LogLevel.Info;
    public string AsString() => Text;
    public string Type() => "INFO";
}

public readonly record struct WarningMessage(string Text) : IMessage {
    public LogLevel LogLevel => LogLevel.Warning;
    public string AsString() => Text;
    public string Type() => "WARNING";
}

public readonly record struct ErrorMessage(string Text) : IMessage {
    public LogLevel LogLevel => LogLevel.Error;
    public string AsString() => Text;
    public string Type() => "ERROR";
}

