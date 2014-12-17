/**
 *
 *
 *           @name  primakoff.C
 *          @brief  ROOT script to analyze the _Primakoff reaction_.
 *
 *          The input data are _actual_ data measured in the COMPASS experiment at CERN.
 *
 *          Example usage:
 *          @code
 *          @endcode
 *
 *        @version  1.0
 *           @date  12/17/2014 (02:27:40 PM)
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
#include "TLorentzVector.h"
#include "TH1D.h"
#include "TCanvas.h"

#include "TMath.h"

#include <iostream>

using namespace std;
using namespace TMath;

	int
primakoff(
		   TString TTreeFileName = "primakoff.root",
		   TString TreeName = "PT"
		) {

	/// Instance an input file and open my input `TTree`.
	TFile *inFile = TFile::Open( TTreeFileName, "READ" );
	if ( ( ! inFile ) || inFile->IsZombie()  ) {
		cerr << " >>> Error in opening " << TTreeFileName << endl;
		return 1;
	}

	/// Define a null `TTree` pointer and associate it with the input file via
	/// the method `TFile::GetOpject()`.
	///
	/// @attention This will return a `NULL` pointer if the input `TTree` is empty!
	TTree *tree = NULL;
	inFile->GetObject( TreeName, tree );
	if( ! tree ) {
		cerr << " >>> Error in creating TTree " << endl;
		cerr << " >>> TTree address: " << tree << endl;
		return 2;
	}

	tree->Print();

	//---------------------------------------------------------------------------------//

	// histogram for energies of photons, particles and beam
	TH1D *energy[4] = {};

	/// The first photon has a broad peak.
	TLorentzVector *g1 = new TLorentzVector();
	/// @attention The method 'SetBranchAddress()` requires the address of a pointer.
	tree->SetBranchAddress( "g1", &g1 );

	/// These photons are indistinguishable but the histograms are not equal!
	/// Here we distingueshed them by saying "photon 1 has more energy than photon 2",
	/// on average.
	TLorentzVector *g2 = new TLorentzVector();
	tree->SetBranchAddress( "g2", &g2 );

	TLorentzVector *pip = new TLorentzVector();
	tree->SetBranchAddress( "pip", &pip );

	/// Energy of the beam is not very constant.
	/// We would expect a peak at \f$190\f$\,GeV.
	/// COMPASS doesn't really measure the energy of the beam directly. It reconstruct it
	/// from the scattering angles and energies.
	/// What is happening here is that the reconstruction is wrong.
	TLorentzVector *beam = new TLorentzVector();
	tree->SetBranchAddress( "beam", &beam );


	energy[0] = new TH1D( "energy", "Energy for #nu_{1}", 200, 0, 200 );
	energy[1] = new TH1D( "energy", "Energy for #nu_{2}", 200, 0, 20 );
	energy[2] = new TH1D( "energy", "Energy for #pi^{0}", 200, 0, 160 );
	energy[3] = new TH1D( "energy", "Energy for beam", 200, 160, 220 );
	for ( unsigned int j = 0; j < tree->GetEntries(); ++ j ) {
		tree->GetEntry(j);

		// fill histograms
		energy[0]->Fill( g1->T() );
		energy[1]->Fill( g2->T() );
		energy[2]->Fill( pip->T() );
		energy[3]->Fill( beam->T() );
	}

	// plot histograms
	for ( unsigned short k = 0; k < 4; ++ k) {
		new TCanvas();
		energy[k]->DrawCopy( "E" );
	}

	// XXX this doesn't work!!
//	delete [] energy;

	//---------------------------------------------------------------------------------//


	TH1D *invMass = new TH1D( "IM", "Invariant mass of #nu_{1} + #nu_{2} system", 300, 0, .2 );
	TH1D *invMassCut = new TH1D( "IMC", "Invariant mass of #nu_{1} + #nu_{2} system (cut)", 300, 0, .2 );
	TH1D *piEnergyCut[2] = {
		new TH1D( "PEC", "#pi^{0} energy for m_{#gamma#gamma} #in [0.06, 0.08] GeV;E_{#pi^{0}} GeV;", 200, 0, 160 ),
		new TH1D( "PEC", "#pi^{0} energy for m_{#gamma#gamma} #in [0.115, 0.15] GeV;E_{#pi^{0}} GeV;", 200, 0, 160 )
	};


	TLorentzVector *GammaGamma = new TLorentzVector();
	double m;
	for ( unsigned int j = 0; j < tree->GetEntries(); ++ j ) {
		tree->GetEntry(j);

		// gamma-gamma 4-momentum
		*GammaGamma = *g1 + *g2;

		/// One can use the `TLorentzVector::M()` to get the mass of the particle or can
		/// take \f$m^2 = (p_1 + p_2) ^2\f$.
		m = GammaGamma->M();

		// fill mass histogram
		invMass->Fill( m );


		/// By applying the cut \f$187 < E_\textup{beam} < 193\f$ we are getting rid of
		/// some noise which comes from the the beam dispersion in energy.
		if ( 187. < beam->T() && beam->T() < 193. ) {
			invMassCut->Fill( m );
		}
		
		if( .06 < m && m < .08 ) {
			piEnergyCut[0]->Fill( pip->T() );
		}

		if( .115 < m && m < .15 ) {
			piEnergyCut[1]->Fill( pip->T() );
		}
	}

	new TCanvas();
	invMass->DrawCopy( "E" );
	new TCanvas();
	invMassCut->DrawCopy( "E" );

	// plot pion
	for ( unsigned short k = 0; k < 2; ++ k ) {
		new TCanvas();
		piEnergyCut[k]->DrawCopy( "E" );
	}


	return 0;
}
