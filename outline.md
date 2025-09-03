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

## Session 1
* Training environment
* Reference materials
* File system
* Shell
* POSIX API
* Sample application
* Practice

## Session 2
* Time
* Memory Management: Virtual vs Phys
* Process
* Signals (~SW Interrupts)
* IPC
* Sample application
* Practice

## Session 3
* Development tools
  * make
  * gdb
    * [How do we debug remotely?](https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.prog/topic/devel_Debug_session.html)
  * System logging: slogger2, slog2info (~Unix dmesg)
  * ssh

* Threading => Real-time OS & Thread Priority
  * Task queue & worker model
* IPC
  * Socket

* Practice #3: IPC with Socket & debugging

## Session 4
* Service-Oriented Architecture & vsomeip
* Practice #4: convert sample application from Session 3 to use vsomeip
