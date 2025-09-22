# QNX - Session 1
* [Development Environment](./illustrations/env-development.md)
* [Training Environment](./illustrations/env-training.md)

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
    * `/proc/self/`

* [Dynamic-linked/Shared Libraries](https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.sys_arch/topic/dll.html)

  * Example
    ```
    #include <stdio.h>

    int main() {
      printf("Hello!");
      // ...
    }
    ```

    * Where is `printf` implemented? [Linux] `libc`
    * Is its implementation embedded in the application? No

  * How to link an executable with a shared library?
    ```
    gcc –o <executable file path> <object files> –L<look-up path> –l<library name>
    ```

  * How shared libraries are located at runtime? `${LD_LIBRARY_PATH}` ([QNX Security Note](https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.sys_arch/topic/dll_Runtime_linker.html))

  * How to identify dependencies? [ldd](https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.utilities/topic/l/ldd.html)
    ```
    $ ldd /usr/bin/gcc
    linux-vdso.so.1 (0x00007fff2b0b7000)
    libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x000074896c200000)
    /lib64/ld-linux-x86-64.so.2 (0x000074896c59f000)
    ```

  * [Executables & Shared Libraries](./illustrations/fs-shared-libs.md)

  * References
    * [How does it work on QNX?](https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.sys_arch/topic/dll.html)
    * [C Library Reference](https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.lib_ref/topic/full_safety_a.html)


* How are they installed?
  * BSP
  * App. Store & Package Management => N/A
  * Manual compilation and installation

* [File permissions](https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.user_guide/topic/files_PERMISSIONS.html)
  ```
  $ ll /usr/lib
    total 23836
    drwxr-xr-x 75 root root     4096 Feb 28  2025 ./
    drwxr-xr-x 14 root root     4096 Oct 26  2022 ../
    drwxr-xr-x  2 root root     4096 Feb 28  2025 apparmor/
    drwxr-xr-x  5 root root     4096 Feb 28  2025 apt/
    ...
    drwxr-xr-x  2 root root     4096 Feb 28  2025 cloud-init/
    -rwxr-xr-x  1 root root     1075 Dec  8  2021 cnf-update-db*
    ...

  $ ll /usr/bin/gcc
    lrwxrwxrwx 1 root root 6 Aug  5  2021 /usr/bin/gcc -> gcc-11*

  $ ll /usr/bin/gcc-11
    lrwxrwxrwx 1 root root 23 May 13  2023 /usr/bin/gcc-11 -> x86_64-linux-gnu-gcc-11*
  
  $ ll /usr/bin/x86_64-linux-gnu-gcc-11
    -rwxr-xr-x 1 root root 928584 May 13  2023 /usr/bin/x86_64-linux-gnu-gcc-11*
  ```

  * Is it a regular file or a directory or a symbolic link?
  * What could `Owner`/`Owner Group`/`Others` do with the file?
  * Users & Groups
    * `/etc/group`: `group_name:password:GID:user_list`
    * `/etc/passwd`: `name:password:UID:GID:GECOS:directory:shell`

  * Note: directories, regular files or symbolic links are all considered as `Files`
    ```
    In a QNX OS system, almost everything is a file; devices, data, and even services are all typically represented as files.
    ```

* Open Discussions: is it possible? and how to implement those concept in AUTOSAR System?
  * Filesystem
  * Install new SWCs at runtime
  * Shared libraries

## Shell/Command Interpreter
* [What is Shell/Command Line Interpreter?](./illustrations/shell.md)
  * [QNX Shells](https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.user_guide/topic/cmdline_Shell.html): `ksh` (default)/`uesh`/`esh`/`fesh`
  * [The first line of a script can identify the interpreter to use](https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.user_guide/topic/scripts_First_line.html): 
    ```
    #! interpreter [arg]
    <contents>
    ```

    * Example: `#! /bin/sh`

