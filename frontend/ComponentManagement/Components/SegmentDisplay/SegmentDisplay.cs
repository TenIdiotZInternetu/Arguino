using ComponentManagement.Circuitry;
using ComponentManagement.Scenes;

namespace ComponentManagement.Components.SegmentDisplay;

public class SegmentDisplay : Component {
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
                outPin.SetValue(pin.IsHigh && segmentPin.IsHigh);
            }
        }
        
        if (SEGMENT_PINS.Contains(pin.Name)) {
            foreach (var digit in DIGIT_PINS) {
                Pin digitPin = GetPin(digit)!;
                Pin outPin = GetOutPin(digitPin, pin)!;
                outPin.SetValue(pin.IsHigh && digitPin.IsHigh);
            }
        }
    }

    private Pin? GetOutPin(Pin digitPin, Pin segmentPin) {
        string pinName = OUT_PIN_PREFIX + digitPin.Name![1] + segmentPin.Name;
        return GetPin(pinName);
    }

    private bool IsOutPin(Pin pin) {
        return pin.Name?.StartsWith(OUT_PIN_PREFIX) ?? false;
    }
}