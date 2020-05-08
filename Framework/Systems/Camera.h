#pragma once

class Camera : public SingletonBase<Camera>
{
	friend class SingletonBase<Camera>;
public:
	void Move();

	void SetPosition(Vector3 position);

	void Update();
	void Render();

	void UpdateView();

	Matrix& GetView() { return view; }
	Matrix& GetProjection() { return proj; }

	void UnProjection(Vector3* out, Vector3 source, Matrix world);
	void SetPositionToOrigin();

	void SetTileMapBoundary(RectEdge* edges) { this->mapBoundary = *edges; }

private:
	Camera();
	~Camera();

private:
	VPBuffer* VPB = nullptr;
	D3DXMATRIX view, proj;

	D3DXVECTOR3 position = D3DXVECTOR3(0, 0, 0);
	D3DXVECTOR3 forward = D3DXVECTOR3(0, 0, 1);
	D3DXVECTOR3 up = D3DXVECTOR3(0, 1, 0);

	float cameraSpeed = 100.0f;

	float tempX = 0, tempY = 0;
	RectEdge mapBoundary;
};