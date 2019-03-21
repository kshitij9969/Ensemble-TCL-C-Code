#include <stdio.h>
#include <math.h>
/*Peripherals will come here.*/

int N = 2;			                                    /*Number of TCLs. */
float W = 1.73;			                                /*Natural frequency. */
float Wfft = 0.2980;		                            /*Wfft computed from the signals received. */
float alpha[2];			                                /*Delay */
float duty_cycle = 0.5;		                          /*Duty cycle. */
float coupling = 0.267;		                          /*coupling factor. */
float x0[2];			                                  /*Initital conditions. */
float k1, k2, k3, k4;		                            /*Runge-kutta paramters. */
float p = 14000;		                                /*Power of individual TCL. */
float R = 0.002;	                                	/*Thermal Resistance. */
float C = 10000;		                                /*Thermal Capacitance. */
float T = 3;			                                  /*Time period. */
float Ta = 32;			                                /*Ambient temperature. */
float h = 0.1;			                                /*Time width */
float t_final = 100;	                            	/*Time duration. */
float tspan[1000];		                              /*Time vector. */
float switch_signal[2][(1000)];	                    /*Vector to store switching functions. */
float pi = 3.1415;		                              /*Defining the constant pi. */
float s0 = 0.7316;		                              /*Bais */
unsigned int i;			                                /*Iteration paramters. */
unsigned int j;                                     
float k = 0.267;
int m = 1;
float phi_heavi[2][1000];                           /*for storing values of phi.*/


void setvalues ()                                   /*Run this once at the begining to set time and alpha.*//*tested with correct output*/
{
  
  for (i = 0; i < (1000); i++)                      /*Time */
    {
      float a = 0;
      tspan[i] = a;
      a = a + h;
    }


  for (i = 0; i < (N - 1); i++)                     /*Alpha*/
    {
      alpha[i] = (2 * pi * i * m) / N;
    }

}

int heaviside (float x)		                        /*Heaviside function. *//*Tested with correct output... */
{
  if (x > 0)
    {
      return 1;
    }
  else
    {
      return 0;
    }
}

int summation (float x[], int i, int N)	             /*Summation function *//*Tested with correct output... */
{
  int store = 0;
  for (int l = 0; l < N; l++)
    {
      if (l != i)
	{
	  store = store + heaviside (sin ((double) x[l]));
	}
    }
  return store;
}

float Phidot (float x, float arr[], float alpha[], float W, float N, int i)	/*Phidot equation. */
{
  float phi_dot;
  int sum = summation (arr, i, N);
  phi_dot = W + k * (sum - (N - 1) * heaviside (sin ((double) x) + alpha[i]));
  return phi_dot;
}


int main ()
{
  setvalues ();
  float phi_heavi_present;
  float phi_heavi_past;
  phi_heavi[0][0] = 0.3865;	                                                /*Initial conditions. */
  phi_heavi[1][0] = 0.7203;
  N = 2;
  W = 1.73;

  float arr;
  float arr1[N];
  for (int u = 1; u <= 10; u++)
    {
      for (int n_tcl = 0; n_tcl < N; n_tcl++)
	{
	  for (int e = 0; e < N; e++)
	    {
	      arr1[e] = phi_heavi[e][u - 1];
	    }
	  arr = phi_heavi[n_tcl][u - 1];


	  k1 = h * Phidot (arr, arr1, alpha, W, N, n_tcl);
	  k2 = h * Phidot ((arr + k1 / 2), arr1, alpha, W, N, n_tcl);
	  k3 = h * Phidot ((arr + k2 / 2), arr1, alpha, W, N, n_tcl);
	  k4 = h * Phidot ((arr + k3), arr1, alpha, W, N, n_tcl);
	  phi_heavi_past = phi_heavi[n_tcl][u - 1];
	  phi_heavi_present      =     phi_heavi_past    +     ((k1 + 2 * k2 + 2 * k3 + k4) / 6);
	  phi_heavi[n_tcl][u]    =     phi_heavi_present;

	  phi_heavi_past         =     phi_heavi[n_tcl][u];
	}

    }
    
    /*------------------------------ONLY FOR DISPLAYING VALUES OF PHIs.------------------------------------------*/
  for (int f = 0; f < 10; f++)
    {
      printf ("%f ", phi_heavi[0][f]);
    }
  printf ("\n");
  for (int f = 0; f < 10; f++)
    {
      printf ("%f ", phi_heavi[1][f]);

    }
    }
