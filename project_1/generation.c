#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void newGeneration(int lifespanCount);

void newGeneration(int lifespanCount) {
	if (lifespanCount > 0) {
		// spawn a child
		pid_t pid = fork();

		// deal with errors in forking
		if (pid < 0) {
			printf("Error forking\n");
			exit(1);
		}

		if (pid == 0) { // child process
			printf("\t[Child, PID: %d]: I was called with descendant count=%d. I'll have %d descendant(s).\n", getpid(), lifespanCount, (lifespanCount - 1));

			// call newGeneration with a parameter for one less descendant
			newGeneration(lifespanCount - 1);

			// when the initial parent process confirms its child has exited
        	// print out the exit value of its child
			printf("[Parent, PID: %d]: Child %d finished with status code %d. It's now my turn to exit.\n", getpid(), (getpid() + 1), (lifespanCount - 1));

			exit(0);
		} else { // parent process
			// print statement indicating pid and that of its child
			printf("[Parent, PID: %d]: I am waiting for PID %d to finish.\n", getpid(), pid);

			// wait until their child returns and exits
			waitpid(pid, NULL, 0);
		}
	}
}

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
	printf("Read seed value: %u\n", seed);
	srand(seed);
    
	// choose the number of children
	int lifespanCount = rand() % 8 + 5;	// rand() % (12-5+1)+5
	printf("Random descendant count: %d\n", lifespanCount);
	printf("Time to meet the kids/grandkids/great grand kids/...\n");

    if (lifespanCount != 0) {
        // call newGeneration()
        newGeneration(lifespanCount);
    }

	return 0;
}
