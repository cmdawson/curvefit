#ifndef _INC_CURVEFITTER_H
#define _INC_CURVEFITTER_H

#include <iostream>
#include "levmar.h"
#include "interfaces/iinstrument.h"


class CurveFitter
{
private: 
	std::vector<const IInstrument*> _instruments;
	std::vector<double> _expected;
	
	double _opts[LM_OPTS_SZ], _info[LM_INFO_SZ];

	ICurve& _curve;

	// used by levmar
	std::vector<double> _residuals;
	std::vector<double> _workspace;
	std::vector<double> _covar;
	
	size_t ws_size;

	void calcResiduals(double* residuals);

	static void _objectiveFunc(double* p, double* hx, int m, int n, void *adata)
	{
		CurveFitter* fitter = static_cast<CurveFitter*>(adata);
		fitter->_curve.setFitParams(p);

		for (int i=0;i<n;i++)
		{
			hx[i] = fitter->_instruments[i]->PresentValue(fitter->_valdate, fitter->_curve);
		}
	}
	

public:
	boost::gregorian::date _valdate;

	CurveFitter(const boost::gregorian::date& valuedate, ICurve& curve);
	~CurveFitter();

	void addInstrument(const IInstrument& instrument, const double& expectedValue);


	bool fitCurve();
};

#endif
