#include <stdio.h> 
#include <stdlib.h> 
//#include <unistd.h>

int main()
{

	int my_array[100];
	int i;

	srand(0); //Seed the random number generator

	//Loop over your array and fill it up!:
	for (i = 0; i < 100; i++) 
	{
	    my_array[i] = rand();
	}

	for (i = 0; i < 100; i++) 
	{
	    printf("[%d]  %d \n", i, my_array[i]);
	}

	//sort it method 1
	//take 0 array member as lowest value
	


}
