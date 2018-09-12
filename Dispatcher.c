/*
 * Copyright (c) 2017 Edgar Daniel Rojas Vazquez
 *
 * File name: Dispatcher.c
 *
 * Author:  Edgar Daniel Rojas Vazquez
 *
 * Purpose: Supports routines for the scheduling of processes and
 *          calcultion of average waiting time.
 *
 * References:
 *          Sorting algorithms based on those seen in class and
 *          in notes from Abelardo Lopez Lagunas' slides
 *
 * Revision history:
 *          https://github.com/EdgarDanielRojas/Scheduler
 *
 * Error handling:
 *          None
 *
 * Notes:
 *          None
 *
 */
#include <stdio.h>   /*Used for the printf function*/
#include <glib.h>    /*Used so we can use the GList double linked list*/
#include "Process.h" /*Used for the data structures and sorting functions*/

/*!
* \brief Prints the average wait time of a list.
*
* \param process_list Head of the process list that will have the average wait time of it´s elements calculated
* \param tipo Text that contains the type of algorithm that was used on the list.
*
* \return output Prints out the type of algorithm used and the average wait time for that algorithm
*
* \details The function takes a pointer to the head of the list and using it
  loops through the whole list, calculating the wait time for each data element (process_p)
  . Once it calculates the wait time of the element it adds it to the total wait time of the
  proceses and divides it by the number of processes which was taken using a counter. g_list_length
  could also be used but it does the same thing in it´s function definition.
*/
void PrintAverageWaitTime(GList *process_list, char *tipo)
{
    //We create an empty GList pointer
    GList *l;
    //We initialize our counters, sum will hold the accumulated wait time and number counts the processes
    int sum = 0, number = 0;
    /*
  We initialize our for loop which will cycle through each one of the processes.
  It does this using our empty GList pointer created earlier and a for loop.
  The calculation of the wait time is taken from the equation
  lastruntime-processarrival-processtimeran.
  */
    for (l = process_list; l != NULL; l = l->next)
    {
        Process node = l->data;
        number++;
        sum += node->process_lastruntime - node->process_arrival - node->process_runtime;
    }
    //We calculate the average time dividing the total wait time by the number of processes.
    float avg = (float)sum / number;
    //The output is presented to the user.
    printf("Average wait time for %s Algorithm : %f\n", tipo, avg);
}

/*!
* \brief Adds processes to a running list.
*
* \param running_list Head of the running list to which we shall add processes.
* \param result Head of our results taken from our search function.
*
* \return GList* Returns the new head of the running list once elements are added.
*
* \details The way the function is used is that it adds elements based on arrival time.
  When it is called it is know that the result in it´s data field holds a process that
  has arrived at the time it is called. Once it's data element is added to the running list
  we use it's arrival time to compare with the rest of the elements in the result list.
  This is because more than one element can arrive at the same time. In the case that an element
  does not have a same arrival time we stop searching the list. In the case that an element does
  have the same arrival time, it is added to the list.
*/
GList *addToList(GList *running_list, GList *result)
{
    //We insert the element from result into the running list
    //this is done because we know that that element is the next to run.
    running_list = g_list_insert(running_list, result->data, -1);
    Process p = result->data;
    GList *l;
    result = result->next;
    //A cycle is done to check if any other process arrives at
    //the same time as the first process added to the list.
    for (l = result; l != NULL; l = l->next)
    {
        Process node = l->data;
        if (node->process_arrival == p->process_arrival)
            running_list = g_list_insert(running_list, result->data, -1);
        else
            break; //Added to save computing time
    }
    //The GList pointer is returned.
    return running_list;
}

/*!
* \brief Applies the First Come First Serve algorithm to a process list.
*
* \param process_list Head of the process list containing all the processes ordered by arrival time.
*
* \return output Calls PrintAverageWaitTime().
*
* \details The function creates a copy of the process list arriving using CopyList() so
  the copy list can be manipulated without fear of changing the original values of the list.
  Once the copy is made, the algorithm calculates the time the process runs by assigning it the
  current time and making the time equal to the time plus the process' cpu burst. We can do this
  since the algorithm is sequential.
*/
void FirstCome(GList *process_list)
{
    //Creation of an exact copy of the list
    GList *fc = CopyList(process_list);
    //Running process variable declared
    Process running;
    int time = 0;
    GList *l;
    /*
  We cycle through the list in a sequential matter. As we
  know that the processes run as they arrive and that
  there are no gaps in between processes, we can assume that
  process(i) will run when all processes arriving before it
  run. So we have a counter that adds up all the times of processes
  that have run.
  */
    for (l = fc; l != NULL; l = l->next)
    {
        running = l->data;
        running->process_lastruntime = time;
        time += running->process_burst;
    }
    //The average wait time is displayed
    PrintAverageWaitTime(fc, "FCFS");
    //The list is destroyed to save memory
    DestroyList(fc);
}

