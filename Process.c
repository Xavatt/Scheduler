/*
 * Copyright (c) 2017 Xavier Guinto & Gustavo Santamaria
 *
 * File name:Process.c
 *
 * Author Xavier Guinto & Gustavo Santamaria
 *
 * Purpose: Supports routines for the creation of processes
 *
 * Notes:
 *          Not as complicated as it seems, provides the necessary functions
 *          for the creation and handling of processes and process list.
 *
 *
 */
#include <stdio.h>   /*Used for input and output manipulation (printf)*/
#include <stdlib.h>  /*Used for memory manipulation*/
#include <glib.h>    /*Used so we can use the GList double linked list*/
#include "Process.h" /*Used to access data structures and enums*/

/*
* Creates a process and adds it to the process list.
* @param process_list Pointer to the process list
* @param id The process id number
* @param Arrival The process Arrival Time
* @param Burst The process CPU Burst
* @param Priority The process Priority
* return GList pointer to the new head of the list
*
* For this function to work the pointer to the process list must be
  of type GList as that is what we are using to organize our processes.

  The function takes the information necesary for the creation of a process
  using the Process data structure. After allocating the necesary memory space
  and filling in the information we use the g_list_insert function included in the
  glib library to add it to the end of the list and return the pointer to the start of the new start
  of the list.
*/

GList *CreateProcess(GList *process_list, int id, int arrival, int burst, int priority, int algo)
{
    /* Memory is allocated for our new process */
    Process node = (Process)malloc(sizeof(struct process_p));
    /* All fields are assigned their corresponding data */
    node->process_id = id;
    node->process_arrival = arrival;
    node->process_burst = burst;
    node->process_priority = priority;
    node->process_runtime = 0;
    node->process_lastruntime = 0;
    node->process_remainingcycles = burst;
    /* The new GList header is returned */
    return g_list_insert(process_list, node, -1);
}

/*
* Make some comparation of two pointers in this case (gpointer a & gpointer b)
*
*  a Pointer to the data in GList a that it is comparing
*  b Pointer to the data in GList b that it is comparing
*
* return 0 if they are equal, a negative value if the first element comes before the second,
* or a positive value if the first element comes after the second
*/
gint sortFunctionID(gpointer a, gpointer b)
{
    /* Conversion of gpointer to a process is realized */
    /* ¡BE CAREFUL! We could have easily put Process instead of gpointer */
    /*  doesn't affect performance. */
    Process aa = a;
    Process bb = b;
    /* Comparison is realized */
    return aa->process_id - bb->process_id;
}

/*
* The Function Compares processes by arrival time.
*
* Receive param a Pointer to the data in GList a that it is comparing
* Receive param b Pointer to the data in GList b that it is comparing
*
* return 0 if they are equal, a negative value if the first element comes before the second,
* or a positive value if the first element comes after the second
*
*/
gint sortFunctionArrival(gpointer a, gpointer b)
{
    /* Conversion of gpointer to a process is realized */
    /* ¡BE CAREFUL! We could have easily put Process instead of gpointer */
    /*  doesn't affect performance. */
    Process aa = a;
    Process bb = b;
    /* Comparison is realized */
    return aa->process_arrival - bb->process_arrival;
}

/*
* The Function Compares processes by priority.
*
* Receive param a Pointer to the data in GList a that it is comparing
* Receive param b Pointer to the data in GList b that it is comparing
*
* return 0 if they are equal, a negative value if the first element comes before the second,
* or a positive value if the first element comes after the second
*
*/
gint sortFunctionPriority(gpointer a, gpointer b)
{
    /* Conversion of gpointer to a process is realized */
    /* ¡BE CAREFUL! We could have easily put Process instead of gpointer */
    /*  doesn't affect performance. */
    Process aa = a;
    Process bb = b;
    /* Comparison is realized */
    int compare = aa->process_priority - bb->process_priority;
    /* If equal a sort by ID is realized */
    if (compare == 0)
        compare = sortFunctionID(a, b);
    return compare;
}

