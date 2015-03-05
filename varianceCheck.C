/**
 *
 *
 *           @name  varianceCheck.C
 *
 *        @version  1.0
 *           @date  03/05/2015 (06:28:34 PM)
 *       @revision  none
 *       @compiler  gcc
 *
 *
 *         @author  P. Di Giglio (github.com/pdigiglio), <p.digiglio91@gmail.com>
 *        @company  
 *          @brief  
 *
 *          Example usage:
 *          @code
 *          @endcode
 *
 *
 */


#include <cstdlib>
#include <iostream>
#include <ctime>

const size_t SIZE = 100000;

	int
varianceCheck() {

	srand( time( NULL ) );


	const double scale = 10.;

	/**
	 * Declare and fill an array of doubles
	 */
	double *data = new double[SIZE];
	double mean = 0.;
	for ( unsigned int j = 0; j < SIZE; ++ j ) {
		data[j] = (double) scale * rand() / RAND_MAX;

		/* evaluate mean while generating numbers */
		mean += data[j];
	}

	/* normalize mean */
	mean /= (double) SIZE;

	
	/**
	 * Compare three ways for evaluating the variance
	 */
	double varianceBad = 0.;
	double varianceGood = 0.;
	double varianceDontKnow = 0.;

	for ( unsigned int j = 0; j < SIZE; ++ j ) {

		varianceBad += data[j]*data[j];
		varianceDontKnow += data[j] * data[j] - mean * mean;
		varianceGood += ( data[j] - mean ) * ( data[j] - mean );

	}
	
	varianceBad /= SIZE;
	varianceGood /= SIZE;
	varianceDontKnow /= SIZE;

	std::cout << "(bad, good, don't know): " << std::endl
		<< varianceBad << std::endl
		<< varianceGood << std::endl
		<< varianceDontKnow << std::endl;

	return 0;
}
