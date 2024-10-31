/*  *********************************************************************
  simulation.c - student file

  06/23/2020 - created HW14
  ********************************************************************* */
#include <stdio.h>
#include <stdlib.h>
#include "ODEsolvers.h"

#define TANK1  0
#define TANK2  1
#define POSITION 0
#define VELOCITY 1               
/****************************************************************************
Differential Equations of a dual tank filler.  Use with
     the simulator HW problem

Where: simParm *sim - simulation constants
    double t   - current time
    double *x  - current state
      x[0] - tank 1 level
      x[1] - tank 2 level

   double *dx - derivative of states, results are returned here

Returns: nothing
Error:   none
****************************************************************************/
void tank(simParm *sim, double t, double *x, double *dx) {
   // change in level of the top tank. (inflow - outflow)/area
   // In flow of 0.5, out flow based on 0.13*sqrt(height)
   // Height growth is the overall flow difference / area of the tank
   dx[TANK1] = (0.5 - 0.13 * sqrt(x[TANK1])) / 2.0;
   // change in level of the 2nd tank. (inflow - outflow)/area
   // In flow is out flow from the previous tank
   // Height growth is the overall flow difference / area of the tank
   dx[TANK2] = (0.13 * sqrt(x[TANK1]) - .20 * sqrt(x[TANK2])) / 2.0;
} // end tank


 
/**********************************************************************
Differential Equations of a spring and mass with dampening.  Use with
     the simulator HW problem

Where: simParm *sim - simulation constants
    double t   - current time
   double *x  - current state
      x[0] - position
      x[1] - velocity

   double *dx - derivative of states, results are returned here

Returns: nothing
Error:   none
**********************************************************************/
void springMass(simParm *sim, double t, double *x, double *dx) {
   // 
   // Given: f = ma & f = -ky  ->  -ky = ma -> a = -(k/m)*y   
   // Given: a = dv/dt  -> dv/dt = -(k/m)*y
   //
   // Assume: x[0] - position, x[1] - velocity
   // k = 128,  m = 2, damp = 0.2   use: sim->damp
   //
   //             dampening*velocity    spring*position     
   //   dx[VELOCITY] = ;   // change velocity = acceleration (* time in integration)
   //   dx[POSITION] = ;  // change in position = velocity (* time in integration)

   return;
} // end springMass
