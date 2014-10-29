/*
 * ==================================================================
 *
 *       Filename:  first_script.C
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  22/10/2014 14:22:32
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  P. Di Giglio (), p.digiglio91@gmail.com
 *   Organization:  
 *
 * ==================================================================
 */

// XXX never leave unnamed functions

{
	// do this in every unnamed script
	gROOT->Reset();

	double a = 3.3;
	// drop 'std' namespace because it is default in ROOT
	cout << a / 3 << endl;
}
