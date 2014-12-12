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
	
	TTree *multivar = new TTree( inTree, "name" );
	multivar->Print();

	TFile *inFile = TFile::Open( inTree, "READ" );
	if ( ( ! inFile ) || inFile->IsZombie()  ) {
		cerr << " >>> Error in opening " << inTree << endl;
		return 1;
	}
	
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
