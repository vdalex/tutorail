//A simple C program to demonstrate use of pthread basic functions
//Please note that the below program may compile only with C compilers with
//pthread   library (gcc m_thread_1.c -lpthread).

#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h>
#include <pthread.h> 
  
// A normal C function that is executed as a thread  
// when its name is specified in pthread_create() 
void *myThreadFun(void *vargp) 
{ 
    sleep(1); 
    printf("Printing GeeksQuiz from Thread \n"); 
    return NULL; 
} 
   
int main() 
{ 
    pthread_t thread_id; 
    printf("Before Thread\n"); 
    pthread_create(&thread_id, NULL, myThreadFun, NULL); 


	//The first argument is a pointer to thread_id which is set by this function.
	//The second argument specifies attributes. If the value is NULL, then default
	//attributes shall be used.
	//The third argument is name of function to be executed for the thread to be created.
	//The fourth argument is used to pass arguments to thread.

    pthread_join(thread_id, NULL); 
	//The pthread_join() function for threads is the equivalent of wait() for processes. A call to
	//pthread_join blocks the calling thread until the thread with identifier equal to the first
	//argument terminates. 
    
	printf("After Thread\n"); 
    exit(0); 
}

