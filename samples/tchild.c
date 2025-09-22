#include "common.h"

#include <errno.h>
#include <signal.h>   // For signal handling functions (signal, sigaction)
#include <time.h>
#include <unistd.h>   // For sleep

// Global flag to control the child thread's execution
static volatile sig_atomic_t alive_flag = 0;

// Signal handler function for SIGUSR1
void sigusr1_handler(int signum) {
    alive_flag = 0;
}

// Thread function for the child thread
void* child_thread_func(void* arg) {
    alive_flag = 1;

    if (SIG_ERR == signal(SIGUSR1, sigusr1_handler)) {
        loge("Cannot register SIGUSR1 handler: %d!", errno);
        alive_flag = 0;
        return NULL;
    }

    // Warning: not thread-safe!!!
    while (alive_flag) {
        time_t current_time;
        struct tm* time_info;
        char time_string[9];

        // Get current time
        time(&current_time);
        time_info = localtime(&current_time);

        // Format time as hh:mm:ss
        strftime(time_string, sizeof(time_string), "%H:%M:%S", time_info);

        // Print the timestamp
        logi("Timestamp: %s\n", time_string);

        // Sleep for 10 seconds
        sleep(10);
    }
    logi("Child thread terminating.\n");

    alive_flag = 0;
    return NULL;
}
