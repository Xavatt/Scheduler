/*
 * Copyright (c) 2017 Xavier Guinto & Gustavo Santamaria
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
 *
 * Notes:
 *          None
 *
 */

void FirstCome(GList * process_list);

void NonPreemptive(GList * process_list,int type);

void Preemptive(GList * process_list,int type);

void RoundRobin(GList * process_list, int quantum);
