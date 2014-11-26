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

using namespace TMath;

	double
myGauss ( double *x, double *p ) {
	return Exp( - x[0] * x[0] / ( 2 * p[1] * p[1] ) ) / ( Sqrt( 2 * Pi() ) * p[1] ); 
}

	void
radioactive_decay ( void ) {
	/// Initialize the seed for the random generator (the default argument is `0`).
	gRandom->SetSeed();


	double mean = 5.; /*! Fix the mean of kinetic energy to \f$5\,\textup{MeV}\f$. */
	double sigma = 1.; /*! Fix the std. deviation of kinetic energy to \f$1\,\textup{MeV}\f$. */

	const double pars[] = { mean, sigma };

	/// Declare a `TF1` function containing `myGauss` function.
	TF1 *myFunc = new TF1(
			"MG",         /* name */
			myGauss,      /* what function to call */
			- Infinity(), /* minimum value (from) */
			Infinity(),   /* maximum value (to) */
			2 );          /* parameters number */

//	/// Declare a `TF1` function containing the built-in `gaus()` function.
//	TF1 *Func = new TF1(
//			"G",         /* name */
//			"gaus(5,1)",      /* what function to call */
//			- Infinity(), /* minimum value (from) */
//			Infinity(),   /* maximum value (to) */
//			2 );          /* parameters number */
	
	cerr << "Decay probability per second: " << myFunc->Integral( 9.2, 2000 /* Infinity() */, pars ) << endl;

//	for ( unsigned int j = 0; j < 1000; ++ j )
//		cout << gRandom->Gaus( mean, sigma ) << endl;

	unsigned int nuclei = 200000;
	const unsigned int time = 10000;

	TH1D *histo = new TH1D( "", ";Time [s]; Nuclei", time, 0, time );

	clock_t start = clock();
//	#pragma omp parallel for ordered
	register unsigned int n;
	for ( unsigned int t = 1; t <= time; ++ t ) {
		cerr << t << endl;
		cout << nuclei << endl;

//		cout << t << " " << nuclei << endl;
		// fill the histogram
		histo->SetBinContent( t, nuclei );

		for ( n = nuclei; n > 0; -- n ) {
			/// If the random value exceeds the threshold decrease the number of nuclei.
			if( gRandom->Gaus( mean, sigma ) > 9.2 )
				-- nuclei;
		}
	}

	cerr << "Time: " << (double) ( clock() - start ) / CLOCKS_PER_SEC << endl;
	histo->Draw();
	return ;
}		/* -----  end of function radioactive_decay.C  ----- */
