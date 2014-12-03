/**
 *
 *
 *           @file  detector_efficiency.C
 *          @brief  
 *
 *          Example usage:
 *          @code
 *          @endcode
 *
 *        @version  1.0
 *           @date  11/30/2014 (01:43:35 PM)
 *       @revision  none
 *       @compiler  gcc
 *
 *
 *         @author  P. Di Giglio (github.com/pdigiglio), <p.digiglio91@gmail.com>
 *        @company  
 *
 */

#include "TF1.h"

/// @brief The efficiency for a 3-layers detector as a function of the probability.
///
/// The efficiency in this case is just \f$P(3;3,p) = p^3\f$.
	double
threeLayersEfficiency( const double *x, const double *p ) {
	return x[0] * x[0] * x[0];
}

/// @brief The efficiency for a 4-layers detector as a function of the probability.
///
/// The efficiency in this case is \f$P(3;4,p) + P(4;4,p) = p^3(4-3p)\f$.
	double
fourLayersEfficiency( const double *x, const double *p ) {
	return x[0] * x[0] * x[0] * ( 4. - 3. * x[0] );
}

/// @brief The efficiency for a 5-layers detector as a function of the probability.
///
/// The efficiency in this case is\f$P(3;5,p) + P(4;5,p) + P(5;5,p) = p^3(10-15p+6p^2)\f$.
	double
fiveLayersEfficiency( const double *x, const double *p ) {
	return x[0] * x[0] * x[0] * ( 10. + 6. * x[0] * x[0] - 15. * x[0] );
}

	void
detector_efficiency ( void ) {

	/// Declare three `TF1` objects that represent the efficiency as a function of the
	/// probability \f$p\f$ for a single layer to detect the particle.
	TF1 *three = new TF1( "ThreeLayers", threeLayersEfficiency, 0., 1., 0);
	three->SetLineColor( kRed );
	three->Draw();
	cerr << " >> Layer efficiency for a 3-layers deector: " << three->GetX( .99, 0., 1. ) << endl;

	TF1 *four = new TF1( "FourLayers", fourLayersEfficiency, 0., 1., 0);
	four->SetLineColor( kBlue );
	four->Draw("same");
	cerr << " >> Layer efficiency for a 4-layers deector: " << four->GetX( .99, 0., 1. ) << endl;

	TF1 *five = new TF1( "FiveLayers", fiveLayersEfficiency, 0., 1., 0);
	five->SetLineColor( kGreen );
	five->Draw("same");
	cerr << " >> Layer efficiency for a 5-layers deector: " << five->GetX( .99, 0., 1. ) << endl;
}
