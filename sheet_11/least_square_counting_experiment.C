/**
 *
 *
 *           @name  least_square_counting_experiment.C
 *          @brief 
 *
 *          This ROOT script generates a histogram with 10 equidistant bins in the range
 *          \f$[x_\textup{min},x_\textup{max}]\f$ filled with random counts that in each bin follow a Poisson
 *          distribution. The underlying _pdf_ of \f$x\f$ is assumed to be Gaussian:
 *          \f[
 *              g(x;\mu,\sigma) = N_\textup{entries}\frac{\mathrm{e}^{-(x-\mu)^2\!/2\sigma^2}}{\sqrt{2\pi}\,\sigma}.
 *          \f]
 *
 *          Example usage:
 *          @code
 *          root -l least_square_counting_experiment.C+
 *          @endcode
 *
 *
 *        @version  1.0
 *           @date  01/18/2015 (07:53:52 PM)
 *       @revision  none
 *       @compiler  gcc
 *
 *
 *         @author  P. Di Giglio (github.com/pdigiglio), <p.digiglio91@gmail.com>
 *        @company  
 *
 */
#include <iostream>
#include <ctime>

#include "TF1.h"
#include "TH1D.h"
#include "TRandom.h"

#include "TCanvas.h"
#include "TMath.h"

using namespace std;

const unsigned int tries = 10000;

//! @brief Lower bound for \f$x\f$.
const double xMin = 1.;

//! @brief Upper bound for \f$x\f$.
const double xMax = 3.;

//! @brief Number of bins for \f$x\f$.
const unsigned int bins = 10;

const double binWidth = (double) ( xMax - xMin ) / bins;

const double mean  = 2.;
const double sigma = .5;
/**
 * @brief Normalization of the Gaussian.
 *
 * __Key point:__ The counting of each bin is Poisson-distributed. When this number gets
 * bigger, the Poisson distribution is similar to a Gaussian so the pulls plot will be
 * described by a gaussian with std. deviation equals to 1 and mean equals to 0.
 *
 */
const double norm  = 10000.;

//! @brief Useful parameter array to allow looping later in the program.
double par[3] = { norm * binWidth, mean, sigma };
//double parApp[3] = { norm * binWidth, mean, sigma };

	int
