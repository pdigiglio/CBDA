/**
 *
 *
 *           @name  least_square_polynomial.C
 *
 *        @version  1.0
 *           @date  01/17/2015 (02:15:42 PM)
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

#include <iostream>

#include "TString.h"
#include "TGraphErrors.h"
#include "TCanvas.h"
#include "TF1.h"

#include "TH1D.h"
#include "TH2D.h"

const unsigned int MaxDOF = 5;

	int
least_square_polynomial ( TString dataPointFile = "LSDataPoints.dat" ) {

	/** Define a `TGraphErrors` containing data points. */
	TGraphErrors *dataPointGraph = new TGraphErrors( dataPointFile, "%lg %lg %lg" );
//	dataPointGraph->Draw();

	TCanvas *canv[MaxDOF] = {};

	/// Declare a dummy function to handle fit \f$\chi^2\f$ and parameters and a dummy
	/// string to generate the polynomials names.
	TF1 *f[MaxDOF] = {};
	TString fitFunctionName = "";
	for ( unsigned int d = 0; d < MaxDOF; ++ d ) {
		std::cout << std::endl << " >> Using " <<  d << "-degree(s) polynomial for fit!" << std::endl;


		/**
		 * Use predefined polynomial functions to fit data. The number is referred to the
		 * degree of the polynomial, i.e. \f$P_0 \equiv p_0\f$, \f$P_1(x) = p_0 + p_1x\f$
		 * and so on.
		 */
		fitFunctionName = "pol" + TString::Itoa( d, 10 );
		/* 
		 * "+0Q" in the options = add function (don't replace it), don't draw it after the
		 * fit and don't print fit results on screen
		 */
		dataPointGraph->Fit( fitFunctionName, "+0Q" );

		// get function and print fit parameters
		f[d] = dataPointGraph->GetFunction( fitFunctionName );
		std::cout << "________ChiSquare: " << f[d]->GetChisquare() << std::endl;
		std::cout << "______# of DOF(s): " << f[d]->GetNDF() << std::endl;
		
		// change line color
		f[d]->SetLineColor( d + 2 );

		// draw data and fit
		canv[d] = new TCanvas( TString::Itoa( d, 10 ), TString::Itoa( d, 10 ) + "-degree(s) polynomial" );
		dataPointGraph->Draw();
		f[d]->DrawCopy( "same" );
	}

	//-----------------------------------------------------------------------------------//
	
	/**
	 * Plot the histogram for the 0-degree polynomial \f$\chi^2\f$ (i.e. 1 free parameter)
	 * around its minimum.
	 */
	TF1 *pol = new TF1( "name", "pol0" );

	/**
	 * When I set the histogram range to \f$\hat p_0 \pm\sigma_{\hat p_0}\f$ I see that
	 * \f$\chi^2(\hat p_0 \pm \sigma_{\hat p_0}) - \chi^2(\hat p_0 ) = 1\f$.
	 */
	const double halfRange = f[0]->GetParError(0);//.02;
	const double parMin = f[0]->GetParameter(0) - halfRange;
	const double parMax = parMin + 2 * halfRange;

	const unsigned int parBins = 100;

	TH1D *oneParam = new TH1D( "dummy", "", parBins, parMin, parMax );
	for ( unsigned int b = 0; b < parBins; ++ b ) {
		pol->SetParameter( 0, parMin + b * 2 * halfRange / parBins );
		oneParam->SetBinContent( b + 1, dataPointGraph->Chisquare( pol ) -  dataPointGraph->Chisquare( f[0] ));
	}

	new TCanvas();
	oneParam->DrawCopy();
	delete pol;

	/**
	 * Plot the histogram for the 1-degree polynomial \f$\chi^2\f$ (i.e. 2 free parameter)
	 * around its minimum.
	 */
	pol = new TF1( "name", "pol2" );

	const double halfRange1 = f[1]->GetParError(0);//.02;
	const double halfRange2 = f[1]->GetParError(1);//.02;

	const double parMin1 = f[1]->GetParameter(0) - halfRange1;
	const double parMax1 = parMin1 + 2 * halfRange1;

	const double parMin2 = f[1]->GetParameter(1) - halfRange2;
	const double parMax2 = parMin2 + 2 * halfRange2;

	const unsigned int parBinsNew = 1000;

	TH2D *twoParam = new TH2D(
			"dummy",
			"#Delta#chi^{2}(#theta_{0},#theta_{1});#theta_{0};#theta_{1}",
			parBinsNew, parMin1, parMax1,
			parBinsNew, parMin2, parMax2
		);
	for ( unsigned int b = 0; b < parBinsNew; ++ b ) {
		pol->SetParameter( 0, parMin1 + b * 2 * halfRange1 / parBinsNew );

		for ( unsigned int bb = 0; bb < parBinsNew; ++ bb ) {
			pol->SetParameter( 1, parMin2 + bb * 2 * halfRange2 / parBinsNew );
			twoParam->SetBinContent( b + 1, bb + 1, dataPointGraph->Chisquare( pol ) -  dataPointGraph->Chisquare( f[1] ));
		}
	}

	new TCanvas();
	twoParam->Draw( "colz" );
//	cout << f[0]->GetParameter(0) << endl;
//	pol->SetParameter( 0, f[0]->GetParameter(0)  );
//	cout << dataPointGraph->Chisquare( pol ) -  dataPointGraph->Chisquare( f[0] ) << endl;

//	new TCanvas();
	/**
	 * The method `TGraphErrors::Chisquare( TF1 f )` returns the f$\chi^2_\textup{min}\f$
	 * for the function `f`.
	 */
//	dataPointGraph->Chisquare( f[0] );

//	delete [] canv;

	return 0;
}
