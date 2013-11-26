/*
 * Random.h
 *
 *  Created on: 28 Dec 2012
 *      Author: tom
 */

#ifndef RANDOM_H_
#define RANDOM_H_

#include "standard.h"

class Random {
private:
public:
	Random();
	virtual ~Random();
	//static unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	static unsigned seed;
	static std::default_random_engine generator;
	static std::mt19937_64 generator64;
	static double unifRand();
	static double weibull(double gamma);
	static double exponential(double gamma);
	static double weibullMax(double gamma, double u, long long n);
	static double weibullMax(double gamma, long long n);
	static double weibullTrunc(double gamma);
};

#endif /* RANDOM_H_ */
