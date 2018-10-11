/*
/*
/*	union {
/*    float    fl;
/*    uint32_t dw;
/*	} f;
/*	int s = ( f.dw >> 31 ) ? -1 : 1;   /* Знак */
/*	int e = ( f.dw >> 23 ) & 0xFF;     /* Порядок */
/*	int m =                            /* Мантисса */
/*  e ?
/*    ( f.dw & 0x7FFFFF ) | 0x800000 :
/*    ( f.dw & 0x7FFFFF ) << 1;
/*
/*

*/

#include <stdio.h>
#include <string.h>
 
union Data {
   int i;
   float f;
   unsigned char str[4];
};
 
int main( ) {

	union Data data;        

//	data.i = 10;
//	printf( "data.i : %d\n", data.i);
  
	data.f = 220.5;
	printf( "data.f : %f\n", data.f);

	printf("float as hex: 0x");
   
	for (char i = 4; i >0 ; )
	{   
		i--;
		printf("%02x", data.str[i]);
	}

	return 0;
}
