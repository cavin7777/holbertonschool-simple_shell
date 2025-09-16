#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

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

		if (line[read - 1] == '\n')
			line[read - 1] = '\0';

		if (*line == '\0')
			continue;

		pid = fork();
		if (pid == -1)
		{
			perror("./shell");
			free(line);
			return (1);
		}

		if (pid == 0)
		{
			char *argv[2] = {line, NULL};

			if (execvp(line, argv) == -1)
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

	free(line);
	return (0);
}
