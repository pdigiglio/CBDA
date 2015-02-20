/**
 *
 *           @name  beam_calorimeter.C
 *          @brief  
 *
 *        @version  1.0
 *           @date  30/10/2014 (23:07:33)
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
using namespace std;


#include "TMath.h"
#include "TH1D.h"

//! @brief Detector resolution (= bin size).
const double res = .1;

//! @brief Measured energies in GeV.
double E[] = {
		3.8, 6.2, 4.5, 4.9, 
		4.5, 4.8, 6.8, 4.8,
		7.0, 5.0, 4.6, 6.5,
		5.1, 6.5, 6.2, 4.9,
		4.8, 4.7, 4.9, 3.8,
		6.0, 4.6, 5.8, 5.0,
		5.8, 4.9, 4.1, 4.9,
		5.0, 4.6, 6.4, 4.0,
		5.0, 3.6, 3.9, 3.6,
		5.8, 4.3, 5.6, 4.6
};

//! @brief Length of the array (i.e. number of measures)
const unsigned short int length = sizeof( E ) / sizeof( double );

int
beam_calorimeter ( void ) {
	
	/** @par
	 * _Evaluate mean from data_.
	 *
	 * Here the mean is evaluated just summing up all the single measures `E[i]` and
	 * dividing by their number at the very end.
	 */
	double sum = E[0];
	for ( unsigned short int i = 1; i < length; ++ i ) {
		sum += E[i];
	}

	double mean1 = sum / length;
	

	// find maximum and minimum value to fix the histogram range
	const double EMax = TMath::MaxElement( length, E );
	const double EMin = TMath::MinElement( length, E );

	// number of bins
	const unsigned int nBins = ( (double) ( EMax - EMin ) / res ) + 1;

	// declare a new histogram
	TH1D *histo = new TH1D( "", "", nBins, EMin - .5 * res, EMax + .5 * res );

	/**
	 * @par
	 * _Fill the histogram with the data_.
	 *
	 * The function prototype is  
	 *
	 * `void FillN(Int_t ntimes, const Double_t* x, const Double_t* w, Int_t stride = 1)`
	 * 
	 * where
	 * @param ntimes length of both arrays
	 * @param x array of data
	 * @param w array of weights
	 * @param stride step size throught arrays `x` and `y`
	 *
	 */
	histo->FillN( length, E, 0 );

//	return 0;

	/**
	 * @par
	 * _Find cumulative distribution_.
	 * 
	 * Each bin contains the sum of the previous bin contents in the measures histogram.
	 */
	TH1D *cumulative = new TH1D( "", "", nBins, EMin - .5 * res, EMax + .5 * res );
	cumulative->SetLineColor( kRed );

	// `sum` was declared for the first mean
	sum = 0.;
	double mean = 0., norm = 0.;

	// check is ROOT is cheating me
//	if ( nBins != histo->GetNbinsX() ) {
//		cerr << "Number of bins not matching! Exiting." << endl;
//		cerr << "nBins = " << nBins << ", Actual number of bins = "
//			<< histo->GetNbinsX() << endl;
//		return 1;
//	}


	unsigned short int medianBin = 0;
	for ( unsigned short int i = 1; i <= nBins; ++ i ) {

		// evaluate cumulative distribution
		sum += histo->GetBinContent( i );
		
		/**
		 * @par
		 * _Evaluate the median_.
		 *
		 * Since the area of the histogram is the number of measures (i.e. `length`), to
		 * find the median just search for the maximum bin such that
		 * \f$b_1 + b_2 + \dots + b_\textup{median} \le L/2\f$,
		 * where \f$b_j\f$ stands for the content of the \f$j\f$-th bin and \f$L\f$ for the
		 * `length` value.
		 *
		 * @attention the sum will be \f$\le L/2\f$ for each bin with index smaller than
		 * the one which is the median, that's why one looks for their maximum!
		 *
		 */
		if ( sum <= length / 2 )
			medianBin ++;
	
		// fill histogram
		cumulative->SetBinContent( i, sum );

		/**
		 * @par
		 * _Evaluate mean_.
		 *
		 *
		 * Here the mean is evaluated by using
		 * \f[
		 * \frac{\sum_j n_j E_j}{\sum_k n_k}
		 * \f]
		 * where \f$n_i\f$ is the number of times the measure \f$E_i\f$ occurred in our
		 * sample.
		 */
		mean += histo->GetXaxis()->GetBinCenter( i ) * histo->GetBinContent( i );
		norm += histo->GetBinContent( i );
//		cout << histo->GetBinCenter( i ) << " " << histo->GetBinContent( i ) << " " << mean << endl;
	}

	// normalize
//	cumulative->Scale( 1 / cumulative->GetBinContent( nBins ) );
//	cumulative->Draw();
	histo->Draw("");

	/**
	 * @par
	 * _Mode_.
	 *
	 * Is just the bin with more entries than all the others.
	 */
	unsigned int modeBin =  histo->GetMaximumBin();
	cout << endl
		 << "Mode:\t" << histo->GetXaxis()->GetBinCenter( modeBin )
		 << " with frequency " << histo->GetBinContent( modeBin )
		 << endl << endl;

	cout << "Median:\t" <<  histo->GetXaxis()->GetBinCenter( medianBin ) << endl << endl;
	
	cout << "Mean (freq.):\t" << mean / norm << endl
		 << "Mean (data):\t" << mean1 << endl
		 << "Mean (histo):\t" << histo->GetMean() << endl;

	return 0;
}
