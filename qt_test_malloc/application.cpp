#include <stdio.h> /* printf, scanf, NULL */
#include <stdlib.h> /* malloc, free, rand */
#include <string.h>
#include "structtype.h"
#include "application.h"

int app() {

//read user input
//allocate memory for array of structures
//print memory location of array
//print location of each individual structure
//fill structures from txt file, use whitespace as name name delimiter

//can array in structure be allocated dinamically?
//can structure be rebuilded dynamically?
//reallocate database1[position].name increazing size by 2;

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


    /*Init struct with different function*/
    func_struct_t st_mult;

    st_mult.a = 5;
    st_mult.b = 6;
    st_mult.pFunc = &f_mult;

    st_mult.pFunc(st_mult.a, st_mult.b);


    st_mult.pFunc = &f_add;

    st_mult.pFunc(st_mult.a, st_mult.b);


    st_mult.pFunc = &f_diff;

    st_mult.pFunc(st_mult.a, st_mult.b);



    setbuf(stdout, NULL);
//    setbuf(stdin, NULL);

    printf("\nEnter size of database: ");
    scan_result = scanf("%4d", &count);

    database = (data_struct_t*) malloc((count)*(sizeof(data_struct_t)));
    database1 = (data_struct1_t*) malloc((count)*(sizeof(data_struct1_t)));

    printf("\nScan tesult: %d \n", scan_result);
    printf("Database records qty: %d \n", count);
    printf("Database memory address: %p \n", database);
    printf("Database1 memory address: %p \n", database1);

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
                database1[position].amount = i;
                database1[position].pname = (char*) malloc(i);

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
//        printf("\n%d: %s", i, database[i].name);
        cp = (char*) &(database1[i].pname);
        //&cp - address cp variable in memory

        printf("\n%d: %s %p %p", i, database1[i].pname, database1[i].pname, cp);
        database1[i].amount = (database1[i].amount)*2;
        database1[i].pname = (char*)realloc(database1[i].pname, database1[i].amount);
        printf("\n%d: %s %p %p", i, database1[i].pname, database1[i].pname, cp);
        toBitstream(database1[i].pname);
    }

    printf("\n");

    printf("{");
    printf("\"Database\":");
    printf("[");

    //out database as json file
    for (i = 0; i < count-1; i++)
    {
        printf("{");
        printf("\"lenght\":%d,\"word\":\"%s\"",database1[i].amount, database1[i].pname);
        printf("},");

    }

    printf("{");
    printf("\"lenght\":%d,\"word\":\"%s\"",database1[i].amount, database1[i].pname);
    printf("}");

    printf("]");
    printf("}");

    for (i = 0; i < count; i++)
    {
        cp = database1[i].pname;
        free(cp);
    }

    printf("\n");
    free(database);
    free(database1);


//    {"menu": {
//      "id": "file",
//      "value": "File",
//      "popup": {
//        "menuitem": [
//          {"value": "New", "onclick": "CreateNewDoc()"},
//          {"value": "Open", "onclick": "OpenDoc()"},
//          {"value": "Close", "onclick": "CloseDoc()"}
//        ]
//      }
//    }}


    return 0;
}


int f_mult(int a, int b)
{
    printf("\nMultiply result: %d", a*b);
    return a*b;
}

int f_add(int a, int b)
{
    printf("\nAddition result: %d", a+b);
    return a+b;
}

int f_diff(int a, int b)
{
    printf("\nSubstract result: %d", a-b);
    return a-b;
}

void toBitstream(char *pBuff)
{
    char shift = 0;
    char val = 0;

    printf(" ");

    do
    {
        val = *pBuff;
        shift = 8;
        do
        {
            shift--;
            if ((val&(1<<shift)) == 0) printf("01");
            else printf("10");

        }
        while (shift != 0);

        pBuff++;
        printf(" "); //space between letters

    }
    while (*pBuff != '\0');
}




