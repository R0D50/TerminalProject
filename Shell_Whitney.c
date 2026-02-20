#include <stdio.h>	// printf, fgets
#include <stdlib.h>	// exit, malloc
#include <string.h>	// strtok, strcmp
#include <unistd.h>	// fork. execvp, chdir, getcwd
#include <sys/wait.h>	// wait
#include <fcntl.h>	// open
#include <errno.h>	// errno
#include <sys/types.h>

#define MAX_LINE 1024	// Maximum length of input line
#define MAX_ARGS 100	// Maximum number if arguments

void print_prompt() {
	char cwd[1024];

	//getscwd gets the absolute path of the current directory
	if (getcwd(cwd, sifeof(cwd)) != NULL){
		printf("%s$", cwd);
		fflush(stdout);	// ensures prompt immediately prints
	} else {
		perror("getcwd");
	}
}

int main() {
	char line[MAX_LINE];	//Stores raw user input

	while (1) {
		//show prompt
		print_prompt();

		//read user input
		if (fgets(line, MAX_LINE, stdin) == NULL) {
			printf("\n");
			break;
		}

		//removes newline character from input
		line[strcspn(line, "\n")] = '\0';

		if (strlen(line) == 0)
			continue;

		char *args[MAX_ARGS];	//argument list for execvp
		int i = 0;

		char *input_file = NULL;
		char *output_file = NULL;

		// tokenize input by spaces
		char *token = strtok(line, " ");

		while (token != NULL) {

			// detects input direction
			if (strcmp(token, "<") == 0) {
				token = strtok(NULL, " ");
				input_file = token;
			}

			// detects output direction
			else if (strcmp(token, ">") == 0) {
				token = strtok(NULL, " ");
				output_file = token;
			}
			// Otherwise it is a normal argument
			else {
				args[i] = token;
				i++;
			}
			token = strtok(NULL, " ");
		}

		args[i] = NULL;	//execvp requires NULL-terminated array

		if (args[0] == NULL)
			continue;

		//exit comman terminates shelll
		if (strcmp(args[0], "exit") == 0)
			break;

		//cd must run in parent(not the child)
		if (strcmp(args[0],"cd") == 0) {
			if (args[1] == NULL) {
				fprintf(stdrr, "cd: missing argument\n");
			} else if {chdir(args[1]) != 0) {
				perror ("cd");
			}
			continue;
		}

		pid_t pid = fork();

		if (pid < 0) {
			perror("fork");
			continue;
		}

		if (pid == 0) {


			if (input_file != NULL) {
				int fd = open(input_file, O_RDONLY);
				if (fd < 0) {
					perror("open input");
					exit(EXIT_FAILURE);
				}
				dup2(fd, STDIN_FILENO);
				close(fd);
			}

			if (output_file != NULL) {
				int fd = open(output_file, O_WONGLY| O_CREAT | O_TRUNC, 0644);
				if (fd < 0) {
					perror("open output");
					exit(EXIT_FAILURE);

				}
				dup2(fd, STDOUT_FILENO);
				close(fd);
			}

			// replace child process with command
			execvp(args[0], args);

			fprintf(stderr, "Error %d (%s)\n", errno, strerror(errno));
			exit(EXIT_FAILURE);
		}

		else {
			wait(NULL);
		}
	}
	return 0;
}
