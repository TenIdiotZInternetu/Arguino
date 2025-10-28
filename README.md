Prerequisites:  
- Boost (with ASIO)
- arduino-cli
- .NET 9

Current usage workflow:
  1. Run `preprocess.sh test` from the root directory
      - this will create `sketch.cpp` in the `core/executable` that will be linked with the rest of simulator
  2. Build `core/CMakeLists.txt`
  3. Run core
     - This will start the simulator and the TCP server, that exposes Arduino State through requests on port 8888
  4. Build and run gui
     - Starts client, that hooks on the port 8888, reads the state, and updates the Avalonia UI
