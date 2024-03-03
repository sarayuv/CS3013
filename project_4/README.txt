CS 3013 Project 4 - Sarayu Vijayanagaram

This C program simulates and analyzes the performance of different scheduling algorithms including first-in, first-out, shortest job first, and round robin. It is also supposed to evaluate performance metrics like response time, turnaround time, and wait time for each algorithm. The FIFO implementation is straightforward as the jobs execute in the order that they arrive. For SJF, an array of pointers to jobs is created and the data from the linked list of jobs is copied into an array so that the jobs can be sorted. The array is sorted based on runtime so that the shortest job is executed first. For RR, the jobs are executed for the given timeslice that is passed into the function. The remaining runtime is calculated after every execution so that the value is updated. 
Not all of the test cases pass and I was unable to implement the policy analysis of phase 2 without messing up the output of first phase. However, I do have answers for the workload examples.

workload_1.in:
Instruction - Design a workload of a least 5 jobs for the RR scheduler such that the wait time is the same as the response time for all jobs. Assume a time slice of 3 time units. 

Each job in this workload has the same wait time as its response time. Job 1's runtime is 4 (less than the timeslice), so it executes entirely. It's wait time and response time are the same since it starts immediately when it arrives. Job 2, 3, 4, and 5 are right after one another and all complete in one time slice.
Job 1 (Wait time = 0, Response time = 0, Turnaround time = 4)
Job 2 (Wait time = 0, Response time = 4, Turnaround time = 7)
Job 3 (Wait time = 0, Response time = 7, Turnaround time = 10)
Job 4 (Wait time = 0, Response time = 10, Turnaround time = 13)
Job 5 (Wait time = 0, Response time = 13, Turnaround time = 16)

workload_2.in:
Instruction - Design a workload of at least 5 jobs such that the average turnaround time of the FIFO scheduler is approximately 10 times that of the SJF scheduler. 

(FIFO) 
Job 1 (Wait time = 0, Response time = 0, Turnaround time = 10)
Job 2 (Wait time = 10, Response time = 10, Turnaround time = 30)
Job 3 (Wait time = 30, Response time = 30, Turnaround time = 35)
Job 4 (Wait time = 35, Response time = 35, Turnaround time = 50)
Job 5 (Wait time = 50, Response time = 50, Turnaround time = 75)
(SJF) 
Job 3 (Wait time = 0, Response time = 0, Turnaround time = 5)
Job 1 (Wait time = 5, Response time = 5, Turnaround time = 15)
Job 4 (Wait time = 15, Response time = 15, Turnaround time = 30)
Job 2 (Wait time = 30, Response time = 30, Turnaround time = 50)
Job 5 (Wait time = 50, Response time = 50, Turnaround time = 75)

workload_3.in:
Instruction - Design a workload of at least 5 jobs such that FIFO, SJF, and RR produce the same average response time, turnaround time, and wait time. Assume a time slice of 3 time units.

(FIFO)
Job 1 (Wait time = 0, Response time = 0, Turnaround time = 5)
Job 2 (Wait time = 5, Response time = 5, Turnaround time = 11)
Job 3 (Wait time = 11, Response time = 11, Turnaround time = 18)
Job 4 (Wait time = 18, Response time = 18, Turnaround time = 22)
Job 5 (Wait time = 22, Response time = 22, Turnaround time = 27)
(SJF)
Job 1 (Wait time = 0, Response time = 0, Turnaround time = 5)
Job 2 (Wait time = 5, Response time = 5, Turnaround time = 11)
Job 3 (Wait time = 11, Response time = 11, Turnaround time = 18)
Job 4 (Wait time = 18, Response time = 18, Turnaround time = 22)
Job 5 (Wait time = 22, Response time = 22, Turnaround time = 27)
(RR)
Job 1 (Wait time = 0, Response time = 0, Turnaround time = 5)
Job 2 (Wait time = 3, Response time = 3, Turnaround time = 9)
Job 3 (Wait time = 6, Response time = 6, Turnaround time = 13)
Job 4 (Wait time = 9, Response time = 9, Turnaround time = 17)
Job 5 (Wait time = 12, Response time = 12, Turnaround time = 21)

workload_4.in:
Instruction - Design a workload of at least 5 jobs such that RR produces an average wait time of less than 5 time units but a turnaround time greater than 100 time units. Assume a time slice of 3 time units.

(RR)
Job 1 (Wait time = 0, Response time = 0, Turnaround time = 30)
Job 2 (Wait time = 3, Response time = 3, Turnaround time = 23)
Job 3 (Wait time = 6, Response time = 6, Turnaround time = 21)
Job 4 (Wait time = 9, Response time = 9, Turnaround time = 49)
Job 5 (Wait time = 12, Response time = 12, Turnaround time = 37)

workload_5.in:
Instruction - Design a workload of exactly 3 jobs such that FIFO produces an average response time of exactly 5 time units and an average turnaround time of exactly 13 time units. Assume that the first job has a duration of 3 time units. 

(FIFO)
Job 1 (Wait time = 0, Response time = 0, Turnaround time = 2)
Job 2 (Wait time = 2, Response time = 2, Turnaround time = 8)
Job 3 (Wait time = 8, Response time = 8, Turnaround time = 16)


To Run:
make
chmod +x *
./run_tests.sh
OR ./run_tests.sh -t #
