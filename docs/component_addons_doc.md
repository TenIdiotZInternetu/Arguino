# Component Addons Documentation
Extensibility of the GUI through custom components and circuits is an important part the project. Here are necessary steps and requirements to create and use your own component.

### Notation notes
- YAML
  - angle brackets must be replaced by proper parameters, i.e. `name: <string>`
  - properties with question mark are optional, i.e. `optional?: ...`
  - Vector2 is represented as a string of 2 float values separated by a space, i.e. `vector: 7.27 6.9` 

## Defintions file structure
All of the components must reside in one directory, referred to as "definitions directory". Each component is represented by exactly one directory, referred to as "component directory", placed in the definitions directory. The definitions directory must be supplied through the `--components | -c` command line argument.

These files must be present in the component directory:
- C# behavior script
- YAML component configuration
- 1 or more .svg images

The yaml file must have the same name as the component directory, i.e. `MyComponent/MyComponent.yaml`.

The SVGs will be referenced in behavior script by their file name, excluding the .svg extension. For example `image.svg` will be referred to as `image`.

## YAML component configuration
This file defines common properties of the custom component type.

```
name: <string>
description?: <string>
imageSize?: <Vector2>
pins?: <uint>
pinNames?:
  - <string>
  - ...
  - ...
```

#### name
Name of the component type as displayed in the UI. (not used yet)

#### description
Brief description of the component as displayed in the UI. (not used yet)

#### imageSize
Desired size of the component's sprites in pixels for default zoom level of the canvas. Defaults to <50, 50>.

#### pins
Number of unnamed pins. These pins are identified by their 0-based index in both the scene definition and in the component logic.

#### pinNames
List of unique names for each pin. These pins are identified by their given name in both the scene defintion and in the component logic.

## C# behavior script
This C# class will determine how the component interacts with the circuit. The file must added to the `ComponentManagement.csproj` project. It must contain a class inheriting from the abstract `Component` class. In order to compile, the user's class must also define a constructor like this:

`public MyComponent(string definitionPath) : base(definitionPath) { ... }`

However, no code is required in the body of the constructor. The constructor can be used for initialization of properties known before the circuit created. The `Component` class provides most of the necessary API.

### Component functions an properties

#### virtual void OnInitialized()
Gets called when all of the component instances have been initialized and scene created.

#### virtual void OnPinConnected(Pin)
Gets called when a component's pin connects to an electric node. The connected pin is passed as argument.

#### virtual void OnPinDisconnected(Pin)
Gets called when a component's pin cdisonnects from an electric node. The disconnected pin is passed as argument.

#### virtual void OnPinStateChanged(Pin)
Gets called when a component's pin detected a change in digital state of a node it has been connected to. It can be thought of as changing input value on the pin. Pins set to WriteOnly mode by design don't detect this change. The detected pin is passed as argument.

#### virtual void OnControlPress(Vector2)
Gets called when the component has been pressed on the circuit canvas. The cursor position at the time of the press is passed as argument.

#### virtual void OnControlRelease
Gets called when the component has been released on the circuit canvas.

#### void UpdateSprite(string)
Changes the .svg image drawn to the canvas. The new image is given by the svg name specified in the argument.

#### Pin? GetPin(string)
Returns a Pin object with the name given in the argument. The name must be defined in the component's `namedPins` list. Returns null if no such pin exists.

#### Pin? GetPin(uint)
Returns a Pin object with the id given in the argument. The allowed range of Ids is from `0` to `pins + namedPins.Length`. With unnamed pins starting from the id `0` and named pins starting from the id `pins`. Returns null if the pin id is out of range.

### Pin functions an properties
#### uint Pin.Id
Id of the pin, as described in the `GetPin(uint)` section.

#### string Pin.Name
Name of the pin as described in the component configuration.

#### bool Pin.IsDriving
True if the pin is driving. That is, if it actively forces the digital state on the connected node to be high. If the node has at least 1 driving pin, it's digital state will be high. It will be low otherwise.

#### bool Pin.IsHigh
True if the digital state of the connected node is high.

#### bool Pin.IsLow
True if the digital state of the connected node is low.

#### bool Pin.IsWriteOnly
WriteOnly pins don't react to changes in the connected node's digital state.

#### bool Pin.IsReadOnly
ReadOnly pins cannot be set to driving.

#### bool Pin.IsConnected
Whether is the pin connected to any electrical node.

#### void Pin.MakeReadOnly()
Sets the pin to ReadOnly mode. ReadOnly pins cannot be set to driving.

#### void Pin.MakeWriteOnly()
Sets the pin to WriteOnly mode. WriteOnly pins don't react to changes in the connected node's digital state.

#### void Pin.MakeGeneral()
Sets the pin to General mode. General pins don't pose any restrictions to reading or writing.

#### void Pin.SetValue(bool)
If the argument is true, it sets the pin to be driving. Otherwise, the pin set to not drive.

#### void Pin.SetHigh()
Sets the pin to be driving. Same as `SetValue(true)`.

#### void Pin.SetLow()
Sets the pin to be not driving. Same as `SetValue(false)`.

## YAML scene definition
This file component instances present in the scene, and how are their pins connected together in nodes. The path of this file is supplied through the `--scene | -s` command line argument. There are no limits to where the scene can be placed.

```
components:
  <Type name>:
    <Instance name>:
      position: <Vector2>
      rotation?: <float>
      scale?: <Vector2>
    <Instance name>:
      ...
nodes:
  - [ <Instance name>.<Pin name>, ...]
  - ...
  - ...
```

#### components
Dictionary of component type objects. These objects are dictionaries of component instances of that type. These instances define properties of the individual components. 

#### Type name
String. Name of the component type. This must match the component directory name.

#### Instance name
String. Name of the component instance. It identifies the instance within the node definition and in the UI. It must unique across all instances within the components dictionary.

#### position
Coordinates on the circuit canvas, with origin in the upper left corner.

#### rotation
Clockwise rotation angle in degrees on the circuit canvas.

#### scale
Relative scaling along x an y axis on the circuit canvas.

#### nodes
List of lists, where the inner element is an instance-pin pair separated by dot. Instance name must present in the components dictionary. Pin name must be present in the definition configuration. In the case of unnamed pins, pin name is simply its index.