least_square_counting_experiment () {

	clock_t start = clock();

	/* Define a Gaussian function */
	TF1 *gaussFunc = new TF1( "gaussFunc", "gausn", xMin, xMax );
	/* Set function parameters */
	gaussFunc->SetParameters( norm, mean, sigma );
	
	/* NO
	 * Evaluate explicitly the expected area of the histograms since  the normalization
	 * _is not_ the area because the normalization is referred to the whole real axis.
	 */
//	for ( unsigned int b = 0; b < bins; ++ b ) {
//		parInt[0] += gaussFunc->Integral( xMin + b * binWidth, xMin + ( b + 1 ) * binWidth );
//		parApp[0] += binWidth * gaussFunc->Eval( xMin + ( b + .5 ) * binWidth );
//	}
//
//	/* normalize areas */
//	parApp[0] *= binWidth;
//	parInt[0] *= binWidth;
//	cout << "Estimated areas: " << parInt[0] << " (int) and " << parApp[0] << " (approx)" << endl;
//	cout << gaussFunc->Integral( xMin, xMax) << endl;
	
//	gaussFunc->Draw();


	/* histograms to be filled in each cycle */
	TH1D *histoIntegral = nullptr, *histo = nullptr;
	double count, error;

	/* histograms for the fitting parameters */
	TH1D *histoInt[] = {
		new TH1D( "normInt", "Best estimates for the norm [integral]", 100, -2.5, 2.5 ),
		new TH1D( "meanInt", "Best estimates for the mean [integral]", 100, -2.5, 2.5 ),
		new TH1D( "sigmaInt", "Best estimates for the sigma [integral]", 100, -2.5, 2.5 )
	};

	TH1D *histoChiSquareInt[] = {
		new TH1D( "normChiSquareInt", "#chi^{2}-distribution for norm [integral]", 100, 0, 7 + 2 * TMath::Sqrt( 14. ) ),
		new TH1D( "meanChiSquareInt", "#chi^{2}-distribution for mean [integral]", 100, 0, 7 + 2 * TMath::Sqrt( 14. ) ),
		new TH1D( "sigmaChiSquareInt", "#chi^{2}-distribution for sigma [integral]", 100, 0, 7 + 2 * TMath::Sqrt( 14. ) )
	};

	TH1D *histoChiSquareProbInt[] = {
		new TH1D( "normChiSquareProbInt", "#chi^{2}-probability distribution for norm [integral]", 100, 0, 1 ),
		new TH1D( "meanChiSquareProbInt", "#chi^{2}-probability distribution for mean [integral]", 100, 0, 1 ),
		new TH1D( "sigmaChiSquareProbInt", "#chi^{2}-probability distribution for sigma [integral]", 100, 0, 1 )
	};

	TH1D *histoApp[] = {
		new TH1D( "normApp", "Best estimates for the norm", 100, -2.5, 2.5 ),
		new TH1D( "meanApp", "Best estimates for the mean", 100, -2.5, 2.5 ),
		new TH1D( "sigmaApp", "Best estimates for the sigma", 100, -2.5, 2.5 )
	};

	TH1D *histoChiSquareApp[] = {
		new TH1D( "normChiSquareApp", "#chi^{2}-distribution for norm [approximate]", 100, 0, 7 + 2 * TMath::Sqrt( 14. ) ),
		new TH1D( "meanChiSquareApp", "#chi^{2}-distribution for mean [approximate]", 100, 0, 7 + 2 * TMath::Sqrt( 14. ) ),
		new TH1D( "sigmaChiSquareApp", "#chi^{2}-distribution for sigma [approximate]", 100, 0, 7 + 2 * TMath::Sqrt( 14. ) )
	};

	TH1D *histoChiSquareProbApp[] = {
		new TH1D( "normChiSquareProbApp", "#chi^{2}-probability distribution for norm [approximate]", 100, 0, 1 ),
		new TH1D( "meanChiSquareProbApp", "#chi^{2}-probability distribution for mean [approximate]", 100, 0, 1 ),
		new TH1D( "sigmaChiSquareProbApp", "#chi^{2}-probability distribution for sigma [approximate]", 100, 0, 1 )
	};

	/* dummy help functin pointer */
	TF1 *dummy = nullptr;
	
	for ( unsigned int t = 0; t < tries; ++ t  ) {
		
		/* define new histograms to be filled */
		histoIntegral = new TH1D( "histoIntegral", "Integral", bins, xMin, xMax );
		histo = new TH1D( "histo", "Approximate", bins, xMin, xMax );

		/* Fill the histogram */
		for ( unsigned int b = 0; b < bins; ++ b ) {
	
			/**
			 * @par
			 * _Fill histograms_.
			 *
			 * __By integrating:__ 
			 * Evaluate first each bin by integrating the gaussian in the interval range
			 * \f$[x_\textup{min bin},x_\textup{max bin}]\f$ i.e. integrating on the bin
			 * \f[
			 *     \int_{x_\textup{min bin}}^{x_\textup{max bin}}g(x;\mu,\sigma)\,\mathrm{d}x.
			 * \f]
			 * This gives the _actual_ expected count in that interval, regardless of its
			 * size.
			 *
			 * __By approximating:__
			 * Then approximate the previous integral with
			 * \f$g(x_\textup{bin center};\mu,\sigma) |x_\textup{min} - x_\textup{max} |/N_\textup{bins}\f$:
			 * this should be a good approximation when \f$|x_\textup{min} - x_\textup{max} |/N_\textup{bins}\f$
			 * is small. One expects that this approximation gets worse as the dimension
			 * of the interval gets bigger.
			 *
			 *
			 * @par
			 * _Pearson's vs. Neyman's_.
			 *
			 * Using Neyman's method, the pulls are biased: they are not described by a 
			 * Gaussian centered in 0 with std. deviation of 1. With Pearson's method the
			 * situation is way better. Anyway, the difference gets small when the normalization
			 * `norm` gets larger.
			 *
			 * Means of pulls using Neyman's method
			 * @code
			 * HistoIntegral normalization: -0.0736411 +/- 0.0105053
			 * HistoIntegral mean:           0.0147785 +/- 0.0104721
			 * HistoIntegral sigma:         -0.0235648 +/- 0.0104084
			 *
			 * HistoApp normalization:      -0.0954914 +/- 0.0104236
			 * HistoApp mean:                0.0188087 +/- 0.0105688
			 * HistoApp sigma:              -0.0708095 +/- 0.0105565
			 * @endcode
			 *
			 * Means of pulls using Pearsons's method
			 * @code
			 * HistoIntegral normalization: 0.001691 +/- 0.0105121
			 * HistoIntegral mean:          0.0144328 +/- 0.0103931
			 * HistoIntegral sigma:         0.0134234 +/- 0.0103798
			 *
			 * HistoApp normalization:     -0.0146808 +/- 0.010397
			 * HistoApp mean:               0.0203636 +/- 0.0105606
			 * HistoApp sigma:             -0.0324551 +/- 0.0105097
			 * @endcode
			 *
			 * _They sould all be compatible with 0._
			 *
			 *
			 * It can also be seen from the distribution of \f$P(\chi^2>\chi^2_\textup{obs})\f$
			 * that when using Pearson's method the distribution is closer to the one we 
			 * expect for the Gussian (i.e. a flat distribution).
			 * 
			 */

			// Neyman's \f$\chi^2\f$.
//			error = gRandom->Poisson( gaussFunc->Integral( xMin + b * binWidth, xMin + ( b + 1 ) * binWidth) );
//			count = error;

			// Pearsons's \f$\chi^2\f$.
			error = gaussFunc->Integral( xMin + b * binWidth, xMin + ( b + 1 ) * binWidth );
			count = gRandom->Poisson( error );

			histoIntegral->SetBinContent( b + 1, count );
			histoIntegral->SetBinError( b + 1, TMath::Sqrt( error ) );

			// Neyman's \f$\chi^2\f$.
//			error = gRandom->Poisson( binWidth * gaussFunc->Eval( xMin + ( b + .5 ) * binWidth ) );
//			count = error;

			// Neyman's \f$\chi^2\f$.
			error = binWidth * gaussFunc->Eval( xMin + ( b + .5 ) * binWidth );
			count = gRandom->Poisson( error );

			histo->SetBinContent( b + 1, count );
			histo->SetBinError( b + 1, TMath::Sqrt( error ) );
		}

//		TCanvas *canvInt = new TCanvas( "int", "integral" );
//		histoIntegral->Draw();
//		cout << histoIntegral->Integral() << endl;

		/**
		 * @attention
		 * The method `TH1D::Integral()` will return the number of entries in the histogram
		 * i.e. the _sum of all bins value_. This is not the _real_ area beneath the histogram.
		 * In fact, in this case, with \f$[x_\textup{min},x_\textup{max}] = [1,3]\f$ and
		 * \f$10\f$ bins, if the number of enties is \f$10000\f$ then the _real_ integral
		 * is \f$N_\textup{entries}(x_\textup{max}-x_\textup{min}) / N_\textup{bins} = 2000\f$.
		 */

		/**
		 * @par
		 * _Fit histograms_.
		 *
		 * Here I fit my integral with the option
		 *     1. "Q" = don't print fit parameters on screen;
		 *     2. "I" = use the integral of the (fitting) function over the bin instead of the 
		 *     function ad the bin center
		 *     3. "0" = don't draw a plot containing the function after the fit
		 *
		 * @attention The histograms which have been generated using the integral over the
		 * bin _require_ the `I` option! The fit will be wrong otherwise!
		 */
		histoIntegral->Fit( "gausn", "QI0" );
		
		dummy = histoIntegral->GetFunction( "gausn" );
		for ( unsigned short int m = 0; m < 3; ++ m ) {
			histoInt[m]->Fill( ( dummy->GetParameter( m ) - par[m] ) / dummy->GetParError( m ) );
		
			histoChiSquareInt[m]->Fill( dummy->GetChisquare() );
			histoChiSquareProbInt[m]->Fill( TMath::Prob( dummy->GetChisquare(), dummy->GetNDF() ) );

//			cout << dummy->GetNDF() << endl;

//			cout << ( dummy->GetParameter( m ) - par[m] ) / dummy->GetParError( 1 ) << endl;

		}

//		dummy = histoIntegral->GetFunction( "gausn" );
//		histoInt[2]->Fill( ( dummy->GetParameter( 2 ) - par[2] ) / dummy->GetParError( 2 ) );
//		cout << dummy->GetParameter( 2 )  << " " << par[2] << endl;

//			cout << ( dummy->GetParameter( m ) - par[m] ) / dummy->GetParError( 1 ) << endl;

//
//		TCanvas *canvApprox = new TCanvas( "appr", "approximate" );
//		histo->Draw();

		histo->Fit( "gausn","+Q0" );
		dummy = histo->GetFunction( "gausn" );
		for ( unsigned short int m = 0; m < 3; ++ m ) {
			histoApp[m]->Fill( ( dummy->GetParameter( m ) - par[m] ) / dummy->GetParError( m ) );

			histoChiSquareApp[m]->Fill( dummy->GetChisquare() );
			histoChiSquareProbApp[m]->Fill( TMath::Prob( dummy->GetChisquare(), dummy->GetNDF() ) );
		}

		/* delete the histograms to avoid memory leacks */
		delete histoIntegral;
		delete histo;

//		cout << "loop " << t << " over tries " << tries << endl;
	}

		// XXX if you delete the histograms, you'll have a segmentation violation here!
//		histoIntegral->Draw();
//		TCanvas *canvApprox = new TCanvas( "appr", "approximate" );
//		histo->Draw();

	new TCanvas();
	histoInt[0]->Draw();
	histoInt[0]->Fit( "gausn", "Q" );

	new TCanvas();
	histoChiSquareInt[0]->Draw();
	
	new TCanvas();
	histoChiSquareProbInt[0]->Draw();
	
	cout << "HistoIntegral normalization: "
		 << histoInt[0]->GetFunction( "gausn" )->GetParameter(1)
		 << " +/- "
		 << histoInt[0]->GetFunction( "gausn" )->GetParError(1)
		 << endl;

	new TCanvas();
	histoInt[1]->Draw();
	histoInt[1]->Fit( "gausn", "Q" );

	new TCanvas();
	histoChiSquareInt[1]->Draw();
	
	new TCanvas();
	histoChiSquareProbInt[1]->Draw();
	
	cout << "HistoIntegral mean: "
		 << histoInt[1]->GetFunction( "gausn" )->GetParameter(1)
		 << " +/- "
		 << histoInt[1]->GetFunction( "gausn" )->GetParError(1)
		 << endl;
	
	new TCanvas();
	histoInt[2]->Draw();
	histoInt[2]->Fit( "gausn", "Q" );

	new TCanvas();
	histoChiSquareInt[2]->Draw();
	
	new TCanvas();
	histoChiSquareProbInt[2]->Draw();

	cout << "HistoIntegral sigma: "
		 << histoInt[2]->GetFunction( "gausn" )->GetParameter(1)
		 << " +/- "
		 << histoInt[2]->GetFunction( "gausn" )->GetParError(1)
		 << endl;

	new TCanvas();
	histoApp[0]->Draw();
	histoApp[0]->Fit( "gausn", "Q" );

	new TCanvas();
	histoChiSquareApp[0]->Draw();
	
	new TCanvas();
	histoChiSquareProbApp[0]->Draw();
	
	cout << "HistoApp normalization: "
		 << histoApp[0]->GetFunction( "gausn" )->GetParameter(1)
		 << " +/- "
		 << histoApp[0]->GetFunction( "gausn" )->GetParError(1)
		 << endl;
	
	new TCanvas();
	histoApp[1]->Draw();
	histoApp[1]->Fit( "gausn", "Q" );

	new TCanvas();
	histoChiSquareApp[1]->Draw();
	
	new TCanvas();
	histoChiSquareProbApp[1]->Draw();
	
	cout << "HistoApp mean: "
		 << histoApp[1]->GetFunction( "gausn" )->GetParameter(1)
		 << " +/- "
		 << histoApp[1]->GetFunction( "gausn" )->GetParError(1)
		 << endl;
	
	new TCanvas();
	histoApp[2]->Draw();
	histoApp[2]->Fit( "gausn", "Q" );

	new TCanvas();
	histoChiSquareApp[2]->Draw();
	
	new TCanvas();
	histoChiSquareProbApp[2]->Draw();
	
	cout << "HistoApp sigma: "
		 << histoApp[2]->GetFunction( "gausn" )->GetParameter(1)
		 << " +/- "
		 << histoApp[2]->GetFunction( "gausn" )->GetParError(1)
		 << endl;

	cout << " >> Execution time " << (double) (  clock() - start )/ CLOCKS_PER_SEC << endl;
	return 0;
}
