/**
 *
 *
 *           @name  linearRegression.C
 *          @brief  Implement a linear regression
 *
 *          This script is used to prove that the temperature in Vancouver is _not_ a linear
 *          funtction of the year i.e. it has not linearly increased over 100 years.
 *
 *          Example usage:
 *          @code
 *          	root -l linearRegression.C+
 *          @endcode
 *
 *        @version  1.0
 *           @date  01/14/2015 (02:10:26 PM)
 *       @revision  none
 *       @compiler  gcc
 *
 *
 *         @author  P. Di Giglio (github.com/pdigiglio), <p.digiglio91@gmail.com>
 *        @company  
 *
 */

#include <iostream>
using namespace std;


#include "TString.h"

#include "TGraph.h"
#include "TTree.h"
#include "TMath.h"

#include "TH1.h"
#include "TF1.h"
#include "TCanvas.h"


/**
 * @brief The main function.
 *
 * @param input Name of the data file containing average yearly temperatures in Vancouver.
 */
	int
linearRegression( const TString input = "linearRegression.dat" ) {

	// Read `input` data file.
	TTree *dataTree = new TTree();
	dataTree->ReadFile( input );

	// Define a new canvas for plotting.
	TCanvas *histoCanvas = new TCanvas( "Name", "data plot via TTree" );
	dataTree->Draw( "year:temp" );

//	return 0;

	// Get the number of data points.
	const unsigned int dataPoints = dataTree->GetSelectedRows();
	cout << endl
		 << " >> Number of data points: "
		 << dataPoints
		 << endl;

	// Get the addresses of the data arrays
	const double *xColumn = dataTree->GetV1();
	const double *yColumn = dataTree->GetV2();

	// Define a new `TGraph` containing data
	TGraph *dataGraph = new TGraph( dataPoints, xColumn, yColumn );

	// Define another `TCanvas` to plot the `TGraph`.
	TCanvas *graphCanvas = new TCanvas( "dummyName", "data plot via TGraph" );
	dataGraph->Draw( "A*" );

//	return 0;

	/**
	 * @par
	 * _Evaluate means_.
	 *
	 * Evaluate mean for \f$x\f$, \f$y\f$, \f$y^2\f$, \f$xy\f$, \f$x^2\f$. They are needed
	 * to evaluate variance and covariance later.
	 */
	double xMean = (double) 0;
	double yMean = (double) 0;
	double xyMean = (double) 0;
	double xxMean = (double) 0;
	double yyMean = (double) 0;
	for ( unsigned int j = 0; j < dataPoints; ++ j ) {
		xMean += xColumn[j];
		yMean += yColumn[j];
		xyMean += xColumn[j] * yColumn[j];
		xxMean += xColumn[j] * xColumn[j];
		xyMean += yColumn[j] * yColumn[j];
	}

	// Normalize results
	xMean /= (double) dataPoints;
	yMean /= (double) dataPoints;
	xyMean /= (double) dataPoints;
	xxMean /= (double) dataPoints;
	yyMean /= (double) dataPoints;

	const double xMeanSquare = xMean * xMean;
	const double xMeanyMean = xMean * yMean;

	/**
	 * Evaluate variance for \f$x\f$ and the covariance.
	 */
	double xVariance = (double) 0;
	double covariance = (double) 0;
	for ( unsigned int j = 0; j < dataPoints; ++ j ) {
		xVariance += xColumn[j] * xColumn[j] - xMeanSquare;
		covariance += yColumn[j] * xColumn[j] - xMeanyMean;
	}

	xVariance /= (double) dataPoints;
	covariance /= (double) dataPoints;


	/**
	 * With variance and covariance evaluate \f$\hat a = \mathrm{cov}[x,y]/\mathrm{var}[x]\f$
	 * and \f$\hat b = \langle y \rangle - \hat a \langle x \rangle\f$.
	 */
	const double aBest = covariance / xVariance;
	const double bBest = yMean - aBest * xMean;

	cout << endl
		 << " >> Best parameter estimates: "
		 << endl
		 << "_______a: " << aBest
		 << endl
		 << "_______b: " << bBest
		 << endl;
	
	// Define a `TF1` formula with the fitting function \f$a+bx\f$.
	TF1 *fitLine = new TF1(
			"fitLine",
			"[0]*x + [1]",
			TMath::MinElement( dataPoints, xColumn ),
			TMath::MaxElement( dataPoints, xColumn )
	);

	// Insert the parameters \f$a\f$ and \f$b\f$ into the formula.
	fitLine->SetParameter( 0, aBest );
	fitLine->SetParameter( 1, bBest );

	// Draw over the same `TGraph` plot.
	fitLine->Draw( "same" );

	/**
	 * @par
	 * _Evaluate \f$\chi^2\f$_.
	 *
	 * With a linear function, the \f$\chi^2(a,b)\f$ distribution is
	 * \f$\chi^2(a,b) = n(\langle y^2\rangle + a^2\langle y^2\rangle
	 * + b^2 -a\langle xy\rangle - b\langle y\rangle - ab \langle x\rangle)\f$.
	 * Nevertheless, this formula is __numerically not stable__. In fact, if I try to use it
	 * I __get a result which is crap!__
	 * That's why the \f$\chi^2\f$ is evaluated by looping over the measures and averaging
	 * \f$(y_i - \hat a x_i - \hat b)^2\f$. This gives __the right result.__
	 */
//	double chiSquare = - aBest * xyMean - bBest * yMean + aBest * bBest * xMean;
//	chiSquare *= 2;
//	chiSquare += yyMean + aBest * aBest * xxMean + bBest* bBest;
//	chiSquare *= dataPoints;

	double chiSquare = 0.;
	double tmp = 0.;
	for ( unsigned int j = 0; j < dataPoints; ++ j ) {
		tmp = yColumn[j] - aBest * xColumn[j] - bBest;
		chiSquare += tmp * tmp;
	}

//	chiSquare /= (double) dataPoints;

	unsigned int nPars = 2;
	cout << endl << " >> ChiSquare for the fit: " << chiSquare << endl;
	cout << " >> DOFs: "
		 << dataPoints << " (# of data) - "
		 << nPars << " (# of estimated par.) = "
		 << dataPoints - nPars << endl;
	cout << " >> Reduced ChiSquare for the fit: " << chiSquare/( (double) dataPoints - nPars) << endl;
	cout << " >> Expected reduced ChiSquare for the fit: 1 +/- " << TMath::Sqrt( 2 / ( (double) dataPoints - nPars ) ) << endl;

	/**
	 * @par
	 * _Delete `TCanvas`es_.
	 *
	 * @attention If you delete canvas in a ROOT script, you will not see the plots
	 * anymore!
	 */
//	delete histoCanvas;
//	delete graphCanvas;

	return 0;
}
