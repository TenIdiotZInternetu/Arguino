using ComponentManagement.Circuitry;
using ComponentManagement.Scenes;

namespace ComponentManagement.Components.ShiftRegister8Bit;

public class ShiftRegister8Bit : Component {
    public const int OUTPUT_BITS = 8;
    
    private Pin _dataPin = null!; 
    private Pin _clockPin = null!; 
    private Pin _latchPin = null!;
    private Pin _dataPassPin = null!;

    private byte _storedValue;
    private byte _readValue;
    
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
        const byte lastBit = 0x80;
        bool overflownBit = (_readValue & lastBit) != 0;
        _dataPassPin.SetValue(overflownBit);
        
        _readValue <<= 1;
        if (bit) {
            _readValue |= 1;
        }
    }

    private void StoreValue() {
        _storedValue = _readValue;
        
        for (int bit = 0; bit < OUTPUT_BITS; bit++) {
            Pin outPin = GetPin((uint) bit)!;
            bool isHigh = (_storedValue >> bit & 1) == 1;
            outPin.SetValue(isHigh);
        }
    }

    private bool IsOutPin(Pin pin) => pin.Name == null;
}