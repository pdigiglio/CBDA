/**
 * @file charge.C
 * @brief Simple ROOT script to plot an histogram from a data file using `TTree`.
 *
 * Take a `.txt` file as input, store it in a `TTree`. Then plot the histogram of the
 * whole data file and get the mean value. To do it in a smart way, one can pass through
 * a `TH1F` histogram. Then apply a `TCut` and evaluate again the mean. This is to show
 * that the mean estimator is _not robust_ i.e. is sensitive to distribution tails.
 */

#include <iostream>

#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
// To use cutting data condition.
#include "TCut.h"
#include "TCanvas.h"

/**
 * @brief The main function of the ROOT script.
 *
 * @param inFileName name of the input text file containing data
 * @param treeName name I will give to my `TTree` object
 * @param descriptor string containing the descriptor for the branches of the `TTree`
 */
    int
charge ( const char inFileName[] = "chargedata.txt",
         const char   treeName[] = "charge",
         const char descriptor[] = "charge/D" ) {

    /// Declare an empty `TTree` using `new` memory allocator.
    TTree *tree = new TTree();
	
	/// Read the input file using the `inFileName` name and the `descriptor` parameters.
	/// In this case I only have one branch (which I will call "charge")
	/// of `double` values.
    tree->ReadFile( inFileName, descriptor );

	// evaluate maximum and minimum for my histogram
    double min = tree->GetMinimum( "charge" );
    double max = tree->GetMaximum( "charge" );
    double binWidth = .5;
	// number of bins of the histogram
    int bins = ( (double) ( max - min ) / binWidth );

    /// Create a dumb histogram to evaluate mean and root mean square. This is useful
	/// because then I can access them via `TH1::GetMean()` and `TH1::GetRMS()` methods.
    TH1D *histogram = new TH1D( "histo", "Charge not cutted", bins + 2 , min - binWidth, max + binWidth );
    TH1D *histogram2 = new TH1D( "histo2", "Charge cutted", bins + 2 , min - binWidth, max + binWidth );

    /// Draw the branch filling my histogram (this will evaluate mean and RMS).
    tree->Draw( "charge >> histo" );

	cout << " ~~~ NO FILTER RESULTS ~~~ " << endl;
    cout << "Mean: " << histogram->GetMean() << " RMS: " << histogram->GetRMS() << endl;

    /// I add a filter for my data, ignoring all values greater than \f$30\,\mbox{nC}\f$.
	TCut chargeCut = "charge < 30";

	new TCanvas;
	/** Then I plot and process them using the same histogram to get mean and RMS. */
	tree->Draw( "charge >> histo2", chargeCut );

	cout << " ~~~ FILTER RESULTS ~~~ " << endl;
    cout << "Mean: " << histogram->GetMean() << " RMS: " << histogram->GetRMS() << endl;
    
	return 0;
}
