# QNX - Session 3
## Threading & Synchronization
* Creation: `pthread_create()`
  * [How do we customize the thread's attributes?](https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.lib_ref/topic/p/pthread_attr_init.html)

    * Scheduling policy attribute
    * scheduling parameters attribute
    * Detach state attribute
    * Stack attributes
    * Note: QNX only supports `PTHREAD_SCOPE_SYSTEM`

  * Joinable vs Detachable
  * Thread name
  * Is it possible to change a thread 's attributes after creation?
    * Scheduling Policy and Priority
    * Detached State
    * Name

* Termination
  * How do we detect a thread 's termination?
* Priority & Scheduling Policy
  * `SCHED_SPORADIC`
  * [Optional] [Real-time Linux](https://blogs.oracle.com/linux/post/task-priority)

    <details>
      <summary>Click to expand!</summary>

      * Real-time policies: 3 policies are supported: SCHED_RR, SCHED_FIFO and SCHED_DEADLINE.
        * Tasks with SCHED_DEADLINE scheduling policy will be called deadline tasks

      * Non real-time policies: SCHED_OTHER, SCHED_BATCH and SCHED_IDLE
        * Tasks with SCHED_OTHER scheduling policy will be called normal tasks

      * Priority order: `SCHED_DEADLINE > SCHED_RR/SCHED_FIFO > SCHED_OTHER > SCHED_BATCH > SCHED_IDLE`

      * [Task priority range](https://blogs.oracle.com/content/published/api/v1.1/assets/CONTFB03B61F7C5643DDBDB591B316ECF3C0/Medium?cb=_cache_82a7&format=jpg&channelToken=3189ef66cf584820b5b19e6b10792d6f)

        > All deadline tasks get a priority of -1.

      * [List all threads with priorities](https://gitlab.com/procps-ng/procps/-/issues/111#note_105333538)

      ```
      $ ps -em -o pid,tid,class,pri_baz,rtprio,ni,comm
        PID     TID CLS BAZ RTPRIO  NI COMMAND
        158       - -     -      -   - systemd-resolve
        -       158 TS  120      -   0 -

        159       - -     -      -   - systemd-timesyn
        -       159 TS  120      -   0 -
        -       163 TS  120      -   0 -
      ```

    </details>

    

## Socket

