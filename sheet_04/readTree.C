/**
 * @file readTree.C
 *
 * @brief Example of reading a ROOT tree.
 *
 */

#include <iostream>

#include "TFile.h"
#include "TTree.h"
#include "TH1.h"


/**
 * @brief The main function
 *
 * @oaram inFileName actual file name of the tree
 * @param treeName the name of the tree contained in the file
 */
void readTree(const char* inFileName = "test.root",
              const char* treeName   = "test")
{

	// step I: open file and retreive ROOT tree
	TFile* inFile = TFile::Open(inFileName, "READ");
	// check if file exists and is okay
	if ( not inFile or inFile->IsZombie() ) {
		cerr << "error opening file '" << inFileName << "'" << endl;
		return;
	}

	// get tree from file
	TTree* tree = 0;
	inFile->GetObject(treeName, tree);
	// check if tree extists and is TTree
	if ( not tree ) {
		cerr << "error finding tree '" << treeName << "'" << endl;
		return;
	}

	// step II: define leaf variables and connect them to branches
	int       nHits;

	// this is just an overestimated size for the number of hits
	const int maxSize = 500;  // careful here
	double    hitAmp  [maxSize];
	double    hitCoord[maxSize][3];
	tree->SetBranchAddress("nHits",    &nHits);
	tree->SetBranchAddress("hitAmp",   hitAmp);
	tree->SetBranchAddress("hitCoord", hitCoord);

	// step III: main data processing; loop over events
	TH1D* hist      = new TH1D("hist", "Example", 100, -60, 60);
	int   nmbEvents = tree->GetEntries();
	for (int i = 0; i < nmbEvents; ++i) {  // event loop
		tree->GetEntry(i);  // load data for event i into leaf variables

//		cout << nHits << " " << maxSize << endl;
		
		// place your analysis code here
		for (int j = 0; j < nHits; ++j) {
			hist->Fill( hitCoord[j][2] ); }
	}
	// histogram drawing, fitting, parameter extraction, ...
	hist->Draw("E");

}
