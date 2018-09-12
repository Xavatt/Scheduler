# Scheduler

Scheduler Project

## Objective

Given a list of processes, the goal of this project is to implement a simple process scheduler capable of simulating the behavior of the following scheduling:

- **First Come First Served (FCFS)**
- **Preemtive and Non-Preemtive Shortest Job First (SJF) & Priority**
- **Round Robin (RR)**

The simulator should produce as output the avarage waiting time and average response time for all six scheduling algorithms described above.

## Goals

The implementation of scheduling algorithm will test the comprehension of the scheduling algorithms, as well as verify that the student is capable of using basic dynamic data structures, file managment,and general programming methodologies.

## Problem Statement

We are requiered to implement a process scheduler capable of implementing the following algorithms:

- First Come First Served (FCFS)
- Non-Preemptive Shortest Job First (SJF)
- Non-Preemptive Priority
- Preemtive Shortest Job First (SJF)
- Preemtive Priority
- Round Robin (RR)

Our Program will take as input a _text file_ with an arbitrary number of processes. Each line in the file specifies the **Priority**, **CPU Burst**, and **Arrival Time** for a process. For Round-Robin the **quantum** will be specified as a number in the first line of the file (an integer number in the line). We can assume that the input text fille is passed as a parameter to our program upon invocation. For example:

- _scheduler process_file.txt_

As mentioned before, the file specifies one process per line with the following format: process_id, arrival_rime, CPU_burst, priority.

If the line starts with the # symbol then We must assume that the line is a comment and We simply ignore it. As an example, We are going to take the following table with information about five processes and _quantum_ of 3 unitis of time.

**Table 1. Example of five porcesses**


| Process | Arrival Time | CPU Burst | Priority |
| :-----: | :----------: | :-------: | :------: |
| P1      | 0            | 3         | 3        |
| P2      | 2            | 3         | 0        |
| P3      | 5            | 6         | 2        |
| P4      | 6            | 2         | 1        |
| P5      | 6            | 7         | 3        |

Then the text file describing the above table would be:

|3|
| ----- |
|1 0 3 3|
|2 2 3 0|
|3 5 6 2|
|4 6 2 1|
|5 6 7 3|

If there are gaps between the time a process finishes execution and another process starting execution that means that the CPU was idle and this will have an effect on the average waiting
time and average response time.

## Expected Outputs

The program should out put the average waiting time and average response time for each of the six scheduling algorithms in the following order: First Come First Served (FCFS), Non-Preemptive Shortest Job First (SJF), Non-Preemptive Priority, Preemptive Shortest Job First (SJF), Preemptive Priority, and Round Robin. Use 32-bit floating point variables with four digits of precision when you display your results.

**Consider the processes described in Table 1. The Expected Results Are:**

| Algorithm               | Average Wait Time | Average Response Time | Number of Process |
| :---------------------: | :---------------: | :-------------------: | :---------------: |
| FCFS                    | 3.2000            | 7.0000                | 5                 |
| Non-Preemptive Priority | 2.4000            | 6.2000                | 5                 |
| Non-Preemptive SJF      | 2.4000            | 6.2000                | 5                 |
| Preemptive Priority     | 4.2000            | 5.4000                | 5                 |
| Preemtive SJF           | 2.4000            | 6.2000                | 5                 |
| Round Robin             | 3.6000            | 5.8000                | 5                 |

# Problem Statement