#include "camera.h"
#include "graphics.h"

Camera::Camera(int width, int height, float angle, DirectX::XMFLOAT3 scaleFactors)
{
	this->width = width;
	this->height = height;
	this->angle = angle;
	this->cameraHorizontalSpeed = 0.3f;
	this->scaleFactors = scaleFactors;
	this->cameraX = GAME_WIDTH / 2;
	this->cameraY = GAME_HEIGHT / 2;
	D3DXMatrixOrthoLH(&orthographicMatrix, width, -height, 0.0f, 1.0f);
	D3DXMatrixIdentity(&identityMatrix);

}

Camera::~Camera()
{

}

void Camera::Update()
{
	if (cameraX >= GAME_WIDTH * 6) {
		this->cameraX = GAME_WIDTH / 2;
	}
	else {
		cameraX = this->getCameraX() + this->cameraHorizontalSpeed;
	}
	//int cameraX = 1000, cameraY = 1000;

	this->viewMatrix = D3DXMATRIX(
		// product of transformation * scaleMatrix * rotationMatrix
		scaleFactors.x * cos(angle), scaleFactors.x *sin(angle), 0, 0,
		-scaleFactors.y *sin(angle), scaleFactors.y* cos(angle), 0, 0,
		0, 0, scaleFactors.z, 0,
		-cameraX * scaleFactors.x*cos(angle) + cameraY * scaleFactors.y*sin(angle), -cameraX * scaleFactors.y *sin(angle) - cameraY * scaleFactors.y*cos(angle), 0, 1
	);
}

void Camera::setTransform(Graphics *graphics) const
{
	//set transform on the projection
	graphics->get3Ddevice()->SetTransform(D3DTS_PROJECTION, &orthographicMatrix);
	graphics->get3Ddevice()->SetTransform(D3DTS_WORLD, &identityMatrix);
	graphics->get3Ddevice()->SetTransform(D3DTS_VIEW, &viewMatrix);



}
float Camera::getCameraX()
{
	return cameraX;
}
float Camera::getCameraY()
{
	return cameraY;
}