#ifndef SCHEDULER_H
#define SCHEDULER_H

struct job
{
    int id;
    int runtime;
    char job_trace[20];
    struct job *next;
};

void add_to_list(struct job *head, int id, char *job_trace, int runtime);
void file_list(char *job_pointer, struct job *job);
int fifo(struct job *head);
int sjf(struct job *head);
void rr(struct job *head, int timeslice);

#endif
