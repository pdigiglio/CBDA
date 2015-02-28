/**
 * @file buildTree.C
 *
 * @brief ROOT script to build a `TTree`
 */

#include "TFile.h"
#include "TTree.h"
#include "TRandom.h"


/**
 * @brief The main function
 *
 * @param outFileName file name of the output `TTree`
 * @param treeName name of the actual `TTree`
 */
void buildTree(const char* outFileName = "test.root",
               const char* treeName    = "test")
{
	// step I: create file
	TFile* outFile = TFile::Open(outFileName, "RECREATE");

	// step II: create tree
	TTree* tree = new TTree(treeName, "");

	// step III: add branches to tree
	int nHits;
	tree->Branch("nHits", &nHits, "nHits/I");

	const int maxSize = 500;  // careful here
	double hitAmp[maxSize];
	tree->Branch("hitAmp", hitAmp, "hitAmp[nHits]/D");
	
    double hitCoord[maxSize][3];
	tree->Branch("hitCoord", hitCoord, "hitCoord[nHits][3]/D");

	// step IV: fill tree
	int nEvents = 10000;
	int maxHit  = 10;
	gRandom->SetSeed();  // initialize ROOT's random generator
	for (int i = 0; i < nEvents; ++i) {  // loop over events
		// simulate experimental data using random values
		nHits = gRandom->Integer(maxHit) + 1;  // 1 ... maxHit
		for (int j = 0; j < nHits; ++j) {  // loop over hits in event
			hitAmp[j]      = gRandom->Landau(100, 20);
			hitCoord[j][0] = gRandom->Gaus( 0,  1);
			hitCoord[j][1] = gRandom->Gaus( 0,  1);
			hitCoord[j][2] = gRandom->Gaus(10, 10);
		}
		tree->Fill();  // fill values for event i into tree
	}

	// step V: write data to disk
	outFile->Write();  // write tree
	outFile->Close();  // also deletes tree
	delete outFile;
}
