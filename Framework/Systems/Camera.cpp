#include "Framework.h"
#include "Camera.h"

void Camera::Move()
{
	if (KeyPress(VK_LSHIFT))
	{
		if (KeyPressUp)
			position.y += cameraSpeed * Time::Delta();
		else if (KeyPressDown)
			position.y -= cameraSpeed * Time::Delta();

		if (KeyPressLeft)
			position.x -= cameraSpeed * Time::Delta();
		else if (KeyPressRight)
			position.x += cameraSpeed * Time::Delta();
	}
	
}

void Camera::SetPosition(Vector3 position)
{
	if ((position.x - WinMaxWidth / 2 < mapBoundary.LT.x
		|| position.x + WinMaxWidth / 2 > mapBoundary.RB.x) == false)
	{
		tempX = position.x - WinMaxWidth / 2;
	}
	if ((position.y - WinMaxHeight / 2 < mapBoundary.RB.y
		|| position.y + WinMaxHeight / 2 > mapBoundary.LT.y) == false)
	{
		tempY = position.y - WinMaxHeight / 2;
	}

	this->position = Vector3(tempX, tempY, 0);
	UpdateView();
}

void Camera::Update()
{
	Move();
	UpdateView();
}

void Camera::Render()
{
	VPB->SetVSBuffer(1);
}

void Camera::UpdateView()
{
	D3DXMatrixLookAtLH(&view, &position, &(position+forward), &up);
	VPB->SetView(view);
}

void Camera::UnProjection(Vector3 * out, Vector3 source, Matrix world)
{
	// local - world - view - projection -viewport
	// x = (NCC.x + 1) * viewport.width * 0.5f + viewport.topLeftX;
	// y = (1 - NDC.y) * viewport.height * 0.5f + viewport.topLeftY;
	// viewport. 은 0으로 제거하면

	Vector3 position = source;

	out->x = (position.x * 2.0f / WinMaxWidth) - 1.0f;
	out->y = (position.y * 2.0f / WinMaxHeight) - 1.0f;
	out->z = position.z;

	Matrix wvp = world * view * proj;
	D3DXMatrixInverse(&wvp, nullptr, &wvp);	// wvp를 역행렬 취해서 wvp에 넣어줌
	D3DXVec3TransformCoord(out, out, &wvp); // out은 local 공간이 됨

}

void Camera::SetPositionToOrigin()
{
	D3DXMatrixLookAtLH(&view, &Vector3(0, 0, 0), &forward, &up);
	VPB->SetView(view);
	VPB->SetVSBuffer(1);
}

Camera::Camera()
{
	VPB = new VPBuffer();

	D3DXMatrixIdentity(&view);

	//D3DXMatrixOrthoLH(&proj, WinMaxWidth, WinMaxHeight, 0, 1);
	D3DXMatrixOrthoOffCenterLH(&proj, 0, WinMaxWidth, 0, WinMaxHeight, 0, 1);		// 좌측하단을 (0, 0)으로 만들기
	VPB->SetView(view);
	VPB->SetProj(proj);
}

Camera::~Camera()
{
	SAFE_DELETE(VPB);
}
