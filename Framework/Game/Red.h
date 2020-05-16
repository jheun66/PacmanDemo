#pragma once
#include "Ghost.h"

class Red : public Ghost
{
public:
	Red(Vector3 position, Vector3 size);
	~Red();

	void Update();
	void ChangeAnimClip() override;

private:

};