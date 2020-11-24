#include<stdio.h>
#include<sys/types.h>	
#include<stdlib.h>		// used for malloc and free
#include<time.h>  		// used for gettime

struct Process  {
        int pid;		// process's unique id
        int mem;	  // memeory demand of the process	
        int cyc;	  // number of cycles a process spends in the system
        int flag;	  // for flagging the non-vacant memory locations
        void *memory_pointer;	// points to the initial address of the process's address space
};

struct freepool  // to implement Best-Fit and to hold free memory
{
  int block_size;
  void *add_pointer;
};

struct Process p[100];   // structured array to store process details
struct freepool fp[100]; 
struct timespec start = {0,0}, end = {0,0};
static long int memory_used = 0; // tracks the total memory used
void* memory_block = NULL;  // initialize to null pointer before using the malloc
int current_process[100];
int processes_completed=0;  // to keep track of number of processes that are done with the execution
static int a = 0;				//free pool array element variable	
static int ap = 0;


void *my_malloc(size_t size, int flag)
{		
  int *address = NULL;								
  if (flag == -1)
  {									
      address =  memory_block + memory_used;				
      memory_used = memory_used + size;							
      printf("Memory used : %ld\n",memory_used/1024);
  }
  else 
  {
      address = fp[flag].add_pointer;					
      memory_used = memory_used + size;
  }
  return address;
}

void my_free(void *addr, size_t size)
{
memory_used = memory_used - size;
ap += 1;
fp[a].add_pointer = addr;
fp[a].block_size = size;
printf("Freeing %zu KB of memory\n",size);
}

void *process (void *details)
{
	int value  = *(int*)details;
	printf("*** Process %d with pid %d execution started ***\n ", value+1 ,p[value].pid);
	for(int j=0; j<p[value].cyc/1000 ; j++)
	{} 
	if (p[value].memory_pointer != NULL)
  {
	  my_free(p[value].memory_pointer, p[value].mem * 1024);
  }	
	p[value].flag = 1;
	printf("--- Process %d with pid : %d is terminated ---\n ",value+1,  p[value].pid);
	processes_completed += 1;
	return 0;
}

void start_process(int *value)  // creates a thread to represent process
{
pthread_t tid;													
if (pthread_create(&tid, NULL, process,(void*)value) < 0)	
    printf("Error Occured\n");
}

/***main function ****/
int main() 
{
int triggerProcess_cycle=0;		// variable for 50 cycles delay
int process_count;              // total number of processes to be generated
int id = 1000;		         	// initialize random pid
int memory = 0;                 // contains total memory requested
int cycles = 0;                 // contains total number of process cycles
double Total_time;              // total time for execution

// allocating the main memory as 1MB (case 3b)
int total_memory = 1024*1024*1;
memory_block = malloc(total_memory);
printf("Enter the number of processes to be generated \n");
 scanf("%d",&process_count);
for (int i=0; i<process_count; i++) 
{
  printf("\n Process Number : %d", i+1);
  p[i].pid = id + 1;
  printf("\n\t Process pid : %d", p[i].pid);
  p[i].mem = (rand()%369)+10;  // randomly allocate memory required
  printf("\n\t Memory in KB : %d", p[i].mem);
  p[i].cyc = (rand()%3124)+325; // randomly allocate number of cycles
  printf("\n\t No. of cycles : %d", p[i].cyc);
  memory = memory + p[i].mem;
  cycles = cycles + p[i].cyc;
  id++;
}
  printf("\nTotal memory required (in KB) is = %d\n", memory);
  clock_gettime(CLOCK_MONOTONIC, &start);  // get the start time

for (int i=0; i<process_count; i++)
{
	if ((total_memory-memory_used) < p[i].mem * 1024)
	{
		do
		{
			printf("----------WAITING for memory to be released for process %d\n",i+1);
		}while ((total_memory-memory_used) < p[i].mem * 1024);  
      
      int bestIdx = -1;
      for(int k=1; k<ap; k++)
      {
        if (fp[k].block_size >= p[i].mem) 
        { 
                if (bestIdx == -1) 
                    bestIdx = k; 
                else if (fp[bestIdx].block_size > fp[k].block_size) 
                    bestIdx = k; 
        } 
      }
      if(bestIdx == -1)
      {
        // wait
      }
      else
      {
       for(int j=bestIdx; j<ap; j++)
       {
         fp[j].block_size = fp[j+1].block_size;
         fp[j].add_pointer = fp[j+1].add_pointer;
       }
      }         
      printf("bestIdx %d\n", bestIdx);
		p[i].memory_pointer = my_malloc(p[i].mem * 1024, bestIdx);  // Allocating memory from free pool
	} 
	else 
	{
    printf("Memory is allocated to process : %d\n", i+1);
		p[i].memory_pointer = my_malloc(p[i].mem * 1024, -1); // Allocating memory from Mainmemory
	}
	p[i].flag = 0;
	current_process[i] = i;
	for(triggerProcess_cycle = 0; triggerProcess_cycle < 0.05; triggerProcess_cycle++)
  {
    // Do nothing
  }
  triggerProcess_cycle = 0;
  start_process(&current_process[i]);
	}
	while(processes_completed != process_count);

clock_gettime(CLOCK_MONOTONIC, &end);
  Total_time = ((double)end.tv_sec + 1.0e-9*end.tv_nsec) - ((double)start.tv_sec + 1.0e-9*start.tv_nsec);
  printf("Total time for excution : %.8f seconds\n", Total_time);
pthread_exit();  // destroy all the threads
return 0;
}