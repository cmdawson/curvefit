#ifndef _INC_YIELDCURVE_H
#define _INC_YIELDCURVE_H
#include <algorithm>
#include <cmath>
#include "interfaces/icurve.h"

class YieldCurve : public ICurve
{
private:
	std::vector<double> _m, _df;
	double* _fparams;

public:
	YieldCurve() {}
	YieldCurve(std::vector<double> m, std::vector<double> df)
		: _m(m.begin(), m.end()), _df(df.begin(), df.end()), _fparams(NULL) {}

	~YieldCurve() {}

	size_t getNumParams(void) const
	{
		return _df.size();
	}

	double* getParams(void)
	{
		return &(_df[0]);
	}

	void setFitParams(double* params)
	{
		_fparams = params;
	}

	void clearFitParams(void)
	{
		_fparams = NULL;
	}

	double interpolate(const double& t) const
	{
		const double* ay = _fparams == NULL ? &(_df[0]) : _fparams;

		if (t < _m[0])
			return std::exp(t * log(ay[0]) / _m[0]); 
			
		std::vector<double>::const_iterator low = std::lower_bound(_m.begin(), _m.end(), t);
		int li = low == _m.end() ? _m.size()-1 : (low - _m.begin());

		double m = (t - _m[li-1]) / (_m[li] - _m[li-1]);
		return std::exp(m*std::log(ay[li]) + (1-m)*std::log(ay[li-1]));
	}
};

#endif
