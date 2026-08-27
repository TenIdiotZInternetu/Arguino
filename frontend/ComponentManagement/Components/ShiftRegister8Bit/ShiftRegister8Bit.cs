using System.ComponentModel;
using System.Text;
using ComponentManagement.Circuitry;
using Logger;
using SkiaSharp;
using Component = ComponentManagement.Scenes.Component;

namespace ComponentManagement.Components.ShiftRegister8Bit;

public class ShiftRegister8Bit : Component {
    private struct _CompMessage(Component Comp, string Text) : IMessage {
        public LogLevel LogLevel => LogLevel.Debug;
        public string AsString() => Text;
        public string Type() => "DEBUG:" + Comp.Name;
    }
    
    public const int OUTPUT_BITS = 8;
    public const byte MS_BIT = 0b10000000;
    public const byte LS_BIT = 0b00000001;
    
    private Pin _dataPin = null!; 
    private Pin _clockPin = null!; 
    private Pin _latchPin = null!;
    private Pin _dataPassPin = null!;

    private byte _storedValue;
    private byte _readValue;

    private List<bool> _debugReadValues = [];
    
    public ShiftRegister8Bit(string typeName) : base(typeName) { }

    internal override void OnInitialized() {
        _dataPin = GetPin("data")!;
        _clockPin = GetPin("clock")!;
        _latchPin = GetPin("latch")!;
        _dataPassPin = GetPin("data_pass")!;
        
        _dataPin.MakeReadOnly();
        _clockPin.MakeReadOnly();
        _latchPin.MakeReadOnly();
        _dataPassPin.MakeWriteOnly();

        for (uint bit = 0; bit < OUTPUT_BITS; bit++) {
            GetPin(bit)!.MakeWriteOnly();
        }
    }

    public override void OnPinStateChanged(Pin pin) {
        if (pin == _clockPin && _clockPin.IsHigh) {
            LoadNextBit(_dataPin.IsHigh);
        }

        if (pin == _latchPin && _latchPin.IsHigh) {
            StoreValue();
        }
    }

    private void LoadNextBit(bool bit) {
        _readValue <<= 1;
        if (bit) {
            _readValue |= LS_BIT;
        }
        
        bool overflownBit = (_readValue & MS_BIT) != 0;
        _dataPassPin.SetValue(overflownBit);
        Debug_UpdateInput(bit);
    }

    private void StoreValue() {
        _storedValue = _readValue;
        
        for (int bit = 0; bit < OUTPUT_BITS; bit++) {
            Pin outPin = GetPin((uint) bit)!;
            bool isHigh = (_storedValue >> bit & 1) == 1;
            outPin.SetValue(isHigh);
        }
        
        Debug_Log();
    }

    private bool IsOutPin(Pin pin) => pin.Name == null;

    private void Debug_UpdateInput(bool bit) =>
        _debugReadValues.Add(bit);

    private void Debug_Log() {
        if (ComponentManager.Logger?.Verbosity != LogLevel.Debug) {
            return;
        }
        
        StringBuilder sb = new();
        
        sb.Append("IN =");
        for (int i = 0; i < _debugReadValues.Count; i++) {
            if (i % 8 == 0) {
                sb.Append(' ');
            }
            sb.Append(_debugReadValues[i] ? 1 : 0);
        }
        ComponentManager.Logger.Log(new _CompMessage(this, sb.ToString()));
        sb.Clear();

        sb.Append("OUT = ");
        string storedValAsBinaryStr = Convert.ToString(_storedValue, 2).PadLeft(OUTPUT_BITS, '0');
        sb.Append(storedValAsBinaryStr);
        ComponentManager.Logger.Log(new _CompMessage(this, sb.ToString()));
        _debugReadValues.Clear();
    }
}