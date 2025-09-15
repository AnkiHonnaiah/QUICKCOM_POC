# Speed Limit Warning Example

## Intro

This example shows how to use the QuickCom bundle for creating a simple application for Autosar Adaptive, based on Service Oriented Architecture. The application consists of two Executables that communicate using one Service, which in turn has one Event. The Client subscribes to the event and the Server provides updates for it.
The SpeedLimitMonitor executable simulates an application that checks if the current vehicle speed is above the detected speed limit. Then it sends an event in the form of an enumeration to indicate that the speed limit is not over the limit (NOT_OVER), it is slightly over the limit (OVER_LOW) or it is a lot over the limit (OVER_HIGH). The actual implementation in this example, simply iterates through the above values.
The WarningDisplay executable receives the event and depending on the value it will display the appropriate warning message.

The workflow is as follows:

- Define the Service interface and the executables (a.k.a the Model)
- Generate C++ API for the service interface
- Implement the application logic using the generated API
- Build the executables (using cmake)
- Run the executables

**IMPORTANT:** Before running any of the commands shown below, make sure your system fulfills the [system requirements] of the bundle. Alternatively use the devcontainer configuration included with the bundle.

## Model definition

For creating the model we use a json configuration file based on the QuickCom schema file, located in folder *./ModelBuilder/schema*. For more details see [online documentation].

The model itself can be found in the *Model/Config/SpeedLimitMonitorApplication.json* file. We recommend using VSCode for editing the file since it provides good support for working with schema files such as autocomplete features.
Once the model is defined in the json file then run the script that builds the corresponding AUTOSAR model. The model is stored in a temporary workspace in the AUTOSAR standard format of arxml files. This is needed as intermediate step for generating the C++ API, but there is no need to interact with the arxml files.

Start a console terminal in the `scripts` folder of the SpeedLimitMonitor example. All paths used in the scripts are relative to this folder.
Run

```zsh
./1_runGenerators.sh
```

The script builds the model and generates the code in one call.
The generated output will be stored at `../tmp/src-gen`

## Implement the application logic

With the generated API available, the application logic can be implemented using any C++ IDE of preference. We use CMake as a build system for the example application.
The application code is located under the Cpp folder and has the following structure:

- SpeedLimitWarningApplication
  - *CMakeLists.txt* - Main cmake file for the project
  - *LocalPaths.cmake* - contains variables that need to adapted to the local environment
  - *include* - contains header files that are common for all executables
  - *src*
    - *common* - source code that is common for all executables
    - *SpeedMonitorExecutable* - the code and cmake file for building the SpeedMonitor executable
    - *WarningDisplayExecutable* - the code and cmake file for building the WarningDisplay executable

## Build the executables

Run

```zsh
./2_buildApplication.sh
```

to execute the cmake command that compiles and links the application using the toolchain defined in the LocalPaths.cmake file (configured for gcc11).
Once the executables are built, they are installed in the path specified LocalPaths.cmake file (configured path: `./deploy`)
CMake will install the executable binary under `./deploy/<executableName>/bin` and the necessary runtime configuration under `./deploy/<executableName>/etc`. This folder structure is mandatory.

## Run the executables

The two executables can now be run and communicate with each other. There are two variants on how they should communicate. These variants,also refered to as the "deployment" or "binding" of the services, are: SomeIp and IPC.

By default, the service interfaces are deployed to IPC binding. This is pretty easy to configure and also the more performant deployment variant. However, IPC binding is only limited to executables running on the same machine and the communication between those executables cannot be traced as any other network traffic.

If you need to realize Inter-ECU-Communication or just to trace the traffic then it is equired to deploy interfaces on SomeIP.

### Variant: Deployment on IPC

For the IPC communication to be possible, the following commands need to be executed in a console:

```zsh
sudo ip link set lo multicast on
sudo ip route add ff01::0/16 dev lo
```

Otherwise it could result in such runtime errors:

```zsh
[AP00 vImc][ERROR] IPC multicast ipv6 client failed to initialize. Reason:{"Domain":"Osab", "Code":11, "Msg":"A communication partner could not be reached.", "User":"Host is unreachable.", "Data":101}
FATAL: IPC multicast client connect request failed.
```

For the communication over IPC to be established, the IPC service discovery daemon needs to be executed first. Do this by running:

```zsh
./3a_runIPCDaemon.sh
```

Then, start two console windows, one for the server executable (SpeedMonitorExecutable) and one for the client (WarningDisplayExecutable)
On one of them run:

```zsh
./3_runApplicationServer
```

and on the other:

```zsh
./3_runApplicationClient
```

Observe the log output and verify that the server is printing messages that look like this:

```zsh
[AP00 PSWI][INFO] PPortSpeedLimitServiceProvider::SendLimitNotOver(): Sending an event with new limit state. New value = OVER_LOW
```

The client is printing messages that look like this:

```zsh
[1713254070113537][AP01 PROC][INFO] YELLOW WARNING!
```

### Variant: Deployment on SomeIP

For realizing the deployment on SomeIP, you need to adapt the json model and you need to add the run the SomeIP daemon before the executables.

#### Model definition

The SomeIP binding needs to be configured explicitly in the model json file. Therefore you need to configure some general settings for the SomeIP communication as seen below.

```json
    "DeploymentConfig": {
        "NetworkConfig": {
            "UnicastIpAddress": "192.168.7.2",
            "ServiceDiscoveryIpAddress": "224.0.0.17"
        },
        "ServiceDeploymentConfig": {
            "DefaultDeploymentType": "SOMEIP"
        }
```

**Note:** The UnicastIpAddress should match to that of your network interface. To find out the address of your ethernet interface, open a terminal and type:

```zsh
ip address
```

#### Run the executables

The prebuilt SomeIP Daemon and its configuration files need to be installed in the same location as you application executables.
This is done in the script _3b_RunSomeIpDaemon.sh_
The SomeIP Daemon uses a configuration file _someipd-posix.json_ where all generated someip_config.json files should be listed.
The file is automatically created by the script and looks like below:

```json
{
    "applications": [
     "../SpeedMonitorExecutable/etc/someip_config.json", 
     "../WarningDisplayExecutable/etc/someip_config.json"
    ]
}
```

Since the executables will now communicate over SomeIp, the daemon needs to be started first.
This can be done by running:

```zsh
./3b_runSomeIPDaemon.sh
```

As soon as the SomeIp Daemon is running, all other applications using SomeIP Binding could now be started.

```zsh
./3_runApplicationServer
./3_runApplicationClient
```

[system requirements]: <https://help.vector.com/QuickCom/current/en/Help/html/index.html#about_easycom>
[online documentation]: <https://help.vector.com/QuickCom>
