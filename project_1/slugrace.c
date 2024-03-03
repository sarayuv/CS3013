#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

int main() {
/*     // spawn 4 children processes using a loop with a counter
    pid_t child[4];

    //clock_gettime();

    for (int i = 1; i <= 4; i++) {
        child[i] = fork();

        if (child < 0) {
            printf("Forking error\n");
            exit(1);
        } else if (child == 0) {
            // run the slug executable
            char *args[] = {"slugrace", i, NULL};
            execvp("./slug", args);

            exit(1);
        }
    }

    while (1) { // parent process that runs while exit code is 1
        int done = 0;

        // check if children have finished
        for (int i = 1; i <= 4; i++) {
            int standing;
            pid_t pid = waitpid(child[i]. &standing, WNOHANG); // change to usleep()?

            if (pid == 0) {
                done = 0;
            } else if (pid < 0) {
                printf("Error");
                exit(1);
            } else { // finished
                done++;

                printf("Child %d has crossed the finish line! It took %.6f seconds\n", i, 0.00000);
            }
        }

        if (done == 4) {
            // all children are done
            printf("The race is over! It took %.6f seconds\n", 0.00000);
            break;
        }
    } */

    return 0;
}
