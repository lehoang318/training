#include "common.h"

#include <errno.h>
#include <pthread.h>
#include <signal.h>   // For signal handling functions (signal, sigaction)
#include <time.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Thread function for the child thread
void* child_thread_func(void* arg) {
    struct timespec ts;
    int ret = 1;

    while (0 != ret) {
        time_t current_time;
        struct tm time_info_buffer;
        struct tm* time_info;
        char time_string[] = "HH:MM:SS";

        // Get current time
        time(&current_time);
        time_info = localtime_r(&current_time, &time_info_buffer);

        // Format time as hh:mm:ss
        if (time_info == NULL) {
            loge("Failed to get local time.");
        } else {
            strftime(time_string, sizeof(time_string), "%H:%M:%S", time_info);
            // Print the timestamp
            logi("Timestamp: %s", time_string);
        }

        /* Get the current time */
        if (clock_gettime(CLOCK_REALTIME, &ts) == -1) {
            loge("Failed to get time: %d!", errno);
            return NULL;
        }

        /* Add 10 seconds to the current time to set the timeout */
        ts.tv_sec += 10;
        ret = pthread_mutex_timedlock(&mutex, &ts);
    }

    ret = pthread_mutex_unlock(&mutex);
    if (0 != ret) {
        loge("Failed to release mutex: %d!", ret);
        return NULL;
    }

    ret = pthread_mutex_destroy(&mutex);
    if (0 != ret) {
        loge("Failed to destroy mutex: %d!", ret);
    }

    logi("Terminating ...\n");

    return NULL;
}
