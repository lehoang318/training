# QNX - Session 2
* [Memory Management](https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.prog/topic/memory_MEMMGT_.html)
  * Memory Management Unit
  * Translation Look-aside Buffer (TLB)
  * Mapping Virtual-Physical
  * How to access peripherals' registers?

* Process

* Signals (~SW Interrupts)

* IPC
  * Message passing (introduction only)
  * Shared memory
    * Flows
      * Owner setup shared memory area: size & characteristics (access permissions)
      * Peers map the shared memory area to their vm space

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

  * Kernel calls

* Practice #2: IPC with Shared Memory