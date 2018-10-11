#include<pthread.h>
#include<stdio.h>

void *mythread1();	//thread prototype
void *mythread2();

int main(){
	pthread_t thread[4];
	//starting the thread
	pthread_create(&thread[0],NULL,mythread1,NULL);
	pthread_create(&thread[1],NULL,mythread2,NULL);
	//waiting for completion
	pthread_join(thread[0],NULL);
	pthread_join(thread[1],NULL);
		
	return 0;
}

//thread definition
void *mythread1(){
	int i;
	for(i=0;i<5000000;i++)
		//printf("Thread 1 Running\n");
}
void *mythread2(){
	int i;
	for(i=0;i<5000000;i++)
		//printf("Thread 2 Running\n");
}
