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
particle_detection () {
    /// Fix the detector efficiency.
	const double p = .989;
	const double recur = 1. / p - 1.;
	const unsigned int n = 100;
	
	TH1D *bHist = new TH1D( "bHist", "Binomial", 2 * n + 1, -.5, 2 * n + .5 );
	/**
	 * Use the recursive formula
	 * \f[
	 * P(k+1;n,p) = \frac{n-k}{k+1}\frac{p}{1-p}\,P(k;n,p).
	 * \f]
     * The recursion is performed backwards: from \f$k=n\f$ to \f$k=0\f$. Since \f$p = 0,989 \simeq1\f$,
     * when one evaluates \f$P(0;n,p) = (1-p)^n \simeq 10^{-n}\f$, for \f$n\gtrsim15\f$ (in `double`
     * precision) one would get a vanishing value at the first step. Then the corrisponding plot 
     * would be a constant zero-plot.
	 */
	double binom = TMath::Power( p, (double) n );
	int j = n;
	do {
		// fill the histogram
		bHist->Fill( j , binom );
		binom *= recur * ( (double) j ) / ( (double) n - ( -- j )  );
	} while ( j > 0 );

	bHist->Draw();

	// compare with a built-in Binomial distribution
//	TF1 *bCmpr = new TF1( "bCmpr", myBinom, 0, n, 2 );
//	bCmpr->SetParameters( p, n );
//	bCmpr->SetLineColor( kRed );
//	bCmpr->Draw( "SAME" );
	
    /// The poisson distribution is defined as
    /// \f[
    /// P(k;\lambda) = \frac{\lambda^k e^{-\lambda}}{k!}
    /// \f]
    /// where \f$\lambda = np\f$ and \f$\sigma^2 = np(1-p)\f$. This becomes a close approximation to
    /// the binomial distribution in the limit \f$p\to0\land n\to\infty\f$ while
    /// \f$ np = \textup{const}\f$. This means that the occurrenve of the event in the time interval
    /// is not greater than one. This is not the case since \f$p=0,989\f$.
	TH1D *pHist = new TH1D( "pHist", "Poisson", 2 * n + 1, -.5, 2 * n + .5 );
	const double lambda = n * p;
	double poisson = TMath::Exp( - lambda );
    // reuse the variable defined above
	j = 0;
	do {
		// fill the histogram (indexes of histograms start from 1)
		pHist->Fill( j + 1, poisson );
		poisson *= lambda / ( ++ j );
	} while ( j < 2 * n );

	pHist->Draw( "same" );

}
