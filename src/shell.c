#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_INPUT 1024
#define MAX_ARGS 64

int main()
{
setvbuf(stdin, NULL, _IONBF, 0);
    char input[MAX_INPUT];
    char *args[MAX_ARGS];

    printf("\n");
    printf("========================================\n");
    printf("          OSSP CUSTOM SHELL\n");
    printf("========================================\n");
    printf("Type 'help' to see available commands.\n\n");

    while (1)
    {
        printf("my_shell> ");
        fflush(stdout);

        /* Read command from user */
        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            break;
        }

        /* Split command into words */
        int i = 0;

        args[i] = strtok(input, " \t\n");

        while (args[i] != NULL && i < MAX_ARGS - 1)
        {
            i++;
            args[i] = strtok(NULL, " \t\n");
        }

        /* User pressed Enter without typing anything */
        if (args[0] == NULL)
        {
            continue;
        }

        /* Exit shell */
        if (strcmp(args[0], "exit") == 0)
        {
            printf("Exiting custom shell...\n");
            break;
        }

        /* Help command */
        if (strcmp(args[0], "help") == 0)
        {
            printf("\nAvailable examples:\n");
            printf("  ls\n");
            printf("  pwd\n");
            printf("  date\n");
            printf("  whoami\n");
            printf("  echo Hello\n");
            printf("  clear\n");
            printf("  cd <folder>\n");
            printf("  project\n");
            printf("  exit\n\n");

            continue;
        }

        /* cd must be handled by the shell itself */
        if (strcmp(args[0], "cd") == 0)
        {
            if (args[1] == NULL)
            {
                chdir(getenv("HOME"));
            }
            else
            {
                if (chdir(args[1]) != 0)
                {
                    perror("cd");
                }
            }

            continue;
        }

        /*
         * Special command:
         * Launch our multithreaded mutex application.
         */
        if (strcmp(args[0], "project") == 0)
        {
            pid_t pid = fork();

            if (pid < 0)
            {
                perror("fork");
            }
            else if (pid == 0)
            {
                execl("./project", "project", NULL);

                perror("Unable to run project");
                exit(1);
            }
            else
            {
                waitpid(pid, NULL, 0);
            }

            continue;
        }

        /*
         * All normal Linux commands:
         * ls, pwd, date, echo, whoami, etc.
         */
        pid_t pid = fork();

        if (pid < 0)
        {
            perror("fork");
        }

        else if (pid == 0)
        {
            execvp(args[0], args);

            perror("Command not found");
            exit(1);
        }

        else
        {
            waitpid(pid, NULL, 0);
        }
    }

    return 0;
}
