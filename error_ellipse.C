/**
 *
 *
 *           @name  error_ellipse.C
 *
 *          @brief  ROOT script to see what happens to a 2-dim. Gaussian distribution under
 *          chance of coordinates.
 *
 *          Example usage:
 *          @code
 *          	root -l error_ellipse.C+
 *          @endcode
 *
 *        @version  1.0
 *           @date  12/10/2014 (02:51:55 PM)
 *       @revision  none
 *       @compiler  gcc
 *
 *         @author  P. Di Giglio (github.com/pdigiglio), <p.digiglio91@gmail.com>
 *        @company  
 *
 *
 */

#include "TF2.h"
#include "TMath.h"
#include "TCanvas.h"

using namespace TMath;

/// @brief 2-dimensional Gaussian function improved
///
/// @param x Array of variables, in this case \f$(x,y)\f$
/// @param p Array of parameters, in this case \f$(\langle x\rangle, \sigma_x, \langle y \rangle, \sigma_y )\f$ 
	double
my2DGaussImp( double *x, double *p ) {
	// auxiliary variable for exponent
	register double expoX = ( *x - *p ) / p[1];
//	expoX *= expoX;

	register double expoY = ( x[1] - p[2] )/ p[3];
//	expoY *= expoY;

	// return the value
	return Exp( - .5 *( expoX * expoX + expoY * expoY ) )/ ( Pi() * p[1] * p[3] );
}

/// @brief 2-dimensional rotated Gaussian function
///
/// This function is calls `my2DGaussImp` and implements the rotation.
/// Given a rotation of a certain angle \f$\phi\f$ around one axis parallel to the 
/// \f$z\f$-axis with coordinates \f$(\langle x \rangle, \langle y \rangle, z)\f$, with 
/// \f$z\f$ not fixed, the function rotates backwards the coordinates and returns
/// `my2DGaussImp` evaluated in the new coordinates.
///
/// @attention The angle _must_ be given in radiants!
///
/// @param x Array of variables, in this case \f$(x,y)\f$
/// @param p Array of parameters, in this case \f$(\langle x\rangle, \sigma_x, \langle y \rangle, \sigma_y, \phi )\f$ 
	double
my2DGaussRot( double *x, double *p ) {
	const double s = Sin( p[4] );
	const double c = Cos( p[4] );

	*x -= *p;
	x[1] -= p[2];

	// evaluate the new point
	double y[] = {
		(*x) * c - x[1] * s,
		x[1] * c + (*x) * s
	};
//	cout << x[0] - p[0] << " " << x[1] - p[2] << " " << p[1] << " " << p[3] << " " << p[4] << endl;

	// return the value
	// Now `p[]` has lenght of 5 but the 5th parameter will be ignored in `my2DGaussImp`.
	return my2DGaussImp( y, p );
}
	int
error_ellipse ( void ) {
	// fix parameters
	const double meanX = 0.;
	const double meanY = 0.;
	const double sigmaX = 1.;
	const double sigmaY = 2.;
	const double phi = 30 * DegToRad();

	// non-rotated gaussian
	TF2 *nrG = new TF2( "NRG", my2DGaussRot, -6, 6, -6, 6, 4);
	nrG->SetParameters( meanX, sigmaX, meanY, sigmaY );

    nrG->SetNpx(100);
    nrG->SetNpy(100);
    nrG->SetLineWidth(.01);

	new TCanvas( "nrG", "Non-rotated Gaussian" );
	nrG->DrawCopy( "surf3" );


	// rotated gaussian
	TF2 *rG = new TF2( "NRG", my2DGaussRot, -6, 6, -6, 6, 5);
	rG->SetParameters( meanX, sigmaX, meanY, sigmaY, phi );

    rG->SetNpx(100);
    rG->SetNpy(100);
    rG->SetLineWidth(.01);

	new TCanvas( "rG", "Rotated Gaussian" );
	rG->DrawCopy( "surf3" );

	// contour levels
	double clvs[] = {
//		nrG->Eval( meanX, meanY),
		nrG->Eval( meanX, meanY) / Sqrt( E() ),
		nrG->Eval( meanX, meanY) / E(),
//		nrG->Eval( meanX, meanY) / ( Sqrt( E() ) * E() )
	};

	nrG->SetContour( sizeof(clvs) / sizeof(double), clvs );
	new TCanvas("contour", "contour levels");
	nrG->Draw();

	rG->SetContour( sizeof(clvs) / sizeof(double), clvs );
	rG->SetLineColor( kBlue );
	rG->Draw("same");

	return 0;
}
