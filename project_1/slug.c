#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    // check for the right number of args
    if (argc != 2) {
        printf("Error in number of arguments");
        exit(1);
    }

    // make the arg an int to easily use
    int slug = atoi(argv[1]);

    // string for the file name
    char fileName[17];

    // assign the file name based on the command line argument
    if (slug != 1 && slug != 2 && slug != 3 && slug != 4) {
        printf("Choose 1, 2, 3, or 4\n");
        exit(1);
    } else {
        snprintf(fileName, 17, "seed_slug_%d.txt", slug);
    }

    // open the selected file
    FILE* seedFile = fopen(fileName, "r");
	if (seedFile == NULL) {
		return 0;
	}

	// read and set the seed
	unsigned int seed;
	fscanf(seedFile, "%u", &seed);
	fclose(seedFile);
	printf("[Slug PID: %d] Read seed value: %u\n", getpid(), seed);
    srand(seed);

    int seconds = (rand() % 5 + 2); // between 2 and 6 seconds
    int coinFlip = (rand() % 2); 

    printf("[Slug PID: %d] Delay time is %d seconds. Coin flip: %d\n", getpid(), seconds, coinFlip);

    // wait before proceeding
    sleep(seconds);

    // determine which command to run
    if (coinFlip == 1) {
        printf("[Slug PID: %d] I'll get the job done. Eventually...\n", getpid());
        
        // run the "last -i -x" command
        // make args to be able to use execvp
        char *args1[] = {"last", "-i", "-x", NULL};
        execvp("last", args1);
    } else {
        printf("[Slug PID: %d] Break time is over! I am running the 'id --group' command.\n", getpid());
        
        // run the "id --group" command
        // make args to be able to use execvp with 2 args
        char *args2[] = {"id", "--group", NULL};
        execvp("id", args2);
        
        exit(0);
    }

    return 0;
}
