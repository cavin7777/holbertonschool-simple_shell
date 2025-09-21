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
	char *token;
	char *argv[64];
	int argc;

	while (1)
	{
		if (interactive)
		{
			printf("$simple_shell$ ");
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

		if (line[0] == '\0')
			continue;

		argc = 0;
		token = strtok(line, " \t\r\n");

		while (token != NULL && argc < 63)
		{
			argv[argc] = token;
			token = strtok(NULL, " \t\r\n");
			argc++;
		}
		argv[argc] = NULL;

		if (argc == 0)
			continue;

		if (strcmp(argv[0], "exit") == 0)
			break;

		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			continue;
		}

		if (pid == 0)
		{

			if (execve(argv[0], argv, environ) == -1)
			{
				perror("Error:");
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
