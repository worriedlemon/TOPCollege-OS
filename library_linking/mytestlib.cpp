/*
* Simple library that needs to be set up as dynamic linked library (.dll).
* In this library there is only one simple function my_sqrt, which
* calculates square root of the number with Heron's iterative formula.
*/

#ifndef MYTEST_LIB_H
#define MYTEST_LIB_H

#include "mylib_os_defs.h"

#ifdef __cplusplus
extern "C"
{
#endif

DLL_EXPORT double MYLIB_API my_sqrt(double value)
{
    constexpr double acc = 1e-6;

	if (value < 0) return -1;
	if (value == 0 || value == 1) return value;

	double result = value / 2;
	while (result - value / result > acc || result - value / result < -acc)
	{
		result = 0.5 * (result + value / result);
	}

	return result;
}

#ifdef __cplusplus
}
#endif

#endif // MYTEST_LIB_H
