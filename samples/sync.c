#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

static int flag_sync = 0;

static pthread_mutex_t print_mutex = PTHREAD_MUTEX_INITIALIZER;

static void print_raw(const char* msg, useconds_t gap) {
    for (int i = 0; 3 > i; i++) {
        printf("%s | ", msg);
        usleep(gap);
    }

    printf("\n");
}

static void print(const char* msg, useconds_t gap) {
    if (flag_sync) {
        pthread_mutex_lock(&print_mutex);
    }

    print_raw(msg, gap);

    if (flag_sync) {
        pthread_mutex_unlock(&print_mutex);
    }
}

// Child thread function
void *child_thread_function(void *arg) {
    for (int i = 0; i < 6; ++i) {
        print("**************************", 8000);
        sleep(1);
    }
    return NULL;
}

// Main thread function (implicitly the main function)
int main(int argc, char * argv[]) {
    pthread_t child_thread;

    flag_sync = (1 < argc) && ('1' == argv[1][0]);

    if (flag_sync) {
        printf("SYNC ON!!!\n\n");
    } else {
        printf("SYNC OFF!!!\n\n");
    }

    // Create the child thread
    pthread_create(&child_thread, NULL, child_thread_function, NULL);

    // Main thread prints 'a' to 'z' 10 times
    for (int i = 0; i < 5; ++i) {
        print("ABCDEFGHIJKLMNOPQRSTUVWXYZ", 10000);
        sleep(1);
    }

    // Wait for the child thread to complete
    pthread_join(child_thread, NULL);

    // Main thread prints "All done" after the child thread finishes
    printf("All done\n");

    return 0;
}
