#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>


int main(void)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t read;
	pid_t pid;
	int status;
	int interactive = isatty(STDIN_FILENO);

	while (1)
	{
		if (interactive)
		{
			printf("$simple_shell$ ");
			fflush(stdout);
		}

		read = getline(&line, &len, stdin);
		if (read == -1)
		{
			if (interactive)
				printf("\n");
			break;
		}

		char *cmd = strtok(line, " \n");

		while (cmd != NULL)
		{
			char *arg = strtok(cmd, " ");

			if (arg && *arg != '\0')
			{
				pid = fork();
				if (pid == -1)
				{
					perror("fork");
					break;
				}

				if (pid == 0)
				{
					char *argv[2];

					argv[0] = arg;
					argv[1] = NULL;

					if (execve(line, argv, environ) == -1)
					{
						perror("./shell");
						exit(EXIT_FAILURE);
					}
				}
				else
				{
				waitpid(pid, &status, 0);
				}
			}

			cmd = strtok(NULL, "\n");
		}
	}
	free(line);
	return (0);
}
