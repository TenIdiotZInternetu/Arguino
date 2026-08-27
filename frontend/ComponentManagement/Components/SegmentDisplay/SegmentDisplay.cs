using System.Net.Mime;
using System.Text;
using ComponentManagement.Circuitry;
using ComponentManagement.Scenes;
using Logger;

namespace ComponentManagement.Components.SegmentDisplay;

public class SegmentDisplay : Component {
    private struct _LogMessage(string Text) : IMessage {
        public LogLevel LogLevel => LogLevel.Debug;
        public string AsString() => Text;
        public string Type() => "DEBUG-7SEGMENT";
    }
    
    private static readonly string[] DIGIT_PINS = ["D1", "D2", "D3", "D4"];
    private static readonly string[] SEGMENT_PINS = ["A", "B", "C", "D", "E", "F", "G", "DP"];

    private const string OUT_PIN_PREFIX = "out_";

    public SegmentDisplay(string typeName) : base(typeName) { }

    internal override void OnInitialized() {
        foreach (var pin in Pins.Where(IsOutPin)) {
            pin.MakeWriteOnly();
        }

        foreach (var pin in Pins.Where(p => !IsOutPin(p))) {
            pin.MakeReadOnly();
        }
    }

    public override void OnPinStateChanged(Pin pin) {
        if (DIGIT_PINS.Contains(pin.Name)) {
            foreach (var segment in SEGMENT_PINS) {
                Pin segmentPin = GetPin(segment)!;
                Pin outPin = GetOutPin(pin, segmentPin)!;
                outPin.SetValue(pin.IsLow && segmentPin.IsLow);
            }
        }
        
        if (SEGMENT_PINS.Contains(pin.Name)) {
            foreach (var digit in DIGIT_PINS) {
                Pin digitPin = GetPin(digit)!;
                Pin outPin = GetOutPin(digitPin, pin)!;
                outPin.SetValue(pin.IsLow && digitPin.IsLow);
            }
        }

        Debug_ChangeState();
    }

    private Pin? GetOutPin(Pin digitPin, Pin segmentPin) =>
        GetOutPin(digitPin.Name!, segmentPin.Name!);

    private Pin? GetOutPin(string digit, string segment) {
        string pinName = OUT_PIN_PREFIX + digit[1] + segment;
        return GetPin(pinName);
    }

    private bool IsOutPin(Pin pin) {
        return pin.Name?.StartsWith(OUT_PIN_PREFIX) ?? false;
    }

    private void Debug_ChangeState() {
        if (ComponentManager.Logger?.Verbosity != LogLevel.Debug) {
            return;
        }

        StringBuilder sb = new();

        sb.Append("IN = ");
        sb.Append(" DIG: ");
        foreach (var digit in DIGIT_PINS) {
            Pin digPin = GetPin(digit)!; 
            sb.Append(digPin.IsHigh ? "1" : "0");
        }
        sb.Append(" | SEG: ");
        foreach (var seg in SEGMENT_PINS) {
            Pin segPin = GetPin(seg)!; 
            sb.Append(segPin.IsHigh ? "1" : "0");
        }
        ComponentManager.Logger.Log(new _LogMessage(sb.ToString()));
        sb.Clear();        

        sb.Append("OUT = ");
        foreach (var digit in DIGIT_PINS) {
            sb.Append(digit + ": ");
            foreach (var seg in SEGMENT_PINS) {
                Pin outPin = GetOutPin(digit, seg)!;
                sb.Append(outPin.IsHigh ? "1" : "0");
            }
            sb.Append(" | ");
        }
        ComponentManager.Logger.Log(new _LogMessage(sb.ToString()));
    }
}