* Startup behaviors
  * [When ksh starts as a login shell, it executes these profiles, if they exist and are executable](https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.user_guide/topic/environment_LoggingIn.html)
    1. [/etc/profile](https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.user_guide/topic/environment_etc_profile.html), which then shall invoke all scripts in `/etc/profile.d/`
    2. [${HOME}/.profile](https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.user_guide/topic/environment_profile.html)
  * For non-login shell, if the script pointed by ${ENV} exists and is executable, it shall be executed
  * At BSP level, refer to [Startup Programs](https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.building/topic/startup/startup_about.html)

* Redirect I/O
  * Special File Descriptors: stdin (0), stdout(1), and stderr(2)
    ```
    $ ll /proc/self/fd
      total 0
      dr-x------ 2 k318 k318  4 Sep  3 22:06 ./
      dr-xr-xr-x 9 k318 k318  0 Sep  3 22:06 ../
      lrwx------ 1 k318 k318 64 Sep  3 22:06 0 -> /dev/pts/0
      lrwx------ 1 k318 k318 64 Sep  3 22:06 1 -> /dev/pts/0
      lrwx------ 1 k318 k318 64 Sep  3 22:06 2 -> /dev/pts/0
    ```

  * Redirect output to a file: `ll /proc/self/fd > ./run.log`

  * Redirect input
    ```
    $ cat ./run.log
      total 0
      dr-x------ 2 k318 k318  4 Sep  3 22:09 ./
      dr-xr-xr-x 9 k318 k318  0 Sep  3 22:09 ../
      lrwx------ 1 k318 k318 64 Sep  3 22:09 0 -> /dev/pts/0
      l-wx------ 1 k318 k318 64 Sep  3 22:09 1 -> /home/k318/wspace/run.log
      lrwx------ 1 k318 k318 64 Sep  3 22:09 2 -> /dev/pts/0
      lr-x------ 1 k318 k318 64 Sep  3 22:09 3 -> /proc/93/fd/

    $ grep lr < ./run.log
      lrwx------ 1 k318 k318 64 Sep  3 22:09 0 -> /dev/pts/0
      lrwx------ 1 k318 k318 64 Sep  3 22:09 2 -> /dev/pts/0
      lr-x------ 1 k318 k318 64 Sep  3 22:09 3 -> /proc/93/fd/
    ```

  * Pipe: `ll /proc/self/fd | grep lr`

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
  * `.`: to read and execute the contents of a file within the current shell session without creating a new process
    * Note: Environment Variables exist within the scope of a process (its child processes shall inherit them)
      => After `. <shell script>`, any changes in Environment Variables shall be persisted in the current shell.

## POSIX API
* [Executables & Shared Libraries](./illustrations/fs-shared-libs.md)

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
```
$ cd samples/
$ make sys_info
gcc -c sys_info.c
gcc -o sys_info.bin sys_info.o

$ ./sys_info.bin
Operating System Information:
PRETTY_NAME="Ubuntu 22.04.5 LTS"
NAME="Ubuntu"
...
Disclaimer: This application was generated by GenAI.

$ make clean
rm -f *.o *.map *.bin

```

## Practice
**Process Information Inspector**

Create an application named `process_info` that inspects a running process based on its process identifier (PID).

**Usage**:  
`process_info <pid>`  
* `<pid>`: The identifier of a running process (must be an integer).

**Requirements**:  
1. **Argument Validation**:  
  * Verify that exactly one command-line argument is provided.  
  * Ensure the argument is a valid integer representing a process ID.

2. **Process Validation**:  
  * Confirm that a process with the specified PID exists.

3. **Information Display**:  
  * If the PID is valid, retrieve data from `/proc/<pid>` and display:  
    * Executable name of the process.  
    * Size of the data segment.  
    * Size of the stack.
    * Threads: a list of threads belong to the process
      * Format for each thread: `<Thread ID> | <Scheduling Policy> | <Priority> | <CPU Time>`

**Notes**:  
* The application should handle errors gracefully (e.g., invalid argument count, non-integer PID, or non-existent process) and provide appropriate error messages.  
* Use the `/proc/<pid>` filesystem to gather the required process information.
