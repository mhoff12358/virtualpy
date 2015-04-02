#pragma once

#include <array>

class Quaternion {
public:
	Quaternion(std::array<float, 4> val);
	Quaternion(float xval, float yval, float zval, float wval);
	std::array<float, 4> GetArray() const;

	Quaternion operator*(const Quaternion& other) const;
	Quaternion ToPower(const float& other) const;

	float x, y, z, w;

	static Quaternion Slerp(const Quaternion& q0, const Quaternion& q1, float weight);
};
