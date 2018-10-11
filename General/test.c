#include <stdio.h>

int main() {
   /* my first program in C */
   /*trying to reverse byte char order in text */
   char text[]="Optimistic Expectations1";
   char i, n, m;
   char *p;
   
   n=sizeof(text);
   n--;
   n--;
   
   printf("Storage size for array : %d \n", n);
   printf("Input array : %s \n", text);
   
   
   for (i=0; i<=n/2; i++)
   {
      m=text[i];
      text[i]=text[n-i];
      text[n-i]=m;
      printf("Output array : %s \n", text);
   }
   
   printf("Output array : %s \n", text);
   
   
   p=&text[0];
   printf("Pointer to some element : %p \n", p);
   p=&text[4];
   printf("Pointer to some element : %p \n", p);
   printf("Pointer to some element : %c \n", *p);
   printf("Pointer to some element : %c \n", *(p+1));
   
   return 0;
}
