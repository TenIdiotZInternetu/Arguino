namespace TcpAdapter;

public struct ArduinoState {
    public const uint ANALOG_PIN_COUNT = 6;
    public const uint DIGITAL_PIN_COUNT = 14;

    public double[] AnalogPins { get; private set; } = new double[ANALOG_PIN_COUNT];
    public bool[] DigitalPins { get; private set; } = new bool[DIGITAL_PIN_COUNT];

    public ArduinoState() {}
}