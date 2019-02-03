/*structtype.h - all struct type definitions*/

typedef struct {
	char name[255];
    int amount;
	int value;

} data_struct_t;

typedef struct {
	char *pname;
    int amount;
	int value;

} data_struct1_t;


/*For test with pointers to function*/
typedef struct {
    int a;
    int b;
    int c;
    int (*pFunc)(int a, int b);

} func_struct_t;

int f_mult(int a, int b);
int f_add(int a, int b);
int f_diff(int a, int b);

/*For test classes*/

void toBitstream(char *pBuff);
