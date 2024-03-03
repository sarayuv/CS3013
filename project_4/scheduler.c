#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct job
{
    int id;
    int runtime;
    char job_trace[100];
    struct job *next;
};

void add_to_list(struct job *head, int id, char *job_trace, int runtime);
void file_list(char *job_pointer, struct job *job);
int fifo(struct job *head);
int sjf(struct job *head);
void rr(struct job *head, int timeslice);

void add_to_list(struct job *head, int id, char *job_trace, int runtime)
{
    // find last node in the linked list
    struct job *current = head;
    while (current->next != NULL)
    {
        current = current->next;
    }

    // create new node
    struct job *new_job = (struct job *)malloc(sizeof(struct job));
    new_job->id = id;
    strcpy(new_job->job_trace, job_trace);
    new_job->runtime = runtime;
    new_job->next = NULL;

    // link new node to end of linked list
    current->next = new_job;
}

void file_list(char *job_pointer, struct job *job)
{
    // open file
    FILE *file = fopen(job_pointer, "r");
    if (file == NULL)
    {
        printf("Error: Can't open the file\n");
        return;
    }

    // read job information and add data to linked list
    int counter = 0;
    char buffer[20];

    while (fscanf(file, "%s", buffer) == 1)
    {
        add_to_list(job, counter, buffer, atoi(buffer));
        counter++;
    }

    fclose(file);
}

int compare_runtime(const void *a, const void *b)
{
    const struct job *job_a = *((const struct job **)a);
    const struct job *job_b = *((const struct job **)b);

    // returns a negative value if runtime of a is less than b's, 0 if equal
    return job_a->runtime - job_b->runtime;
}

int fifo(struct job *head)
{
    printf("Execution trace with FIFO:\n");

    // go through linked list and print run statements
    struct job *current = head;
    while (current != NULL)
    {
        printf("Job %d ran for: %d\n", current->id, current->runtime);
        current = current->next;
    }

    printf("End of execution with FIFO.\n");

    return 0;
}

int sjf(struct job *head)
{
    printf("Execution trace with SJF:\n");

    // count number of jobs
    int num_jobs = 0;
    struct job *current = head;
    while (current != NULL)
    {
        num_jobs++;
        current = current->next;
    }

    // create array of pointers to jobs
    struct job **job_array = malloc(num_jobs * sizeof(struct job *));
    current = head;

    // copy linked list into array for sorting
    for (int i = 0; i < num_jobs; i++)
    {
        job_array[i] = current;
        current = current->next;
    }

    // sort array based on runtime
    // store the value at each element at index i into k
    // shift elements greater than key to the right until correct position is found
    for (int i = 1; i < num_jobs; i++)
    {
        struct job *key = job_array[i];
        int j = i - 1;
        while (j >= 0 && job_array[j]->runtime > key->runtime)
        {
            job_array[j + 1] = job_array[j];
            j = j - 1;
        }
        job_array[j + 1] = key;
    }

    // print run statements
    for (int i = 0; i < num_jobs; i++)
    {
        printf("Job %d ran for: %d\n", job_array[i]->id, job_array[i]->runtime);
    }
    printf("End of execution with SJF.\n");

    // free memory
    free(job_array);

    return 0;
}

void rr(struct job *head, int timeslice)
{
    printf("Execution trace with RR:\n");

    // go through linked list
    struct job *current = head;
    while (current != NULL)
    {
        // remaining runtime
        int rem_runtime = current->runtime;

        // execute in timeslices until job is finished
        while (rem_runtime > 0)
        {
            // calculate runtime for the current timeslice
            int runtime;
            if (rem_runtime > timeslice)
            {
                runtime = timeslice;
            }
            else
            {
                runtime = rem_runtime;
            }

            // print run statement with the timeslice it ran for
            printf("Job %d ran for: %d\n", current->id, runtime);

            // update the remaining runtime
            rem_runtime -= runtime;
        }

        // move onto next job in linked list
        current = current->next;
    }

    printf("End of execution with RR.\n");
}

int main(int argc, char **argv)
{
    // check for valid number of arguments
    if (argc != 4)
    {
        printf("Error: Need 4 arguments\n");
        return 1;
    }

    // get argument information from command-line
    char *job_trace = argv[2];
    int timeslice = atoi(argv[3]); // for rr

    // initialize head of linked list
    struct job *head = (struct job *)malloc(sizeof(struct job));
    head->next = NULL;

    // read information from file and add to linked list
    file_list(job_trace, head);

    // check which scheduling policy to use and call the respective function
    if (strcmp(argv[1], "FIFO") == 0)
    {
        fifo(head->next);
    }
    else if (strcmp(argv[1], "SJF") == 0)
    {
        sjf(head->next);
    }
    else if (strcmp(argv[1], "RR") == 0)
    {
        rr(head->next, timeslice);
    }
    else
    {
        printf("Error: Error identifying scheduling policy\n");
        return 1;
    }

    // free memory
    struct job *current = head;
    while (current != NULL)
    {
        struct job *temp = current;
        current = current->next;
        free(temp);
    }

    return 0;
}
