using System.Text;

namespace TcpAdapter;

public class StateEncoder : IEncoder<ArduinoState>
{
    public ArduinoState Decode(string message) {
        string[] pins = message.Split(':', StringSplitOptions.RemoveEmptyEntries);
        ArduinoState newState = new();
        
        // TODO: Validate pin count
        for (uint i = 0; i < pins.Length; i++) {
            newState.PinModes[i] = pins[i][0] switch {
                'I' => ArduinoState.PinMode.In,
                'O' => ArduinoState.PinMode.Out,
            };
            
            newState.DigitalPins[i] = pins[i][1] switch {
                '0' => false,
                '1' => true
            };
        }

        return newState;
    }

    public string Encode(ArduinoState data) {
        StringBuilder sb = new();
        foreach (bool pin in data.DigitalPins) {
            sb.Append($":{(pin ? '1' : '0')}");
        }
        
        return sb.ToString();
    }
}