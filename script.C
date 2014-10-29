/*
 * ==================================================================
 *
 *       Filename:  script.C
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  22/10/2014 14:39:09
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  P. Di Giglio (), p.digiglio91@gmail.com
 *   Organization:  
 *
 * ==================================================================
 */
#include <iostream>

using namespace std;

// When a function has the same name of a script, it's executed automatically
void
script ( int i ) {
	double x = 3.3;
	cout << x / i << endl;
}		/* -----  end of function script  ----- */
