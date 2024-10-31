/*  ===========================================================================
      ODEsolvers.h - Professor header file for ODEsolvers module 
      Version: 1.02
      Juan C. Cockburn, Nov. 2012
      R. Repka      11/11/2015 - Improved prototypes
      R. Repka      12/13/2015 - Added disturbance time an value, restructured
      R. Repka      11/20/2017 - Added simple tank simulation
      R. Repka      06/23/2020 - Added spring and pendulum simulations
    =========================================================================== */
#ifndef _ODESOLVERS_H_
#define _ODESOLVERS_H_

#include <stdlib.h>
#include <math.h>

#define  NX (2)     /* Number of simulator state variables */

typedef struct {
    int neq;      /* Number of simulation equations               */
    double h;     /* time step                                    */
    double damp;  /* dampening value, if used                     */
} simParm; 

/*  ===========================================================================
    These are function pointer prototypes.  The function pointed to will contain:
    
    A function which defines the right-hand side of a  system of differential equations
    in state-space form.  This function must be of the form
    
 =========================================================================== */
typedef void (*funArgs) (simParm *sim, double t, double *x, double *dx);

/* Other function prototypes */
void eu(simParm *sim,  double t0, double *x0, funArgs f);
void rk2(simParm *sim, double t0, double *x0, funArgs f);
void rk3(simParm *sim, double t0, double *x0, funArgs f);
void rk4(simParm *sim, double t0, double *x0, funArgs f);
void tank(simParm *sim, double t, double *x, double *dx);
void springMass(simParm *sim, double t, double *x, double *dx);

#endif /* _ODESOLVERS_H_ */