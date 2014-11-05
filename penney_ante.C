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

	void
showbits( short int x ) {
	for( short int i = ( sizeof( short int ) * 8 ) - 1; i >= 0; -- i )
		( x & ( 1 << i ) ) ? putchar('1') : putchar('0');

	printf("\n");
}	/* -----  end of function shobbits  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  new_sequence
 *  Description:  find the new sequence from the one given as argument
 * =====================================================================================
 */
	unsigned short int
new_sequence ( unsigned short int oldSeq ) {
	// (0,..., 0, 0, c2, c1 )
	unsigned short newSeq = oldSeq & 3;
	// (0,..., 0, c2, c1, 0 )
	newSeq <<= 1;
	// (0,..., 0, c2, c1, c2 )
	newSeq |= ( newSeq >> 2 );

	// (0,..., 0, c2, c1, c2 )
	return ( newSeq ^ 4 );
}	/* -----  end of function new_sequence  ----- */

	int
//penney_ante ( void ) {
main ( void ) {
	gRandom->SetSeed( 0 );

	// number of coin tosses
	const int Tries = 1000000;

	// 7 is 00...00111 in binary
	const unsigned short int cut = 7;

	// choose a random 3-sequence
	const unsigned short Choice = gRandom->Integer( 8 );
//	showbits( Choice );
	const unsigned short notChoice = new_sequence( Choice );
//	showbits( notChoice );

	// histogram
	TH1I *histo = new TH1I( "Coin Toss", "Wins;Sequencences;N. of wins", 2, .5, 2.5 );

	// list of results (first 3 outcomes taken randomly)
	unsigned short int results = gRandom->Integer(8);
	unsigned int i1 = 0, i2 = 0;
	for ( unsigned int i = 0; i < Tries; ++ i ) {

		// check if I got the right (non-)Choice
		if( Choice == results ) {
//			histo->Fill( 1, 1 );
			++ i1;
//			cout << "Choice wins: " << histo->GetBinContent( 1 ) << endl;
		}
		if ( notChoice == results ) {
//			histo->Fill( 2, 1 );
			++ i2;
//			cout << "notChoice wins: " << histo->GetBinContent( 2 ) << endl;
		}

		// free a bit
		results <<= 1;
		// push the new coin toss
		results |= gRandom->Integer( 2 );
		// cut the result
		results &= cut;
	}

	cout << (long double) i1/i2 << endl;
//	cout << "choice: " << i1 << " not-choice: " << i2 << endl;

//	histo->Draw();
	exit(0);
	return 0;
}		/* -----  end of function penny_ante  ----- */
