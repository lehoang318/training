#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#include <semaphore.h>

static int flag_sync = 0;

static sem_t sem_timing;
static pthread_mutex_t m_stdout = PTHREAD_MUTEX_INITIALIZER;

static void print_raw(const char* msg, useconds_t gap) {
    printf("[Thread #%d] ", gettid());
    for (int i = 0; 3 > i; i++) {
        printf("%s | ", msg);
        usleep(gap);
    }

    printf("\n");
}

static void print(const char* msg, useconds_t gap) {
    if (flag_sync) {
        pthread_mutex_lock(&m_stdout);
    }

    print_raw(msg, gap);

    if (flag_sync) {
        pthread_mutex_unlock(&m_stdout);
    }
}

// Child thread function
void *child_thread_function(void *arg) {
    sem_post(&sem_timing);

    for (int i = 0; i < 6; ++i) {
        print("**************************", 10000);
        usleep(1000);
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

    if (sem_init(&sem_timing, 0, 0) != 0) {
        perror("Semaphore initialization failed");
        return 1;
    }

    // Create the child thread
    pthread_create(&child_thread, NULL, child_thread_function, NULL);

    sem_wait(&sem_timing);
    // Main thread prints 'a' to 'z' 10 times
    for (int i = 0; i < 5; ++i) {
        print("ABCDEFGHIJKLMNOPQRSTUVWXYZ", 10000);
        usleep(1000);
    }

    // Wait for the child thread to complete
    pthread_join(child_thread, NULL);

    sem_destroy(&sem_timing);

    // Main thread prints "All done" after the child thread finishes
    printf("All done\n");

    return 0;
}
