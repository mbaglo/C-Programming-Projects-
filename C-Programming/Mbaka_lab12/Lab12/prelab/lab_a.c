/*--------------------------------------------------------------------------
  Simple scanf test program
  student copy
--------------------------------------------------------------------------*/
#include <stdio.h>                    /* ex2a_scan.c */
#include <stdlib.h>                   /* for exit() function */

const double mi2km = 1.609;
double convert(double mi) {return (mi * mi2km);}
int main() {
   double miles; 
   printf("Miles to Km Conversion (enter negative number to end)\n");
   do {
      printf("Input distance in miles: ");
      // Your code here
    /*  scanf("%lf", &miles);  // Not checking the return value of scanf

        if (miles <= 0) {
            break; // Exit loop if input is zero or negative
        }
            printf("\n%f miles = %f km\n", miles, convert(miles)); */
       
       // Use scanf to read double, add error checking for non-numeric input
        if (scanf("%lf", &miles) != 1) {
            // If scanf cannot parse input, print error message and exit with error code
            printf("Error, nothing parsed\n");
            exit(EXIT_FAILURE);
        }

        if (miles > 0) {
            printf("\n%f miles = %f km\n", miles, convert(miles));
        }
        
   } while (miles > 0.0);
   return 0;
}