/*
Parser, for finding matches in input stream
Parser wait for minimum symbol lenght, and start comparision

*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "app.h"

int main ()
{
    char input_stream[] = "$%^&*JKLM\r\nCLOSED\r\n\r\n\ERROR\r\nACIVATED++++---1234567-----";
    char input_buffer[sizeof(input_stream)];
    int i;
    char message1[] = "CLOSED";
    char message2[] = "ERROR";

    char action1[] = "Closed all session for maintainance";
    char action2[] = "Session Error";

    message_t message_db[] =
    {
        {message1, sizeof(message1) - 1, action1},
        {message2, sizeof(message2) - 1, action2}
    };

    alt_state_t alternative;


    alternative.fill_lengt = 4;


    /*Print message database*/
    for (i = 0; i < MESSAGE_COUNT; i++)
    {
        printf("\nMessage %d is: %s, message lenght is: %d, action is: %s", i, (message_db[i].message_p), (message_db[i].message_lenght), (message_db[i].action_p));
    }

    printf("\n");

}
