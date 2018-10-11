#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structtype.h"

int main() {

//read user input
//allocate memory for array of structures
//print memory location of array
//print location of each individual structure
//fill structures from txt file, use whitespace as name name delimiter

//can array in structure be allocated dinamically?
//can structure be rebuilded dynamically?

	unsigned count;
	unsigned position;
	unsigned i;
	
	int scan_result;
	char buffer[255];	
	char c;
	char c_new;
	char c_old;


	FILE* fp = NULL;

	data_struct_t *database;
	data_struct1_t *database1;
	//data_struct_t database[];
	char *cp; 

	printf("Enter size of database: ");
	scan_result = scanf("%4d", &count);

	database = malloc(count*(sizeof(data_struct_t)));
	database1 = malloc(count*(sizeof(data_struct1_t)));

	printf("\nScan tesult: %d \n", scan_result);
	printf("Database records qty: %d \n", count);
	printf("Database memory address: %p \n", database);

	scan_result = sizeof(data_struct_t);
	printf("Storage size for data_struct_t : %d \n", scan_result);

	scan_result = sizeof(data_struct1_t);
	printf("Storage size for data_struct1_t : %d \n", scan_result);
	


	fp = fopen("file.txt","r");
	if(fp == NULL) 
	{
		perror("Error in opening file");
		return(-1);
	}

	c_new = 1;
	c_old = 1;
	position = 0;
	i = 0;

//Parsing txt fail for words and adding words to database fields
	do
	{
		c = (char)fgetc(fp);
		if(feof(fp)) 
		{
			break ;
		}

		if ((c == ' ') || (c == '!') || (c == '.') || (c == ',') || (c == '?') || (c == '\n') || (c == ':') || (c == ';') || (c == '-'))
		{
			c_new = 0; //non printed symbol detected
		}
		else
		{
			c_new = 1;
		}

		if ((c_new == 1) && (c_old == 0))			
		{
			if((position == 0) && (i == 0))
			{
				printf("\n%d: %c", position, c); //add new database record	
				buffer[i] = c;			
				database[position].name[i++] = c;
					
			}
			else
			{
				buffer[i] = '\0';				
				database[position].name[i++] = '\0'; //add string terminator
				database[position].amount = i;	
							
				database1[position].pname = malloc(i);
				memcpy(database1[position].pname, buffer, i);

				if (position < (count-1)) 
				{
					i = 0;				
					position++;
					printf("\n%d: %c", position, c); //add new database record				
					buffer[i] = c;					
					database[position].name[i++] = c;				
				}
				else
				{
					break;
				}
			}			
		}		
		
		if ((c_new == 1) && (c_old == 1))
		{
			printf("%c", c);
			buffer[i] = c;
			database[position].name[i++] = c;
						
		}
		c_old = c_new;
		
	} 
	while(1);

	fclose(fp);
	printf("\n\nDatabase filling complete!");
	printf("\nDatabase out:\n");

	for (i = 0; i < count; i++)
	{	
		printf("\n%d: %s", i, database[i].name);
		cp = &database1[i].pname;
		printf("\n%d: %s %p %p", i, database1[i].pname, database1[i].pname, cp);
	}

	
	printf("\n");
	free(database);

	return 0;
}
