/* math.c - fabs, pow, nthroot */

#include <math.h>

/*------------------------------------------------------------------------
 * fabs - compute the floating point absolute value of a number
 *------------------------------------------------------------------------
 */
static double		fabs(
		double	x						/* base value					*/
	)
{
	if(x == NAN) {
		return NAN;
	}

	if(x < 0) {
		x = -x;
	}
	return x;
}


/*------------------------------------------------------------------------
 * pow - compute the a raised to the power of n
 *------------------------------------------------------------------------
 */
static double	pow(
	 double		a,					/* base value					*/
	 int		n					/* power value					*/
	)
{
	/**
	 * Reminder: use nthroot() to compute nth root of base,
	 * because pow() does NOT support double precision exponents.
	 */

	double temp;

	if(a == 0) {
		return 0;
	}

	if(a < 0 || a == NAN) {
		return NAN;
	}

	if(n == 0) {
		return 1;
	}

	temp = pow(a, n/2);
	if(n % 2 == 0) {
		return temp*temp;
	} else {
		if(n > 0) {
			return a * temp * temp;
		} else {
			return (temp * temp) / a;
		}
	}

	return NAN;
}


#define EPSILON 0.000001

/*------------------------------------------------------------------------
 * delta_x_k - compute the value of delta x for Newton's method
 *------------------------------------------------------------------------
 */
static double delta_x_k(
	 double x_k, 	/* Current x_k value	*/
	 int n, 		/* n - root				*/
	 double a		/* base value			*/
	)
{
	return ((a / pow(x_k, n-1)) - x_k) / n;
}


/*------------------------------------------------------------------------
 * nthroot - compute the nth root of an input 'a'
 *------------------------------------------------------------------------
 */
double	nthroot(
	 double		a,	/* base value			*/
	 int		n	/* root value			*/
	)
{
	double x_k = 1.0;
	double d_x_k = delta_x_k(x_k, n, a);

	while(fabs(d_x_k) > EPSILON) {
		x_k += d_x_k;
		d_x_k = delta_x_k(x_k, n, a);
	}

	return x_k;
}
