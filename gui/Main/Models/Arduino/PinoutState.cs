namespace gui.Models.Arduino;

public class PinoutState {
    const uint ANALOG_PIN_COUNT = 6;
    const uint DIGITAL_PIN_COUNT = 14;

    private double[] _analogPins = new double[ANALOG_PIN_COUNT];
    private bool[] _digitalPins = new bool[DIGITAL_PIN_COUNT];

    public double GetAnalog(uint pin) {
        return _analogPins[pin];
    }

    public bool SetAnalog(uint pin, double value) {
        if (pin >= ANALOG_PIN_COUNT) return false;
        _analogPins[pin] = value;
        return false;
    }
    
    public bool GetDigital(uint pin) {
        return _digitalPins[pin];
    }

    public bool SetDigital(uint pin, bool value) {
        if (pin >= DIGITAL_PIN_COUNT) return false;
        _digitalPins[pin] = value;
        return true;
    }
}