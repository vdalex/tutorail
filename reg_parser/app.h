#define MESSAGE_COUNT	2
#define MAX_MESSAGE_LENGHT	10

typedef struct
{
    char *message_p;
    int message_lenght; //message lenght excluding final 0x0;
    char *action_p;
} message_t;

typedef struct
{
    char message_buffer[MAX_MESSAGE_LENGHT];
    int fill_lengt;
    int buffer_position;
} alt_state_t;
