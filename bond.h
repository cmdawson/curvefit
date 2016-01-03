#ifndef _INC_SWAP_H
#define _INC_SWAP_H

#include "interfaces/iinstrument.h"

class Bond : public IInstrument
{
public:
	typedef boost::gregorian::date date;

	struct Cashflow
	{
		date paydate;
		double yrfrac;
	};

private:
	double _principal;
	double _coupon;
	date _start;
	date _maturity;
	std::vector<Cashflow> _cashflows;

public:
	Bond(date& effective, date& maturity, double cpn, int cpnfreq);
	~Bond();

	const std::vector<Cashflow>& getCashflows(void) const
	{
		return _cashflows;
	}

	double PresentValue(const date& valdate, const ICurve& curve) const;

	const date& Maturity(void) const
	{
		return _maturity;
	}
};

#endif
