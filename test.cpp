#include <iostream>
#include "curves/yieldcurve.h"
#include "curves/nsscurve.h"
#include "bond.h"
#include "curvefitter.h"

using namespace std;
using namespace boost;

void test_nsscurve(void);
void test_yieldcurve(void);

YieldCurve build_yieldcurve(void);
NssCurve build_nsscurve(void);

void test_bond(void);
void print_curve(const ICurve& curve);

void test_fitter(void);
vector<Bond> build_benchmarks(void);

int main(void)
{
	/*test_nsscurve();
	cout << endl;

	test_yieldcurve();
	cout << endl;

	test_bond();*/

	test_fitter();
}

void test_fitter()
{
	double tenors[] = {1,2,3,5,7,10,15,20,30};
	vector<double> maturities(tenors,tenors+9), dfacs(9,0.95);
	
	YieldCurve curve(maturities, dfacs);
	NssCurve curve2 = build_nsscurve();

	gregorian::date value_date(2015,12,28);

	CurveFitter fitter(value_date, curve);
	CurveFitter fitter2(value_date, curve2);

	vector<Bond> bonds = build_benchmarks();
	for (size_t i=0;i<bonds.size();i++)
	{
		fitter.addInstrument(bonds[i], 100.0);
		fitter2.addInstrument(bonds[i], 100.0);
	}

	bool ok = fitter.fitCurve();
	bool ok2 = fitter2.fitCurve();

	//cout << "OK: " << ok << "," << ok2 << endl;

	double t = 0.05;
	cout << "t,r,r2" << endl;
	while (t <= 30)
	{
		double r = -log(curve.interpolate(t)) / t;
		double r2 = -log(curve2.interpolate(t)) / t;
		cout << t << "," << r << "," << r2 << endl;
		t += 0.05;
	}
}


void test_bond(void)
{
	cout << "Bond:" << endl << endl;

	YieldCurve curve = build_yieldcurve();

	gregorian::date start(2015,12,30), maturity(2025,12,31);
	Bond bond(start, maturity, 0.05, 2);

	gregorian::date value_date(2016,1,2);

	cout << "PV: " << bond.PresentValue(value_date, curve) << " (YC)" << endl;

	NssCurve curve2 = build_nsscurve();

	cout << "PV: " << bond.PresentValue(value_date, curve2) << " (NSS)" << endl;
}


NssCurve build_nsscurve(void)
{
	vector<double> params;
	params.push_back(0.03); // beta0
	params.push_back(0.04); // beta1
	params.push_back(0.05); // beta2
	params.push_back(0.055); // beta3
	params.push_back(7.0); // tau
	params.push_back(15.0); // tau2

	NssCurve curve(params);

	return curve;
}

void test_nsscurve(void)
{
	cout << "NSS Curve:" << endl << endl;

	NssCurve curve = build_nsscurve();

	print_curve(curve);

	//params[0] = 0.01;
	//curve.setFitParams(&(params[0]));
	//print_curve(curve);
}


YieldCurve build_yieldcurve(void)
{
	vector<double> maturities, dfacs;

	maturities.push_back(0.5); dfacs.push_back(0.995);
	maturities.push_back(1); dfacs.push_back(0.98);
	maturities.push_back(2); dfacs.push_back(0.97);
	maturities.push_back(3); dfacs.push_back(0.95);
	maturities.push_back(5); dfacs.push_back(0.92);
	maturities.push_back(10); dfacs.push_back(0.84);

	YieldCurve curve(maturities, dfacs);
	
	return curve;
}


void test_yieldcurve(void)
{
	cout << "Yield Curve (constant forward interpolation):" << endl << endl;

	YieldCurve curve = build_yieldcurve();

	print_curve(curve);

	//dfacs[5] = 0.8;
	//curve.setFitParams(&(dfacs[0]));
	//print_curve(curve);
}

void print_curve(const ICurve& curve)
{
	cout << "t\tdf" << endl;
	double t = 0.25;
	while (t <= 20)
	{
		cout << t << "\t" << curve.interpolate(t) << endl;
		t += 2.25;
	}
}

vector<Bond> build_benchmarks(void)
{
	vector<Bond> benchmarks = vector<Bond>();
	gregorian::date start(2015,12,28), maturity(2016,12,31);
	benchmarks.push_back(Bond(start, maturity, 0.03, 4));	// 1Y

	maturity = gregorian::date(2017,12,30);
	benchmarks.push_back(Bond(start, maturity, 0.032, 2));	// 2Y

	maturity = gregorian::date(2018,12,28);
	benchmarks.push_back(Bond(start, maturity, 0.035, 2));	// 3Y

	maturity = gregorian::date(2020,12,29);
	benchmarks.push_back(Bond(start, maturity, 0.04, 2));	// 5Y

	maturity = gregorian::date(2022,12,31);
	benchmarks.push_back(Bond(start, maturity, 0.04, 2));	// 7Y

	maturity = gregorian::date(2025,12,31);
	benchmarks.push_back(Bond(start, maturity, 0.0375, 2));	// 10Y

	maturity = gregorian::date(2030,12,30);
	benchmarks.push_back(Bond(start, maturity, 0.0375, 2));	// 15Y

	maturity = gregorian::date(2035,12,31);
	benchmarks.push_back(Bond(start, maturity, 0.037, 2));	// 20Y

	maturity = gregorian::date(2046,1,3);
	benchmarks.push_back(Bond(start, maturity, 0.036, 2));	// 30Y

	return benchmarks;
}
