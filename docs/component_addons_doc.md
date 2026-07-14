## Defining components and scenes
Extensibility of the frontend through custom components and circuits is an important part the project. Here are necessary steps and requirements to create and use new components.

### Required file structure
All components must reside in one directory, referred to as *definitions directory*. Each component is represented by exactly one directory, referred to as *component directory*, placed in the definitions directory. The definitions directory must be supplied through the `-c` command line argument. These files must be present in the component directory:

* C# behavior script
* YAML component configuration
* One or more .svg images

The yaml file must have the same name as the component directory, i.e. `MyComponent/MyComponent.yaml`. The SVGs will be referenced in behavior script by their file name, excluding the .svg extension. For example `image.svg` will be referred to as `image`.

#### Notes on used YAML notation
In the next subsections, we will describe the expected structures of the YAML files using this notation:
* angle brackets must be replaced by proper parameters, i.e. `name: <string>`
* properties with question mark are optional, i.e. `optional?: ...`
* Vector2 is represented as a string of 2 float values separated by a space, i.e. `vector: 7.27 6.9` 

### Component configuration file specification
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

* **`name`**: Name of the component type as displayed in the UI. (not used yet)
* **`description`**: Brief description of the component as displayed in the UI. (not used yet)
* **`imageSize`**: Desired size of the component's sprites in pixels for default zoom level of the canvas. Defaults to <50, 50>.
* **`pins`**: Number of unnamed pins. These pins are identified by their 0-based index in both the scene definition and in the component logic.
* **`pinNames`**: List of unique names for each pin. These pins are identified by their given name in both the scene defintion and in the component logic.

### Component behavior script specification
How does the component interact with the rest of the circuit is described by the C# class with the named the same as the component type. This class must inherit from the abstract `Component` class. The file containing the class must be added to the `ComponentManagement.csproj` project. In order to compile, the user's class must also define a constructor like this:

    public MyComponent(string definitionPath) 
    : base(definitionPath) { ... }

However, no code is required in the body of the constructor. The constructor can be used for initialization of properties known before the scene is created. The `Component` class provides most of the necessary API.

#### Component functions and properties
* **`virtual void OnInitialized()`**: Gets called when the component instance has been completed and all of its assets are available (config, pins, sprites, transform, etc.)
* **`virtual void OnPinConnected(Pin)`**: Gets called when a component's pin connects to an electric node. The connected pin is passed as argument.
* **`virtual void OnPinDisconnected(Pin)`**: Gets called when a component's pin disconnects from an electric node. The disconnected pin is passed as argument.
* **`virtual void OnPinStateChanged(Pin)`**: Gets called when a component's pin detected a change in digital state of a node it has been connected to. It can be thought of as changing input value on the pin. Pins set to write only mode by design don't detect this change. The detected pin is passed as argument.
* **`virtual void OnControlPress(Vector2)`**: Gets called when the component has been pressed on the circuit canvas. The cursor position at the time of the press is passed as argument.
* **`virtual void OnControlRelease`**: Gets called when the component has been released on the circuit canvas.
* **`void UpdateSprite(string)`**: Changes the .svg image drawn to the canvas. The new image is given by the svg name specified in the argument.
* **`Pin? GetPin(string)`**: Returns a Pin object with the name given in the argument. The name must be defined in the component's `namedPins` list. Returns null if no such pin exists.
* **`Pin? GetPin(uint)`**: Returns a Pin object with the ID given in the argument. The allowed range of Ids is from `0` to pins + namedPins.Length. With unnamed pins starting from the ID `0` and named pins starting from the ID `pins`. Returns null if the pin ID is out of range.

#### Pin functions and properties
* **`uint Pin.Id`**: Id of the pin, as described in the `GetPin(uint)` section.
* **`string Pin.Name`**: Name of the pin as described in the component configuration.
* **`bool Pin.IsDriving`**: True if the pin is driving. That is, if it actively forces the digital state on the connected node to be high. If the node has at least 1 driving pin, its digital state will be high. It will be low otherwise.
* **`bool Pin.IsHigh`**: True if the digital state of the connected node is high.
* **`bool Pin.IsLow`**: True if the digital state of the connected node is low.
* **`bool Pin.IsWriteOnly`**: Write only pins don't react to changes in the connected node's digital state.
* **`bool Pin.IsReadOnly`**: Read only pins cannot be set to driving.
* **`bool Pin.IsConnected`**: Whether is the pin connected to any electrical node.
* **`void Pin.MakeReadOnly()`**: Sets the pin to read only mode. Read only pins cannot be set to driving.
* **`void Pin.MakeWriteOnly()`**: Sets the pin to write only mode. Write only pins don't react to changes in the connected node's digital state.
* **`void Pin.MakeGeneral()`**: Sets the pin to general mode. General pins don't pose any restrictions to reading or writing.
* **`void Pin.SetValue(bool)`**: If the argument is true, it sets the pin to be driving. Otherwise, the pin set to not drive.
* **`void Pin.SetHigh()`**: Sets the pin to be driving. Same as `SetValue(true)`.
* **`void Pin.SetLow()`**: Sets the pin to be not driving. Same as `SetValue(false)`.

### Defining scenes
The scene YAML file describes component instances present in the scene, and how are their pins connected together in nodes. The path of this file is supplied through the `-s` command line argument. There are no restriction to where the scene file has to be placed.

```
components:
  <Type name>:
    <Instance name>:
      position: <Vector2>
      rotation?: <float>
      scale?: <Vector2>
      extraProps?:
        <Property 1>: <Value 1>
        <Property 2>: <Value 2>
        ...
    <Instance name>:
      ...
nodes:
  - [ <Instance name>.<Pin name>, ...]
  - ...
  - ...
```

* **`components`**: Dictionary of component type objects. These objects are dictionaries of component instances of that type. These instances define properties of the individual components. 
* **`Type name`**: String. Name of the component type. This must match the component directory name.
* **`Instance name`**: String. Name of the component instance. It identifies the instance within the node definition and in the UI. It must be unique across all instances within the components dictionary.
* **`position`**: Coordinates on the circuit canvas, with origin in the upper left corner. The coordinates must be positive, and lower than the expected size of the canvas in pixels.
* **`rotation`**: Clockwise rotation angle in degrees on the circuit canvas.
* **`scale`**: Relative scaling along x and y axis on the circuit canvas.
* **`extraProps`**: A directory of custom properties defined in the component subclass as keys and values to assign to them. The properties must match the case and the type of the property. The property must be public and have a public setter.
* **`nodes`**: List of lists. The inner list represents all pins connected to the same electrical node. Its element is an instance-pin pair separated by a dot. Instance name must be present in the components dictionary. Pin name must be present in the component's configuration. In the case of unnamed pins, pin name is simply its index.