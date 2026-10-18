#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <signal.h>

#define MAX_COMMANDS 32
#define MAX_ARGS 64
#define MAX_LINE 4096

// Remove extra spaces
char *trim(char *s) {
    while (isspace((unsigned char)*s))
        s++;

    char *end = s + strlen(s);

    while (end > s && isspace((unsigned char)end[-1]))
        end--;

    *end = '\0';
    return s;
}

// Display help menu
void show_help(void) {
    printf("\n========== HELP MENU ==========\n");

    printf("\n1. ONE PROCESS\n");
    printf("   echo hello\n");
    printf("   Output: hello\n");

    printf("\n2. TWO PROCESSES\n");
    printf("   echo hello world | wc -w\n");
    printf("   Output: 2\n");

    printf("\n3. THREE PROCESSES\n");
    printf("   echo hello world | tr a-z A-Z | rev\n");
    printf("   Output: DLROW OLLEH\n");

    printf("\n4. FOUR PROCESSES\n");
    printf("   seq 1 12 | grep 1 | sort -nr | head -n 2\n");
    printf("   Output: 12 and 11\n");

    printf("\nhelp - Show help menu\n");
    printf("exit - Exit program\n");

    printf("\nEach command runs as a child process.\n");
    printf("Pipes connect the output of one command\n");
    printf("to the input of the next command.\n");
    printf("Simple arguments only; no quotes or redirection.\n");
    printf("================================\n\n");
}

// Split pipeline into commands
int parse(char *line,
          char *args[MAX_COMMANDS][MAX_ARGS],
          int *count) {

    int n = 0;
    char *start = line;

    for (char *p = line;; p++) {

        if (*p != '|' && *p != '\0')
            continue;

        int last = (*p == '\0');
        *p = '\0';

        char *part = trim(start);

        if (*part == '\0' || n >= MAX_COMMANDS)
            return -1;

        int j = 0;
        char *save = NULL;

        char *word = strtok_r(part, " \t", &save);

        while (word != NULL) {
            if (j >= MAX_ARGS - 1)
                return -1;

            args[n][j++] = word;
            word = strtok_r(NULL, " \t", &save);
        }

        args[n][j] = NULL;
        n++;

        if (last)
            break;

        start = p + 1;
    }

    *count = n;
    return 0;
}

// Create and execute child processes
int run_pipeline(char *args[MAX_COMMANDS][MAX_ARGS],
                 int n) {

    pid_t children[MAX_COMMANDS];

    int previous_read = -1;
    int started = 0;
    int failed = 0;

    printf("\n--- Creating %d Process(es) ---\n", n);
    fflush(stdout);

    for (int i = 0; i < n; i++) {

        int fd[2] = {-1, -1};

        // Create pipe except for last command
        if (i < n - 1) {
            if (pipe(fd) == -1) {
                perror("pipe");
                failed = 1;
                break;
            }
        }

        pid_t pid = fork();

        if (pid < 0) {
            perror("fork");

            if (fd[0] != -1) close(fd[0]);
            if (fd[1] != -1) close(fd[1]);

            failed = 1;
            break;
        }

        if (pid == 0) {

            // Redirect input from previous pipe
            if (previous_read != -1) {
                if (dup2(previous_read, STDIN_FILENO) == -1) {
                    perror("dup2 input");
                    _exit(126);
                }
            }

            // Redirect output to next pipe
            if (fd[1] != -1) {
                if (dup2(fd[1], STDOUT_FILENO) == -1) {
                    perror("dup2 output");
                    _exit(126);
                }
            }

            // Close unnecessary descriptors
            if (previous_read != -1)
                close(previous_read);

            if (fd[0] != -1)
                close(fd[0]);

            if (fd[1] != -1)
                close(fd[1]);

            // Execute command
            execvp(args[i][0], args[i]);

            fprintf(stderr,
                    "Cannot execute %s: %s\n",
                    args[i][0], strerror(errno));

            _exit(127);
        }

        children[started++] = pid;

        printf("[Parent] Process %d: %s (PID %ld)\n",
               i + 1, args[i][0], (long)pid);
        fflush(stdout);

        // Parent closes unused ends
        if (previous_read != -1)
            close(previous_read);

        if (fd[1] != -1)
            close(fd[1]);

        previous_read = fd[0];
    }

    if (previous_read != -1)
        close(previous_read);

    // Wait for all created children
    for (int i = 0; i < started; i++) {

        int status;
        pid_t result;

        do {
            result = waitpid(children[i], &status, 0);
        } while (result == -1 && errno == EINTR);

        if (result == -1) {
            perror("waitpid");
            failed = 1;
        }
        else if (WIFEXITED(status) &&
                 WEXITSTATUS(status) == 0) {

            printf("[Parent] Process %d completed\n", i + 1);
        }
        else if (WIFSIGNALED(status) &&
                 WTERMSIG(status) == SIGPIPE) {

            printf("[Parent] Process %d stopped (pipe closed)\n",
                   i + 1);
        }
        else {
            printf("[Parent] Process %d failed\n", i + 1);
            failed = 1;
        }
    }

    return failed;
}

int main(void) {

    char line[MAX_LINE];
    char *args[MAX_COMMANDS][MAX_ARGS];

    printf("======================================\n");
    printf(" MULTI-PROCESS COMMAND PIPELINE SYSTEM\n");
    printf("======================================\n");

    show_help();

    while (1) {

        printf("pipeline> ");
        fflush(stdout);

        if (fgets(line, sizeof(line), stdin) == NULL)
            break;

        // Reject overly long input
        if (strchr(line, '\n') == NULL &&
            !feof(stdin)) {

            int c;

            while ((c = getchar()) != '\n' && c != EOF)
                ;

            printf("Input too long!\n");
            continue;
        }

        line[strcspn(line, "\n")] = '\0';

        char *input = trim(line);

        if (*input == '\0')
            continue;

        if (strcmp(input, "exit") == 0) {
            printf("Exiting pipeline system...\n");
            break;
        }

        if (strcmp(input, "help") == 0) {
            show_help();
            continue;
        }

        int n = 0;

        if (parse(input, args, &n) == -1) {
            printf("Invalid pipeline command!\n");
            continue;
        }

        int result = run_pipeline(args, n);

        if (result == 0)
            printf("[Pipeline completed successfully]\n\n");
        else
            printf("[Pipeline completed with errors]\n\n");
    }

    return 0;
}
