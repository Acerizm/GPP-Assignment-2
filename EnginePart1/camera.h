#ifndef _CAMERA_H
#define _CAMERA_H
#define WIN32_LEAN_AND_MEAN

#include <DirectXMath.h>
#include "player.h"
#include <string>
class Camera
{
private:
	float angle;
	DirectX::XMFLOAT3 scaleFactors;
	D3DXMATRIX orthographicMatrix;
	D3DXMATRIX identityMatrix;
	D3DXMATRIX viewMatrix;
	float cameraX;
	float cameraY;
	float cameraHorizontalSpeed;
	string cameraState;

public:
	Player * mainPlayer;

	Camera(int width, int height, float angle, DirectX::XMFLOAT3 scaleFactors);
	~Camera();

	//update the camera
	void Update();
	void setTransform(Graphics *grpahics) const;

	int width;
	int height;

	float getCameraX();
	float getCameraY();
	void setCameraState(string value) {
		this->cameraState = value;
	}
	string getCameraState() {
		return this->cameraState;
	}

};


#endif