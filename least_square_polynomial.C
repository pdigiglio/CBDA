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
		std::cout << std::endl << " >> Using " <<  d << " parameter(s)!" << std::endl;


		/**
		 * Use predefined polynomial functions to fit data. The number is referred to the
		 * degree of the polynomial, i.e. \f$P_0 \equiv p_0\f$, \f$P_1(x) = p_0 + p_1x\f$
		 * and so on.
		 */
		fitFunctionName = "pol" + TString::Itoa( d, 10 );
		dataPointGraph->Fit( fitFunctionName, "0" );

		// get function and print fit parameters
		f[d] = dataPointGraph->GetFunction( fitFunctionName );
		std::cout << "________ChiSquare: " << f[d]->GetChisquare() << std::endl;
		std::cout << "______# of DOF(s): " << f[d]->GetNDF() << std::endl;
		
		// change line color
		f[d]->SetLineColor( d + 2 );

		// draw data and fit
		canv[d] = new TCanvas( TString::Itoa( d, 10 ), TString::Itoa( d, 10 ) + " parameters" );
		dataPointGraph->Draw();
		f[d]->DrawCopy( "same" );
	}

	//-----------------------------------------------------------------------------------//
	
//	new TCanvas();
	/**
	 * The method `TGraphErrors::Chisquare( TF1 f )` returns the f$\chi^2_\textup{min}\f$
	 * for the function `f`.
	 */
//	dataPointGraph->Chisquare( f[0] );

//	delete [] canv;

	return 0;
}
