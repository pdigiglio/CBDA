#include <iostream>

#include "TFile.h"
#include "TTree.h"
#include "TH1.h"

/**
 * @desc Auxiliary function to evaluate the energy sum.
 *
 * This is actually quite useless since it's not recocnized in the `TTree::Draw()`
 * string parsing. Maybe it should be translated into a `TFormula`.
 */
    double
sum ( const double *a ) {
    return a[0] + a[1] + a[2];
}

/**
 * @desc The main function.
 *
 * @param inFileName name of the `.root` file containing the tree
 * @param treeName name of the `TTree` instance
 */
    void
draw3PiEnergySum( const char* inFileName = "3piTree.root",
                  const char* treeName   = "threePiTree" ) {

	/// Open file (in read-only mode) and retreive ROOT tree.
	TFile* inFile = TFile::Open(inFileName, "READ");

	/// Check if file exists and is okay.
	if ( not inFile or inFile->IsZombie() ) {
		cerr << "error opening file '" << inFileName << "'" << endl;
		return;
	}

	/// Declare a `NULL` pointer of `TTree` type.
	TTree* tree = NULL;
	/// Read the input tree into the `tree` pointer. This will automatically load all
	/// branches!
	inFile->GetObject( treeName, tree );
	// check if tree extists and is TTree
	if ( not tree ) {
		cerr << "error finding tree '" << treeName << "'" << endl;
		return;
	}

	// step II: define leaf variables and connect them to branches
//	double outPionEnergies[3];
//	tree->SetBranchAddress( "outPionEnergies", outPionEnergies );

	// step III: main data processing; loop over events
//	int   nmbEvents = tree->GetEntries();
//	for (int i = 0; i < nmbEvents; ++i) {  // event loop
//		tree->GetEntry(i);  // load data for event i into leaf variables
//
        // evaluate sum of enerfies
//        sum( outPionEnergies );
//	}

	/// Draw the sum of the energy components of the pion. They are stored in a
	/// 3-dimensional branch-array called `outPionEnergies[]`.
    tree->Draw("outPionEnergies[0] + outPionEnergies[1] + outPionEnergies[2]");
}
