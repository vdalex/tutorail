/*Experimental DSP engine for simple DCR SDR radio */
/*Created 2018-06-27 21:36:00 */
/*by Oleksandr Vdovenko */

#include <stdio.h>

#define HILBLENGHT 7
#define LPFLENGHT 11 //odd value only!!!
#define HALF_LPFLENGHT LPFLENGHT/2

int hilbert_engine (int in_sample); //receive I and Q data
int lpf_engine (int in_sample);
 
//void agc_engine (void);
//void signal_condition_engine (void);
//void phase_correction_engine (void);

//int filter[HILBLENGHT];


int main ()
{
	char i;	

	printf("Hilbert Engine test: \n");	
	
	for (i=0; i<HILBLENGHT+16; i++)
	{
		hilbert_engine(i);
	}

		
	printf("\nLPF Engine test: \n");

	for (i=0; i<LPFLENGHT+16; i++)
	{
		lpf_engine(i);
	}

	return 0;
	
}

int hilbert_engine (int in_sample)
{
	static int buffer[2*HILBLENGHT];
	static char ntap = HILBLENGHT;
	static char fir_head = 0;
	int out_sample = 0;
	char j;
	
	fir_head = (fir_head == 0) ? (ntap - 1) : (fir_head - 1);
	buffer[fir_head] = buffer [fir_head + ntap] = in_sample; //put sample in buffer
		
	for (j = 0; j < HILBLENGHT; j++)
	{
		//out_sample + = buffer [fir_head + j]*filter[j]; 
		printf("%d\t ", buffer [fir_head + j]);

	}

	printf("\n");
	return out_sample;
}


/*LPF simmetrical FIR engine */
/*It adds 2 sample and multiply by filter coefficient`*/


int lpf_engine (int in_sample)
{
	static int buffer[2*LPFLENGHT];
	static char ntap = LPFLENGHT;
	static char fir_head = 0;
	int out_sample = 0;
	char j;
	char n;
	char b_head;
	char b_tail;
	
	fir_head = (fir_head == 0) ? (ntap - 1) : (fir_head - 1);
	buffer[fir_head] = buffer [fir_head + ntap] = in_sample; //put sample in buffer

	n = HALF_LPFLENGHT;
	b_head = fir_head + HALF_LPFLENGHT;
	b_tail = b_head;
	
	for (j = 0; j < LPFLENGHT; j++)
	{
		//out_sample + = buffer [fir_head + j]*filter[j]; 
		if (j == HALF_LPFLENGHT) 
		{
			printf("<%d>\t ", buffer [fir_head + j]);
		}
		else
		{		
			printf("%d\t ", buffer [fir_head + j]);
		}
	}
	
		//kv = k [HALF_LPFLENGHT];
		//v += kv*x [b_head];
	printf("\n%d ", buffer [b_head]);//middle tap	

	do
	{	
		//kv = k [-- n];
		//v += kv * (x [-- b_head] + x [++ b_tail]);
		-- n;
		printf("%d+%d ", buffer [-- b_head], buffer [++ b_tail]);

	}
	while (n != 0);	
	
	printf("\n");	

	return out_sample;
}

int phase_correction_engine (void)
{
	return 0;	
}

