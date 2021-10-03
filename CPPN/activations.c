#include "CPPN/activations.h"

#include <math.h>

#include "tools/utils.h"

double fast_tanh(double x)
{
    return x / (1 + double_abs(x));
}