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

double hopfield(double x)
{
    if (x > 0.5)
        return 1.0;
    else
        return 0.0;
}

double sigmoid(double x)
{
    return 1.0 / (1.0 + exp(-x));
}