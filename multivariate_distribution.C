/**
 *
 *
 *           @name  multivariate_distribution.C
 *          @brief  
 *
 *          Example usage:
 *          @code
 *          	root -l multivariate_distribution.C+
 *          @endcode
 *
 *        @version  1.0
 *           @date  12/12/2014 (01:49:27 PM)
 *       @revision  none
 *       @compiler  gcc
 *
 *
 *         @author  P. Di Giglio (github.com/pdigiglio), <p.digiglio91@gmail.com>
 *        @company  
 *
 *
 */


#include "TString.h"
#include "TTree.h"
#include "TFile.h"
#include "TH2D.h"
#include "TMath.h"

#include <iostream>

using namespace std;
using namespace TMath;

const unsigned short int D = 6; //! Dimension of distribution.

	int
multivariate_distribution ( TString inTree = "multivariateDistrData.root" ) {

	// XXX Question: is there any way to show the content of a .root file whithin a script?
	// i.e. without loading it in ROOT

	/// Instance an input file and open my input `TTree`.
	TFile *inFile = TFile::Open( inTree, "READ" );
	if ( ( ! inFile ) || inFile->IsZombie()  ) {
		cerr << " >>> Error in opening " << inTree << endl;
		return 1;
	}

	/// Define a null `TTree` pointer and associate it with the input file via
	/// the method `TFile::GetOpject()`.
	///
	/// @attention This will return a `NULL` pointer if the input `TTree` is empty!
	TTree *multiFromFile = NULL;
	inFile->GetObject( "t", multiFromFile );
	if( ! multiFromFile ) {
		cerr << " >>> Error in creating TTree " << endl;
		cerr << " >>> TTree address: " << multiFromFile << endl;
		return 2;
	}

	/// Print the content of the `TTree` and see that there is one branch with descriptor
	/// `d[6]/D`.
	multiFromFile->Print();

	/// Declare a 6-dimensional `double` array and associate it with `d[6]/D`.
	double events[D] = {};
	multiFromFile->SetBranchAddress( "data", events );


	double mean[D] = {};
	for ( unsigned int event = 0; event < multiFromFile->GetEntries() ; ++ event ) {
		// put entry in my array
		multiFromFile->GetEntry( event );

		for ( unsigned short int i = 0; i < D; ++ i )
			mean[i] += events[i];
	}


	cerr << "~~~~~ begin (my)MEANS ~~~~~ "<< endl;
	for ( unsigned short int i = 0; i < D; ++ i ) {
		mean[i] /= multiFromFile->GetEntries();
		cerr << mean[i] << " ";
	}
	cerr << endl << "~~~~~ end (my)MEANS ~~~~~ "<< endl << endl;

	cerr << "~~~~~ begin COV ~~~~~ "<< endl;

	double cov = 0.;
	double cov1 = 0.;
	double cov2 = 0.;
	

	/// Declare a dummy 2D histogram pointer to evaluate covariance.
	/// @attention When you do an histogram, you _lose_ information
//	TH2D *dummy = NULL;
	for ( unsigned short int i = 1; i <= D; ++ i ) {
		for ( unsigned short int j = 0; j < i; ++ j ) {

			// Create empty dummy histogram.
//			dummy = new TH2D();

			// cycle over all entries
			for ( unsigned int event = 0; event < multiFromFile->GetEntries() ; ++ event ) {
				// get the event
				multiFromFile->GetEntry( event );

				// fill histogram
//				dummy->Fill( events[i], events[j] );

				// evaluate covariance
				cov += ( events[i] - mean[i] ) * ( events[j] - mean[j] );
				cov1 += events[i] * events[j];
				// this should be the true value because I know the cross product must
				// vanish in the average
				cov2 += events[i] * events[j] - mean[i] * mean[j];
			}

			// print results
			cout << cov / ( multiFromFile->GetEntries() - 1 )
				<< " (" << cov1 / multiFromFile->GetEntries() - mean[i] * mean[j] << ")"
				<< " [" << cov2 / multiFromFile->GetEntries() << "]"
//				<< " {" << dummy->GetCovariance() << "}"
				<< "\t";

			// reset variables
			cov  = 0.;
			cov1 = 0.;
			cov2 = 0.;
			delete dummy;
		}

		cout << endl << endl;
	}
	cerr << "~~~~~ end COV ~~~~~ "<< endl;

//	dummy->Draw();

	return 0;
}
