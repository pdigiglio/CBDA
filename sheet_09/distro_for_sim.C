/**
 *
 *
 *           @name  24_distro_for_sim.C
 *
 *        @version  1.0
 *           @date  01/06/2015 (10:46:24 PM)
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

#include <ctime>
#include <iostream>

#include "TH1I.h"
#include "TH1D.h"
#include "TF1.h"
#include "TRandom.h"
#include "TMath.h"
#include "TCanvas.h"
#include "TString.h"

using namespace TMath;

const unsigned int N = 100000;


const unsigned int lambda = 10000;
const double inv_lambda = 1. / lambda;

/**
 * @brief Probability of reaction.
 *
 * Out of a sample of \f$10^6\f$ trials, we expect \f$\lambda = 10^4\f$ countings so
 * \f$\delta p = 10^{-2}\f$
 */
const double p = (double) N * inv_lambda;

const double st_dev = Sqrt( lambda );


/** @brief The number of dof when I generalize to dof > 1. */
const unsigned short int dof = 200;


/**
 * @brief Evaluate \f$\chi^2\f$ for one dof.
 *
 * For each measurement of \f$x_i\f$, the error is referred to the actual counting so
 * \f$\sigma_i^2 = x_i\f$ (here I am using the fact that measures are distributed according
 *  to a Poisson function) and the \f$\chi^2_i\f$ is evaluating using _that_ error.
 *
 * @return \f$\chi^2 = (x-\mu)^2\!/\sigma^2 = (x - \lambda)^2\!/x\f$. 
 */
	inline double
ChiSquare( double x ) {
	return ( x - lambda ) * ( x - lambda ) / x;
}

/**
 * @brief Maximum between two numbers.
 */
	inline double
max( double a, double b ) {
	return ( ( a > b ) ? a : b );
}

/**
 * @brief The \f$\chi^2\f$-distribution for one dof.
 *
 * The normalization coefficient (the only one in which the number of dof enters) has
 * been already evaluated for \f$n_\textup{dof} = 1\f$
 *
 * @param x array of random variables (only `x[0]` is needed)
 * @param p array of parameters (only `p[0]` = num. dof is [__not!__] used)
 */
	double
ChiSquarePDF1dof( double *x, double *p ) {
	return Exp( - .5 * x[0] ) / Sqrt( 2. * Pi() * x[0] );
}

/**
 * @brief The \f$\chi^2\f$-distribution for arbitrarily many dof.
 *
 * @param x array of random variables (only `x[0]` is needed)
 * @param p array of parameters (only `p[0]` = num. dof is used)
 *
 * @return \f[\frac{(\chi^2)^{n/2 - 1}e^{-\chi^2\!/2}}{2^{n/2}\Gamma(n/2)}\f]
 */
	double
ChiSquarePDF( double *x, double *p ) {
	return .5 * Power( .5 * x[0], .5 * p[0] - 1) * Exp( - .5 * x[0] ) / Gamma( .5 * p[0] );
}

	int
