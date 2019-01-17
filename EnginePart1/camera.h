#ifndef _CAMERA_H
#define _CAMERA_H
#define WIN32_LEAN_AND_MEAN

#include <DirectXMath.h>
#include "player.h"

class Camera 
{
private:
	float angle;
	DirectX::XMFLOAT3 scaleFactors;
	D3DXMATRIX orthographicMatrix;
	D3DXMATRIX identityMatrix;
	D3DXMATRIX viewMatrix;
	//Player *mainPlayer;
	float cameraX;
	float cameraY;

public:
	Player * mainPlayer;

	Camera(int width, int height, float angle, DirectX::XMFLOAT3 scaleFactors,Player *mainPlayer);
	~Camera();

	//update the camera
	void Update();
	//follow the sprite
	void Follow(Player *mainPlayer);
	void UnFollow();
	bool isFollowing() const;
	void setTransform(Graphics *grpahics) const;

	int width;
	int height;

	float getCameraX();
	float getCameraY();

};


#endif