/*!
* \brief Used to find a process that arrives at a given time.
*
* \param a GPointer with the direction to a process.
* \param b Pointer to an integer
*
* \return Integer used to determine if the search has a match.
*
* \details The function works alongside g_list_find_custom to find a process running
  at a given time. The pointer received is converted to a Process and it's process_arrival
  time is substracted the time we pass as a parameter. If 0, then we found a process arriving at that time.
*/
gint funcArrival(gpointer a, int *b)
{
    //Conversion of the pointer to a Process
    Process aa = a;
    //The comparison is made
    return aa->process_arrival - *b;
}

/*!
* \brief Applies a NonPreemptive algorithm to a process list.
*
* \param process_list Head of the process list containing all the processes ordered by arrival time.
* \param type enum value of sort_type
*
* \return output Calls PrintAverageWaitTime().
*
* \details The function creates a copy of the process list arriving using CopyList() so
  the copy list can be manipulated without fear of changing the original values of the list.
  Once the copy is made, the algorithm applied is basically the same for the two type
  of NonPreemptive algorithms (SJF and Priority). First we create a new list called runningList
  which will house the current processes in queue. Next the algorithm enters a loop where
  it will exit once the runningList is NULL. Inside the algorithm there are basically two
  conditions we have to check, if a process arrives at our current time and if our current
  running process has finished. If a process arrive, the queue is sorted based on the
  NonPreemptive algorithm chosen. If a process finishes running, then we remove it from
  the runningList and run the next process. We increment the time with each iteration and
  once it finishes we print the averag wait time using PrintAverageWaitTime().
*/
void NonPreemptive(GList *process_list, int type)
{
    // Variables are initialized
    GList *np = CopyList(process_list); //Our copy
    GList *runningList = NULL;          //The running list or queue
    GList *result = NULL;               //Result of searches
    Process running;                    //Running process
    int time = 0;                       //Current time of execution
    char *string;
    //We assign the first process in the list to running
    running = np->data;
    do
    {
        //A lookup is made to see if a process has arrived at the current time
        result = g_list_find_custom(np, &time, (GCompareFunc)funcArrival);
        //If the result isn't null we add the process to the running list and sort the list
        //based on the NonPreemptive algorithm chosen
        if (result != NULL)
        {
            runningList = addToList(runningList, result);
            if (type == PRIORITY)
                runningList = SortProcessList(runningList, PRIORITY);
            else if (type == CPUBURST)
                runningList = SortProcessList(runningList, CPUBURST);
        }
        //A check is made to see if the running process has finished executing
        if (running->process_burst == time - running->process_lastruntime)
        {
            //If it did, it is removed from the running list and if the list isn't
            //empty, a new process is put to run.
            runningList = g_list_remove(runningList, running);
            if (runningList != NULL)
            {
                running = runningList->data;
                running->process_lastruntime = time;
            }
        }
        //Time is incremented to signal that a cycle has passed
        time++;
    } while (runningList != NULL);
    //A string is asigned a value depending on the type of NonPreemptive algorithm used
    if (type == PRIORITY)
        string = "NonPreemptive Priority";
    else
        string = "NonPreemptive SJF";
    //Average wait time is printed to the screen and both list are destroyed
    PrintAverageWaitTime(np, string);
    DestroyList(np);
    DestroyList(runningList);
}

/*!
* \brief Applies a Preemptive algorithm to a process list.
*
* \param process_list Head of the process list containing all the processes ordered by arrival time.
* \param type enum value of sort_type
*
* \return output Calls PrintAverageWaitTime().
*
* \details The function creates a copy of the process list arriving using CopyList() so
  the copy list can be manipulated without fear of changing the original values of the list.
  Once the copy is made, the algorithm applied is basically the same for the two type
  of Preemptive algorithms (SJF and Priority). First we create a new list called runningList
  which will house the current processes in queue. Next the algorithm enters a loop where
  it will exit once the runningList is NULL. Inside the algorithm there are basically two
  conditions we have to check, if a process arrives at our current time and if our current
  running process has finished. The first to be checked is if the process has finished
  execution, if it has, we remove it from our runningList and execute the next process in
  queue. If a process arrives at our current time we add it to the runningList and
  sort said list. In the case that the process running is no longer the process
  at the head of the queue, we switch out the process with the process at the head of the queue.
   We increment the time with each iteration and once it finishes we print the
   average wait time using PrintAverageWaitTime().
  */
