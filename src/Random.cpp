/*
 * Random.cpp
 *
 *  Created on: 28 Dec 2012
 *      Author: tom
 */
#include "Random.h"

unsigned Random::seed = std::chrono::system_clock::now().time_since_epoch().count();
std::default_random_engine Random::generator = std::default_random_engine(seed);
std::mt19937_64 Random::generator64 = std::mt19937_64(seed);

// unsigned Random::seed = e((unsigned int)time(0));;

Random::Random() {

}

double Random::exponential(double lambda) {
	std::exponential_distribution<double> distribution(lambda);
	distribution.reset();
	return distribution(generator);
}

double Random::unifRand()
{
	return rand() / double(RAND_MAX);
}

double Random::weibull(double gamma)
{
	std::weibull_distribution<double> distribution(gamma,1.0);
	distribution.reset();
	return distribution(generator64);
}

double Random::weibullMax(double gamma, double u, long long n)
{
	return pow( -log( 1-exp(log( u )/n) ) , 1/gamma);
}

double Random::weibullTrunc(double gamma) 
{
	std::uniform_real_distribution<double> distribution(0.0,1.0);
	distribution.reset();
	return pow( -log( distribution(generator64) ) + pow(2,gamma) , 1/gamma);
}

double Random::weibullMax(double gamma, long long n)
{
	std::uniform_real_distribution<double> distribution(0.0,1.0);
	distribution.reset();
	return weibullMax(gamma, distribution(generator64), n);
}

Random::~Random() {
	// TODO Auto-generated destructor stub
}
