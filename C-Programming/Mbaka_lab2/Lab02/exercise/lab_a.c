/****************************************************************************
 10/05/2019 R. Repka    Minor formatting changes
****************************************************************************/
#include <stdio.h> 
int main() 
{ 	
   short int si           = 1;
   unsigned short int usi = 2;
   unsigned int ui        = 3;
   int i                  = 4;
   long int li            = 5;
   unsigned long int uli  = 6;
   long long int lli      = 7;
   unsigned long long int ulli = 8;
   signed char sc         = (char)56;
   unsigned char uc       = (char)57;
   float f        = 11.0;
   double d       = 12.0;
   long double ld = 13.0;  
    
   printf("short int              value: %hd,  Size: %ld bytes\n",  si,  sizeof(short int)); 
   printf("unsigned short int     value: %hu,  Size: %ld bytes\n",  usi, sizeof(unsigned short int)); 
   printf("unsigned int           value: %u,  Size: %ld bytes\n",   ui, sizeof(unsigned int)); 
   printf("int                    value: %d,  Size: %ld bytes\n",   i,  sizeof(int)); 
   printf("long int               value: %ld,  Size: %ld bytes\n",  li,  sizeof(long int)); 
   printf("unsigned long int      value: %lu,  Size: %ld bytes\n", uli,  sizeof(unsigned long int)); 
   printf("long long int          value: %lld,  Size: %ld bytes\n", lli,  sizeof(long long int)); 
   printf("unsigned long long int value: %llu,  Size: %ld bytes\n", ulli, sizeof(unsigned long long int)); 
   printf("signed char            value: %c,  Size: %ld bytes\n",  sc,  sizeof(signed char)); 
   printf("unsigned char          value: %c,  Size: %ld bytes\n",  uc,  sizeof(unsigned char)); 
   printf("float                  value: %f,  Size: %ld bytes\n",   f,  sizeof(float)); 
   printf("double                 value: %lf,  Size: %ld bytes\n",   d,  sizeof(double)); 
   printf("long double            value: %Lf,  Size: %ld bytes\n",  ld,  sizeof(long double)); 	

   printf("Different data '%f'  '%5.3f'  '%9.4f'\n", 123.456789, 123.456789, 123.456789);  
   return 0; 
} 
