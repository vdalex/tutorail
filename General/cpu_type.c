#include <stdio.h>
#include <string.h>
#include <ctype.h>

 
#define CPUINFO_BUFFER_SIZE  (1024*4)
#define NE10_ERR1 2
#define NE10_ERR2 4
#define NE10_ERR3 8
 
 
int main (void)
{
	FILE* infofile = NULL;               // To open the file /proc/cpuinfo
	char    cpuinfo[CPUINFO_BUFFER_SIZE];  // The buffer to read in the string
	unsigned  bytes = 0;                     // Numbers of bytes read from the file
	int   i = 0;                         // Temporary loop counter
 
	memset (cpuinfo, 0, CPUINFO_BUFFER_SIZE);
	infofile = fopen ("/proc/cpuinfo", "r");

	if (!infofile)
	{
		printf("ERROR: couldn't read file \"/proc/cpuinfo\".\n");
		return NE10_ERR1;
	}
  
	bytes    = fread (cpuinfo, 1, sizeof (cpuinfo), infofile);
	fclose (infofile);
  
	if (0 == bytes || CPUINFO_BUFFER_SIZE == bytes)
	{
		printf ("ERROR: Couldn't read the file \"/proc/cpuinfo\". NE10_init() failed.\n");
		return NE10_ERR2;
	}
  
	while ('\0' != cpuinfo[i])
	{
		cpuinfo[i] = (char) tolower (cpuinfo[i]);
		++i;
	}
  
	printf ("%s", cpuinfo);
//	if (strstr ((const char *)cpuinfo, "neon") != NULL ||
//	strstr ((const char *)cpuinfo, "asimd") != NULL)
//	{
//		is_NEON_available = NE10_OK;
//	}
	return 0;
}
