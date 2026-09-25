#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_INPUT 1024
#define MAX_ARGS 64

void launch_new_terminal()
{
    char cwd[PATH_MAX];

    const char *distro = getenv("WSL_DISTRO_NAME");

    if (distro == NULL)
    {
        return;
    }

    if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        perror("getcwd");
        exit(1);
    }

    char command[4096];

    snprintf(
        command,
        sizeof(command),
        "cmd.exe /c \"wt.exe\" -w -1 "
        "wsl.exe -d %s --cd %s "
        "--exec ./my_shell --child",
        distro,
        cwd
    );

    int result = system(command);

    if (result != 0)
    {
        printf("Unable to open new terminal window.\n");
    }
}
void print_prompt()
{
    printf("\033[1;32mmy_shell\033[0m$ ");
    fflush(stdout);
}

int main(int argc, char *argv[])

{
    /*
     * When started normally from Ubuntu,
     * open the custom shell in a NEW terminal.
     *
     * --child means we are already inside
     * that new terminal, so don't open another one.
     */
    if (getenv("WSL_DISTRO_NAME") != NULL)
    {
        if (!(argc > 1 && strcmp(argv[1], "--child") == 0))
        {
            launch_new_terminal();
            return 0;
        }
    }
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
        print_prompt();

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