void Preemptive(GList *process_list, int type)
{
    //Our variables are initialized and a copy of our original list made.
    GList *p = CopyList(process_list);
    GList *runningList = NULL; //Running list
    GList *result = NULL;      //Result of searches
    Process running;           //Current running process
    int time = 0;              //Current time of execution
    int timeRunning = 0;       //Time the current process has been running
    char *string;
    //First element of the list assigned to running
    running = p->data;
    do
    {
        //First condition is checked, if the running process has finished we
        //remove it from the list and put another process into execution with
        //it's last run time equal to the current time and timerunning is reset.
        //if our list is empty, we allow running to be NULL
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
        //We search for a process that has arrived at the current time
        result = g_list_find_custom(p, &time, (GCompareFunc)funcArrival);
        //If the result isn't NULL we add the process to the runningList and sort by PRIORITY
        if (result != NULL)
        {
            runningList = addToList(runningList, result);
            if (type == PRIORITY)
                runningList = SortProcessList(runningList, PRIORITY);
            else if (type == CPUBURST)
                runningList = SortProcessList(runningList, CPUBURST);
            //If running is NULL we get the head of the queue and run it
            //The lastruntime is set to current time and the time running is reset
            if (running == NULL)
            {
                running = runningList->data;
                running->process_lastruntime = time;
                timeRunning = 0;
            }
            //If running isn't NULL and isn't the current head of the queue then we change it
            // to the current head. We modify it's total runtime and set the new running's
            //last runtime to the current time. We also reset time running.
            else if (running != runningList->data)
            {
                int processtime = running->process_runtime;
                running->process_runtime = processtime + timeRunning;
                running = runningList->data;
                running->process_lastruntime = time;
                timeRunning = 0;
            }
        }
        //Current time and time running are incremented
        time++;
        timeRunning++;
        //If running isn't NULL we change the remaining cycles it has.
        if (running != NULL)
            running->process_remainingcycles = running->process_remainingcycles - 1;
    } while (runningList != NULL);
    //A string is assigned to indicate the type of sort.
    if (type == PRIORITY)
        string = "Preemptive Priority";
    else
        string = "Preemptive SJF";
    //The average wait time is printed and both list are destroyed.
    PrintAverageWaitTime(p, string);
    DestroyList(p);
    DestroyList(runningList);
}

/*!
* \brief Applies the Round Robin algorithm to a process list.
*
* \param process_list Head of the process list containing all the processes ordered by arrival time.
* \param quantum The size of the quantum used in the algorithm
*
* \return output Calls PrintAverageWaitTime().
*
* \details The function creates a copy of the process list arriving using CopyList() so
  the copy list can be manipulated without fear of changing the original values of the list.
  First we create a new list called runningList
  which will house the current processes in queue. Next the algorithm enters a loop where
  it will exit once the runningList is NULL. Inside the algorithm there are basically
  three conditions we must check. The first one is if the current running process has
  finished running. If that is the case the process is removed from the queue and the Next
  process in queue is executed. The second one is if the current running process has run
  for the determined quantum. If it has then it is put to the end of the queue so it may execute
  later. Lastly we search the process list to find if a process arrives at the current time.
  If we do find a process then we put it at the end of the queue so it may execute later. All this
  is repeated until all processes run and the runningList is left at NULL.
   We increment the time with each iteration and once it finishes we print the
   average wait time using PrintAverageWaitTime().
  */
void RoundRobin(GList *process_list, int quantum)
{
    //Our variables are initialized and a copy of our original list is made
    GList *rr = CopyList(process_list);
    GList *runningList = NULL; //Used as a queue for processes to execute
    GList *result = NULL;      //Result of searches
    Process running;           //Current running process
    int time = 0;              //Current time
    int timeRunning = 0;       //Time current process has run
    running = rr->data;        //We assign the first element in the list to running
    do
    {
        //We check if the current process has finished running
        if (running->process_remainingcycles == 0)
        {
            //If it has, it is removed from the queue
            runningList = g_list_remove(runningList, running);
            //If the queue isn't empty, the next process in the queue is ran.
            //It's last run time is set to the current time and the time running is reset
            if (runningList != NULL)
            {
                running = runningList->data;
                running->process_lastruntime = time;
                timeRunning = 0;
            } //If the queue is empty, running is left NULL
            else
            {
                running = NULL;
            }
        }
        //The next condition checked is if the current process has running
        // for the specified quantum.
        if (timeRunning == quantum)
        {
            //If it has, it's runtime is recalculated and it is put at the end of the running list
            running->process_runtime = running->process_runtime + timeRunning;
            runningList = g_list_remove(runningList, running);
            runningList = g_list_insert(runningList, running, -1);
            //If the runningList isn't null we run the next process in the queue
            //We set it's last run time to the current time and the time running gets reset
            if (runningList != NULL)
            {
                running = runningList->data;
                running->process_lastruntime = time;
                timeRunning = 0;
            }
        }
        //We search the process list to see if there is a process that arrives at the current time.
        result = g_list_find_custom(rr, &time, (GCompareFunc)funcArrival);
        if (result != NULL)
        {
            //If there is a non null result, we add the process to the queue or runningList
            runningList = addToList(runningList, result);
            //If the current running process is null we assign it the newly arrived process.
            //it's time running is reset and last runtime is set to the current time.
            if (running == NULL)
            {
                running = runningList->data;
                timeRunning = 0;
                running->process_lastruntime = time;
            }
        }
        //Both current time and time running are incremented with each cycle
        time++;
        timeRunning++;
        //If running isn't NULL we can subtract one cycle from it's remaining cycles
        if (running != NULL)
            running->process_remainingcycles = running->process_remainingcycles - 1;
    } while (runningList != NULL);
    //The average wait time is printed and both list are destroyed
    PrintAverageWaitTime(rr, "Round Robin");
    DestroyList(rr);
    DestroyList(runningList);
}
