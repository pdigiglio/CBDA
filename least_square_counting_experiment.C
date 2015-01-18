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

const unsigned int tries = 100000;

const double xMin = 1.;
const double xMax = 3.;
const unsigned int bins = 10;

const double binWidth = ( xMax - xMin ) / bins;

const double mean  = 2.;
const double sigma = .5;
const double norm  = 10000.;

double parInt[3] = { 0., mean, sigma };
double parApp[3] = { 0., mean, sigma };

	int
least_square_counting_experiment () {

	clock_t start = clock();

	/* Define a Gaussian function */
	TF1 *gaussFunc = new TF1( "gaussFunc", "gausn", xMin, xMax );
	/* Set function parameters */
	gaussFunc->SetParameters( norm, mean, sigma );
	
	/**
	 * Evaluate explicitly the expected area of the histograms since  the normalization
	 * _is not_ the area because the normalization is referred to the whole real axis.
	 */
	for ( unsigned int b = 0; b < bins; ++ b ) {
		parInt[0] += gaussFunc->Integral( xMin + b * binWidth, xMin + ( b + 1 ) * binWidth );
		parApp[0] += binWidth * gaussFunc->Eval( xMin + ( b + .5 ) * binWidth );
	}

	/* normalize areas */
	parApp[0] *= binWidth;
	parInt[0] *= binWidth;
	cout << "Estimated areas: " << parInt[0] << " (int) and " << parApp[0] << " (approx)" << endl;
//	cout << gaussFunc->Integral( xMin, xMax) << endl;
	
//	gaussFunc->Draw();


	/* histograms to be filled in each cycle */
	TH1D *histoIntegral = nullptr, *histo = nullptr;
	double count;

	/* histograms for the fitting parameters */
	TH1D *histoInt[] = {
		new TH1D( "normInt", "Best estimates for the norm", 50, -1.5, 1.5 ),
		new TH1D( "meanInt", "Best estimates for the mean", 50, -1.5, 1.5 ),
		new TH1D( "sigmaInt", "Best estimates for the sigma", 50, -1.5, 1.5 ),
	};

	TH1D *histoApp[] = {
		new TH1D( "normApp", "Best estimates for the norm", 50, -1.5, 1.5 ),
		new TH1D( "meanApp", "Best estimates for the mean", 50, -1.5, 1.5 ),
		new TH1D( "sigmaApp", "Best estimates for the sigma", 50, -1.5, 1.5 ),
	};

	/* dummy help functin pointer */
	TF1 *dummy = nullptr;
	
	for ( unsigned int t = 0; t < tries; ++ t  ) {
		
		/* define new histograms to be filled */
		histoIntegral = new TH1D( "histoIntegral", "Integral", bins, xMin, xMax );
		histo = new TH1D( "histo", "Approximate", bins, xMin, xMax );

		/** Fill the histogram */
		for ( unsigned int b = 0; b < bins; ++ b ) {
	
			/**
			 * Evaluate first each bin by integrating the gaussian in the interval
			 * \f$[x_\textup{min bin},x_\textup{max bin}]\f$ i.e. integrating on the bin
			 * \f[
			 *     \int_{x_\textup{min bin}}^{x_\textup{max bin}}g(x;\mu,\sigma)\,\mathrm{d}x.
			 * \f]
			 */
			count = gRandom->Poisson( gaussFunc->Integral( xMin + b * binWidth, xMin + ( b + 1 ) * binWidth ) );
			histoIntegral->SetBinContent( b + 1, count );
			histoIntegral->SetBinError( b + 1, TMath::Sqrt( count ) );

			/**
			 * Then approximate the previous integral with
			 * \f$g(x_\textup{bin center};\mu,\sigma) |x_\textup{min} - x_\textup{max} |/N_\textup{bins}\f$:
			 * this should be a good approximation when \f$|x_\textup{min} - x_\textup{max} |/N_\textup{bins}\f$
			 * is small.
			 */
			count = gRandom->Poisson( binWidth * gaussFunc->Eval( xMin + ( b + .5 ) * binWidth ) );
			histo->SetBinContent( b + 1, count );
			histo->SetBinError( b + 1, TMath::Sqrt( count ) );
		}

//		TCanvas *canvInt = new TCanvas( "int", "integral" );
//		cout << histoIntegral->Integral() << endl;

		/**
		 * The method `TH1D::Integral()` will return the number of entries in the histogram
		 * i.e. the _sum of all bins value_. This is not the _real_ area beneath the histogram.
		 * In fact, in this case, with \f$[x_\textup{min},x_\textup{max}] = [1,3]\f$ and
		 * \f$10\f$ bins, if the number of enties is \f$10000\f$ then the _real_ integral
		 * is \f$N_\textup{entries}(x_\textup{max}-x_\textup{min}) / N_\textup{bins} = 2000\f$.
		 */
//		histoIntegral->Draw();
		histoIntegral->Fit( "gausn", "Q0" );
		
		dummy = histoIntegral->GetFunction( "gausn" );
		for ( unsigned short int m = 0; m < 3; ++ m ) {
			histoInt[m]->Fill( ( dummy->GetParameter( m ) - parInt[m] ) / dummy->GetParError( m ) );
		}
//		cout << ( dummy->GetParameter( 1 ) - mean ) / dummy->GetParError( 1 ) << endl;
		

//		TCanvas *canvApprox = new TCanvas( "appr", "approximate" );
//		histo->Draw();
		histo->Fit( "gausn","Q0" );

		dummy = histo->GetFunction( "gausn" );
		for ( unsigned short int m = 0; m < 3; ++ m ) {
			histoApp[m]->Fill( ( dummy->GetParameter( m ) - parApp[m] ) / dummy->GetParError( m ) );
		}

		/* delete the histograms to avoid memory leacks */
		delete histoIntegral;
		delete histo;
	}

	new TCanvas();
	histoInt[0]->Draw();
	histoInt[0]->Fit( "gausn", "Q" );
	cout << "HistoIntegral normalization: "
		 << histoInt[0]->GetFunction( "gausn" )->GetParameter(1)
		 << " +/- "
		 << histoInt[0]->GetFunction( "gausn" )->GetParError(1)
		 << endl;
	
	new TCanvas();
	histoInt[1]->Draw();
	histoInt[1]->Fit( "gausn", "Q" );
	cout << "HistoIntegral mean: "
		 << histoInt[1]->GetFunction( "gausn" )->GetParameter(1)
		 << " +/- "
		 << histoInt[1]->GetFunction( "gausn" )->GetParError(1)
		 << endl;
	
	new TCanvas();
	histoInt[2]->Draw();
	histoInt[2]->Fit( "gausn", "Q" );
	cout << "HistoIntegral sigma: "
		 << histoInt[2]->GetFunction( "gausn" )->GetParameter(1)
		 << " +/- "
		 << histoInt[2]->GetFunction( "gausn" )->GetParError(1)
		 << endl;

	new TCanvas();
	histoApp[0]->Draw();
	histoApp[0]->Fit( "gausn", "Q" );
	cout << "HistoIntegral normalization: "
		 << histoApp[0]->GetFunction( "gausn" )->GetParameter(1)
		 << " +/- "
		 << histoApp[0]->GetFunction( "gausn" )->GetParError(1)
		 << endl;
	
	new TCanvas();
	histoApp[1]->Draw();
	histoApp[1]->Fit( "gausn", "Q" );
	cout << "HistoIntegral mean: "
		 << histoApp[1]->GetFunction( "gausn" )->GetParameter(1)
		 << " +/- "
		 << histoApp[1]->GetFunction( "gausn" )->GetParError(1)
		 << endl;
	
	new TCanvas();
	histoApp[2]->Draw();
	histoApp[2]->Fit( "gausn", "Q" );
	cout << "HistoIntegral sigma: "
		 << histoApp[2]->GetFunction( "gausn" )->GetParameter(1)
		 << " +/- "
		 << histoApp[2]->GetFunction( "gausn" )->GetParError(1)
		 << endl;

	cout << " >> Execution time " << (double) (  clock() - start )/ CLOCKS_PER_SEC << endl;
	return 0;
}
