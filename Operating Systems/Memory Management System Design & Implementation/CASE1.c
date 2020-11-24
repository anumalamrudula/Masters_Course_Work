#include <stdio.h>
#include <stdlib.h>    // used for malloc and free
#include <time.h>      // used for gettime
#include <sys/types.h>

struct Process
{
  int pid;               // process's unique id
  int mem;               // memeory demand of the process
  int cyc;               // number of cycles a process spends in the system
  void *memory_pointer;  // points to the initial address of the process's address space
};

struct Process p[100];   // structured array to store process details
struct timespec start = {0,0}, end = {0,0};
static long int memory_used = 0; // tracks the total memory used
void* memory_block = NULL;  // initialize to null pointer before using the malloc
int current_process[100];
int processes_completed=0;  // to keep track of number of processes that are done with the execution

void *process(void *details) // process execution and memory release
{
  int value = *(int*)details;
  printf("*** Process %d with pid %d execution started ***\n ", value+1 ,p[value].pid);
  for(int j = 0; j < p[value].cyc/1000 ; j++)
	{
    // Do nothing
  } 
  if(p[value].memory_pointer != NULL)
  {
    free(p[value].memory_pointer);
  }
  printf("--- Process %d with pid : %d is terminated ---\n ",value,  p[value].pid);
  processes_completed = processes_completed + 1;
  return 0;
}

void start_process(int *value) // creates a thread to represent process
{
pthread_t threadid;												
if (pthread_create(&threadid, NULL, process,(void*)value) < 0)
    printf("Error Occured\n");
}

int main(void) 
{
  int triggerProcess_cycle = 0;
  int process_count;            // total number of processes to be generated
  int id = 1000;                // initialize random pid
  int memory = 0;               // contains total memory requested
  int cycles = 0;               // contains total number of process cycles
  double Total_time;            // total time for execution
  memory_block = malloc(1024*1024*10);
  printf("Enter the number of processes to be generated \n");
  scanf("%d",&process_count);
  for(int i=0; i<process_count; i++)
  {
    printf("\n Process Number : %d", i+1);
    p[i].pid = id + 1;
    printf("\n\t Process pid : %d", p[i].pid);
    p[i].mem = (rand()%369)+10;  // randomly allocate memory required
    printf("\n\t Memory in KB : %d", p[i].mem);
    p[i].cyc = (rand()%1824)+20; // randomly allocate number of cycles
    printf("\n\t No. of cycles : %d", p[i].cyc);
    memory = memory + p[i].mem;
    cycles = cycles + p[i].cyc;
    id++;
  }
  printf("\nTotal memory required (in KB) is = %d\n", memory);
  clock_gettime(CLOCK_MONOTONIC, &start);  // get the start time

  for(int i=0; i<process_count; i++)
  {
    if((10485760-memory_used) > p[i].mem * 1024)
    {
      printf("Memory is allocated to process : %d\n", i+1);
      p[i].memory_pointer = malloc(p[i].mem * 1024);
    }
    else
    {
      printf("No enough memory to allocate to process %d\n", i+1);
    }
    current_process[i] = i;
    for(triggerProcess_cycle = 0; triggerProcess_cycle < 0.05; triggerProcess_cycle++)
    {
      // Do nothing
    }
    triggerProcess_cycle = 0;
    start_process(&current_process[i]);
  }while(processes_completed != process_count);

  clock_gettime(CLOCK_MONOTONIC, &end);  // get the end time
  Total_time = ((double)end.tv_sec + 1.0e-9*end.tv_nsec) - ((double)start.tv_sec + 1.0e-9*start.tv_nsec);
  printf("Total time for excution : %.8f seconds\n", Total_time);

  pthread_exit();  // destroy all the threads

  return 0;
}