/**
 *
 *
 *           @name  particle_detection.C
 *
 *        @version  1.0
 *           @date  11/29/2014 (06:29:02 PM)
 *       @revision  none
 *       @compiler  gcc
 *
 *
 *         @author  P. Di Giglio (github.com/pdigiglio), <p.digiglio91@gmail.com>
 *        @company  
 *          @brief  
 *
 *          Example usage:
 *          @code
 *          @endcode
 *
 *
 */

#include "TMath.h"
#include "TH1D.h"
#include "TF1.h"

	double
myBinom ( double *x, double *par ) {
	return TMath::BinomialI( par[0], par[1], (int) x[0] );
}

	void
particle_detectio () {
    /// Fix the detector efficiency.
	const double p = .989;
	const double recur = p / ( 1 - p );
	const unsigned int n = 100;
	
	TH1D *bHist = new TH1D( "bHist", "Binomial", 2 * n + 1, -.5, 2 * n + .5 );
	/**
	 * Use the recursive formula
	 * \f[
	 * P(k+1;n,p) = \frac{n-k}{k+1}\frac{p}{1-p}\,P(k;n,p)
	 * \f]
	 */
	double binom = TMath::Power( 1. - p, (double) n );
	unsigned int j = 0;
	do {
		// fill the histogram (indexes of histograms start from 1)
		bHist->Fill( j + 1, binom );
		binom *= recur * ( n - j ) / ( ++ j );
	} while ( j < n );

	bHist->Draw();

	// compare with a built-in Binomial distribution
//	TF1 *bCmpr = new TF1( "bCmpr", myBinom, 0, n, 2 );
//	bCmpr->SetParameters( p, n );
//	bCmpr->SetLineColor( kRed );
//	bCmpr->Draw( "SAME" );
	
	TH1D *pHist = new TH1D( "pHist", "Poisson", 2 * n + 1, -.5, 2 * n + .5 );
	const double lambda = n * p;
	double poisson = TMath::Exp( - lambda );
	j = 0;
	do {
		// fill the histogram (indexes of histograms start from 1)
		pHist->Fill( j + 1, poisson );
		poisson *= lambda / ( ++ j );
	} while ( j < 2 * n );

	pHist->Draw( "same" );

}
