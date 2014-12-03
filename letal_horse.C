/**
 *
 *
 *           @file  letal_horse.C
 *          @brief  Distribution of letal horse accident of the Prussian Army.
 *
 * 			These values are Poisson distribuite and the aim of this ROOT script is to 
 * 			proove it.
 *
 *          Example usage:
 *          @code
 *          	root -l letal_horse.C+
 *          @endcode
 *
 *        @version  1.0
 *           @date  11/29/2014 (11:53:26 PM)
 *       @revision  none
 *       @compiler  gcc
 *
 *
 *         @author  P. Di Giglio (github.com/pdigiglio), <p.digiglio91@gmail.com>
 *        @company  
 *
 *
 */

#include "TH1I.h"
#include "TF1.h"
#include "TMath.h"


void
letal_horse ( void ) {
	/// Use an array to store the occurrence of the number of deaths. The index of the
	/// array represents the number of deaths i.e. 109 years per corp with 0 deaths
	/// has been registered.
	const unsigned short int deaths[] = { 109, 65, 2, 3, 1 };
	const unsigned short int length = sizeof( deaths ) / sizeof( short ); 

	// XXX If I don't know a priori the distribution of my data, how can I extimate
	// the error bars on the histogram entries?

	/// Declare and fill an histogram of integers.
	TH1I *histo = new TH1I( "", "", length + 1, -.5, length + .5 );
	for ( unsigned short int j = 0; j < length; ++ j )
		histo->Fill( j, deaths[j] );

	histo->Draw();

	// Define a poissonian distribution
	TF1 *pssn = new TF1( "pssn", "[1] * TMath::Poisson(x, [0])", 0., length );
	/// Use the histogram mean as the \f$\lambda\f$ parameter.
	pssn->SetParameter( 0, histo->GetMean() );
	/// In this case the histogram bars have width = 1 so the `TH1D::Integral()` and
	/// `TH1D::GetEntries()` values are the same.
	pssn->SetParameter( 1, histo->Integral() );

	pssn->Draw( "SAME" );
}
