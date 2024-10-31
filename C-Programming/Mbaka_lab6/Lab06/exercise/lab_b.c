/*-----------------------------------------------------------------------
  Demonstrates the power of the ## concatenate command and the # string
  conversion commands  Also demonstrates problems with macros
-----------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MACRO1(N)                             \
    { int i ## N;                             \
      printf("Looping variable 'i%s'\n", #N); \
      for(i ## N=0; i ## N < N; i ## N++) {   \
      printf("%d\n",i ## N);                  \
    } /* End for */ } /* End macro */

#define f1()                     \
  {int _var = 10;                 \
  printf("var in f1: %d\n", _var);}

#define MUL_FIVE(mul) (mul * 5)

#define ABS(x) ((x) < 0 ? -(x) : (x))

#define MIN(a, b) ((a) < (b) ? (a) : (b))


int charCount(char *str);
void f2();
    
#if TEST==1
int main(int argc, char *argv[]) {
   int i = 2;
   /* The looping variable will be _t_3  */
   MACRO1(3);
   MACRO1(3);
   MACRO1(i);
   return(0);
}

#elif TEST==2
int main(int argc, char *argv[]) {
   f1();
   return(0);
}


#elif TEST==3
int main(int argc, char *argv[]) {
   int n = 5;
   int m = MUL_FIVE(n + 5);
   printf("MUL_FIVE: %d\n", m);
}

#elif TEST==4
int main(int argc, char *argv[]) {
   int x, result; 
   
   // Use the real absolute value function
   x = -5;
   result = abs(x++);
   printf("Function result: %d\n", result);
   printf("X IS: %d\n", x);
   
   // Use the Macro absolute value function
   x = -5;
   result = ABS(x++);
   printf("Macro result: %d\n", result);
   printf("X IS: %d\n", x);

   return(0);
}

#elif TEST==5
int main(int argc, char *argv[]) {
   char *str1 = "Hi", *str2 = "Hello";
   int minCharCount = MIN(charCount(str1), charCount(str2));
   printf("Minimum char count of(%s, %s): %d\n", str1, str2, minCharCount);
   return(0);
}

#else
int main(int argc, char *argv[]) {
   printf("Compile with the -DTEST=<num> macro\n");
}


#endif


void f2() {
   int var1 = 5;
   printf("var1 in f2: %d\n", var1);
}

/* counts # of chars in a given string*/
int charCount(char *str) {
   return(strlen(str));
}