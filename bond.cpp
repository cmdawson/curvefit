#include "bond.h"

using namespace std;
using namespace boost::gregorian;

Bond::Bond(date& start, date& maturity, double coupon, int freq)
	: _cashflows(), _start(start), _maturity(maturity), _coupon(coupon)
{
	_principal = 100.0;

	date astart = start, aend = start + months(12/freq);

	while (aend < _maturity)
	{
		double yrfrac = (aend - astart).days() / 365.0;
		Cashflow cflow = {aend+days(1), yrfrac};
		_cashflows.push_back(cflow);
		astart = aend;
		aend = aend + months(12/freq);
	}

	double yrfrac = (_maturity - astart).days() / 365.0;
	Cashflow cflow = {_maturity+days(1), yrfrac};
	_cashflows.push_back(cflow);
}

Bond::~Bond()
{
}

double Bond::PresentValue(const date& valdate, const ICurve& curve) const
{
	double pv = 0.0;
	vector<Cashflow>::const_iterator ci = _cashflows.begin();
	while (ci != _cashflows.end())
	{
		if (ci->paydate > valdate)
		{
			double t = (ci->paydate - valdate).days() / 365.0;
			pv += _principal * _coupon * ci->yrfrac * curve.interpolate(t);
		}
		ci++;
	}

	double t = (_maturity - valdate).days() / 365.0;
	pv += _principal * curve.interpolate(t);

	return pv;
}
