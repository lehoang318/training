# QNX Training
* Purpose
  * To help the team minimize their adaptation time when switching from AUTOSAR/Baremetal => QNX

* Goals
  * Team members are able to independently program and debug basic applications

* Objectives
  * To provide information about
    * Development environment (tools, frameworks)
    * Basic concepts
      * File system
      * Memory Management
      * Processes and Threads
      * Inter-Process Communication
      * SOA & vSomeIP
    * Official support resources
  * To provide practical exercises about the basic concepts

## Preparation
* Setup environment
  * **WSL**
  * Cygwin

* Differences between Development Env. & Training Env.!!!

* [P-Recommendation] Wit QNX, it's usually possible to emulate your application in your PC before testing/deploying in real hardware!

## Session 1
* QNX Documentation | Linux Man
* File System
  * Dynamic-linked/Shared Libraries
  * [Where everything is stored](https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.user_guide/topic/files_FILELOCATIONS.html)

    | Windows Locations | QNX Locations with similar purposes |
    |---|---|
    | `C:\Windows\System32\` | `/bin`, `/sbin`, `/lib`, `/usr/bin`, `/usr/lib` |
    | `C:\Program Files` | `/usr/local/` |
    | `C:\Users\%USERNAME%` | `${HOME}` or `/home/${USER}` (or `/root` for root user) |

    * [P-Recommendation] for user-specific binaries' installation: `${HOME}/.local/`

  * Virtual files
    * `/dev/*`: Device files
    * `/proc/*`: Processes information
      * `/proc/<PID>/status`

  * How are they installed?
    * BSP
    * App. Store & Package Management => N/A
    * Manual compilation and installation

  * [Discussion] Is it possible to change/install new SWCs at runtime in AUTOSAR Systems?

* User management
  * root
  * groups
  * permission bits

* Environment Variables
  * `PATH`
  * `ld` and LD_LIBRARY_PATH
  * `which`/`where`
  * Example
    ```
    $ which gcc
    /usr/bin/gcc

    $ gcc --version
    gcc (Ubuntu 11.4.0-1ubuntu1~22.04) 11.4.0
    Copyright (C) 2021 Free Software Foundation, Inc.
    This is free software; see the source for copying conditions.  There is NO
    warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
    ```

* How does Shell work?
  * High-level sequence
    * User -command-> Shell
    * Shell -> Filesystem: locate corresponding executable
    * Shell -> OS: execute

  * Useful utilities
    * `ls`, `find`
    * `mkdir`, `move`
    * `env`
    * `pidin`, `ps`

* [Startup behaviors](https://www.qnx.com/developers/docs/7.1/#com.qnx.doc.neutrino.user_guide/topic/environment_LoggingIn.html)

* What is POSIX?
* Hello world!!!
  * Command line arguments
  * Environment variables

* Practice #1
  * Process command line arguments
  * Access environment variables
  * Check process info


## Session 2
* Time
  * System tick
  * Monotonic vs Wall-clock
  * Wall-clock: local vs gmt

* Memory Management: Virtual vs Phys

* Process

* Signals (~SW Interrupts)

* IPC
  * Shared memory
  * Message passing (introduction only)

* Practice #2: IPC with Shared Memory

## Session 3
* Development tools
  * make
  * gdb
    * [How do we debug remotely?](https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.prog/topic/devel_Debug_session.html)
  * System logging: slogger2, slog2info (~Unix dmesg)
  * ssh

* Threading => Real-time OS & Thread Priority
* IPC
  * Socket

* Practice #3: IPC with Socket & debugging

## Session 4
* Service-Oriented Architecture & vsomeip
* Practice #4: convert sample application from Session 3 to use vsomeip
