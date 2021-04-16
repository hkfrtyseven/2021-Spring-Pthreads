/*
 * par_sumsq.c
 *
 * CS 446.646 Project 1 (Pthreads)
 *
 * Compile with --std=c99
 *
 * Version 1.0	//  First Attempt a Pthread programming
 * 		//  Linked List code taken fromi learn-c.org
 */

#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>	//  First step, include pthread library

// aggregate variables	//  This is the data the pthreads will change
long sum = 0;
long odd = 0;
long min = INT_MAX;
long max = INT_MIN;
bool done = false;

// function prototypes	//  This is the function that the pthreads will execute
void calculate_square(long number);

/*
 * update global aggregate variables given a number
 * for use in pthreads
 */
void calculate_square(long number)
{

  // calculate the square
  long the_square = number * number;

  // ok that was not so hard, but let's pretend it was
  // simulate how hard it is to square this number!
  sleep(number);

  // let's add this to our (global) sum
  sum += the_square;

  // now we also tabulate some (meaningless) statistics
  if (number % 2 == 1) {
    // how many of our numbers were odd?
    odd++;
  }

  // what was the smallest one we had to deal with?
  if (number < min) {
    min = number;
  }

  // and what was the biggest one?
  if (number > max) {
    max = number;
  }
}

  // Linked list implementation for wait/process
typedef struct node {

  char process;
  long timeCost;

  struct node* next;
} node_Link;


int main(int argc, char* argv[])
{
  // # of thread var
  int thread_count = 0;

  // Initialize Linked List
  node_Link* head = NULL;
  head = (node_Link*) malloc(sizeof(node_Link));
  head->next = NULL;

  // check and parse command line options
  if (argc != 2) {
    printf("Usage: sumsq <infile>\n");
    exit(EXIT_FAILURE);
  }
  char *fn = argv[1];

  // Read number of worker threads
  thread_count = strtol(argv[2], NULL, 10);

  // load numbers and add them to the queue
  FILE* fin = fopen(fn, "r");
  char action;
  long num;

  node_Link* current = NULL;
  current = head;
  while (fscanf(fin, "%c %ld\n", &action, &num) == 2) {
    // Read values from file into linked list
    current->process = action;
    current->timeCost = num;

    current->next = (node_Link*) malloc(sizeof(node_Link));
    current = current->next;

    /*
     * sumsq version
     *
    if (action == 'p') {            // process, do some work
      calculate_square(num);
    } else if (action == 'w') {     // wait, nothing new happening
      sleep(num);
    } else {
      printf("ERROR: Unrecognized action: '%c'\n", action);
      exit(EXIT_FAILURE);
    }
    */
  }
  fclose(fin);
  
  // print results
  printf("%ld %ld %ld %ld\n", sum, odd, min, max);
  
  // clean up and return
  return (EXIT_SUCCESS);
}

