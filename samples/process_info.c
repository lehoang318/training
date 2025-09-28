#include <dirent.h>
#include <errno.h>
#include <limits.h>
#include <linux/sched.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

/**
 * Parses and validates a command-line argument as a positive integer PID.
 *
 * @param arg The string argument to parse.
 * @param pid_out A pointer to a long where the parsed PID will be stored.
 * @return true if the argument is a valid PID, false otherwise.
 */
static bool parse_pid_from_arg(const char *arg, long *pid_out) {
    char *endptr;
    long pid_long = strtol(arg, &endptr, 10);

    // Check for conversion errors and if the entire string was consumed.
    if (*endptr != '\0' || endptr == arg || pid_long <= 0) {
        fprintf(stderr, "Error: Invalid PID '%s'. PID must be a positive integer.\n", arg);
        return false;
    }

    *pid_out = pid_long;
    return true;
}

/**
 * Checks if a process with the given PID exists by inspecting the /proc filesystem.
 *
 * @param pid The process ID to check.
 * @return true if the process exists, false otherwise.
 */
static bool does_process_exist(long pid) {
    // Build the path to the process directory in /proc.
    char proc_path[256];
    snprintf(proc_path, sizeof(proc_path), "/proc/%ld", pid);

    // Use stat() to check if the directory exists.
    struct stat st;
    if (stat(proc_path, &st) == 0) {
        // stat() succeeded, now check if it's a directory.
        if (S_ISDIR(st.st_mode)) {
            return true;
        } else {
            // This case is rare, but it's good practice to handle it.
            fprintf(stderr, "Error: Found an entry for PID %ld in /proc, but it is not a directory.\n", pid);
            return false;
        }
    } else {
        // stat() failed, check errno to see why.
        if (errno == ENOENT) {
            return false;
        } else {
            // Handle other potential errors, such as lack of permission.
            perror("Error while checking for process");
            return false;
        }
    }
}

/**
 * Gets the scheduling policy name from a numerical value.
 *
 * @param policy_num The policy number from /proc/<pid>/stat.
 * @return A string representing the policy name.
 */
static const char *get_sched_policy_name(int policy_num) {
    switch (policy_num) {
        case SCHED_NORMAL:
            return "Normal";  // 0
        case SCHED_FIFO:
            return "FIFO";  // 1
        case SCHED_RR:
            return "RR";  // 2
        case SCHED_BATCH:
            return "Batch";  // 3
        case SCHED_IDLE:
            return "Idle";  // 5
        case SCHED_DEADLINE:
            return "Deadline";  // 6
        default:
            return "Unknown";
    }
}

static int is_realtime(int policy_num) {
    switch (policy_num) {
        case SCHED_NORMAL:  // 0
        case SCHED_BATCH:   // 3
        case SCHED_IDLE:    // 5
            return 0;

        case SCHED_DEADLINE:  // 6
        case SCHED_FIFO:      // 1
        case SCHED_RR:        // 2
            return 1;
        default:
            return -1;  // Unknown
    }
}

/**
 * Displays detailed information about a process by reading files in the /proc filesystem.
 * References
 * + https://man7.org/linux/man-pages/man5/proc_pid_stat.5.html
 * + [Oracle Linux Blog] Understanding process thread priorities in Linux
 *
 * @param pid The process ID to inspect.
 */
