#ifndef VECTORTEMPLATES_H_
#define VECTORTEMPLATES_H_

#include "standard.h"

template<class T>
inline std::vector<T>& operator/=(std::vector<T>& vector,
		const T& t)
{
	for(typename std::vector<T>::size_type i = 0; 
			i < vector.size(); i++)
	{
		vector[i]/=t; 
	}
	return vector;
}

template<class T>
inline std::vector<T>& operator*=(std::vector<T>& vector,
		const T& t)
{
	for(typename std::vector<T>::size_type i = 0; 
			i < vector.size(); i++)
	{
		vector[i]*=t; 
	}
	return vector;
}
template<class T>
inline std::vector<T>& operator-=(std::vector<T>& vector,
		const T& t)
{
	for(typename std::vector<T>::size_type i = 0; 
			i < vector.size(); i++)
	{
		vector[i]-=t; 
	}
	return vector;
}
template<class T>
inline std::vector<T>& operator+=(std::vector<T>& vector,
		const T& t)
{
	for(typename std::vector<T>::size_type i = 0; 
			i < vector.size(); i++)
	{
		vector[i]+=t; 
	}
	return vector;
}

#endif
