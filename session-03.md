# QNX - Session 3
## Threading & Synchronization
* Sample application
  | Functionality A | Functionality B |
  |---|---|
  | Perform actions based on User commands via `stdin` | Periodically print out current timestamp |

  * Possible solutions
    1. Two processes
    2. One process, one thread
    3. **One process, two threads**

  * With solution #3
    * How do we handle the terminations of the two threads?

* [How do we create a thread?](https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.lib_ref/topic/p/pthread_create.html)
  ```
  #include <pthread.h>

  int pthread_create(pthread_t* thread, const pthread_attr_t* attr, void* (*start_routine)(void* ), void* arg);
  pthread_t pthread_self(void);
  ```

  * [pthread_self()](https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.lib_ref/topic/p/pthread_self.html) vs [gettid()](https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.lib_ref/topic/g/gettid.html)

  * [Which properties could be customized?](https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.getting_started/topic/s1_procs_thread_attr.html)
    * Priority ([PROCMGR_AID_PRIORITY](https://www.qnx.com/developers/docs/8.0/com.qnx.doc.security.system/topic/manual/abilities.html))
    * Scheduling Policy
    * Detach state attribute (`PTHREAD_CREATE_JOINABLE` vs `PTHREAD_CREATE_DETACHED`)
    * Stack attributes
      * [How do we configure main thread 's stack size?](https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.utilities/topic/q/qcc.html)

    * [QNX] [Name](https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.lib_ref/topic/p/pthread_setname_np.html)
    * Notes
      * QNX only supports `PTHREAD_SCOPE_SYSTEM`
      * `pthread_attr_init()` & `pthread_attr_destroy()`
      * Is it possible to change a thread 's attributes after creation?
        * Scheduling Policy and Priority
        * Detached State
        * Name

* [Thread Lifecycle](https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.sys_arch/topic/kernel_Life_Cycle.html)
  * Thread States
  * Exit status
    ```
    #include <pthread.h>
    void pthread_exit(void* value_ptr);
    int pthread_join(pthread_t thread, void** value_ptr);
    ```

    * `value_ptr`, passed to `pthread_exit()`, should point to a global data variable (or dynamic allocated)

  * `PTHREAD_CREATE_JOINABLE` vs `PTHREAD_CREATE_DETACHED`

* How does Thread Scheduler select the next thread to run?
  * [Scheduling Priority](https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.sys_arch/topic/kernel_Scheduling_priority.html)
  * [Scheduling Policies](https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.sys_arch/topic/kernel_SchedulingAlgorithms.html)
    * [**SCHED_SPORADIC**](https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.sys_arch/topic/kernel_Sporadic_scheduling.html) & [Relevant attributes](https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.lib_ref/topic/s/sched_param.html)

  * [When will Rescheduling happen?](https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.prog/topic/overview_Suspending_threads.html)
    * Waiting for events/resources: `pthread_mutex_lock()`, `pthread_cond_wait()`, `pthread_join()`, ...
    * [sleep()](https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.lib_ref/topic/s/sleep.html)
    * [sched_yield()](https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.lib_ref/topic/s/sched_yield.html)

  * [Optional] Real-time Linux

    <details>
      <summary>Click to expand!</summary>

      * [PREEMPT_RT Patch](https://wiki.linuxfoundation.org/realtime/preempt_rt_versions)
      * [Priority & Scheduling Policy](https://blogs.oracle.com/linux/post/task-priority)
        * Real-time policies: 3 policies are supported: `SCHED_RR`, `SCHED_FIFO` and **SCHED_DEADLINE**.
        * Non real-time policies: `SCHED_OTHER` (default), `SCHED_BATCH` and `SCHED_IDLE`
        * Priority order: `SCHED_DEADLINE > SCHED_RR/SCHED_FIFO > SCHED_OTHER > SCHED_BATCH > SCHED_IDLE`

      * [List all threads with priorities](https://gitlab.com/procps-ng/procps/-/issues/111#note_105333538)

        ```
        $ ps -m -p 159 -o pid,tid,class,pri_baz,rtprio,ni,state,comm
          PID     TID CLS BAZ RTPRIO  NI COMMAND
          159       - -     -      -   - systemd-timesyn
          -       159 TS  120      -   0 -
          -       163 TS  120      -   0 -
        ```

    </details>

* [Processes vs Threads](https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.getting_started/topic/s1_procs_Comparisons.html)
  * [Safety] Freedom from Interference (FFI)
  * Synchronization & Communication
  * Startup & Shutdown
  * Context Switching

## Synchronization
* [Mutexes](https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.sys_arch/topic/kernel_Mutexes.html)
  * [Attributes](https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.lib_ref/topic/p/pthread_mutexattr_init.html)
    ```
    #include <pthread.h>

    int pthread_mutexattr_init(pthread_mutexattr_t* attr);
    int pthread_mutexattr_destroy(pthread_mutexattr_t* attr);
    ```

  * Life Cycle
    ```
    #include <pthread.h>
    int pthread_mutex_init(pthread_mutex_t* mutex, const pthread_mutexattr_t* attr);

    int pthread_mutex_lock(pthread_mutex_t* mutex);
    int pthread_mutex_trylock(pthread_mutex_t* mutex);
    int pthread_mutex_timedlock(pthread_mutex_t * mutex, const struct timespec * abs_timeout);

    int pthread_mutex_unlock(pthread_mutex_t* mutex);

    int pthread_mutex_destroy( pthread_mutex_t* mutex);
    ```


  * [Priority-inversion](https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.lib_ref/topic/p/pthread_mutexattr_setprotocol.html)
    | Time | Thread A (eff. priority: N) | Thread B (eff. priority: M) | Remark |
    |:---:|---|---|---|
    | t0 | pthread_mutex_lock(&mutex_X) | Do something | mutex_X (PTHREAD_PRIO_INHERIT) is not locked <br> => A becomes mutex_X owner. |
    | t1 | Do something | pthread_mutex_lock(&mutex_X) | B shall be blocked. <br> If M > N, eff. priority of A shall be raised. |
    | t2 | pthread_mutex_unlock(&mutex_X) | <being blocked> | B shall be unblocked then becomes new owner of mutex_X. <br> Effective priority of A shall be adjusted. |

  * [PTHREAD_PROCESS_SHARED vs PTHREAD_PROCESS_PRIVATE](https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.lib_ref/topic/p/pthread_mutexattr_getpshared.html)

* [Semaphores](https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.sys_arch/topic/kernel_Semaphores.html)
  > A key advantage of semaphores over mutexes is that semaphores are defined to operate between processes.
  * [Unamed Semaphore](https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.lib_ref/topic/s/sem_init.html)
    ```
    #include <semaphore.h>

    int sem_init(sem_t * sem, int pshared, unsigned value); // `(0 != pshared)` => Process-shared Semaphore.

    int sem_wait(sem_t * sem);  // Decrease the semaphore value by one if success.
    int sem_trywait(sem_t * sem);
    int sem_timedwait(sem_t * sem, const struct timespec * abs_timeout);

    int sem_post(sem_t * sem);  // Increase the semaphore value by one if success

    int sem_destroy(sem_t * sem);
    ```

  * [Named Semaphores](https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.lib_ref/topic/s/sem_open.html)
    ```
    #include <semaphore.h>
    #include <fcntl.h>

    sem_t * sem_open(const char * sem_name, int oflags, ...);

    int sem_wait(sem_t * sem);  // Decrease the semaphore value by one if success.
    int sem_trywait(sem_t * sem);
    int sem_timedwait(sem_t * sem, const struct timespec * abs_timeout);

    int sem_post(sem_t * sem);  // Increase the semaphore value by one if success

    int sem_close(sem_t * sem);
    int sem_unlink(const char * sem_name);  // Destroy a named semaphore
    ```

* [Synchronization via atomic operations](https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.sys_arch/topic/kernel_Sync_atomic.html)

* How do we share Synchronization Objects between Processes?
  * Common workflow (not applicable for Named Semaphores)
    1. Initialize a shared memory object
    2. Map Synchronization Objects into the shared memory area
    3. Initialize Synchronization Objects as Process-shared
    4. ...

  * [Example which employs Shared Memory and Unnamed Semaphores](https://man7.org/linux/man-pages/man3/shm_open.3.html)
