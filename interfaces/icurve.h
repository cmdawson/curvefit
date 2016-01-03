#ifndef _INC_ICURVE_H
#define _INC_ICURVE_H

#include <vector>

class ICurve
{
public:
	virtual size_t getNumParams(void) const = 0;
	virtual double* getParams(void) = 0;
	virtual void setFitParams(double* params) = 0;
	virtual void clearFitParams(void) = 0;
	virtual double interpolate(const double& t) const = 0;
};

#endif
