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

#include <iostream>

using namespace std;

	int
multivariate_distribution ( TString inTree = "multivariateDistrData.root" ) {
	
	/// Instance a `TTree` object and associate it with the input `TTree`.
	TTree *multivar = new TTree( inTree, "name" );
	multivar->Print();

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
	inFile->GetObject( "multivariateDistrData", multiFromFile );
	if( ! multiFromFile ) {
		cerr << " >>> Error in creating TTree " << endl;
		cerr << " >>> TTree address: " << multiFromFile << endl;
		return 2;
	}

	multiFromFile->Print();

	return 0;
}
