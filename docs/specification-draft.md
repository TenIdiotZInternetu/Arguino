# Specification of Architecture

The project is split into two parts: Core and Gui. 

Core is responsible for running the simulation of user's Arduino code. It also runs the TCP server which feeds the state of Arduino's pinout into other client applications.

Gui is an example of a client connected to the Core. It periodically sends messages to ask about the Arduino state, and then reflects that state through a graphical user interface. Electronic components can be added to the Gui and connected to the Arduino board to observe interactions with the user's program.

## Core

Core is written in C++20 and built upon these submodules:

### Simulator
The simulator consists of a reduced version of the standard `Arduino.h`, it's mock implementation and the Arduino State. A single global state is created for the needs of the user's program. The implementations of the standard Arduino functions change this state instead of an actual hardware. The state must be global, since the user will not refer to it in their code, nor will the refer to class or namespace that could contain it.

The Arduino State should at least include:
- States of Analog and digital pins
- Modes of the pins
- Serial Buffer

**Suggestion:**  
Encapsulate the user's code in a same namespace as the mock implementations during preprocessing.

### Executable
This submodule contains the main function. Here, the server and the state are initialized. It also calls `setup()` and `loop()` defined in the user's code.

Before compiling the user's code must be preprocessed by `preprocess.sh`, which turns the user's `.ino` file into a regular `sketch.cpp` source file, that is then linked to the `main.cpp`.

**Note:**  
Currently the preprocess script just calls `arduino-cli` with a preprocess flag. For the current scope of the project this should be enough, but we can extended for a better control over the user's program.

### TCP Adapter
Runs the TCP Server. It is written with the Boost ASIO library in an idiomatic asynchronous approach. When launched it starts listening on the port passed into its constructor. A new Connection Handler with a socket is created. Upon connection, incoming messages are passed through this socket and processed by the handler. Another handler is created and the cycle continues. Type of the handler is specified through the template parameter of the Server.

Handler then reads incoming messages repeatedly until the connection is closed. At least these types of incoming messages should be supported:
- Read Core's arduino state
- Write to Core's state input from the client
- Pause/Unpause the simulation
- Stop/Reset the simulation

During Read and Write operations, an Encoder passed as a template parameter is used to encode an Arduino state into string message, and vice versa, according to a predefined protocol.

The TCP Server must run a on a different thread then the rest of the Core, since ioContext is blocking when run.

**Note**:  
Due to a dependence on the Arduino State, the Handler and Encoder implementations are currently in the library of Executable submodule. Ideally the dependecy should be removed and the implementations moved into the Adapter.


## Gui
Gui application is written in C# and built upon these subprojects.

### Main
The entirety of Avalonia UI is encapsulated in here. Here the entire layout, look and custom controls are defined. It's also the entry point of the application where other systems like the TCP Client and Component System, and potentially the Simulator are initialized.

Planned elements of the Main Window are:
- Canvas with Arduino and the electronic components
    - The canvas should be zoomable and pannable
    - The user's component must be drawn as Svg to the canvas, and their visuals must be able to change during runtime
    - The user has to be able to interact with components like buttons or potentiometers
    - The components should be selectable and their state be visible in an inspector
    - The components could be potentially moved on canvas when selected
    - Wires that connect components could be potentially drawn on the canvas
- Inspector with the Arduino State or a selected component
- Panel with logs
- Panel with Serial output
- Buttons to start, pause, stop and step the simulation
- Options to serialize the current state/workspace
- Potentially a component library, to freely add components to the Canvas
- Timer

**Note:**  
Avalonia UI doesn't have builtin tools for rendering Svgs as elements. But this functionality can be added, as Avalonia's render system is built on SkiaSharp library, which supports Svgs. I used a package that allows changing the DOM of loaded Skia Svgs.

### TCP Adapter
Counterpart to Core's TCP Adapter. Messages are sent through TCP Client to the Core's Server, and the response is handled by the IMessageHandler. This adapter keeps it's own Arduino State that is periodically updated through the Read message. Read and Write messages are again processed by an Encoder. Upon reading a message, events according to type of the message are raised that can be subscribed to in the rest of the app.

### Component Management
The app should be extensible with user defined Electronic components.While the Core must be recompiled everytime the user changes the Arduino sketch, the Gui should be built once and ready-to-go solution. That means the configuration that building the code for these components must be done during the Gui's runtime.

Each of the components should be located in a single directory, which is passed as a command line argument. Then each component is represented by a single directory. At least these files are must be present in the directory to define the configuration:
- Configuration (.yaml/.json)
  - Component metadata (i.e. name)
  - Pin names, pin arrays
  - Child components
    - Their names
    - Visual positioning relative to the parent
    - Connections to the parent's pins
- Behavior script (preferably .cs with Roslyn)
  - initialization of the component
  - fields visible to the inspector
  - functions subscribed to the pin events (OnRisingEdge, OnFallingEdge, etc.)
  - modified svg references to show during diffferent states of the component
- Visuals (one or more .svg)

During intialization a Component Graph must be created with Arduino state as its "root". An edge between two components represent pair of pins that are wired together. A node holds list of all connected components, and an object of the user created component type. When value in wire changes, the connected component is notified by raising an event. The behavior script than changes the state of the component potentially triggering changes in other wired components.

A library for the behaviour scripts must be prepared for the users to use, to abstract away the intricacies of showing the components on the canvas and interacting with graph. The scripts should be able to compile during intitialization with Roslyn. The ViewModel of a component needs only a reference to the svg that currently represents the state of the component.

Common components like LEDs, buttons, logic gates, display etc., should be already included, and provide interface for use in more complicated components imported by the user.