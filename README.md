# MICROSAR QuickCom bundle

## Intro

MICROSAR QuickCom is a lightweight development kit for communication management. It provides several tools to help you set up Service-Oriented Communication (SOC) for your automotive application:

- JSON-based configuration of your communication API
- Adaptive SDK for generating customized C++ API
- Example App to use as a base for your first application

The API is designed in accordance with the latest automotive standards and can be used in conjunction with specific bindings, such as SOME/IP or IPC, as the service-oriented transport protocol layer.

## Contents

The downloaded bundle consists of the following:

- SDK

    The SDK automates the development tasks, providing the necessary tools and libraries to support the middleware with minimal direct interaction from the developer. It simplifies integration and streamlines the development process.

- ModelBuilder

    The ModelBuilder uses the QucikCom JSON configuration file to generate proxies, skeletons, and other APIs needed for application development.

- Examples

    In the Examples folder you will find example applications that you can use as a learning tool and as a base for your own application. Please check out the README file which you will find at the root of the Examples folder.

- .devcontainer

    The configuration files for using the QuickCom bundle with a pre-configured development container. This is optional. Otherwise, make sure your system fulfills the system requirements mentioned below.

## System Requirements

MICROSAR QuickCom requires a Linux environment and it is tested with Ubuntu 22.04.

System Requirements
Operating System : Linux (e.g. Ubuntu version 22.04)

Packages :

- [CodeMeter] User Runtime for Linux
- [AxProtector] Runtime for Linux

You can download them at wibu.com: [User Software]

Compiler : gcc11 (default compiler for Ubuntu 22.04)

Tooling :

- CMake Version 3.22 or higher
- Glibc 2.35
- IDE (e.g. Visual Studio Code)
- Docker (optional)

[User Software]: https://www.wibu.com/support/user/user-software.html
[Codemeter]: https://www.wibu.com/support/user/user-software/file/download/14414.html
[AxProtector]: https://www.wibu.com/support/user/user-software/file/download/14473.html
