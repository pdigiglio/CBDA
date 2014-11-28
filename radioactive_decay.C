/**
 *
 *
 *           @name  radioactive_decay.C
 *          @brief  
 *
 *          Example usage, first run ROOT and then the script:
 *          @code
 *          	$ root -l
 *          	[0] .x radioactive_decay.C+
 *          @endcode
 *
 *
 *        @version  1.0
 *           @date  11/25/2014 (09:36:14 PM)
 *       @revision  none
 *       @compiler  gcc
 *
 *
 *         @author  P. Di Giglio (github.com/pdigiglio), <p.digiglio91@gmail.com>
 *        @company  
 *
 */

#include "TRandom.h"
#include "TF1.h"
#include "TH1D.h"
#include "TMath.h"
#include "TCanvas.h"

#include <iostream>

using namespace TMath;
using namespace std;

	double
myGauss ( double *x, double *p ) {
	return Exp( - ( x[0] - p[0] ) * ( x[0] - p[0] ) / ( 2 * p[1] * p[1] ) ) / ( Sqrt( 2 * Pi() ) * p[1] ); 
}

	int
//main ( void ) {
radioactive_decay ( void ) {

	/// Initialize the seed for the random generator (the default argument is `0`).
	gRandom->SetSeed();

	const double mean = 5.;       /*! Fix the mean of kinetic energy to \f$5\,\textup{MeV}\f$. */
	const double sigma = 1.;      /*! Fix the std. deviation of kinetic energy to \f$1\,\textup{MeV}\f$. */
	const double threshold = 9.2; /*! Fix the threshold of kinetic energy to \f$9.2\,\textup{MeV}\f$. */
	const unsigned short int efficiency = 6;

	// array of parameters to pass to myGauss
	const double pars[] = { mean, sigma };

	/// Declare a `TF1` function containing `myGauss` function.
	TF1 *myFunc = new TF1(
			"MG",         /* name */
			myGauss,      /* what function to call */
			- Infinity(), /* minimum value (from) */
			Infinity(),   /* maximum value (to) */
			2 );          /* parameters number */

	/// Declare a `TF1` function containing the built-in `TMath::Gaus()` function.
	TF1 *Func = new TF1( "G", "TMath::Gaus(x, 5, 1, 1)", - Infinity(), Infinity());

	/// To get the probability of a decay per second I evaluate
	/// \f[
	/// \int_{9.2}^\infty \frac{\mathrm{e}^{(x-5)^2\!/2}}{\sqrt{2\pi}\,\sigma} \,\mathrm{d}x 
	/// \f]
	/// using both my function `myFunc` and `Func`.
	/// Actually ROOT doesn't seem to be able to handle \f$(\pm\infty)\f$-limits for 
	/// integrals so I evaluate the integral from \f$9.2\,\f$MeV to some upper limit 
	/// \f$E_0\f$ such that \f$(5 - E_0)/\sigma \gg 1\f$, for example \f$E_0 = 30\f$.
	cerr << "Decay probability per second (using myGauss): "
		<< myFunc->Integral( threshold, 30, pars ) << endl;
	cerr << "Decay probability per second (using TMath::Gaus): "
		<< Func->Integral( threshold, 30 ) << endl;


	/*-----------------------------------------------------------------------------
	 *  NOW IT COMES THE ACTUAL SIMULATION
	 *-----------------------------------------------------------------------------*/
	unsigned int nuclei = 200000;
	const unsigned int time = 1000;

	TH1I *ncl = new TH1I( "", "Radioactive decay;Time [s]; Nuclei", time, 0, time );
	TH1I *decay = new TH1I( "", "Decays per second;N. of decays; Occurrences", 16, -.5, 15.5 );
	TH1I *dtc = new TH1I( "", "Alpha detected per second;N. of decays; Occurrences", 16, -.5, 15.5 );

	// take initial time
	clock_t start = clock();

	double nuclei_err = 0;
	register unsigned int n;
	// start cycling over time
	for ( unsigned int t = 1; t <= time; ++ t ) {
//		cerr << t << endl;
//		cout << t << " "<< nuclei << " " << nuclei_err << endl;

		/// Fill the histogram `ncl` with the current value of `nuclei` counter.
		ncl->SetBinContent( t, nuclei );

		for ( n = nuclei; n > 0; -- n ) {
			/// If the random value exceeds the threshold decrease the number of nuclei.
			if( gRandom->Gaus( mean, sigma ) > threshold )
				-- nuclei;
		}

		/// After one step, I get the number of nuclei from the `ncl` histogram,
		/// substract the current number of nuclei and fill `decay` histogram
		/// with this value.
		decay->Fill( ncl->GetBinContent( t ) - nuclei );

		/// The error for the nuclei number is initially set to zero. Then I use the
		/// propagation of the error using the formula \f$\sqrt{\delta x^2 + \delta y ^ 2}\f$.
		/// Since che number of nuclei \f$n_j\f$ at the \f$j\f$-th step is \f$n_{j-1} - d_j\f$, where
		/// \f$d_j\f$ is the number of decays at the step \f$j\f$, I can evaluate recursively the 
		/// error as \f$\delta n_j^2 = \delta n_{j-1}^2 - \delta d_j^2\f$.
		/// Since the decays are poisson distribuited, \f$\delta d_j^2 = d_j\f$.
		nuclei_err *= nuclei_err;
		nuclei_err += (double) ( ncl->GetBinContent( t ) - nuclei );
		nuclei_err = sqrt( nuclei_err );

		/// Modelize the detector which detects \f$60\,\f$\% of the alpha-particle emitted.
		/// The number of decayd nuclei equals the number of \f$\alpha\f$-particles.
		unsigned int detected = 0;
		for ( unsigned int j = 0; j < ncl->GetBinContent( t ) - nuclei; ++ j ) {
			if( gRandom->Integer(10) < efficiency )
			++ detected;
		}

		dtc->Fill( detected );
	}

	ncl->Draw();

	TF1 *fit = new TF1( "fit", Form( "%d * TMath::Poisson(x, [0])", time ), 0, 20 );
	fit->SetParameter( 0, decay->GetMean() );

	// create new TCanvas for the new plot
	new TCanvas();

	decay->Fit( fit );
	// draw errors
	decay->Draw("E");
//	fit->Draw("same");
	cout << "[decay] Histo mean: " << decay->GetMean() << " Fit par[0]: " << fit->GetParameter(0) << endl;
	// set color line and draw line
//	decay->SetLineColor( kRed );
//	decay->Draw("Csame");

	new TCanvas();

	TF1 *fitDetected = new TF1( "fitDetected", Form( "%d * TMath::Poisson(x, [0])", time ), 0, 20 );
	fitDetected->SetParameter( 0, dtc->GetMean() );
	
	dtc->Fit( fitDetected );
	dtc->Draw( "E" );
	cout << "[dtc] Histo mean: " << dtc->GetMean() << " Fit par[0]: " << fitDetected->GetParameter(0) << endl;
//	cout << dtc->Integral() << endl;
//	dtc->Draw( "Csame" );

	// prints execution time
	cerr << "Time: " << (double) ( clock() - start ) / CLOCKS_PER_SEC << endl;

	return 0;
}		/* -----  end of function radioactive_decay.C  ----- */
