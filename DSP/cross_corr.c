/*
Finding position in sequence 
*/

#include <stdio.h>
#include <stdlib.h> 
#include <time.h> 

#define REFSEQLEN	512
#define INSEQLEN	10*REFSEQLEN
#define POS			1000

void main() 
{
	int in_seq[INSEQLEN];
	int ref_seq[REFSEQLEN];
	long int conv_seq[INSEQLEN];
	int i = 0;
	int pos = 0;
	long int acc = 0;
	

	srand(time(0)); 
	printf("Size of long int = %d \n", (int) sizeof(long int));

/* Fill input sequence with random number */	
	for (i = 0; i < INSEQLEN; i++)
	{ 
        in_seq [i] = rand(); 
        //printf("%d \n", in_seq [i]); 
    }  

/* Fill reference sequence */
	for (i = 0; i < REFSEQLEN; i++)
	{ 
        ref_seq [i] = in_seq [i+POS] = i*10; 
        printf("%d \n", ref_seq [i]); 
    }  

/* Apply some attenuation to input sequence */

/* Run convolution algorithm for finding start position of ref_seq in in_seq */
	
	for (i = 0; i < INSEQLEN-REFSEQLEN; i++)
	{ 
		acc = 0;
		for (pos = 0; pos < REFSEQLEN; pos++)
		{
			acc += ref_seq [pos]*in_seq[i+pos]; 
        }
		conv_seq[i] = acc;
		//printf("[%d] %li \n", i, conv_seq [i]); 
		 
	}  

}
