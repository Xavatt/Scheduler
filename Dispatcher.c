/*
 * Copyright (c) 2017 Xavier Guinto Rios & Gustavo Santamaria
 *
 * File name: Dispatcher.c
 *
 * Author:  Xavier Guinto Rios & Gustavo Santamaria
 *
 * Purpose: Supports routines for the scheduling of processes and
 *          calcultion of average waiting time.
 *
 * References:
 *          Sorting algorithms based on those seen in class and
 *          in notes from Abelardo Lopez Lagunas' slides
 *
 * Error handling:
 *          None
 *
 * Notes:
 *          None
 *
 */
#include <stdio.h>   /* Used for the printf function */
#include <glib.h>    /* Used so we can use the GList double linked list */
#include "Process.h" /* Used for the data structures and sorting functions */

/*!
*  Prints the average wait time of a list.
*
* Receive param process_list Head of the process list that will have the average wait time of it´s elements calculated
* Receive param tipo Text that contains the type of algorithm that was used on the list.
*
* return output Prints out the type of algorithm used and the average wait time for that algorithm
*
*/
void PrintAverageWaitTime(GList *process_list, char *tipo)
{
    /* We create an empty GList pointer */
    GList *l;
    /* We initialize our counters, sum will hold the accumulated wait time and number counts the processes */
    int sum = 0, number = 0;
    /*  We initialize our for loop which will cycle through each one of the processes. It does this using our empty GList pointer created earlier and a for loop.
    *   The calculation of the wait time is taken from the equation
    *   lastruntime-processarrival-processtimeran. */
    for (l = process_list; l != NULL; l = l->next)
    {
        Process node = l->data;
        number++;
        sum += node->process_lastruntime - node->process_arrival - node->process_runtime;
    }
    /* We calculate the average time dividing the total wait time by the number of processes. */
    float avg = (float)sum / number;
    /* The output is presented to the user. */
    printf("Average wait time for %s Algorithm : %f\n", tipo, avg);
}

/*!
* Adds processes to a running list.
*
* Receive param running_list Head of the running list to which we shall add processes.
* Receive param result Head of our results taken from our search function.
*
* return GList* Returns the new head of the running list once elements are added.
*/
GList *addToList(GList *running_list, GList *result)
{
    /* We insert the element from result into the running list */
    /* this is done because we know that that element is the next to run. */
    running_list = g_list_insert(running_list, result->data, -1);
    Process p = result->data;
    GList *l;
    result = result->next;
    /* A cycle is done to check if any other process arrives at the same time as the first process added to the list. */
    for (l = result; l != NULL; l = l->next)
    {
        Process node = l->data;
        if (node->process_arrival == p->process_arrival)
            running_list = g_list_insert(running_list, result->data, -1);
        else
            break; /* Added to save computing time */
    }
    /* The GList pointer is returned. */
    return running_list;
}

/*!
* Applies the First Come First Serve algorithm to a process list.
*
* Receive param process_list Head of the process list containing all the processes ordered by arrival time.
*
* return output Calls PrintAverageWaitTime().
*/
void FirstCome(GList *process_list)
{
    /* Creation of an exact copy of the list */
    GList *fc = CopyList(process_list);
    /* Running process variable declared */
    Process running;
    int time = 0;
    GList *l;
    /*  We cycle through the list in a sequential matter. As we know that the processes run as they arrive and that  there are no gaps in between processes 
    *   we can assume that process(i) will run when all processes arriving before it run. So we have a counter that adds up all the times of processes
    *   that have run. */
    for (l = fc; l != NULL; l = l->next)
    {
        running = l->data;
        running->process_lastruntime = time;
        time += running->process_burst;
    }
    /* The average wait time is displayed */
    PrintAverageWaitTime(fc, "FCFS");
    /* The list is destroyed to save memory */
    DestroyList(fc);
}

