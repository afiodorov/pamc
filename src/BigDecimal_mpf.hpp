template<>
class BigDecimal<mpf_t> : public BigDecimalBase<mpf_t> {
	public:
	BigDecimal();
	virtual ~BigDecimal();
	BigDecimal(mpf_t &value);
	BigDecimal(double value);
	BigDecimal(const BigDecimal &value);
	BigDecimal(const BigDecimalBase &value);
	BigDecimal& operator=(const double &rhs); // avoids a cast

	operator double() const override;
	BigDecimal<mpf_t>& operator= (const BigDecimal &rhs) override;
	BigDecimal<mpf_t>& operator*=(const BigDecimal &rhs) override;
	BigDecimal<mpf_t>& operator/=(const BigDecimal &rhs) override;
	BigDecimal<mpf_t>& operator+=(const BigDecimal &rhs) override;
	BigDecimal<mpf_t>& operator-=(const BigDecimal &rhs) override;

	bool operator>(const BigDecimal &other) const override;
	bool operator<(const BigDecimal &other) const override;
	bool operator==(const BigDecimal &other) const override;

	friend std::ostream& operator<<(std::ostream& osObject, const BigDecimal& bDecimal);
};

inline BigDecimal<mpf_t>::BigDecimal() : BigDecimalBase()
{
	mpf_init(data);
}
inline BigDecimal<mpf_t>::BigDecimal(mpf_t &value) : BigDecimalBase()
{
	mpf_init(data);
	mpf_set(data, value);
}
inline BigDecimal<mpf_t>::BigDecimal(const BigDecimal &value) : BigDecimalBase()
{
	mpf_init(data);
	mpf_set(data, *value.getData());
}
inline BigDecimal<mpf_t>::BigDecimal(const BigDecimalBase &value) : BigDecimalBase()
{
	mpf_init(data);
	mpf_set(data, *value.getData());
}
inline BigDecimal<mpf_t>::BigDecimal(double value) : BigDecimalBase()
{
	mpf_init(data);
	mpf_set_d(data, value);
}	
inline BigDecimal<mpf_t>::~BigDecimal() 
{
	mpf_clear(data);
}
inline BigDecimal<mpf_t>& BigDecimal<mpf_t>::operator=(const BigDecimal &rhs)
{
	mpf_set(data, *rhs.getData());
	return *this;
}
inline BigDecimal<mpf_t>& BigDecimal<mpf_t>::operator*=(const BigDecimal &rhs)
{
	mpf_mul(data, data, *rhs.getData());
	return *this;	
}	
inline BigDecimal<mpf_t>& BigDecimal<mpf_t>::operator/=(const BigDecimal &rhs)
{
	mpf_div(data, data, *rhs.getData());
	return *this;	
}
inline BigDecimal<mpf_t>& BigDecimal<mpf_t>::operator+=(const BigDecimal &rhs)
{
	mpf_add(data, data, *rhs.getData());
	return *this;	
}
inline BigDecimal<mpf_t>& BigDecimal<mpf_t>::operator-=(const BigDecimal &rhs)
{
	mpf_sub(data, data, *rhs.getData());
	return *this;	
}

inline bool BigDecimal<mpf_t>::operator>(const BigDecimal &other) const
{
	if (mpf_cmp(data, *other.getData() ) > 0) return true;
	else return false;
}
inline bool BigDecimal<mpf_t>::operator<(const BigDecimal &other) const
{
	if (mpf_cmp(data, *other.getData() ) < 0) return true;
	else return false;
}
inline bool BigDecimal<mpf_t>::operator==(const BigDecimal &other) const
{
	if (mpf_cmp(data, *other.getData() ) == 0) return true;
	else return false;
}
inline BigDecimal<mpf_t>::operator double() const {
	return mpf_get_d(data);
}


inline BigDecimal<mpf_t> sqrt(const BigDecimal<mpf_t>& bd) {
	mpf_t temp;
	mpf_init(temp);
	mpf_sqrt(temp, *bd.getData());
	return BigDecimal<mpf_t>(temp);
}

inline double log(const BigDecimal<mpf_t>& bd)
{
	std::unique_ptr<signed long int>longIntPtr(new signed long int);
	double d = mpf_get_d_2exp(longIntPtr.get(), *bd.getData() );
       	// data = d * 2^*longIntPtr.get();
	return log(d) + *longIntPtr * log(2);
}

inline const BigDecimal<mpf_t> abs(const BigDecimal<mpf_t>& bd)
{
	mpf_t mResult;
	mpf_init(mResult);
	mpf_abs(mResult, *bd.getData()); 
	BigDecimal<mpf_t> result(mResult);
	return result;
}

inline const BigDecimal<mpf_t> exp(const BigDecimal<mpf_t>& bd) 
{
	mpf_t mResult;
	mpf_init(mResult);
	BigDecimal<mpf_t> a(0.0);
	return a;
}