static void display_process_info(long pid) {
    char path_buffer[PATH_MAX];
    FILE *fp;

    // --- Executable Name ---
    snprintf(path_buffer, sizeof(path_buffer), "/proc/%ld/exe", pid);
    char exe_path[PATH_MAX];
    ssize_t len = readlink(path_buffer, exe_path, sizeof(exe_path) - 1);
    if (len != -1) {
        exe_path[len] = '\0';
        printf("Executable Name: %s\n", exe_path);
    } else {
        perror("Error getting executable name");
        printf("Executable Name: Not available\n");
    }

    // --- Data and Stack Size ---
    snprintf(path_buffer, sizeof(path_buffer), "/proc/%ld/maps", pid);
    fp = fopen(path_buffer, "r");
    if (fp) {
        char line[256];
        long data_size = 0;
        long stack_size = 0;
        while (fgets(line, sizeof(line), fp)) {
            unsigned long start_addr, end_addr;
            if (sscanf(line, "%lx-%lx", &start_addr, &end_addr) == 2) {
                if (strstr(line, "[stack]")) {
                    stack_size = end_addr - start_addr;
                } else if (strstr(line, "[anon:stack]")) {
                    stack_size = end_addr - start_addr;
                } else if (strstr(line, "rw-p") && strstr(line, "/")) {
                    // Simple heuristic for data segment
                    data_size += (end_addr - start_addr);
                }
            }
        }
        fclose(fp);
        printf("Size of Data Segment: %ld bytes\n", data_size);
        printf("Size of Stack: %ld bytes\n", stack_size);
    } else {
        perror("Error reading memory maps");
        printf("Memory information not available.\n");
    }

    // --- Threads Information ---
    snprintf(path_buffer, sizeof(path_buffer), "/proc/%ld/task", pid);
    DIR *dir = opendir(path_buffer);
    if (dir) {
        printf("Threads:\n");
        printf("| Thread ID | Scheduling Policy | Priority | CPU Time |\n");

        struct dirent *entry;
        while ((entry = readdir(dir)) != NULL) {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
                continue;
            }

            long tid = atol(entry->d_name);
            char thread_stat_path[PATH_MAX];
            snprintf(thread_stat_path, sizeof(thread_stat_path), "/proc/%ld/task/%ld/stat", pid, tid);

            FILE *thread_fp = fopen(thread_stat_path, "r");
            if (thread_fp) {
                char stat_line[4096];
                if (fgets(stat_line, sizeof(stat_line), thread_fp)) {
                    unsigned long utime, stime;
                    long priority;
                    unsigned int rt_priority, policy;
                    // Use sscanf to parse specific fields from the stat file
                    sscanf(stat_line,
                           "%*d %*s %*c %*d %*d %*d %*d %*d %*u %*u %*u %*u %*u %lu %lu %*d %*d %ld %*d %*d %*d %*u %*u %*d %*u %*u %*u %*u %*u %*u "
                           "%*u %*u %*u %*u %*u %*u %*u %*d %*d %u %u %*u %*u %*d %*u %*u %*u %*u %*u %*u %*u %*d",
                           &utime, &stime, &priority, &rt_priority, &policy);
                    fclose(thread_fp);

                    long cpu_time_ticks = utime + stime;
                    double cpu_time_sec = (double)cpu_time_ticks / sysconf(_SC_CLK_TCK);

                    if (is_realtime(policy)) {
                        /**
                         * For processes running a real-time scheduling policy: `priority = -rt_priority - 1`
                         * => Mapping from `rt_priority[1:99]` to `priority[-2:-100]`
                         */
                        printf("| %9ld | %17s | %4ld/%3u | %6.2f s |\n", tid, get_sched_policy_name(policy), priority, rt_priority, cpu_time_sec);
                    } else {
                        /**
                         * For processes running under a non-real-time scheduling policy,
                         * `priority` is the raw nice value as represented in the kernel
                         * `nice = priority - 20`
                         * `static_prio = priority + 100`
                         */
                        printf("| %9ld | %17s | %8ld | %6.2f s |\n", tid, get_sched_policy_name(policy), priority + 100, cpu_time_sec);
                    }
                } else {
                    fclose(thread_fp);
                }
            }
        }
        closedir(dir);
    } else {
        perror("Error reading process threads");
        printf("Threads information not available.\n");
    }
}

/*
 * This program validates a command-line argument as a process ID (PID) and
 * checks if a process with that PID exists on a Linux system by inspecting
 * the /proc filesystem.
 *
 * Usage: ./process_info <pid>
 */
int main(int argc, char *argv[]) {
    printf("\n**Disclaimer: This application was generated by GenAI.**\n\n");

    // Requirement 1: Argument Validation
    // Check if exactly one command-line argument is provided.
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <pid>\n", argv[0]);
        fprintf(stderr, "  <pid>: The identifier of a running process (must be a positive integer).\n");
        return EXIT_FAILURE;
    }

    // Call the new static function to parse and validate the PID.
    long pid;
    if (!parse_pid_from_arg(argv[1], &pid)) {
        return EXIT_FAILURE;
    }

    // Requirement 2: Process Validation
    // Call the other static function to check for process existence.
    if (does_process_exist(pid)) {
        printf("Success: A process with PID %ld exists.\n", pid);
        display_process_info(pid);
        return EXIT_SUCCESS;
    } else {
        printf("Error: No process with PID %ld was found.\n", pid);
        return EXIT_FAILURE;
    }
}
