/**
 *
 *
 *           @name  decision_wheel.C
 *          @brief  This program models a _decision wheel_.
 *
 *          Apparently, it's very useful if you are a manager! ;)
 *
 *          Example usage:
 *          @code
 *          root -l decision_wheel.C+
 *          @endcode
 *
 *        @version  1.0
 *           @date  02/23/2015 (02:01:10 PM)
 *       @revision  none
 *       @compiler  gcc
 *
 *
 *         @author  P. Di Giglio (github.com/pdigiglio), <p.digiglio91@gmail.com>
 *        @company  
 *
 *
 */

#include <iostream>
#include "TRandom.h"

using namespace std;

/**
 * @brief Mapping between integers and strings.
 *
 * The length is intentionally left blank so that one can add or remove entries without
 * worrying about code breakings. The size of the table (i.e. its number of elemets) will
 * be staored in `numOutcomes`.
 */
const char *conversionTable[] = {
	"Yes",
	"Today",
	"Pass the buck",
	"Maybe",
	"Reorganize",
	"No",
	"Tomorrow",
	"Sit on it"
};

/**
 * @brief Number of string in the table (\f$N_\textup{choices}\f$).
 */
const unsigned short int numOutcomes = sizeof( conversionTable ) / sizeof( conversionTable[0] );

/**
 * @brief Print a random choice.
 *
 * The functions prints a random outcome of the spinning. A random integer in
 * \f$\{0,\dots, N_\textup{choices}-1\}\f$ will be generated and the corresponding entry
 * of `conversionTable[]` will be printed to the `stdout`.
 *
 * The pdf for the random numbers will be flat, i.e. \f$f(x) \equiv 1/N_\textup{choices}\f$.
 */
	void
choice() {
	cout << endl
		 << "~~~ Spinning the _magic decision wheel_ ~~~ " << endl << endl;

	cout << "What you have to do is: " << endl;
	cout << conversionTable[ gRandom->Integer( numOutcomes ) ] << endl << endl;
}

	int
decision_wheel () {

	choice();

	return 0;
}
