#include "Framework.h"
#include "OBB.h"

OBB::OBB(TransformData * transform)
	: transform(transform)
{

}

OBB::~OBB()
{
}

void OBB::Calculation()
{
	RectEdge r = (*transform).edge;
	centerPos.x = (r.RB.x + r.LT.x) * 0.5f;
	centerPos.y = (r.RB.y + r.LT.y) * 0.5f;
	centerPos.z = 0.0f;
	Vector3 position = (*transform).position;
	centerW = (r.RB - position) * 0.5f;
	centerW.z = 0.0f;

	centerH = (r.LT - position) * 0.5f;
	centerH.z = 0.0f;
}

bool OBB::CheckIntersect(OBB * a, OBB * b)
{
	TransformData* transA = a->transform;
	TransformData* transB = b->transform;

	Vector3 dist = b->centerPos - a->centerPos;

	vector<Vector3> vecs;
	vecs.push_back(a->centerH);
	vecs.push_back(a->centerW);
	vecs.push_back(b->centerH);
	vecs.push_back(b->centerW);

	// separatedAxises
	vector<Vector3> separatedAxises;
	separatedAxises.push_back(Vector3(cosf(D3DXToRadian(transA->rotation)), sinf(D3DXToRadian(transA->rotation)), 0.0f));
	separatedAxises.push_back(Vector3(cosf(D3DXToRadian(transA->rotation + 90.0f)), sinf(D3DXToRadian(transA->rotation + 90.0f)), 0.0f));
	separatedAxises.push_back(Vector3(cosf(D3DXToRadian(transB->rotation)), sinf(D3DXToRadian(transB->rotation)), 0.0f));
	separatedAxises.push_back(Vector3(cosf(D3DXToRadian(transB->rotation + 90.0f)), sinf(D3DXToRadian(transB->rotation + 90.0f)), 0.0f));

	float sum = 0;
	for (int i = 0; i < separatedAxises.size(); i++)
	{
		sum = 0;
		for (int j = 0; j < vecs.size(); j++)
		{
			sum += abs(D3DXVec3Dot(&separatedAxises[i], &vecs[j]));
		}

		if (abs(D3DXVec3Dot(&dist, &separatedAxises[i])) > sum)
			return false;
	}

	return true;
}
