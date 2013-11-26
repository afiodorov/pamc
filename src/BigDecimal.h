#ifndef BIGDECIMAL_H_
#define BIGDECIMAL_H_

#include "standard.h"

template<class T>
class BigDecimal {};

template<class T>
inline bool operator== (const double& number, const BigDecimal<T>& bigDecimal) {
	return (bigDecimal == number); 
}
template<class T>
inline bool operator!= (const double& number, const BigDecimal<T>& bigDecimal) {
	return (bigDecimal != number); 
}
template<class T>
inline bool operator> (const double& number, const BigDecimal<T>& bigDecimal) {
	return (bigDecimal < number); 
}
template<class T>
inline bool operator< (const double& number, const BigDecimal<T>& bigDecimal) {
	return (bigDecimal > number); 
}
template<class T>
inline bool operator<= (const double& number, const BigDecimal<T>& bigDecimal) {
	return (bigDecimal >= number); 
}
template<class T>
inline bool operator>= (const double& number, const BigDecimal<T>& bigDecimal) {
	return (bigDecimal <= number); 
}
template<class T>
class BigDecimalBase {
	public:
	BigDecimalBase() {};
	virtual ~BigDecimalBase() {};
	virtual operator double() const = 0;

	virtual bool operator> (const BigDecimal<T> &other) const = 0;
	virtual bool operator< (const BigDecimal<T> &other) const = 0;
	virtual bool operator==(const BigDecimal<T> &other) const = 0;

	virtual BigDecimal<T>& operator= (const BigDecimal<T> &rhs) = 0;
	virtual BigDecimal<T>& operator*=(const BigDecimal<T> &rhs) = 0;
	virtual BigDecimal<T>& operator/=(const BigDecimal<T> &rhs) = 0;
	virtual BigDecimal<T>& operator+=(const BigDecimal<T> &rhs) = 0;
	virtual BigDecimal<T>& operator-=(const BigDecimal<T> &rhs) = 0;

	// genetic implementation
	const T* getData() const;
	bool operator>=(const BigDecimal<T> &other) const;
	bool operator<=(const BigDecimal<T> &other) const;
	bool operator!=(const BigDecimal<T> &other) const;
	
	const BigDecimal<T> operator+(const BigDecimal<T> &other) const;
	const BigDecimal<T> operator-(const BigDecimal<T> &other) const;
	const BigDecimal<T> operator*(const BigDecimal<T> &other) const;
	const BigDecimal<T> operator/(const BigDecimal<T> &other) const;

	protected:
	T data;
};
template<class T>
inline const T* BigDecimalBase<T>::getData() const
{
	return &data;
}
template<class T>
inline bool BigDecimalBase<T>::operator!=(const BigDecimal<T> &other) const
{
	return !operator==(other);
}
template<class T>
inline bool BigDecimalBase<T>::operator>=(const BigDecimal<T> &other) const
{
	return !operator<(other);
}
template<class T>
inline bool BigDecimalBase<T>::operator<=(const BigDecimal<T> &other) const
{
	return !operator>(other);
}

template<class T>
inline const BigDecimal<T> BigDecimalBase<T>::operator+(const BigDecimal<T> &other) const
{
    // Make a copy of myself.  Same as MyClass result(*this);
    BigDecimal<T> result = *this;
    result += other;
    return result;
}

template<class T>
inline const BigDecimal<T> BigDecimalBase<T>::operator-(const BigDecimal<T> &other) const
{
    BigDecimal<T> result = *this;
    result -= other;
    return result;
}

template<class T>
inline const BigDecimal<T> BigDecimalBase<T>::operator*(const BigDecimal<T> &other) const
{
    BigDecimal<T> result = *this;
    result *= other;
    return result;
}

template<class T>
inline const BigDecimal<T> BigDecimalBase<T>::operator/(const BigDecimal<T> &other) const
{
    BigDecimal<T> result = *this;
    result /= other;
    return result;
}

#include "BigDecimal_mpf.hpp"

inline std::ostream& operator<<(std::ostream& osObject, const BigDecimal<mpf_t>& bd) {
	osObject << (double) bd;
	return osObject;
}

#endif
