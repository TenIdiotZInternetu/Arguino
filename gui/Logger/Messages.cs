namespace Logger;

public record struct InfoMessage(string Text) : IMessage {
    public string AsString() => Text;
    public string Type() => "INFO";
}

public record struct WarningMessage(string Text) : IMessage {
    public string AsString() => Text;
    public string Type() => "WARNING";
}

public record struct ErrorMessage(string Text) : IMessage {
    public string AsString() => Text;
    public string Type() => "ERROR";
}

public record struct DebugMessage(string Text) : IMessage {
    public string AsString() => Text;
    public string Type() => "DEBUG";
}