/*!
* Used to find a process that arrives at a given time.
*
* Receive param a GPointer with the direction to a process.
* Receive param b Pointer to an integer
*
* return Integer used to determine if the search has a match.
*
*/
gint funcArrival(gpointer a, int *b)
{
    /* Conversion of the pointer to a Process */
    Process aa = a;
    /* The comparison is made */
    return aa->process_arrival - *b;
}

/*!
*  Applies a NonPreemptive algorithm to a process list.
*
* Receive param process_list Head of the process list containing all the processes ordered by arrival time.
* Receive param type enum value of sort_type
*
* return output Calls PrintAverageWaitTime().
*
*/
void NonPreemptive(GList *process_list, int type)
{
    /* Variables are initialized */
    GList *np = CopyList(process_list); /* Our copy */
    GList *runningList = NULL;          /* The running list or queue */
    GList *result = NULL;               /* Result of searches */
    Process running;                    /* Running process */
    int time = 0;                       /* Current time of execution */
    char *string;
    /* We assign the first process in the list to running */
    running = np->data;
    do
    {
        /* A lookup is made to see if a process has arrived at the current time */
        result = g_list_find_custom(np, &time, (GCompareFunc)funcArrival);
        /*  If the result isn't null we add the process to the running list and sort the list
        *   based on the NonPreemptive algorithm chosen */
        if (result != NULL)
        {
            runningList = addToList(runningList, result);
            if (type == PRIORITY)
                runningList = SortProcessList(runningList, PRIORITY);
            else if (type == CPUBURST)
                runningList = SortProcessList(runningList, CPUBURST);
        }
        /* A check is made to see if the running process has finished executing */
        if (running->process_burst == time - running->process_lastruntime)
        {
            /*  If it did, it is removed from the running list and if the list isn't
            *   empty, a new process is put to run. */
            runningList = g_list_remove(runningList, running);
            if (runningList != NULL)
            {
                running = runningList->data;
                running->process_lastruntime = time;
            }
        }
        /* Time is incremented to signal that a cycle has passed */
        time++;
    } while (runningList != NULL);
    /* A string is asigned a value depending on the type of NonPreemptive algorithm used */
    if (type == PRIORITY)
        string = "NonPreemptive Priority";
    else
        string = "NonPreemptive SJF";
    /* Average wait time is printed to the screen and both list are destroyed */
    PrintAverageWaitTime(np, string);
    DestroyList(np);
    DestroyList(runningList);
}

/*!
* Applies a Preemptive algorithm to a process list.
*
* Receive param process_list Head of the process list containing all the processes ordered by arrival time.
* Receive param type enum value of sort_type
*
* return output Calls PrintAverageWaitTime().
*
*/
void Preemptive(GList *process_list, int type)
{
    /* Our variables are initialized and a copy of our original list made. */
    GList *p = CopyList(process_list);
    GList *runningList = NULL; /* Running list */
    GList *result = NULL;      /* Result of search */
    Process running;           /* Current running process */
    int time = 0;              /* Current time of execution */
    int timeRunning = 0;       /* Time the current process has been running */
    char *string;
    /* First element of the list assigned to running */
    running = p->data;
    do
    {
        /*  First condition is checked, if the running process has finished we
        *   remove it from the list and put another process into execution with
        *   it's last run time equal to the current time and timerunning is reset.
        *   if our list is empty, we allow running to be NULL */
        if (running->process_remainingcycles == 0)
        {
            runningList = g_list_remove(runningList, running);
            if (runningList != NULL)
            {
                running = runningList->data;
                running->process_lastruntime = time;
                timeRunning = 0;
            }
            else
            {
                running = NULL;
            }
        }
        /* We search for a process that has arrived at the current time */
        result = g_list_find_custom(p, &time, (GCompareFunc)funcArrival);
        /* If the result isn't NULL we add the process to the runningList and sort by PRIORITY */
        if (result != NULL)
        {
            runningList = addToList(runningList, result);
            if (type == PRIORITY)
                runningList = SortProcessList(runningList, PRIORITY);
            else if (type == CPUBURST)
                runningList = SortProcessList(runningList, CPUBURST);
            /* If running is NULL we get the head of the queue and run it The lastruntime is set to current time and the time running is reset */
            if (running == NULL)
            {
                running = runningList->data;
                running->process_lastruntime = time;
                timeRunning = 0;
            }
            /*  If running isn't NULL and isn't the current head of the queue then we change it
            *   to the current head. We modify it's total runtime and set the new running's
            *   last runtime to the current time. We also reset time running. */
            else if (running != runningList->data)
            {
                int processtime = running->process_runtime;
                running->process_runtime = processtime + timeRunning;
                running = runningList->data;
                running->process_lastruntime = time;
                timeRunning = 0;
            }
        }
        /* Current time and time running are incremented */
        time++;
        timeRunning++;
        /* If running isn't NULL we change the remaining cycles it has. */
        if (running != NULL)
            running->process_remainingcycles = running->process_remainingcycles - 1;
    } while (runningList != NULL);
    /* A string is assigned to indicate the type of sort. */
    if (type == PRIORITY)
        string = "Preemptive Priority";
    else
        string = "Preemptive SJF";
    /* The average wait time is printed and both list are destroyed. */
    PrintAverageWaitTime(p, string);
    DestroyList(p);
    DestroyList(runningList);
}

