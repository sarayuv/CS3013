#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    // read random seed from file
	FILE* seedFile = fopen("seed.txt", "r");
	if (seedFile == NULL) {
		return 0;
	}

	// seed srand() function
	unsigned int seed;
	fscanf(seedFile, "%u", &seed);
	fclose(seedFile);
	printf("Random seed value: %d\n", seed);
    srand(seed);

    // create an a pointer to an array with the directory options
    char *dirs[] = {"/home", "/proc", "/proc/sys", "/usr", "/usr/bin", "/bin"};

    printf("It's time to see the world/file system!\n");

    for (int i = 0; i < 6; i++) {
        // use mod operator with the random number and the number 6 for random number
        int randNum = rand() % 6;
        char *folder = dirs[randNum];

        // print the option that was selected
        printf("Selection #%d: %s [SUCCESS]\n", (i + 1), folder);

        // change the current working directory
        chdir(folder);

        // deal with errors in the current working directory
        if (chdir(folder) != 0) {
            printf("Error changing current working directory");
            exit(1);
        }

        // output the value of the current working directory
        char cwd[4096];

        // deal with errors in the current working directory
        if (getcwd(cwd, 4096) == NULL) {
            exit(1);
        }
        printf("Current reported directory: %s\n", cwd);

        // fork off a child that runs the ls -tr command
        pid_t pid = fork();

        // deal with errors in forking
		if (pid < 0) {
			printf("Error forking\n");
			exit(1);
		}

        if (pid == 0) { // child process
			printf("\t[Child, PID: %d]: Executing 'ls -tr' command...\n", getpid());

            // runs the ls -tr command
            // make args to be able to use execvp with 2 args
            char *args[] = {"ls", "-tr", NULL};
            execvp("ls", args);

            // exit successfully
            exit(0);
		} else { // parent process
			// print statement indicating pid of its child that it is waiting on
			printf("[Parent]: I am waiting for PID %d to finish.\n", pid);

			// wait until their child returns and exits
			waitpid(pid, NULL, 0);

            // print statement indicating that the child has finished
            printf("[Parent]: Child %d finished with status code 0. Onward!\n", pid);
		}
    }

    return 0;
}
