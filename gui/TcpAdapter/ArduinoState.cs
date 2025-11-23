namespace TcpAdapter;

public struct ArduinoState {
    const uint ANALOG_PIN_COUNT = 6;
    const uint DIGITAL_PIN_COUNT = 14;

    public double[] AnalogPins { get; private set; } = new double[ANALOG_PIN_COUNT];
    public bool[] DigitalPins { get; private set; } = new bool[DIGITAL_PIN_COUNT];

    public ArduinoState() {}

    public double GetAnalog(uint pin) {
        return AnalogPins[pin];
    }

    public bool SetAnalog(uint pin, double value) {
        if (pin >= ANALOG_PIN_COUNT) return false;
        AnalogPins[pin] = value;
        return true;
    }
    
    public bool GetDigital(uint pin) {
        return DigitalPins[pin];
    }

    public bool SetDigital(uint pin, bool value) {
        if (pin >= DIGITAL_PIN_COUNT) return false;
        DigitalPins[pin] = value;
        return true;
    }
}