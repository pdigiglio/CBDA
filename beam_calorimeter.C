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

	// resolution
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
	
	// find maximum and minimum value
	const double EMax = TMath::MaxElement( length, E );
	const double EMin = TMath::MinElement( length, E );

	// number of bins
	const unsigned int nBins = length + 1;

	TH1D *histo = new TH1D( "", "", nBins, EMin - .5 * res, EMax + .5 * res );
	histo->FillN( length, E, 0 );

	histo->Draw();

	// find cumulative distribution
	TH1D *cumulative = new TH1D( "", "", nBins, EMin - .5 * res, EMax + .5 * res );
	double sum = 0.;
	double mean = 0.;
	for ( unsigned int i = 1; i <= nBins; ++ i ) {
		// evaluate cumulative distribution
		sum += histo->GetBinContent( i );
//		cout << sum << endl;
	
		// fill histogram
		cumulative->SetBinContent( i, sum );

		// evaluate mean
		mean += histo->GetXaxis()->GetBinCenter(i) * histo->GetBinContent( i );
//		mean += histo->GetXaxis()->GetBinLowEdge(i) * histo->GetBinContent( i );
	}

//	cumulative->Draw();

	cout << "Mean: " << mean / nBins << endl;

	return 0;
}
