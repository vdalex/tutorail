#include <stdio.h> 
#include <stdlib.h> 
//#include <unistd.h>


void str_cpy (char * p_src, char * p_dst);

int main()
{

	char src_str[] = "Hello World!!!";
	char dst_str[100];

	
	str_cpy (src_str, dst_str);	
	printf("%s", dst_str);

}

void str_cpy (char * p_src, char * p_dst)
{
	do
	{
		*p_dst++ = *p_src++;

	}
	while (*p_src != 0x00);	
	
}
