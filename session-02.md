# QNX - Session 2
* [Memory Management](https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.prog/topic/memory_MEMMGT_.html)
  * Memory layout for a typical process

    <img src="https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.sys_arch/images/procaddr.svg" width="50%" height="50%">

    * Heap & dynamic memory allocation
    * Stack (each thread has a separated stack)
    * Shared objects
    * Shared libraries

  * Cross processes memory access?
    ```
    $ cd samples
    $ make clean
    $ make memory
    $ setarch $(uname -m) -R ./memory
    ```

  * Virtual memory

    <img src="https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.sys_arch/images/virtual_memory.svg" width="75%" height="75%">

    * [Memory Management Unit](https://developer.arm.com/documentation/101811/0104/The-Memory-Management-Unit--MMU-)

    * [Mapping Virtual-Physical](https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.lib_ref/topic/m/mmap.html)
      
      => How to access peripherals' registers?

* Processes
  * [Definitions](https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.prog/topic/overview_Definitions.html)
    > A thread is a single flow of execution or control.

    > A process is a collection of resources shared by one or more threads

    > Each process lives in its own protected memory space. (QNX/An application as a set of processes)

  * [How do we create/spawn a process?](https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.getting_started/topic/s1_procs_Starting_a_process.html)
    * [P-Recommendation] ?

  * [When will processes be terminated?](https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.prog/topic/process_PROCTERM.html)

  * [How do we detect the termination of a process/ process group?](https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.prog/topic/process_PROCDETECTTERM.html)
    * Why do we care? => **[Safety] Fault detection & reaction**
      * Crashed
      * Hang/Freeze

    * How did the process terminate?
      * What did happen to the process' resources
      * **Exit Code/Termination Status**
        * [Programming] [Status Macros](https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.lib_ref/topic/w/wait.html#wait__StatusMacros)
        * [Shell Script] `$?`

    * `waitpid(pid, &status, WNOHANG)`

  * What will happend if a child process terminates before its parent does?
    * `posix_spawn()` & `POSIX_SPAWN_NOZOMBIE`

* [Inter-process Communication](https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.sys_arch/topic/ipc.html)
  * [**Signals**](https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.sys_arch/topic/ipc_Signals.html)
    * [By default, how does a process react when it receives a signal?](https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.sys_arch/topic/ipc_Signal_summary.html)

      > The default action for most signals is to terminate the process.


    * [How do we override the default reactions?](https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.lib_ref/topic/s/signal.html)
      ```
      #include <signal.h>

      typedef typeof(void (int))  *sighandler_t;
      sighandler_t signal(int signum, sighandler_t handler);                          // ANSI C99 & POSIX

      int sigaction(int sig, const struct sigaction * act, struct sigaction * oact);  // POSIX
      ```

    * How do we send a signal to a [process](https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.lib_ref/topic/k/kill.html)/[thread](https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.lib_ref/topic/p/pthread_kill.html)?

    * Is it possible to block signals? [sigprocmask()](https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.lib_ref/topic/s/sigprocmask.html)

    * How do we detect whether a process was terminated by a signal?
      * `WIFSIGNALED(stat_val)`: Evaluates to nonzero value if the child process terminated from reception of a signal that wasn't caught.
      * `WTERMSIG(stat_val)`: Evaluates to the number of the signal that terminated the child process if the value of `WIFSIGNALED(stat_val)` is nonzero.

    * Note: [The eight special signals cannot be ignored or caught](https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.sys_arch/topic/ipc_Special_signals.html).

  * [**Shared memory**](https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.sys_arch/topic/ipc_Shared_memory.html)
    > ... access to shared memory is in itself unsynchronized.

    > Shared memory is therefore most efficient when used for updating large amounts of data as a block.

    * Workflow
      * Host
        1. Create a shared memory region: [shm_open()](https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.lib_ref/topic/s/shm_open.html)
          > When a new shared memory object is created, the size of the object is set to zero.

        2. Set the memory object's size: [ftruncate()](https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.lib_ref/topic/f/ftruncate.html)
        3. Map the shared memory object into the virtual address space of the calling process: [mmap()](https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.sys_arch/topic/ipc_mmap.html)
        4. Read/Write the shared memory region
        5. Close the file descriptor allocated by `shm_open()` when it is no longer needed: [close()](https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.lib_ref/topic/c/close.html)
        6. Unlink the memory object to completely remove it: [shm_unlink](https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.lib_ref/topic/s/shm_unlink.html)

          * Note: Step #6 could be done by another process.

      * Guest
        1. Open the existing shared memory object using its path name
        2. Map the shared memory object into the virtual address space of the calling process
        3. Read/Write the shared memory region
        4. Close the file descriptor allocated by `shm_open()` when it is no longer needed

  * **Socket** => [session-03.md](./session-03.md)
  * *Pipes*
  * *FIFOs*
  * *POSIX message queues*
  * *Message passing*

* [Time](https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.prog/topic/timing_Time_functions.html)
  * `CLOCK_MONOTONIC`: a monotonic count of time since the kernel was initialized
  * `CLOCK_REALTIME`: the current calendar time/wall-clock (seconds since the Unix Epoch, 00:00:00 January 1, 1970)
  * `Local time = CLOCK_REALTIME + Offset (depends on time zone & dst)`
  * [Execution times for processes and threads](https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.prog/topic/timing_execution_times.html)
  * Notes
    * `CLOCK_REALTIME` could be adjusted at runtime! => should not be used for profiling purposes!
    * "The OS actually just counts time since booting" => `Wall-clock time = Monotonic time + Offset`
    * [Network Time Protocol](https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.utilities/topic/n/ntpd.html)
    * Timestamps & Security: preventing Replay Attacks, ensuring Data Integrity, ...

* [QNX OS Architecture](https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.sys_arch/topic/intro_MICROKERNELARCH.html)

  ![High-level](https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.sys_arch/images/arch_micro.svg)

  * Microkernel (`procnto` system process)
    * Process, Thread & Signals management
    * Scheduling & Synchronization services
    * Memory management
    * Message-passing services
    * Timer services

  * All OS services, except those provided by the mandatory microkernel/process manager module (procnto), are handled via standard processes
    * Filesystem managers
    * Character device managers
    * Native network manager
    * TCP/IP

    > System processes are essentially indistinguishable from any user-written program—they use the same public API and kernel services available to any (suitably privileged) user process.

## Sample Application
* One starter process spawn a list of child processes
  * Send signal to control execution flows of the child processes
  * Detect the termination of child processes
  * Hanle hanging child processes
