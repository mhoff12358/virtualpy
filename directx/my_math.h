

#ifndef __MY_MATH_H_INCLUDED__
#define __MY_MATH_H_INCLUDED__

#include <array>
#include <stdlib.h>
#include <cmath>

using std::array;

//The first two values are what are being interpolated
//The third value is the proportion between the two (a small scale means that
//the first value is weighted heavily, a 1 scale means the second value)
float lerp(float, float, float);
array<float, 3> vertex_normalize(array<float, 3>);
float randf(float, float);

int eucmod(int a, int modulus);

#endif