/*!
* Applies the Round Robin algorithm to a process list.
*
* Receive param process_list Head of the process list containing all the processes ordered by arrival time.
* Receive param quantum The size of the quantum used in the algorithm
*
* return output Calls PrintAverageWaitTime().
*/
void RoundRobin(GList *process_list, int quantum)
{
    /* Our variables are initialized and a copy of our original list is made */
    GList *rr = CopyList(process_list);
    GList *runningList = NULL; /* Used as a queue for processes to execute */
    GList *result = NULL;      /* Result of searches */
    Process running;           /* Current running process */
    int time = 0;              /* Current time */
    int timeRunning = 0;       /* Time current process has run */
    running = rr->data;        /* We assign the first element in the list to running */
    do
    {
        /* We check if the current process has finished running */
        if (running->process_remainingcycles == 0)
        {
            /* If it has, it is removed from the queue */
            runningList = g_list_remove(runningList, running);
            /* If the queue isn't empty, the next process in the queue is ran. It's last run time is set to the current time and the time running is reset */
            if (runningList != NULL)
            {
                running = runningList->data;
                running->process_lastruntime = time;
                timeRunning = 0;
            } /* If the queue is empty, running is left NULL */
            else
            {
                running = NULL;
            }
        }
        /* The next condition checked is if the current process has running for the specified quantum. */
        if (timeRunning == quantum)
        {
            /* If it has, it's runtime is recalculated and it is put at the end of the running list */
            running->process_runtime = running->process_runtime + timeRunning;
            runningList = g_list_remove(runningList, running);
            runningList = g_list_insert(runningList, running, -1);
            /* If the runningList isn't null we run the next process in the queue We set it's last run time to the current time and the time running gets reset */
            if (runningList != NULL)
            {
                running = runningList->data;
                running->process_lastruntime = time;
                timeRunning = 0;
            }
        }
        /* We search the process list to see if there is a process that arrives at the current time. */
        result = g_list_find_custom(rr, &time, (GCompareFunc)funcArrival);
        if (result != NULL)
        {
            /* If there is a non null result, we add the process to the queue or runningList */
            runningList = addToList(runningList, result);
            /* If the current running process is null we assign it the newly arrived process. It's time running is reset and last runtime is set to the current time. */
            if (running == NULL)
            {
                running = runningList->data;
                timeRunning = 0;
                running->process_lastruntime = time;
            }
        }
        /* Both current time and time running are incremented with each cycle */
        time++;
        timeRunning++;
        /* If running isn't NULL we can subtract one cycle from it's remaining cycles */
        if (running != NULL)
            running->process_remainingcycles = running->process_remainingcycles - 1;
    } while (runningList != NULL);
    /* The average wait time is printed and both list are destroyed */
    PrintAverageWaitTime(rr, "Round Robin");
    DestroyList(rr);
    DestroyList(runningList);
}