/*
* The Function Compares processes by cpu burst.
*
* Receive param a Pointer to the data in GList a that it is comparing
* Receive param b Pointer to the data in GList b that it is comparing
*
* return 0 if they are equal, a negative value if the first element comes before the second,
* or a positive value if the first element comes after the second
*
*/
gint sortFunctionCpuBurst(gpointer a, gpointer b)
{
    /* Conversion of gpointer to a process is realized */
    /* ¡BE CAREFUL! We could have easily put Process instead of gpointer */
    /*  doesn't affect performance. */
    Process aa = a;
    Process bb = b;
    /* Comparison is realized */
    int compare = aa->process_remainingcycles - bb->process_remainingcycles;
    /* If equal a sort by ID is realized */
    if (compare == 0)
        compare = sortFunctionID(a, b);
    return compare;
}

/*
* The Function Sorts a process list.
*
* Receive param process_list Pointer to the GList that's the head of the list
* Receive param sort Integer value representing what type of sort to apply
*
* return Pointer to the new head of the GList that was sorted
*
*/
GList *SortProcessList(GList *process_list, int sort)
{
    /* A sort is applied bases on the variable sort */
    if (sort == ID)
        return g_list_sort(process_list, (GCompareFunc)sortFunctionID);
    if (sort == ARRIVAL)
        return g_list_sort(process_list, (GCompareFunc)sortFunctionArrival);
    if (sort == PRIORITY)
        return g_list_sort(process_list, (GCompareFunc)sortFunctionPriority);
    if (sort == CPUBURST)
        return g_list_sort(process_list, (GCompareFunc)sortFunctionCpuBurst);
}

/*
*  Prints a process list.
*
* Receive param process_list Pointer to the GList that's the head of the list
*
* return output Prints the processes with their information
*
*/
void PrintProcessList(GList *process_list)
{
    GList *l;
    /* The list is cycled through printing every nodes information until it reaches a NULL */
    for (l = process_list; l != NULL; l = l->next)
    {
        Process node = l->data;
        printf("%d %d %d %d %d %d %d\n", node->process_id, node->process_arrival, node->process_burst, node->process_priority, node->process_runtime, node->process_lastruntime, node->process_remainingcycles);
    }
}

/*!
*  Copies a process.
*
* Receive param src Pointer to data that needs to be copied
* Receive param data User data (not used)
*
* return New pointer to a process_p structure
*
*/
Process copyFunction(gpointer src, gpointer data)
{
    /* We convert the gpointer src into a Process */
    Process original = src;
    /* We allocate memory for a new Process */
    Process copy = (Process)malloc(sizeof(struct process_p));
    /* Data is copied */
    copy->process_id = original->process_id;
    copy->process_arrival = original->process_arrival;
    copy->process_burst = original->process_burst;
    copy->process_priority = original->process_priority;
    copy->process_runtime = original->process_runtime;
    copy->process_lastruntime = original->process_lastruntime;
    copy->process_remainingcycles = original->process_remainingcycles;
    /* The new pointer is returned */
    return copy;
}

/*
*  Copies a process list.
*
* Receive param process_list Pointer to the head of the process list to be copied
*
* return Exact copy of the list passes as parameter
*
*/
GList *CopyList(GList *process_list)
{
    /* Custom copyfunction is called as required by g_list_copy_deep */
    return g_list_copy_deep(process_list, (GCopyFunc)copyFunction, NULL);
}

/*
* Frees memory of a process.
*
* Receive param node Pointer to the data in a GList
*
* This function frees or deallocates memory used by a given memory address
  (pointer) using the free() function.
*/
void freeNode(gpointer node)
{
    /* Memory is deallocated from the pointer */
    free(node);
}

/*
* Frees memory of a process list.
*
* Receive param process_list Pointer to the head of the process list to be copied
*
*/
void DestroyList(GList *process_list)
{
    /* Memory is deallocated from the GList, including nodes. */
    g_list_free_full(process_list, freeNode);
}
