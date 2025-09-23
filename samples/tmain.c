#include "common.h"

#include <errno.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

extern pthread_mutex_t mutex;
extern void* child_thread_func(void* arg);

int main() {
    pthread_t child_thread;
    char input[256];
    int ret;

    ret = pthread_mutex_lock(&mutex);
    if (0 != ret) {
        loge("Failed to lock mutex: %d!", ret);
        return 1;
    }

    // Create the child thread
    if (0 != pthread_create(&child_thread, NULL, child_thread_func, NULL)) {
        loge("Failed to create thread: %d!", errno);
        return 1;
    }

    logi("Main thread is running. Enter commands:\n");
    logi("u <text>: convert to uppercase\n");
    logi("h <text>: convert to hex\n");
    logi("x: exit and terminate child thread\n");

    // Main thread loop for user input
    while (1) {
        logi("> ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break; // Handle EOF or error
        }

        // Remove the newline character
        input[strcspn(input, "\n")] = 0;

        // Process the command
        if (input[0] == 'u' && input[1] == ' ') {
            to_uppercase(&input[2]);
            logi("Results: `%s`\n", &input[2]);
        } else if (input[0] == 'h' && input[1] == ' ') {
            char * const buffer = to_hex_string(&input[2]);
            if (NULL != buffer) {
                logi("Results: `%s`\n", buffer);
                free(buffer);
            }
        } else if (input[0] == 'x') {
            logi("Informing the child thread ...\n");
            ret = pthread_mutex_unlock(&mutex);
            if (0 != ret) {
                loge("Failed to release mutex: %d!", ret);
                return 1;
            }

            // Wait for the child thread to terminate
            if (pthread_join(child_thread, NULL) != 0) {
                loge("Failed to join thread: %d!", errno);
                return 1;
            }
            logi("Child thread has terminated. Main thread exiting.\n");
            break;
        } else {
            loge("Invalid command.\n");
        }
    }

    return 0;
}
