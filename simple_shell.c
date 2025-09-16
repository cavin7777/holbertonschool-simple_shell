#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>

extern char **environ;

int main(void)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t read;
	pid_t pid;
	int status;

	while (1)
	{

		printf("$simple_shell$ ");
		fflush(stdout);

		read = getline(&line, &len, stdin);
		if (read == -1)
		{
			printf("\n");
			break;
		}

		if (line[read - 1] == '\n')
			line[read - 1] = '\0';

		if (line[0] == '\0')
			continue;

		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			continue;
		}

		if (pid == 0)
		{
			char *argv[2];

			argv[0] = line;
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

	free(line);
	return (0);
}
