# QNX - Session 1
* Development Environment: [env-development.uml](./illustrations/env-development.uml)
* Training environment:  [env-training.uml](./illustrations/env-training.uml)
* References
  * [QNX Documentation](https://www.qnx.com/developers/docs/8.0/#com.qnx.doc.qnxsdp.nav/topic/bookset.html)
  * [QNX Community](https://www.qnx.com/developers/community.html)
    * Note: Data security!!!
  * QNX Customer Support
  * [Beginning Linux Programming](https://www.oreilly.com/library/view/beginning-linux-programming/9780470147627/9781118058619f07.xhtml)
  * IEEE Std 1003.1: POSIX Specifications (not recommended)

* Agenda
  * File System
  * Shell
  * POSIX API
  * Sample application
  * Practice

## File System
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

* [Dynamic-linked/Shared Libraries](https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.sys_arch/topic/dll.html)
  * How to link?
    ```
    gcc –o <executable file path> <object files> –L<look-up path> –l<library name>
    ```

  * How shared libraries are located at runtime? `${LD_LIBRARY_PATH}`
  * How to identify dependencies? [ldd](https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.utilities/topic/l/ldd.html)
  * Examples
    * [C Library Reference](https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.lib_ref/topic/full_safety_a.html)
    * `pthread_create()`
      * [QNX - libc](https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.lib_ref/topic/p/pthread_create.html)
      * [Linux - libpthread](https://man7.org/linux/man-pages/man3/pthread_create.3.html)


* How are they installed?
  * BSP
  * App. Store & Package Management => N/A
  * Manual compilation and installation

* [Discussion] Is it possible to change/install new SWCs at runtime in AUTOSAR Systems?

* [File permissions](https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.user_guide/topic/files_PERMISSIONS.html)
  * Is it a regular file or a directory or a symbolic link?
  * What could `Owner`/`Owner Group`/`Others` do with the file?
  * Note: directories, regular files or symbolic links are all considered as `Files`
    ```
    In a QNX OS system, almost everything is a file; devices, data, and even services are all typically represented as files.
    ```

## Shell/Command Interpreter
* What is Shell?
  * [Interactions](./illustrations/shell-interactions.uml)
  * [Acvities](./illustrations/shell-activities.uml)
  * [QNX Shells](https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.user_guide/topic/cmdline_Shell.html): `uesh`/`esh`/`fesh`/`ksh`

* How to locate executables? => `${PATH}`
    ```
    $ which gcc
    /usr/bin/gcc

    $ gcc --version
    gcc (Ubuntu 11.4.0-1ubuntu1~22.04) 11.4.0
    Copyright (C) 2021 Free Software Foundation, Inc.
    This is free software; see the source for copying conditions.  There is NO
    warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
    ```

* [Built-in Utilities](https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.utilities/topic/about.html)
  * `ls`, `find`
  * `mkdir`, `mv`
  * `env`
  * `pidin`, `ps`
  * `which`

* [What happens when you log in?](https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.user_guide/topic/environment_LoggingIn.html)
  * [/etc/profile](https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.user_guide/topic/environment_etc_profile.html)
  * [${HOME}/.profile](https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.user_guide/topic/environment_profile.html)

* [Startup Programs](https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.building/topic/startup/startup_about.html)

## POSIX API
* Application Programming Interfaces
  * Process creatation & control
    ```
    int execl(/*...*/);         // #include <unistd.h>
    pid_t fork(void);           // #include <unistd.h>
    int posix_spawn(/*...*/);   // #include <spawn.h>
    // ...
    ```

  * File and Directory Operations
    ```
    int open(const char *pathname, int options, ... /*, mode_t mode */);    // #include <fcntl.h>
    int close(int filedes); // #include <unistd.h>
    // ...
    ```

  * Others

* [IEEE Std 1003.1™-2017 Headers](https://pubs.opengroup.org/onlinepubs/9699919799.2018edition/idx/head.html)

## Sample Application
* Hello world!!!
  * Command line arguments
  * Environment variables

## Practice
  * Process command line arguments
  * Access environment variables
  * Check process info
