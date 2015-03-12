/**
 *
 *
 *           @name  maximumLikelihood.C
 *
 *        @version  1.0
 *           @date  03/11/2015 (08:40:13 PM)
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
#include "TF1.h"
#include "TH1F.h"
#include "TRandom.h"

#include <iostream>
using namespace std;
using namespace TMath;

/** @brief Bins of the histogram. */
const unsigned short int nBins = 1000;
const  double xMin =  0.;
const  double xMax = 10.;

const double binSize = ( xMax - xMin ) / nBins;
/** @brief Number of measures. */
const unsigned int nMeasures = 100;
/**
 * @brief Number of experiment
 *
 * At each experiment `nMeasures` data are collected
 */
const unsigned int nExperiments = 10000;

/** @brief Decay rate */
const double tau = 1.;

/**
 * @brief PDF for the exponential decay.
 *
 * @param p[0] decay time \f$\tau\f$.
 * @param p[1] normalization \f$N\f$.
 *
 * @return \f[f(x;\tau,N) = N\,\frac{\mathrm{e}^{-t/\tau}}{\tau}.\f]
 */
	inline double
decayPDF( double x[], double p[] ) {
	return ( p[1] / p[0] ) * TMath::Exp( - x[0] / p[0] );
}

	inline double
Gauss( double x[], double p[] ) {
	return p[0] * Exp( - .5 * Power( ( x[0] - p[1] ) / p[2] , 2. ) ) / Sqrt( 2 * Pi() * p[2] * p[2] );
}

	int
maximumLikelihood () {

	TF1 *expDecay = new TF1( "expDecay", decayPDF, xMin, xMax, 2 );
	expDecay->SetParameter( 0, tau );
	expDecay->FixParameter( 1, binSize * nMeasures );

	TH1F *expDecayHisto = nullptr;
	TH1F *tauPulls = new TH1F( "tauPulls", "#tau", 100,-3.,3.);

//	cout << xMin - .5 * binSize << " "
//		 << xMax + .5 * binSize << " "
//		 << nBins + 1 << " "
//		 << binSize << endl;

	for ( unsigned int trial = 0; trial < nExperiments; ++ trial ) {

		/* If I change the range, the fit sucks */
		expDecayHisto = new TH1F(
				"expDecayHisto",
				"",
				nBins, //+ 1,
				xMin, // - .5 * binSize,
				xMax //+ .5 * binSize
			);

//		// fill histogram with my distribution
//		for ( unsigned int b = 0; b < nBins; ++ b ) {
////			expDecayHisto->Fill( expDecay->GetRandom() );
//			expDecayHisto->FillRandom( "expDecay", 1. );
//		}
		
		// ... or equivalently
		// XXX in this case, the normalization needs to be set to 1!
		expDecay->SetParameter( 1, 1. );
		for ( unsigned int m = 0; m < nMeasures; ++ m ) {
			expDecayHisto->Fill( expDecay->GetRandom() );
		}

		// fix the parameter before the fit
		expDecay->FixParameter( 1, (double) binSize * nMeasures );
		expDecayHisto->Fit( expDecay, "QL" );

//		cout << expDecay->GetParameter( 0 ) << " "
//			 << expDecay->GetParError( 0 ) << endl;

//		cout << expDecay->GetParameter( 1 ) << " "
//			 << expDecay->GetParError( 1 ) << endl;

		tauPulls->Fill( ( expDecay->GetParameter( 0 ) - tau ) / expDecay->GetParError( 0 ) );

		// reset parameter to the true value after fit
		expDecay->SetParameter( 0, tau );

//		expDecayHisto->Draw();
//		break;

		// to avoid memory leak
		delete expDecayHisto;
	}
	
	TF1 *g = new TF1("g", Gauss, -3., 3., 3);

	g->SetParameter( 0, nExperiments * ( 6 /100. )  );
	g->SetParameter( 1, 0. );
	g->SetParameter( 2, 1. );

	tauPulls->Draw();
	g->Draw("same");

	return 0;
}
