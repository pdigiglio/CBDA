/*
 * ==================================================================
 *
 *       Filename:  beam_calorimeter.C
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  30/10/2014 23:07:33
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  P. Di Giglio (), p.digiglio91@gmail.com
 *   Organization:  
 *
 * ==================================================================
 */

int
beam_calorimeter ( void ) {

	// detector resolution (= bin size)
	const double res = .1;
	// measured energies in GeV
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
	
	// length of the array
	const unsigned short int length = sizeof( E ) / sizeof( double );
	
	// 1. evaluates mean from data
	double sum = *E;
	for ( unsigned short int i = 1; i < length; ++ i )
		sum += *( E + i );

	double mean1 = sum / length;
	

	// find maximum and minimum value to fix the histogram range
	const double EMax = TMath::MaxElement( length, E );
	const double EMin = TMath::MinElement( length, E );

	// number of bins
	const unsigned int nBins = ( (double) ( EMax - EMin ) / res ) + 1;

	// declare a new histogram
	TH1D *histo = new TH1D( "", "", nBins, EMin - .5 * res, EMax + .5 * res );
	// fill the histogram with the data
	histo->FillN( length, E, 0 );

//	return 0;

	// find cumulative distribution
	TH1D *cumulative = new TH1D( "", "", nBins, EMin - .5 * res, EMax + .5 * res );
	cumulative->SetLineColor( kRed );

	// sum was declared for the first mean
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
		if ( sum <= length / 2 )
			medianBin ++;
	
		// fill histogram
		cumulative->SetBinContent( i, sum );

		// evaluate mean
		mean += histo->GetXaxis()->GetBinCenter( i ) * histo->GetBinContent( i );
		norm += histo->GetBinContent( i );
//		cout << histo->GetBinCenter( i ) << " " << histo->GetBinContent( i ) << " " << mean << endl;
	}

	// normalize
//	cumulative->Scale( 1 / cumulative->GetBinContent( nBins ) );
//	cumulative->Draw();
	histo->Draw("");

	unsigned int modeBin =  histo->GetMaximumBin();
	cout << "Mode: " << histo->GetXaxis()->GetBinCenter( modeBin )  <<
		" with frequency " << histo->GetBinContent( modeBin ) << endl;
	cout << "Median: " <<  histo->GetXaxis()->GetBinCenter( medianBin ) << endl;
	cout << "Mean: " << mean / norm <<
		"\nMean data: " << mean1 <<
		"\nMean histo: " << histo->GetMean() << endl;

	return 0;
}
