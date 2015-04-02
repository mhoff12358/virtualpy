#include "Quaternion.h"

Quaternion::Quaternion(std::array<float, 4> val) {
	x = val[0];
	y = val[1];
	z = val[2];
	w = val[3];
}

Quaternion::Quaternion(float xval, float yval, float zval, float wval) :
	x(xval), y(yval), z(zval), w(wval) {

}

std::array<float, 4> Quaternion::GetArray() const {
	return { { x, y, z, w } };
}

float Quaternion::dot(const Quaternion& other) const {
	return x*other.x + y*other.y + z*other.z + w*other.w;
}

Quaternion Quaternion::operator*(const Quaternion& other) const {
	return Quaternion(
		w*other.x + x*other.w + y*other.z - z*other.y,
		w*other.y - x*other.z + y*other.w + z*other.x,
		w*other.z + x*other.y - y*other.x + z*other.w,
		w*other.w - x*other.x - y*other.y - z*other.z);
}

Quaternion Quaternion::operator*(const float other) const {
	return Quaternion(x*other, y*other, z*other, w*other);
}

Quaternion Quaternion::ToPower(const float& other) const {
	// To avoid a divide by zero scenario, if theta is 0 (meaning w is 1) then
	// the end result is just the same quaternion
	if (w == 1.0f) {
		return Quaternion(0, 0, 0, 1);
	}
	float theta = acos(w) * 2.0f;
	float imaginary_scale = sin(theta*other / 2.0f) / sin(theta / 2.0f);

	return Quaternion(imaginary_scale*x, imaginary_scale*y, imaginary_scale*z, cos(theta*other/2.0f));
}

Quaternion Quaternion::Slerp(const Quaternion& q0, const Quaternion& q1, float weight) {
	if (q0.dot(q1) >= 0) {
		return q0*((q0.ToPower(-1)*q1).ToPower(weight));
	} else {
		return q0*((q0.ToPower(-1)*(q1*-1)).ToPower(weight));
	}
}