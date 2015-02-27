/**
 * @file transf_variables.C
 */
#include "TMath.h"
#include "TF2.h"
#include "TF1.h"
#include "TCanvas.h"


#include <iostream>
#include <cstdio>
#include <cstdlib>

using namespace TMath;

/**
 * @brief Implementation of a two-dimemsional Gaussian function in Cartesian coordinates.
 *
 * In this function \f$\sigma_x = \sigma_y\f$ so only one parameter for \f$\sigma\f$ is
 * required. This leads to a distribution which is invatiant under rotations around the
 * point \f$(\langle x \rangle, \langle y \rangle)\f$.
 *
 * @param x Array of variables, in this case \f$(x,y)\f$
 * @param p Array of parameters, in this case \f$(\langle x\rangle, \langle y \rangle, \sigma )\f$
 */
    double
my2DGauss( double *x, double *p ) {
    double exponent = ( x[0] - p[0] ) * ( x[0] - p[0] ) + ( x[1] - p[1] ) * ( x[1] - p[1] );
    return Exp( - ( exponent ) / ( 2 * p[2] * p[2] ) ) / ( 2 * Pi() * p[2] * p[2] ); 
}

/**
 * @brief Implementation of a two-dimemsional Gaussian function in Polar coordinates.
 *
 * This function will return
 * \f[
 * \frac{\rho\,\mathrm{e}^{-\rho^2\!/2\sigma}}{2\pi\sigma^2},
 * \f]
 * where \f$\rho^2 = x^2+y^2\f$. Even at this stage (before marginalization) there is no
 * \f$\phi\f$-dependence.
 *
 * @param x Array of variables, in this case \f$(x,y)\f$
 * @param p Array of parameters, in this case \f$( \sigma )\f$ 
 *
 * @attention Only the parameter \f$\sigma\f$ makes sense since if \f$\langle x\rangle \neq 0\f$
 * of \f$\langle y\rangle \neq 0\f$ then the previous expression no longer holds.
 */
    double
my2DGaussPolar( double *x, double *p ) {
    double exponent = ( x[0] ) * ( x[0] ) + ( x[1] ) * ( x[1] );
	return Sqrt( exponent ) * Exp( - ( exponent ) / ( 2 * p[0] * p[0] ) ) / ( 2 * Pi() * p[0] * p[0] ); 
}

/**
 * @brief Implementation of a mono-dimemsional Gaussian function in polar coordinates.
 *
 * This function is obtained by a coordinate transformation from Cartesian to polar coordinates
 * and then a marginalization over the angle \f$\phi\f$.
 *
 * @param x Array of variables, in this case \f$\rho\f$
 * @param p Array of parameters, in this case \f$( \sigma )\f$
 *
 * @attention Here only the parameter \f$\sigma\f$ makes sense: if we had a Gaussian which
 * was not centered in the origin, it would not be invariant for rotations!
 */
    double
my1DGaussPolar( double *x, double *p ) {
    return x[0] * Exp( - .5 * x[0] * x[0] / ( p[0] * p[0] ) ) / ( p[0] * p[0] ); 
}

    int
transf_variables ( void ) {

    TF2 *m2G = new TF2( "my2Gauss", my2DGauss, -5,5,-5,5,3); // -Infinity(), Infinity(), - Infinity(), Infinity(), 3 );
    m2G->SetParameter( 0, 0.);
    m2G->SetParameter( 1, 0.);
    m2G->SetParameter( 2, 1.);
    m2G->SetNpx(100);
    m2G->SetNpy(100);
    m2G->Draw("surf1");

    TF2 *m2GP = new TF2( "my2Gauss", my2DGaussPolar, -5,5,-5,5,1); // -Infinity(), Infinity(), - Infinity(), Infinity(), 3 );
    m2GP->SetParameter( 0, 1. );
    m2GP->SetNpx(200);
    m2GP->SetNpy(200);
    m2GP->SetLineWidth(.07);
    new TCanvas();
    m2GP->Draw("surf4");

    TF1 *m1GP = new TF1( "my1GaussPolar", my1DGaussPolar, 0, 10, 1);
    m1GP->SetParameter( 0, 1. );
    m1GP->SetNpx(200);
    new TCanvas();
    m1GP->Draw();
}
