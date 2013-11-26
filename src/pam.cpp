#include "pam.h"

template<class T1, class T2>
struct interval
{
	T1 lower;
	T2 upper;
	interval(const T1 &lower, const T2 &upper) : lower(lower), upper(upper) {};
};

typedef BigDecimal<mpf_t> bigDecimal;

namespace PAM {
	template<class T>
	BigDecimal<T> findDet(std::vector<BigDecimal<T>>& vector, 
			double addThisToDiagonal = 0) 
	{
	BigDecimal<T> det(0.0);
	BigDecimal<T> x(0.0), y(1.0);

	BigDecimal<T> buffer(0.0); // saves on heap allocations
	BigDecimal<T> addThis(addThisToDiagonal); // saves on heap allocations;
		for (typename std::vector<BigDecimal<T>>::iterator it = vector.begin();
			       it != vector.end(); ++it)
		{
			// f_n = f_{n-1} * (getData() + addThisToDiagonal) - f{n-2}
			buffer = *it;
			buffer += addThis;
			det = y * buffer;
			det -= x; 
			x = y;
			y = det;
		}
		return det;
	}
	template<class T>
	BigDecimal<T> getMax(std::vector<BigDecimal<T>>& vector) 
	{
		BigDecimal<T> max = vector.front();
		for (typename std::vector<BigDecimal<T>>::iterator 
				it = vector.begin(); it != vector.end(); ++it)
		{
			max = (*it > max) ? *it : max;
		}
		return max;
	}
	template<class T>
	BigDecimal<T> getMin(std::vector<BigDecimal<T>>& vector)
	{
		BigDecimal<T> min = vector.front();
		for (typename std::vector<BigDecimal<T>>::iterator 
				it = vector.begin(); it != vector.end(); ++it)
		{
			min = (*it < min) ? *it : min;
		}
		return min;
	}
	template<class T>
	double getLowerbound(const T& max) {
		double maxD = (double) max;
		return maxD - 2 + 2 / ( maxD );
	}
	template<class T>
	interval<double,double> 
		bruteforceEigenvalue(std::vector<BigDecimal<T>>& vector) 
	{
		BigDecimal<T> det;
		BigDecimal<T> detBuffer;
		const BigDecimal<T> zero(0.0);
		const double gamma = 2;
		const double step = -1/gamma * log(0.95) / exp( log(log(vector.size())) * 1/gamma );
		assert(step != 0);
		const int numWhenToStop =  58; // log(0.05) / log(0.95);
		const BigDecimal<T> vectorMax = getMax(vector);
		const double bruteStart = getLowerbound(vectorMax);
		double workingEigenValue = bruteStart;

		short detSignPrevious, detSignCurrent;
		int counter = 0;
		det = findDet(vector, -workingEigenValue);
		detSignCurrent = ( det > zero ) ? 1 : -1;
		workingEigenValue += step;
		do {
			det = findDet(vector, -2-workingEigenValue);
			detSignPrevious = detSignCurrent;
			detSignCurrent = (det > zero) ? 1 : -1;
			if( detSignPrevious != detSignCurrent ) counter = 1;
			else if( (counter >= 1)
					&& (det > detBuffer) ) counter++;
			else counter=0;
			det = detBuffer;
			if( (BigDecimal<T>) workingEigenValue > vectorMax )
				throw std::exception();

			workingEigenValue += step;
		} while(counter < numWhenToStop);
		workingEigenValue -= (numWhenToStop+1)*step;
		double upperBound = workingEigenValue + step;

		return interval<double, double>(workingEigenValue, upperBound);
	}
	template<class T>
	BigDecimal<T> getNorm(const std::vector<BigDecimal<T>>& vector,
			double p = 2)
	{
		assert(p == 2);		

		BigDecimal<T> norm(0);
		for(typename std::vector<BigDecimal<T>>::size_type i = 0; 
				i < vector.size(); i++)
		{
			norm += vector[i]*vector[i]; 
		}
		norm = sqrt(norm);

		return norm;
	}
	template<class T>
	BigDecimal<T> multiplyByMatrix(std::vector<BigDecimal<T>>& vector,
		      const std::vector<BigDecimal<T>>& matrix) 
	{
		assert(vector.size() == matrix.size());

		BigDecimal<T> eigenvalue(0.0);
		BigDecimal<T> previous(0.0);
		BigDecimal<T> current(0.0);
		BigDecimal<T> newval(0.0);
		typename std::vector<BigDecimal<T>>::size_type i;
		// eigenvalue = Sum (v_{i-1} + (matrix(i,i))*v_i + v_{i+1}) * v_i
		for(i = 0; i < vector.size() - 1; i++) {
			current = vector[i];
			newval = previous + current*(matrix[i]) + vector[i+1];
			eigenvalue += current * newval;
			vector[i] = newval;
			previous = current;
		}
		newval = previous + vector[i] * (matrix[i]);
		eigenvalue += vector[i] * newval;
		vector[i] = newval;
		
		return eigenvalue;
	}
}

void handler(int sig) {
	void *array[10];
	size_t size;

	// get void*'s for all entries on the stack
	size = backtrace(array, 10);

	// print out all the frames to stderr
	fprintf(stderr, "Error: signal %d:\n", sig);
	backtrace_symbols_fd(array, size, STDERR_FILENO);
	exit(1);
}

