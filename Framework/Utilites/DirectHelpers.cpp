#include "Framework.h"
#include "DirectHelpers.h"


const Vector3 Values::ZeroVector = Vector3(0, 0, 0);
const Vector3 Values::OneVector = Vector3(1, 1, 1);
const Vector3 Values::RightVector = Vector3(1, 0, 0);
const Vector3 Values::UpVector = Vector3(0, 1, 0);
const Vector3 Values::FwdVector = Vector3(0, 0, 1);
const Vector3 Values::ScreenSize = Vector3(WinMaxWidth, WinMaxHeight, 1);

const Color Values::Red = Color(1, 0, 0, 1);
const Color Values::Green = Color(0, 1, 0, 1);
const Color Values::Blue = Color(0, 0, 1, 1);
const Color Values::Magenta = Color(1, 0, 1, 1);
const Color Values::Black = Color(0, 0, 0, 1);
const Color Values::White = Color(1, 1, 1, 1);
const Color Values::GoodEye = Color(0.63f, 0.8f, 0.63f, 1);

const Matrix Values::Identity = {	1, 0, 0, 0,
									0, 1, 0, 0,
									0, 0, 1, 0,
									0, 0, 0, 1 };

bool Math::Approximation(float origin, float target, float approxValue)
{
	return abs(origin - target) <= approxValue;
}

bool Math::Approximation(Vector3 origin, Vector3 target, float approxValue)
{

	bool result = true;

	result &= Approximation(origin.x, target.x, approxValue);
	result &= Approximation(origin.y, target.y, approxValue);
	result &= Approximation(origin.z, target.z, approxValue);

	return result;
}
