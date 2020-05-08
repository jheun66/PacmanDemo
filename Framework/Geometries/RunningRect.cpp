#include "Framework.h"
#include "RunningRect.h"

float RunningRect::speed = 100;
UINT RunningRect::objCount = 0;

RunningRect::RunningRect(D3DXVECTOR3 position, D3DXVECTOR3 size)
	: Rect(position, size, 0)
{
	index = objCount++;
	label = "RunningRect " + to_string(index);
}

RunningRect::~RunningRect()
{
}

void RunningRect::Update()
{
	if (bJump)
	{
		vy += jumpAccel * Time::Delta();
		position.y += vy * Time::Delta();

		if (position.y <= groundY)
		{
			bJump = false;
			position.y = groundY;
		}
		UpdateWorld();
	}
}

void RunningRect::GUI()
{
	ImGui::Begin("RunningGame");
	{
		if (ImGui::BeginMenu(label.c_str()))
		{
			ImGui::InputFloat3("Position", position, 2, ImGuiInputTextFlags_ReadOnly);
			ImGui::EndMenu();
		}
	}
	ImGui::End();
}

void RunningRect::AutoMove()
{
	position.x -= speed * Time::Delta();
	UpdateWorld();
}

bool RunningRect::CheckOut(float outPos)
{
	if (r.RB.x < outPos)
		return true;
	else
		return false;
}

void RunningRect::Reposition(float xPos)
{
	position.x = xPos;
	UpdateWorld();
}

void RunningRect::Jump()
{
	bJump = true;
	vy = jumpSpeed;


}
