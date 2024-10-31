/*---------------------------------------------------------------------------
  05/08/2017    R. Repka    Make derivative clearer
---------------------------------------------------------------------------*/
#include <math.h>

/*---------------------------------------------------------------------------
  Student framework equations to be solved to help with the root finder 
---------------------------------------------------------------------------*/
#include "rootfinding.h"

/*---------------------------------------------------------------------------
  This function implements the equation to be solved.  
  
  Where: double x - the value to evaluate
  Returns: double - the value of the function at the point
  Errors:  none
---------------------------------------------------------------------------*/
double func1(double x){
  return 0.76 * x * sin(30.0 * x / 52.0) * tan(10.0 * x / 47.0) + 2.9 * cos(x + 2.5) * sin(0.39 * (1.5 + x));

}
/*---------------------------------------------------------------------------
  This function implements the first derivative equation, which is calculated
  via any off-line process you wish (e.g  Wolfram Alpha)
  There is NO requirement to programmatically generate the derivative.
  
  Where: double x - the value to evaluate
  Returns: double - the value of the function at the point
  Errors:  none
---------------------------------------------------------------------------*/
double func1Deriv(double x){
  return 0.76 * sin(30.0 * x / 52.0) * tan(10.0 * x / 47.0) + 
           (0.76 * 30.0 / 52.0) * x * cos(30.0 * x / 52.0) * tan(10.0 * x / 47.0) + 
           (0.76 * 10.0 / 47.0) * x * sin(30.0 * x / 52.0) * (1.0 / (cos(10.0 * x / 47.0) * cos(10.0 * x / 47.0))) - 
           2.9 * sin(x + 2.5) * sin(0.39 * (x + 1.5)) + 
           2.9 * 0.39 * cos(0.39 * (x + 1.5)) * cos(x + 2.5);
}
