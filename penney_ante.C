/*
 * =====================================================================================
 *
 *       Filename:  penney_ante.C
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/01/2014 10:53:58 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  P. Di Giglio (github.com/pdigiglio), p.digiglio91@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */


#include <iostream>

#include <TH1I.h>
#include <TRandom.h>

using namespace std;

	std::string
getBits ( unsigned char x ) {

	std::string output;
	for( short int i = ( sizeof( unsigned char ) * 8 ) - 1; i >= 0; -- i )
		( x & ( 1 << i ) ) ? output.append("1") : output.append("0");
	return output;
}		/* -----  end of function getbits  ----- */

	void
showbits( unsigned char x ) {
	std::cout << getBits( x ) << std::endl;
//	for( short int i = ( sizeof( unsigned char ) * 8 ) - 1; i >= 0; -- i )
//		( x & ( 1 << i ) ) ? putchar('1') : putchar('0');
//
//	printf("\n");
}	/* -----  end of function showbits  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  newSequence
 *  Description:  find the new sequence from the one given as argument
 * =====================================================================================
 */
	unsigned char
newSequence ( unsigned char oldSeq ) {
	// (0,..., 0, 0, c2, c1 )
	unsigned char newSeq = oldSeq & 3;
	// (0,..., 0, c2, c1, 0 )
	newSeq <<= 1;
	// (0,..., 0, c2, c1, c2 )
	newSeq |= ( newSeq >> 2 );

	// (0,..., 0, c2, c1, c2 )
	return ( newSeq ^ 4 );
}	/* -----  end of function newSequence  ----- */


	int
penney_ante ( void ) {
//main ( void ) {

	// set random seed
	gRandom->SetSeed( 0 );

	// number of coin tosses
	const int Tries = 1000000;

	// 7 is 00...00111 in binary
	const unsigned char cut = 7;
   
	// histogram
	TH1I *histo = new TH1I( "Coin Toss", "Wins;Sequencences;N. of wins", 8, -.5, 7.5 );
	TH1I *notHisto = new TH1I( "Coin Toss", "Wins;Sequencences;N. of wins", 8, -.5, 7.5 );
	notHisto->SetLineColor( kRed );

	unsigned char notChoice;
	unsigned char results;
	// string to edit bin labels
	std::string binName;

	bool notWin;
	// cycle over all possible combinations of a 3-string of {1,0} values
	for ( unsigned char Choice = 0; Choice < 8; ++ Choice ) {
//		showbits( Choice );

		// give the value to notChoice
		notChoice = newSequence( Choice );
//		showbits( notChoice );

		// cut the first 5 zeros in the representation
		binName = getBits( Choice ).substr( 5 );
		// set the bin labels to the string of choices
		histo->GetXaxis()->SetBinLabel( Choice + 1, (char *) binName.c_str() );

		// play the game Tries times
		for ( register unsigned int i = 0; i < Tries; ++ i ) {

			// pick up a random 3-string
			results = (unsigned char) gRandom->Integer(8);

			// till someone wins
			notWin = true;
			while (  notWin ) {
				// check if I got the right (non-)Choice
				if( Choice == results ) {
					histo->Fill( (unsigned) Choice, 1 );
					notWin = false;
				}
				// don't put 'else' because sequences can be equal
				if ( notChoice == results ) {
					notHisto->Fill( (unsigned) Choice, 1 );
					notWin = false;
				}

				// free a bit
				results <<= 1;
				// push the new coin toss
				results |= gRandom->Integer( 2 );
				// cut the result
				results &= cut;
			}
		}
	}

	histo->Draw();
	notHisto->Draw("same");

	return 0;
}		/* -----  end of function penny_ante  ----- */
