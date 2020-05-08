#pragma once
class OBB
{
public:
	OBB(TransformData* transform);
	~OBB();

	void Calculation();
	static bool CheckIntersect(OBB* a, OBB* b);

private:
	TransformData* transform;

	Vector3 centerW = Values::ZeroVector;
	Vector3 centerH = Values::ZeroVector;
	Vector3 centerPos = Values::ZeroVector;

};