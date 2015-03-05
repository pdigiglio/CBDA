/**
 *
 *
 *           @name  least_square_fit_non_gaussian.C
 *          @brief  Plot \f$P(\chi^2>\chi^2_\textup{obs})\f$ for non-gaussian distributed
 *         	errors.
 *
 *         	In the script three PDFs are defined, namely a Gaussian, a Laplacian and a
 *         	uniform distribution. Then a simple linear function \f$y=f(x) = mx+q\f$ is
 *         	taken and random numbers around the expected value \f$f(x_i)\f$ are generated
 *         	with one of the previous distributions.
 *
 *         	Generated data are fitted and at the very end the histogram of \f$P(\chi^2>\chi^2_\textup{obs})\f$
 *         	is plotted. For gaussian distributed errors, this is a uniform histogram.
 *
 *          Example usage:
 *          @code
 *          root -l least_square_fit_non_gaussian.C
 *          @endcode
 *
 *        @version  1.0
 *           @date  01/28/2015 (02:28:58 PM)
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

#include <stdlib.h>
#include <time.h>

#include "TF1.h"
#include "TH1D.h"

#include "TMath.h"
#include "TGraphErrors.h"
#include "TCanvas.h"

using namespace TMath;
using namespace std;

/** @brief Lower bound for distribution plots */
const double yMin = -2.;
/** @brief Upper bound for distribution plots */
const double yMax = +2.;

/** @brief Number of times data are generated */
const unsigned int numTries = 500000;

/**
 * @brief Uniform distribution in the interval \f$(a,b)\f$.
 *
 * @param p[0] \f$a\f$ the lower bound of the interval
 * @param p[1] \f$b\f$ the upper bound of the interval
 * @param x[0] the point where to evaluate the distribution
 *
 * @return
 * 	* `0.` if `x[0]` doesn't lie between `p[0]` and `p[1]`;
 * 	* `1./(p[1]-p[0])` otherwise.
 *
 * @attention to save some CPU clock, I don't return the absolute value
 * of the previous quantity so pay attention and insert the parameter in the right order
 * otherwise you'll get a negative PDF.
 */
/*
 * \f[
 * f(x) = \begin{cases}
 * 	1/(b-a) 	%& % <-- allignment doesn't work with doxygen
 * 	\text{if }x\in(a,b),\\
 * 	0			%& % <--    ""        ""     ""   ""    ""
 * 	\text{ otherwise}
 * 	\end{cases}
 * \f]
 */
	double
uniformDistribution( double *x, double *p ) {
	if ( x[0] > p[0] ) {
		if ( x[0] < p[1] )
			return 1. / ( p[1] - p[0] );
	}

	return 0.;
}

/**
 * @brief Evaluate \f$\chi^2(x)\f$ given mean and error.
 *
 * I will _not_ use it into the script.
 */
	double
evaluateChiSquare( double x, double mean, double error ) {
	return ( x - mean ) * ( x - mean ) / ( error * error );
}

/**
 * @brief The reduced \f$\chi^2\f$-distribution for arbitrarily many dof.
 *
 * @param x array of random variables (only `x[0]` is needed)
 * @param p array of parameters (only `p[0]` = num. dof is used)
 *
 * @return \f[f_{\chi^2\!/n}(\chi^2\!/n;n) = \frac{(\chi^2\!/n)^{n/2 - 1}\mathrm{e}^{-\chi^2\!/2n}}{n\,2^{n/2}\Gamma(n/2)}\f]
 */
	double
reducedChiSquarePDF( double *x, double *p ) {
	x[0] *= p[0];
	return p[0] * .5 * Power( .5 * x[0], .5 * p[0] - 1) * Exp( - .5 * x[0] ) / Gamma( .5 * p[0] );
}

	int
