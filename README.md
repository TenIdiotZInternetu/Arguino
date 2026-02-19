## Prerequisites:  
- Boost (with ASIO)
- arduino-cli
- .NET 9

## Current usage workflow:
  1. Run `preprocess.sh <test dir>`. For example `preprocess.sh tests/buttons`
  2. Build `core/CMakeLists.txt`
  3. Run core as `<build dir>/executable/arguino --single-process`
  4. Build and run gui from the `gui` directory with specified scene. For example `<build dir>/Gui -s ./arduinoScene.yaml`

## Command line arguments:
### Core
For stable implementation run with `--single-process`, other options are experimental and subject to change

### Gui
- `-s | --scene <path>` - Path to the .yaml scene defintion [default `./scene.yaml`]
- `-c | --components <path>` - Path to the directory of components definitions [default `./ComponentManager/Components`]
- `-p | --port <int>` - TCP port to listen to. Core is hard-coded to 8888 [default `8888`]
- `--no-tcp` - Skips connection to TCP server at initialization
- `--log-tcp <path>` - Path to the log file for TCP messages [default `./frontend_tcp.log`]
- `--log-circuit <path>` - Path to the log file for general circuitry events [default `./frontend.log`]