int main(int argc, char* argv[] ) 
{
	signal(SIGSEGV, handler); 

	double gamma;
	int base, power, numOfTimes;

	namespace po = boost::program_options;
	po::options_description generic("Generic options");
	generic.add_options()
			("mode,M", po::value<std::string>(), "Mode to run. \n"
					"E: \tfind Eigenvalue by calculating determinant\n"
					"I: \tfind Eigenvalue and Eigenvector by Power Iteration\n"
					"T: \tfind Eigenvalue by both methods")
			("gamma,G", po::value<double>(&gamma)->default_value(2),
					"Gamma in Weibull Distribution")
			("base,B", po::value<int>(&base)->default_value(10),
					"Samples size = base^power")
			("power,P", po::value<int>(&power)->default_value(4),
					"Samples size = base^power")
			("times,T", po::value<int>(&numOfTimes)->default_value(1),
					"Repeat this number of times")
	;

	po::positional_options_description p;
	p.add("mode", 1);
	p.add("gamma", 1);
	p.add("base", 1);
	p.add("power", 1);

	po::variables_map vm;
	po::store(po::command_line_parser(argc, argv)
				.options(generic).positional(p).run(), vm);
	po::notify(vm);
	const unsigned long long SIZE = pow(base, power);

	clock_t timer = clock();
	if (vm.count("mode"))
	{
		switch( vm["mode"].as<std::string>()[0]) {
			case 'E':
			{
				for(int i=0; i<numOfTimes; i++) {
					std::unique_ptr<std::vector<bigDecimal>>
						vectorPtr(new std::vector<bigDecimal>(SIZE));
					for_each(vectorPtr->begin(), vectorPtr->end(), [&gamma] (bigDecimal& bd) 
					{
						bd = (bigDecimal) Random::weibullTrunc(gamma);
					});
					double eigenvalue(PAM::bruteforceEigenvalue(*vectorPtr).lower);
					std::cout << eigenvalue << std::endl;
					double vectorMax(PAM::getMax(*vectorPtr)); 
					double lowerBound = ( vectorMax - 2 + sqrt( (vectorMax-2)*(vectorMax-2) + 8) ) / 2;
					std::cout << "Lower bound: " << lowerBound << std::endl;
					
					if(eigenvalue < lowerBound) {
						std::cout << "Lowerbound is higher than eigenvalue. Terminating." << std::endl;	
						return 1;
					}
				}
			}
			break;
			case 'T':
			{
				for(int i=0; i<numOfTimes; i++) {
					std::unique_ptr<std::vector<bigDecimal>> 
						vectorPtr(new std::vector<bigDecimal>(SIZE, 1.0));
					std::unique_ptr<std::vector<bigDecimal>> 
						matrixPtr(new std::vector<bigDecimal>(SIZE));
					*vectorPtr/=PAM::getNorm(*vectorPtr);
					for_each(vectorPtr->begin(), vectorPtr->end(), [&gamma] (bigDecimal& bd) 
					{
						bd = (bigDecimal) Random::weibull(gamma);
					});
					// *matrixPtr -= (bigDecimal) 2.0;
					double eigenvalue 
						= PAM::bruteforceEigenvalue(*matrixPtr).lower; 	
					bigDecimal workingEigenValue;
					int counter = 0;
					do {
						workingEigenValue =
						       PAM::multiplyByMatrix(*vectorPtr, *matrixPtr);
						*vectorPtr/=PAM::getNorm(*vectorPtr);
						if( eigenvalue <= (double) workingEigenValue ) counter++;
					} while ( counter < 1 );

					const int time = SIZE / 2;
					std::cout << "Eigenvalue: " << eigenvalue << std::endl;
					std::cout << "Max ksi: " << PAM::getMax(*matrixPtr) << std::endl;
					std::cout << "log(|v(0)|)/t: " << log( (*vectorPtr)[time] ) / time << ". "
						<< "v(0): " << (*vectorPtr)[time] << std::endl;
					std::cout << "log(|v_min|)/t: " << log( PAM::getMin(*vectorPtr) ) / time << ". "
						<< "v_min: " << PAM::getMin(*vectorPtr) << std::endl;
					std::cout << "v_max: " << PAM::getMax(*vectorPtr) << std::endl;
				}
			}
			break;
			case 'D':
			{		
				bigDecimal q(0);
				bigDecimal pq(1);
				bigDecimal p(1);
				bigDecimal pp(0);
				bigDecimal buf1(0);
				bigDecimal buf2(0);
				bigDecimal a(2);
				for(int i=0;i<1500;i++) {
				  buf1 = p;
				  p = a * p + pp;
				  pp = buf1;
				  buf2 = q;
				  q = a * q + pq; 
				  pq = buf2;
				}
				std::cout << p/q << std::endl;
			}
			break;
		}
	}
	timer=clock()-timer;
	std::cout << "Finished in " << (double)timer / ((double)CLOCKS_PER_SEC) 
		<< " seconds" << std::endl;
	return 0;
}	

bigDecimal pow(bigDecimal& x, int& n) {
  bigDecimal res(1);
  for(int i=0;i<n;i++) {
	res *= x;
  }
  return res;
}
