## Prerequisites:  
- Boost (with ASIO)
- arduino-cli
- .NET 9

## Current usage workflow:
  1. Run `preprocess.sh <test dir>`. For example `preprocess.sh tests/buttons`
  2. Build `backend/CMakeLists.txt`, target either `arguino` or `arguino-tcp.exe` (they are aliases)
  3. In no specific order
      - Run backend as `<build dir>/executable/arguino`
      - Build and run gui from the `frontend` directory with specified scene. For example `<build dir>/Gui -s ./Scene/buttons.yaml`

## Command line arguments:
### Core
- `--log-simulator <path>` - Path to the log file for simulator events [= `./core.log`]
- `--log-ipc <path>` - Path to the log file for IPC" [= `./core_ipc.log`]
- `-v | --verbosity <Debug | Info | Warning | Error>` - Verbosity of the log files [= `Info`]
#### TCP specific
- `-p | --port <int>` - Port on which to launch TCP server [= `8888`]
#### Shared memory specific
- `-n | --shmem-name <string>` - Name of the shared memory object [= `Arguino-ipc`]
- `-s | --shmem-size <int>` - Size of each of the write and read shared memory segments in pages [= `1`]

### Gui
- `-s | --scene <path>` - Path to the .yaml scene defintion [= `./scene.yaml`]
- `-c | --components <path>` - Path to the directory of components definitions [= `./ComponentManager/Components`]
- `-i | --ipc <None | Tcp | Shmem>` - Interprocess communication technology used for connection with the simulator [= `Shmem`]
- `--log-circuit <path>` - Path to the log file for general circuitry events [= `./frontend.log`]
- `--log-ipc <path>` - Path to the log file for IPC [= `./frontend_ipc.log`]
- `-v | --verbosity <Debug | Info | Warning | Error>` - Verbosity of the log files [= `Info`]
#### TCP specific
- `-p | --port <int>` - TCP port to listen to [= `8888`]
#### Share memory specific
- `--shmem-name` - Name of the memory mapped file [= `Arguino-ipc`]
- `--shmem-size` - Size of the memory mapped region per buffer in pages [= `1`]
 
## Gui controls
- `Mouse wheel scroll` - Zoom and unzoom canvas
- `Mouse wheel press` - Pan canvas
- `Alt hold` - Hold mode. All controls clicked on will remain in a press state when clicked, until the Alt key is released. For example when holding multiple buttons is required 

## Examples
### Scene with 2 buttons and 4 LEDs (WIP)
![scene_example](./res/scene_example.png)
