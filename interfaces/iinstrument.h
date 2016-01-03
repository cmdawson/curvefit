#ifndef _INC_IINSTRUMENT_H
#define _INC_IINSTRUMENT_H
#include <boost/date_time/gregorian/gregorian.hpp>
#include "interfaces/icurve.h"

class IInstrument
{
public:
	virtual double PresentValue(const boost::gregorian::date& valueDate, const ICurve& curve) const = 0;
	virtual const boost::gregorian::date& Maturity() const = 0;
};



#endif
