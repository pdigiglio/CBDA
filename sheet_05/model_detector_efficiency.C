/**
 *
 *
 *           @name  model_detector_efficiency.C
 *          @brief  Model a detector.
 *
 *          In this script I want to see how many signals out of `trials` are caught
 *          by a detector with an efficiency smaller than one.
 *
 *          Example usage:
 *          @code
 *          @endcode
 *
 *        @version  1.0
 *           @date  02/26/2015 (04:50:52 PM)
 *       @revision  none
 *       @compiler  gcc
 *
 *
 *         @author  P. Di Giglio (github.com/pdigiglio), <p.digiglio91@gmail.com>
 *        @company  
 *
 *
 */

#include <iostream>
#include <cstdlib>

#include "TH1I.h"
#include "TF1.h"
#include "TMath.h"


/**
 * @brief
 * Efficiency of detector. This is the probability that the detector catches a signal
 */
const double efficiency = .6;

/**
 * @brief
 * Number of times the experiment is repeated each time.
 */
const unsigned int trials = 15;

const unsigned int tries = 100000;

/**
 * @brief User-defined Binomial distribution
 *
 * @return
 * \f[
 *  binomial
 * \f]
 * @param x[0] \f$k\f$
 * @param p[0] \f$p\f$
 * @param p[1] \f$n\f$
 * @param p[2] \f$A\f$
 */
double
BinomialDistribution( double *x, double *p ) {

	x[0] = (unsigned) ( x[0] + .5 );
	p[1] = (unsigned) p[1];
//	p[2] = (unsigned) p[2];

//	std::cout << x[0] << " " << p[1] << " " << p[2] << std::endl;
	return p[2] * TMath::Power( p[0], x[0] ) * TMath::Power( 1. - p[0], p[1] - x[0] ) * TMath::Binomial( p[1], x[0]);
}

int
model_detector_efficiency () {

	TH1D *catches = new TH1D( "catches", "", trials + 1 , -.5, trials + .5 );

	unsigned int detected;
	for ( unsigned int t = 0; t < tries; ++ t ) {
		detected = 0;

		/* check how many signal are caught */
		for( unsigned int sig = 0; sig < trials; ++ sig )
			detected += ( (double) rand()/RAND_MAX < efficiency );

		catches->Fill( detected );
	}

	TF1 *binomial = new TF1( "binomial", BinomialDistribution, -.5, trials + .5, 3 );
	binomial->SetParameter( 0, efficiency );
	binomial->SetParameter( 1, trials );
	binomial->SetParameter( 2, tries );

	catches->Draw();
	binomial->Draw( "same" );

	return 0;
}
