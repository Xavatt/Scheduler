/*
 * Copyright (c) 2017 Xavier Guinto & Gustavo Santamaria
 *
 * File name:Process.h
 *
 * Author:  Xavier Guinto & Gustavo Santamaria
 *
 * Purpose: Header file that supports routines for the creation of processes
 * 
 * Notes:
 *          Not as complicated as it seems, provides the necessary functions
 *          for the creation and handling of processes and process list.
 *
 */

//We make a typedef to facilitate declaration of process_p structures in the future
typedef struct process_p *Process;

/* Declaration of the data structure process_p that will hold information about
  * each process that will run.
  */
struct process_p
{
    int process_id;              //!< The id of the process
    int process_arrival;         //!< The arrival time of the process
    int process_burst;           //!< The cpu burst of the process
    int process_priority;        //!< The priority of the process
    int process_runtime;         //!< The total time the process has run
    int process_lastruntime;     //!< The last time the process started execution
    int process_remainingcycles; //!< The cpu burst left in the process
};

/*! We declare an enum to facilitate the use of
  *  sort types and algorithm types we may use
  */
enum sort_type
{
    ID //!< Constant for ID
    ,
    ARRIVAL //!< Constant for Arrival
    ,
    PRIORITY //!< Constant for Priority
    ,
    CPUBURST //!< Constant for Cpu burst
};
// Consult documentation or Process.c for more information.
GList *CreateProcess(GList *process_list, int id, int arrival, int burst, int priority, int algo);

GList *SortProcessList(GList *process_list, int sort);

void PrintProcessList(GList *process_list);

GList *CopyList(GList *process_list);

void freeNode(gpointer node);

void DestroyList(GList *process_list);
