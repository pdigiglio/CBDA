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

/** @brief detector efficiency. */
const double p = .989;
const double recur = 1. / p - 1.;
/** @brief Select a power of 2 */
const unsigned short int twoPow = 12;
const unsigned int n = TMath::Power( 2, twoPow );

	void
particle_detection () {
	
	/**
	 * @par
	 * _Binomial distribution_.
	 *
	 * Use the recursive formula
	 * \f[
	 * P(k+1;n,p) = \frac{n-k}{k+1}\frac{p}{1-p}\,P(k;n,p).
	 * \f]
     * The recursion is performed backwards: from \f$k=n\f$ to \f$k=0\f$. Since \f$p = 0,989 \simeq1\f$,
     * when one evaluates \f$P(0;n,p) = (1-p)^{n} \simeq 10^{-2n}\f$, for \f$n\gtrsim8\f$ (in `double`
     * precision) one would get a vanishing value at the first step. Then the corrisponding plot 
     * would be a constant zero-plot.
	 */
	TH1D *bHist = new TH1D( "bHist", "Binomial",
			6 * (int) TMath::Sqrt( n * p) + 1,
			-.5 + n * p - 3 * (int) TMath::Sqrt( n * p ),
			.5 + n * p + 3 * (int) TMath::Sqrt( n * p ) );
 
	long double binom = p; //TMath::Power( p, (double) n );
	for ( unsigned int tp = 0; tp < twoPow; ++tp ) {
		binom *= binom;
		std::cout << binom << std::endl;
	}
//	std::cout << TMath::Power( p, (double) n ) << std::endl;

	unsigned int j = n;
	do {
		/* fill the histogram */
		bHist->Fill( j , binom );
		/* evaluate next value of the binomial distribution */
//		std::cout << binom << " * ";
		binom *= recur * ( (double) j ) / ( (double) ( n - j + 1 )  );
//		std::cout << recur * ( (double) j ) / ( (double) ( n - j + 1 )  ) << std::endl;
		j--;
	} while ( j > 0 );

	bHist->SetLineColor( kGreen );
	bHist->Draw();
//	return;

// compare with a built-in Binomial distribution
//	TF1 *bCmpr = new TF1( "bCmpr", myBinom, 0, n, 2 );
//	bCmpr->SetParameters( p, n );
//	bCmpr->SetLineColor( kRed );
//	bCmpr->Draw( "SAME" );
	
	/**
	 * @par
	 * _Poisson distribution_.
	 *
	 * The poisson distribution is defined as
	 * \f[
	 * P(k;\lambda) = \frac{\lambda^k e^{-\lambda}}{k!}
	 * \f]
	 * where \f$\lambda = np\f$ and \f$\sigma^2 = np(1-p)\f$. This becomes a close approximation to
	 * the binomial distribution in the limit \f$p\to0\land n\to\infty\f$ while
	 * \f$ np = \textup{const}\f$. This means that the occurrenve of the event in the time interval
	 * is not greater than one. This is not the case since \f$p=0,989\f$.
	 *
	 * @attention The recursion stuff _will not work_ for big values of \f$n\f$ since the starting
	 * value for the variable `poisson` will be 0 and it will propagate.
	 */
	TH1D *pHist = new TH1D( "pHist", "Poisson",
			6 * (int) TMath::Sqrt( n * p) + 1,
			-.5 + n * p - 3 * (int) TMath::Sqrt( n * p ),
			.5 + n * p + 3 * (int) TMath::Sqrt( n * p ) );
	const long double lambda = n * p;
//	long double poisson = TMath::Exp( - lambda );
    // reuse the variable defined above
	j = 0;
	do {
		// fill the histogram (indexes of histograms start from 1)
		pHist->Fill( j + 1, TMath::PoissonI( lambda, j ++ ) /*poisson*/ );
        std::cout << TMath::PoissonI( lambda, j ++ ) << std::endl;
//		poisson *= lambda / ((long double) ( ++ j ));
	} while ( j < 2 * n );

	pHist->Draw( "same" );

}
