#include "curvefitter.h"
#include <cmath>

using namespace std;
using namespace boost;

CurveFitter::CurveFitter(const boost::gregorian::date& valdate, ICurve& curve)
	: _instruments(), _expected(), _valdate(valdate), _residuals(), _workspace(), _covar(), _curve(curve)
{

}

CurveFitter::~CurveFitter()
{
}

void CurveFitter::addInstrument(const IInstrument& instrument, const double& evalue)
{
	_instruments.push_back(&instrument);
	_expected.push_back(evalue);
}


bool CurveFitter::fitCurve()
{
	size_t M = _curve.getNumParams();
	size_t N = _instruments.size();
	size_t WS = LM_DIF_WORKSZ(M,N);

	if (_residuals.size() < N)
		_residuals.resize(N);

	if (_workspace.size() < WS)
		_workspace.resize(WS);

	if (_covar.size() < M*M)
		_covar.resize(M*M);

	// Setup the curve
	unsigned IMAX = 500;

  	_opts[0]=LM_INIT_MU;
	_opts[1]=1E-15;
	_opts[2]=1E-15;
	_opts[3]=1E-20;
  	_opts[4]= LM_DIFF_DELTA; // default forward differencing

	double* p = _curve.getParams();
	double* x = &(_expected[0]);
	double* ws = &(_workspace[0]);
	double* covar = &(_covar[0]);

	dlevmar_dif(CurveFitter::_objectiveFunc, p, x, M, N, IMAX, _opts, _info, ws, covar, this);

	/* information regarding the minimization. Set to NULL if don't care
	 * info[0]= ||e||_2 at initial p.
	 * info[1-4]=[ ||e||_2, ||J^T e||_inf,  ||Dp||_2, \mu/max[J^T J]_ii ], all computed at estimated p.
	 * info[5]= # iterations,
	 * info[6]=reason for terminating: 1 - stopped by small gradient J^T e
	 *                                 2 - stopped by small Dp
	 *                                 3 - stopped by itmax
	 *                                 4 - singular matrix. Restart from current p with increased \mu
	 *                                 5 - no further error reduction is possible. Restart with increased mu
	 *                                 6 - stopped by small ||e||_2
	 *                                 7 - stopped by invalid (i.e. NaN or Inf) "func" values; a user error
	 * info[7]= # function evaluations
	 * info[8]= # Jacobian evaluations
	 * info[9]= # linear systems solved, i.e. # attempts for reducing error */

	cout << "Initial e: " << _info[0] << endl;
	cout << "Final e: " << _info[1] << endl;
	cout << "Iterations: " << _info[5] << endl;
	cout << "Reason for terminating: " << _info[6] << endl;
	cout << "Function evaluations: " << _info[7] << endl;
	cout << "Jacobian  evaluations: " << _info[8] << endl;

	_curve.clearFitParams();

	return _info[6] == 6;
}