distro_for_sim ( void ) {
//main( void ) {
	clock_t start = clock();


	//----------------------------------------------------------------------------------//
	//
	// Assignments (a) and (b)
	//
	//----------------------------------------------------------------------------------//

	TH1I *histoPoisson = new TH1I( "", "Poisson (#lambda = " + TString::Itoa( lambda, 10 ) + ")",
			4 * st_dev + 1,
			lambda - 2 * st_dev - .5,
			lambda + 2 * st_dev + .5 );

	/** Since I have only 1 dof, I expect \f$(E[\chi^2],V[\chi^2]) = (1,2)\f$. */
	TH1D *histoChiSquare = new TH1D( "", "#chi^{2}", 51, - .05, 5.05 );

	unsigned int outcome;
	for ( unsigned int n = 0; n < N; ++ n ) {
		outcome = gRandom->Poisson( lambda );

		histoPoisson->Fill( outcome );
//		std::cout << ChiSquare( (double) outcome ) << std::endl;
		histoChiSquare->Fill( ChiSquare( (double) outcome ) );
	}

	new TCanvas( "name", "Single RV distribution");
	histoPoisson->Draw();

	new TCanvas( "foo", "Single-DOF ChiSquare");

	TF1 *CSP = new TF1("name", ChiSquarePDF1dof, 0., 5., 1 );
	histoChiSquare->Scale( 1. / histoChiSquare->Integral(), "WIDTH" );
//	std::cout << histoChiSquare->Integral() << std::endl;
//	std::cout << CSP->Integral(0.,5.) << std::endl;


	histoChiSquare->Draw();
	CSP->Draw( "same" );

//	return 0;

	// -------------------------------------------------------------------------------- //
	
	/**
	 * @par
	 * Now we go from one RV to five or ten RVs.
	 */


	register double combChiSquare;
	

	// histogram for the ChiSquare of all the RVs
	TH1D *histoCombChiSquare = new TH1D( "",
			"Combined #chi^{2} (#lambda = " + TString::Itoa( lambda, 10 ) +
			", dof = " + TString::Itoa( dof, 10 ) + ")",
			100,
			max( 0., dof - 3. * Sqrt( 2 * dof )),
			dof + 3. * Sqrt( 2 * dof )
			);

	// break the cycle to avoid inserting if-tests for the output
	const unsigned int step = 1000;
	for ( unsigned int n = 0; n < N / step; ++ n ) {

		for ( unsigned short int t = 0; t < step; ++ t ) {
			combChiSquare = 0.;

	//		std::cout << "Step " << n << " of " << N << std::endl;
			for ( register unsigned short d = 0; d < dof; ++ d ) {

				/**
				 * @par
				 * _Multiple DOF \f$\chi^2\f$_.
				 *
				 * Here I am evaluating the \f$\chi^2\f$ for a single measure involving
				 * `dof` degrees of freedom. The \f$\chi^2\f$ is the sum of the single
				 * \f$\chi^2_i\f$ for each dof. At the end I take the histogram of the
				 * sum. This is why \f$2000\f$ measures of a single DOF differ from
				 * \f$500\f$ measures of 4 DOF.
				 */
				combChiSquare += ChiSquare( gRandom->Poisson( lambda ) );
			}

			histoCombChiSquare->Fill( combChiSquare );
		}

		std::cout << "Step " << n * step << " of " << N << std::endl;
	}

	new TCanvas( "blabla", "Combined ChiSquare");
	histoCombChiSquare->Scale( 1. / histoCombChiSquare->Integral(), "WIDTH" );
	histoCombChiSquare->Draw();

	// Formula for the Combined ChiSquare (for Poisson)
	TF1 *CCSP = new TF1("#chi^{2}-distribution", ChiSquarePDF,
			max( 0., dof - 3. * Sqrt( 2 * dof )),
			dof + 3. * Sqrt( 2 * dof ),
			1 );

	// Formula for the Gaussian to superpose over the ChiSquare
	TF1 *GChiSquare = new TF1("Gaussian", "gausn(0)",
			max( 0., dof - 3. * Sqrt( 2 * dof )),
			dof + 3. * Sqrt( 2 * dof ) );
	GChiSquare->SetParameters( 1., dof, Sqrt( 2 * dof ) );
	GChiSquare->SetLineColor( 9 );


	// Draw histogram, ChiSquare and Gaussian
	CCSP->SetParameter( 0, dof );
	CCSP->Draw( "same" );
	GChiSquare->Draw( "same" );


	/**
	 * @par
	 * _Point (d)_.
	 *	
	 * When only 50 countings are expected, the Poisson distribution is not well-approximated
	 * by a Gaussian one. We'll see that the \f$\chi^2\f$ distribution for our measures
	 * is not fitted by the curve we expect (which has been derived assuming that measures
	 * are distributed according to a Gaussian).
	 *
	 * This deviation between actual \f$\chi^2\f$-hisotgram and expected shape will also 
	 * appear in the case of 10 DOF. In that plot, both the peak and the tail of the 
	 * distribution will be signigicantly different from the expected curve.
	 *
	 * When \f$\lambda = 50\f$ the Central Theorem Limit is no more satisfied thus the
	 * Poissonian distribution is not well-fitted by a gaussian. Our derivation for the
	 * \f$\chi^2\f$-distribution is valid for Gaussian-distributed RVs so everything will
	 * screw up (i.e. the ChiSquare hypothesis test is no longer a good test because it 
	 * does not apply)!
	 *
	 */


	/**
	 * @par
	 * _Point (e)_.
	 *
	 * When the number of dof is \f$n = 200\f$, then the \f$\chi^2\f$ distribution is
	 * well-approximated by the Gaussian one with \f$(\mu,\sigma^2) = (n, 2n) \f$. Also
	 * in this case, the parameter \f$\lambda\f$ has to be _large enought_ for the Poisson
	 * distribution to be approximated with a Gaussian!
	 */

	std::cout << "Time: " << (double) ( clock() - start ) / CLOCKS_PER_SEC << std::endl;

	return 0;
}
