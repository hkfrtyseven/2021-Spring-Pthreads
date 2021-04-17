/*
 * par_sumsq.c
 *
 * CS 446.646 Project 1 (Pthreads)
 *
 * Compile with --std=c99
 *
 * Version 1.0	//  First Attempt a Pthread programming
 * 		//  Linked List code taken fromi learn-c.org
 *		//  Need to init pthreads -> workerfunction(calculate_square)
 */

#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>	//  First step, include pthread library

// aggregate variables	//  This is the data the pthreads will change
volatile long sum = 0;
volatile long odd = 0;
volatile long min = INT_MAX;
volatile long max = INT_MIN;
volatile bool done = false;

// Mutex
pthread_mutex_t listMutex;
pthread_cond_t listCond;

// function prototype
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
  // Mutex/conditional initialization
  pthread_mutex_init(&listMutex, NULL);
  pthread_cond_init(&listCond, NULL);

  // # of threads
  long thread_count = 0;

  // # of processes
  long num_Proc = 0;

  // Initialize Linked List
  node_Link* head = NULL;
  head = (node_Link*) malloc(sizeof(node_Link));
  head->next = NULL;

  // check and parse command line options
  if (argc != 3) {
    printf("Incorrect Usage: par_sumsq <infile>\n");
    exit(EXIT_FAILURE);
  }
  char *fn = argv[1];

  // Read number of worker threads
  thread_count = strtol(argv[2], NULL, 10);
  if (thread_count <= 0) {
    printf("Invalid thread count entered.");
    exit(EXIT_FAILURE);
  }

  // Initialize pthreads
  pthread_t workers[thread_count];
  long i;
  for (i = 0; i < thread_count; i++) {
    pthread_create(&workers[i], NULL, FUNCTION, NULL);	//  NEED A PTHREAD FUNCTION
  }

  // load numbers and add them to the queue
  FILE* fin = fopen(fn, "r");
  char action;
  long num;

  node_Link* current = NULL;
  current = head;

  while (fscanf(fin, "%c %ld\n", &action, &num) == 2) {
    // Read values from file into linked list
    if (action == 'p') {            // process, add to task list
      current.process = action;
      current.timeCost = num;
      current->nex t = (node_Link*) malloc(sizeof(node_Link));
      current = current->next;
    } else if (action == 'w') {     // wait, nothing new happening
      sleep(num);
    } else {
      printf("ERROR: Unrecognized action: '%c'\n", action);
      exit(EXIT_FAILURE);
    }
  }

  fclose(fin);



  // print results
  printf("%ld %ld %ld %ld\n", sum, odd, min, max);

  // clean up and return
  return (EXIT_SUCCESS);
}

