
#include "my_math.h"

float lerp(float a, float b, float s) {
	return a*(1-s)+b*s;
}

array<float, 3> vertex_normalize(array<float, 3> in_vert) {
	float mag = pow(pow(in_vert[0], 2) + pow(in_vert[1], 2) + pow(in_vert[2], 2), 0.5);
	return array<float, 3>({{in_vert[0]/mag, in_vert[1]/mag, in_vert[2]/mag}});
}

float randf(float min, float max) {
	return ((float)rand())/RAND_MAX*(max-min)+min;
}

int eucmod(int a, int modulus) {
	return (a % modulus) % modulus;
}