least_square_fit_non_gaussian() {
	clock_t start = clock();
	
	TF1 *gauss = new TF1( "gauss", "gausn", yMin, yMax );
	gauss->SetParameters( 1, 0, .5 );

	/**
	 * I want the mean and standard deviation of the uniform distribution to be
	 * 0 and \f$0,5\f$, respectively.
	 *
	 * In order for the mean to be null \f$a = - b\f$ so that the uniform distribution
	 * becomes \f$1/2b\f$.
	 *
	 * I know the variance is \f$4b^2\!/12 = 1/4\f$ so \f$b = \sqrt{3}/2\f$.
	 */
	TF1 *uniform = new TF1 ( "uniform", uniformDistribution, yMin,yMax, 2 );
	uniform->SetParameters( - .5 * Sqrt(3.), .5 * Sqrt(3.) );
	uniform->SetLineColor( 3 );

	/**
	 * Laplacian distribution: use the built-in function `TMath::LaplaceDist()`.
	 *
	 * The variance for the Laplacean distribution \f$L(y;\mu,b)\f$ is \f$2b^2 = 1/4\f$
	 * so \f$b= 1/2\sqrt{2}\f$
	 */
	TF1 *laplace = new TF1( "laplace", "TMath::LaplaceDist(x,[0],[1])", yMin, yMax );
	laplace->SetParameters( 0., 1. / ( 2 * Sqrt( 2. ) )  );
	laplace->SetLineColor( 4 );

	/* plot laplace first to have the plot scaled in the right way */
	laplace->DrawCopy();
	gauss->DrawCopy( "same" );
	uniform->DrawCopy("same");


	/* x range and samples */
	const double xMin =  0.;
	const double xMax = 20.;
	const unsigned int numPoints = 20;
	
	/* slope and offset for the line */
	const double m = .25, q = 1.;
	
	double x[numPoints], y[numPoints], yErr[numPoints], xErr[numPoints] = {};


	TH1D *chiSquareHisto = new TH1D( "chiSquare", "Reduced #chi^{2};#chi^{2};frequency", 100, 0, 4 );
	TH1D *chiSquareProbHisto = new TH1D( "chiSquareProb", "#chi^{2}-probability;Prob( #chi^{2} > #chi^{2}_{obs} );frequency", 200, 0, 1. );
	TF1 *fit = nullptr;
	for ( unsigned int t = 0; t < numTries; ++ t ) {
		for ( unsigned int n = 0; n < numPoints; ++ n ) {
			x[n] = xMin + n * ( xMax - xMin ) / (double) numPoints;


	//		// ------------------------------------------------------------------------------
	//		// XXX this doesn't work due to some ROOT macic! Thanks ROOT, I really love you!
	//
	//		/** Change the mean for my distribution */
	//		laplace->SetParameter( 0, m * x[n] + q );
	//		// plots are correct
	//		laplace->DrawCopy( "same" );
	//		y[n] = laplace->GetRandom();
	//		// ------------------------------------------------------------------------------


			/** Generate a random number */
			y[n] = m * x[n] + q + gauss->GetRandom();

			/**
			 * Distributions are always such that their standard deviation is \f$0,5\f$.
			 */
			yErr[n] = .5;
		}
		
		TGraphErrors *generated = new TGraphErrors( numPoints, x, y, xErr, yErr );
		generated->Fit( "pol1", "+Q0" );

		fit = generated->GetFunction( "pol1");

		chiSquareHisto->Fill( fit->GetChisquare() / fit->GetNDF() );
		chiSquareProbHisto->Fill( Prob( fit->GetChisquare(), fit->GetNDF() ) );

	}
//	new TCanvas();
//	generated->Draw();


	TF1 *redChiSquare = new TF1( "redChiSquare", reducedChiSquarePDF, 0, 4, 1 );
	redChiSquare->SetParameter( 0, 18 );

	new TCanvas();
	chiSquareHisto->Scale( 1. / chiSquareHisto->Integral(), "WIDTH" );
	chiSquareHisto->Draw();
	redChiSquare->Draw( "same" );

	new TCanvas();
	chiSquareProbHisto->Scale( 1. / chiSquareProbHisto->Integral(), "WIDTH" );
	chiSquareProbHisto->Draw();

	for ( unsigned int j = 1; j < 201; ++ j ) {
		cout << chiSquareProbHisto->GetBinCenter(j) << "\t" << chiSquareProbHisto->GetBinContent(j) << endl;
	}

	cout << "Execution time: " << (double) ( clock() - start ) / CLOCKS_PER_SEC << endl;
	return 0;
}
