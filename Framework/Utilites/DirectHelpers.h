#pragma once

//DirectHelpers
struct RectEdge
{
	Vector3 LT;
	Vector3 RB;
};

struct TransformData
{
	RectEdge edge;
	Vector3 position;
	Vector3 size;
	float rotation;
};

class Values
{
public:
	const static Vector3 ZeroVector;
	const static Vector3 OneVector;
	const static Vector3 RightVector;
	const static Vector3 UpVector;
	const static Vector3 FwdVector;
	const static Vector3 ScreenSize;

	const static Color Red;
	const static Color Green;
	const static Color Blue;
	const static Color Magenta;
	const static Color White;
	const static Color Black;
	const static Color GoodEye;

	const static Matrix Identity;
};

class Math
{
public:
	static bool Approximation(float origin, float target, float approxValue);
	static bool Approximation(Vector3 origin, Vector3 target, float approxValue);
};

