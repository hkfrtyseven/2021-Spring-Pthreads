/*
 * par_sumsq.c
 *
 * CS 446.646 Project 1 (Pthreads)
 *
 * Compile with --std=c99
 *
 * By MATT FACQUE
 *
 * Version 1.0	//  First Attempt a Pthread programming
 * 		//  Linked List code taken fromi learn-c.org
 *		//  Need to init pthreads -> workerfunction(calculate_square)
 *	   	//  Errors, but have a pthread function
 * 	   2.0  //  Working Version!
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
volatile bool done = 0;
volatile long num_Proc = 0;

// Global List
typedef struct node {
  char process;
  long timeCost;

  struct node* next;
} node_Link;

volatile node_Link* head = NULL;

// Mutex
pthread_mutex_t listMutex;
pthread_cond_t listCond;

// function prototype
void calculate_square(long number);
int startProc(void* args);
/*
 * update global aggregate variables given a number
 * for use in pthreads
 */
void calculate_square(long number)
{
  // lock the mutex
//  pthread_mutex_lock(&listMutex);

  // calculate the square
  long the_square = number * number;
  
  // ok that was not so hard, but let's pretend it was
  // simulate how hard it is to square this number!
  sleep(number);

  pthread_mutex_lock(&listMutex);
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

  // unlock the mutex
  pthread_mutex_unlock(&listMutex);

}

/*
 * Function to run in pthread
 */
int startProc(void* args) {
  node_Link* next_Node = NULL;
  long val = 0;

//  printf("PTHREAD START \n");

  while (1) {
    // Lock mutex for pthread processessing
    pthread_mutex_lock(&listMutex);

    if (done) {
      pthread_mutex_unlock(&listMutex);
      break;
    }

    next_Node = head->next;
    val = head->timeCost;
    head = next_Node;
    num_Proc--;

//    printf("TEST VAL = %ld \n", val);

    // TEST DONE
    if (num_Proc == 0) {
     done = 1;
    }

    // unlock mutex
    pthread_mutex_unlock(&listMutex);

    calculate_square(val);
  }
  return 0;
}

/*
  // Linked list implementation for wait/process
typedef struct node {

  char process;
  long timeCost;

  struct node* next;
} node_Link;

  // Global linked_list
node_Link* head = NULL;
head = (node_Link*) malloc(sizeof(node_Link);
head->next = NULL;
*/

int main(int argc, char* argv[])
{
  // Complete head of linked list
  head = (node_Link*) malloc(sizeof(node_Link));
  head->next = NULL;

  // Mutex/conditional initialization
  pthread_mutex_init(&listMutex, NULL);
  pthread_cond_init(&listCond, NULL);

  // # of threads
  long thread_count = 0;

  // check and parse command line options
  if (argc != 3) {
    printf("Incorrect Usage: par_sumsq <infile>\n");
    exit(EXIT_FAILURE);
  }
  char *fn = argv[1];

  // Read number of worker threads
  thread_count = strtol(argv[2], NULL, 10);
  // TEST
//  printf("TEST THREADCOUNT = %ld \n", thread_count);
  if (thread_count <= 0) {
    printf("Invalid thread count entered.");
    exit(EXIT_FAILURE);
  }

/*
  // Initialize pthreads
  pthread_t workers[thread_count];
  long i;
  for (i = 0; i < thread_count; i++) {
    pthread_create(&workers[i], NULL, &startProc, NULL);	//  startProc = pthread function
  }
*/

  // load numbers and add them to the queue
  FILE* fin = fopen(fn, "r");
  char action;
  long num;

  // traversal pointer to list
  node_Link* current = NULL;
  current = head;

  while (fscanf(fin, "%c %ld\n", &action, &num) == 2) {
    // Read values from file into linked list
    if (action == 'p') {            // process, add to task list
      // Lock queue additions
      pthread_mutex_lock(&listMutex);

      current->process = action;
      current->timeCost = num;
//      printf("TEST TIMECOST = %ld \n", num);
      current->next = (node_Link*) malloc(sizeof(node_Link));
      current = current->next;
      num_Proc++;

      // Unlock mutex
//      pthread_mutex_unlock(&listMutex);

      // Conditional variable signal to return mutex/predicate
//      pthread_cond_signal(&listCond);

      // Unlock mutex
      pthread_mutex_unlock(&listMutex);

    } else if (action == 'w') {     // wait, nothing new happening
      sleep(num);
    } else {
      printf("ERROR: Unrecognized action: '%c'\n", action);
      exit(EXIT_FAILURE);
    }
  }

  fclose(fin);

//  printf("PTHREAD INIT \n");

  pthread_t workers[thread_count];
  long i;
  for (i = 0; i < thread_count; i++) {
//    printf("WORKER INIT \n");
    pthread_create(&workers[i], NULL, &startProc, NULL);
  }

//  printf("TEST AFTER INIT \n");

  // clean up and return
  for (i = 0; i < thread_count; i++) {
//    printf("WORKER JOIN \n");
    pthread_join(workers[i], NULL);
  }

  // print results
  printf("%ld %ld %ld %ld\n", sum, odd, min, max);

  pthread_mutex_destroy(&listMutex);
  pthread_cond_destroy(&listCond);

  return (EXIT_SUCCESS);
}

