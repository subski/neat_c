#include "CPPN/activations.h"

#include <math.h>

#include "tools/utils.h"

double fast_tanh(double x)
{
    return x / (1 + double_abs(x));
}

double leakyReLu(double x)
{
    double a = 0.001;

    if (x > 0.0)
        return x;
    else
        return x*a;
}