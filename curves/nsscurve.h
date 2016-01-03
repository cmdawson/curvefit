#ifndef _INC_NSSCURVE_H
#define _INC_NSSCURVE_H
#include <cmath>
#include <algorithm>
#include "interfaces/icurve.h"

#include <iostream>

class NssCurve : public ICurve
{
private:
	std::vector<double> _params; // beta0, beta1, beta2, beta3, tau, tau2
	double* _fparams;

public:
	NssCurve() : _params(6), _fparams(NULL) {}
	NssCurve(const std::vector<double>& params)
		: _params(params.begin(), params.end()), _fparams(NULL) {}
	~NssCurve() {}

	size_t getNumParams(void) const
	{
		return 6;
	}

	double* getParams(void)
	{
		return &(_params[0]);
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
		const double* params = _fparams == NULL ? &(_params[0]) : _fparams;

		double beta0 = params[0];
		double beta1 = params[1];
		double beta2 = params[2];
		double beta3 = params[3];
		double tau = params[4];
		double tau2 = params[5];

		if (t == 0.0)
			return 1.0;

		double r = beta0 + beta1*(1-std::exp(-t/tau))*tau/t
			+ beta2*((1-std::exp(-t/tau))*tau/t -std::exp(-t/tau))
			+ beta3*((1-std::exp(-t/tau2))*tau2/t -std::exp(-t/tau2));

		return std::exp(-r*t);
	}
};

#endif
