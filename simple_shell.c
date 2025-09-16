#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void)
{
	char *line;
	size_t len = 0;
	ssize_t read;
	pid_t pid;
	int status;

	while (1)
	{
		if (isatty(STDIN_FILENO))
		printf("$simple_shell$ ");

		read = getline(&line, &len, stdin);

		if (read == -1)
		{
			printf("./shell");
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
			return (1);
		}

		if (pid == 0)
		{
			char *argv[2];

			argv[0] = line;
			argv[1] = NULL;

			execve(line, argv, NULL);

			if (execve(line, argv, NULL) == -1)
				perror("./shell");

		}

		else

			waitpid(pid, &status, 0);
	}

	free(line);
	return (0);
}




