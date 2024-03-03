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
	printf("Seed value: %d\n", seed);
	srand(seed);

	// choose the number of children
	int numOfChildren = rand() % 6 + 8;	// rand() % (13-8+1)+8
	printf("Random child count: %d\n", numOfChildren);
	printf("I'm feeling prolific!\n");

	// generate a random number for each child and store in an array
	int childNumArray[numOfChildren];
	for (int i = 0; i < numOfChildren; i++) {
		childNumArray[i] = rand();
	}

	for (int i = 0; i < numOfChildren; i++) {
		int myRandom = childNumArray[i];
		pid_t ownPid = getpid();

		// determine exit code and wait time
		int exitCode = (myRandom % 50) + 1;
		int waitTime = (myRandom % 3) + 1;

		pid_t pid = fork();

		if (pid < 0) {
			printf("Error forking");
			exit(1);
		} else if (pid == 0) { // child process
			// print message revealing pid, delay time, and return value
			printf("\t[Child, PID: %d]: I am the child and I will wait %d seconds and exit with code %d.\n", ownPid, waitTime, exitCode);

			// wait for indicated amount time and exit with selected exit value
			sleep(waitTime);
			
			printf("\t[Child, PID: %d] Now exiting...\n", ownPid);
			exit(exitCode);
		} else { // parent process
			printf("[Parent]: I am waiting for PID %d to finish.\n", pid);
			waitpid(pid, &exitCode, 0);
			printf("[Parent]: Child %d finished with status code %d. Onward!\n", pid, exitCode);
		}
	}

	return 0;
}
