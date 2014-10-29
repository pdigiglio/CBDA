/*
 * ==================================================================
 *
 *       Filename:  monty_hall.C
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  28/10/2014 23:01:10
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  P. Di Giglio (), p.digiglio91@gmail.com
 *   Organization:  
 *
 * ==================================================================
 */
#include <TRandom.h>
#include <TH1I.h>

#include <time.h>

// number of doors
#define N 3

bool
no_change ( unsigned short int win ) {
	// select a choice
	if ( gRandom->Integer(N) == win )
		return 1;

	return 0;
}

bool
change ( unsigned short int win ) {

	// if you select the wrong door at the beginning, you win
	if( win != gRandom->Integer( N ) )
		return 1;
	
	// if you select the right door at the beginning, you loose
	return 0;
}

bool
no_change ( void ) {
	// select a choice
	if ( gRandom->Integer(N) == 0 )
		return 1;

	return 0;
}

bool
change ( void ) {

	// if you select the wrong door at the beginning, you win
	if( 0 != gRandom->Integer( N ) )
		return 1;
	
	// if you select the right door at the beginning, you loose
	return 0;
}

void
monty_hall ( void ) {
	// initialize random seed
	gRandom->SetSeed( 0 );

	// number of tries
	const unsigned int Tries = 10000;

	// create an integer histogram
	TH1I *histoNoChange = new TH1I( "HistoNoChange", "Win/Lost rates;Tries;N. of Wins", Tries + 1, -.5, Tries + .5 );
	TH1I *histoChange = new TH1I( "HistoChange", "Win/Lost rates;Tries;N. of Wins", Tries + 1, -.5, Tries + .5 );

	// number of wins
	unsigned short int winsNoChange = 0, winsChange = 0;

	// number of door with shinny car in
	// XXX not needed! I can always put the car behind door 0 if the competitor
	// choice is taken randomly
//	unsigned short int winDoor;
	
	for ( unsigned short int i = 0; i < Tries; ++ i ) {
		// select a winning door
//		winDoor = gRandom->Integer(N);

		// update NoChange counter
		winsNoChange += no_change( /* winDoor */ );
		// fill histogram
		histoNoChange->Fill( i, winsNoChange );

		// update Change counter
		winsChange += change( /* winDoor */ );
		// fill histogram
		histoChange->Fill( i, winsChange );
	}

	histoChange->Draw();
	histoNoChange->Draw("SAME");

//	return 0;
}		/* -----  end of function int monty_hall ( void )  ----- */
