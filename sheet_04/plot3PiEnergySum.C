#include <iostream>

#include "TFile.h"
#include "TTree.h"
#include "TH1.h"


void plot3PiEnergySum(const char* inFileName = "3piTree.root",
                      const char* treeName   = "threePiTree")
{

	// step I: open file and retreive ROOT tree
	TFile* inFile = TFile::Open(inFileName, "READ");
	// check if file exists and is okay
	if (not inFile or inFile->IsZombie()) {
		cerr << "error opening file '" << inFileName << "'" << endl;
		return;
	}
	// get tree from file
	TTree* tree = 0;
	inFile->GetObject(treeName, tree);
	// check if tree extists and is TTree
	if (not tree) {
		cerr << "error finding tree '" << treeName << "'" << endl;
		return;
	}

	// step II: define leaf variables and connect them to branches
	double outPionEnergies[3];
	tree->SetBranchAddress("outPionEnergies", outPionEnergies);

	// step III: main data processing; loop over events
	TH1D* hist      = new TH1D("energySum", "Energy Sum", 220, 0, 220);
	int   nmbEvents = tree->GetEntries();

	for ( int i = 0; i < nmbEvents; ++ i ) {  // event loop
		tree->GetEntry(i);  // load data for event i into leaf variables
		
		// place your analysis code here
	
		double eSum = outPionEnergies[0] + outPionEnergies[1] + outPionEnergies[2];
		hist->Fill(eSum);
	}
	// histogram drawing, fitting, parameter extraction, ...
	hist->Draw("E");

}
