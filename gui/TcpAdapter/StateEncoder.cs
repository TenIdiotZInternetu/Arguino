namespace TcpAdapter;

public class StateEncoder : IEncoder<ArduinoState>
{
    public ArduinoState Decode(string message)
    {
        string[] pins = message.Split(':', StringSplitOptions.RemoveEmptyEntries);
        ArduinoState newState = new();

        for (uint i = 0; i < pins.Length; i++)
        {
            bool pinVal = pins[i] == "1";
            newState.SetDigital(i, pinVal);
        }

        return newState;
    }

    public string Encode(ArduinoState data)
    {
        throw new NotImplementedException();
    }
}