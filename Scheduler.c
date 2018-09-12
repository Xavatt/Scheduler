/*
 * Copyright (c) 2012 Abelardo Lopez Lagunas
 *
 * Program: scheduler.c
 *
 * Author:  Abelardo Lopez Lagunas
 *
 * Purpose: This program impelemnets the basic process scheduling
 *          algorithms for the TC2008 class.
 *
 * Usage:
 *          The program reads a text file with the processes. The first
 *          integer in the file is the quantum, followed by four integer
 *          numbers per line describing the process id, arrival time,
 *          cpu burst and priority. The usage form is:
 *
 *          schedule file.txt
 *
 * References:
 *          The material that describe the scheduling algorithms is
 *          covered in my class notes for TC2008
 *
 * File formats:
 *          The input file should have four numbers per list in ASCII
 *          format. The exeption is teh forst line which only has one
 *          integer number that represents the quantum.
 *
 * Restrictions:
 *          If the input file is not in ASCII format the program exits
 *
 * Revision history:
 *
 *          Feb 16 11:57 2011 - File created
 *
 *          May 24 11:56 2012 - Code refactoring & big fixes
 *
 * Error handling:
 *          On any unrecoverable error, the program exits
 *
 * Notes:
 *          This code presents a solution for the first assignment
 *
 * $Id$
 */

/*!
  * \author Edgar Daniel Rojas Vazquez & Abelardo Lopez Lagunas
  * \version 1.0
  * \date 19/09/2017
  * \copyright GNU Public License
  * \mainpage Process scheduler
  * \section intro Introduction
  * This code was made for the class TC2008 Operating Systems.
  * The main focus of this code is to take a file as an input with various processes
  * on it and apply the 6 process scheduling algorithms seen in class. Afterwards
  * it shall print out the average wait time for each algorithm applied. The only
  * code reutilized was that which was provided by the Profesor Abelardo Lopez Lagunas.
  * The files reutilized are scheduler.c, FileIO.h and FileIO.c .
  * \section info Documentation
  * The documentation for this code is accessible from these html file. In the case
  * that the algorithm isn't described thoroughly enough in this documentation, the source
  * code contains more comments with respect to how the algorithms where implemented.
  * \section comp Compilation
  * In order to compile this code it is necessary to have the GLib libraries installed on
  * your machine, once that is done, you need to run the following command to compile all the
  * files using the GLib library.\n gcc scheduler.c Process.c FileIO.c Dispatcher.c `pkg-config --cflags --libs glib-2.0` -Wall -o scheduler
  */

#include <glib.h>       /* Used for data structure manipulation*/
#include <stdlib.h>     /* Used for malloc definition */
#include <stdio.h>      /* Used for fprintf */
#include <string.h>     /* Used for strcmp */
#include <assert.h>     /* Used for the assert macro */
#include "FileIO.h"     /* Definition of file access support functions */
#include "Process.h"    /* Used for handling of processes*/
#include "Dispatcher.h" /* Implementation of the dispatcher algorithms */

/***********************************************************************
 *                       Global constant values                        *
 **********************************************************************/
#define NUMPARAMS 2 //!< Constant used to define the number of parameters we must receive.
#define NUMVAL 4    //!< Constant used to define the number of values we shall read from a file.

/***********************************************************************
 *                          Main entry point                           *
 **********************************************************************/
int main(int argc, const char *argv[])
{

    FILE *fp;                    /* Pointer to the file */
    int quantum = 0;             /* Quantum value for round robin */
    GList *processList_p = NULL; /* Pointer to the process list */
    int parameters[NUMVAL];      /* Process parameters in the line */
    int i;                       /* Number of parameters in the process */

    /* Check if the number of parameters is correct */
    if (argc < NUMPARAMS)
    {
        printf("Need a file with the process information\n");
        printf("Abnormal termination\n");
        return (EXIT_FAILURE);
    }
    else
    {
        /* Open the file and check that it exists */
        fp = fopen(argv[1], "r"); /* Open file for read operation */
        if (!fp)
        { /* There is an error */
            ErrorMsg("main", "filename does not exist or is corrupted");
        }
        else
        {
            /* The first number in the file is the quantum */
            quantum = GetInt(fp);

            if (quantum == EXIT_FAILURE)
            {
                ErrorMsg("main", "Quantum not found");
            }
            else
            {
                /*
                 * Read the process information until the end of file
                 * is reached.
                 */
                while (!feof(fp))
                {
                    /* For every four parameters create a new process */
                    for (i = 0; ((i < NUMVAL) && (!feof(fp))); i++)
                    {
                        parameters[i] = GetInt(fp);
                    }

                    /* Do we have four parameters? */
                    if (i == NUMVAL)
                    {
                        processList_p = CreateProcess(processList_p,
                                                      parameters[0],
                                                      parameters[1],
                                                      parameters[2],
                                                      parameters[3],
                                                      1);
                    }
                }
            }
        }

        /* Start by sorting the processes by arrival time */
        processList_p = SortProcessList(processList_p, ARRIVAL);

#ifdef DEBUG
        /* Now print each element in the list */
        PrintProcessList(processList_p);
#endif

        /*
         * Apply all the scheduling algorithms and print the results
         */
        FirstCome(processList_p);

        NonPreemptive(processList_p, PRIORITY);

        NonPreemptive(processList_p, CPUBURST);

        Preemptive(processList_p, PRIORITY);

        Preemptive(processList_p, CPUBURST);

        RoundRobin(processList_p, quantum);

        /* Deallocate the memory assigned to the list */
        DestroyList(processList_p);

        printf("Program terminated correctly\n");
        return (EXIT_SUCCESS);
    }
}
