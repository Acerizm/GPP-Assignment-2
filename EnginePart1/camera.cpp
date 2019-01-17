#include "camera.h"
#include "graphics.h"

Camera::Camera(int width, int height, float angle, DirectX::XMFLOAT3 scaleFactors,Player *mainPlayer) 
{
	this->width = width;
	this->height = height;
	this->angle = angle;
	this->scaleFactors = scaleFactors;
	this->mainPlayer = mainPlayer;
	D3DXMatrixOrthoLH(&orthographicMatrix, width, -height, 0.0f, 1.0f);
	D3DXMatrixIdentity(&identityMatrix);
	
}

Camera::~Camera() 
{

}

void Camera::Update() 
{
	cameraX = this->width / 2;
	cameraY = this->height / 2;
	//int cameraX = 1000, cameraY = 1000;
	if (this->mainPlayer != nullptr )
	{
		cameraX = this->mainPlayer->getX() + 130;
		cameraY = this->mainPlayer->getY() + 130;
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//ggwp hardest code
		//Right Code
		if (cameraX >= LEVEL1_TILE_WIDTH * LEVEL1_TILE_SCALE - GAME_WIDTH / 2 && cameraY <= GAME_HEIGHT / 2 && cameraY <= LEVEL1_TILE_HEIGHT * LEVEL1_TILE_SCALE - GAME_HEIGHT / 2)
		{
			cameraX = LEVEL1_TILE_WIDTH * LEVEL1_TILE_SCALE - GAME_WIDTH / 2;
		}
		if (cameraX >= LEVEL1_TILE_WIDTH * LEVEL1_TILE_SCALE - GAME_WIDTH / 2 && cameraY >= GAME_HEIGHT / 2 && cameraY <= LEVEL1_TILE_HEIGHT * LEVEL1_TILE_SCALE - GAME_HEIGHT / 2)
		{
			cameraX = LEVEL1_TILE_WIDTH * LEVEL1_TILE_SCALE - GAME_WIDTH / 2;
		}
		if (cameraX >= LEVEL1_TILE_WIDTH * LEVEL1_TILE_SCALE - GAME_WIDTH / 2 && cameraY >= GAME_HEIGHT / 2 && cameraY >= LEVEL1_TILE_HEIGHT * LEVEL1_TILE_SCALE - GAME_HEIGHT / 2)
		{
			cameraX = LEVEL1_TILE_WIDTH * LEVEL1_TILE_SCALE - GAME_WIDTH / 2;
		}

		//Left Code
		if (cameraX <= GAME_WIDTH / 2 && cameraY <= GAME_HEIGHT / 2 && cameraY <= LEVEL1_TILE_HEIGHT * LEVEL1_TILE_SCALE - GAME_HEIGHT / 2)
		{
			cameraX = GAME_WIDTH / 2;
		}
		if (cameraX <= GAME_WIDTH / 2 && cameraY >= GAME_HEIGHT / 2 && cameraY <= LEVEL1_TILE_HEIGHT * LEVEL1_TILE_SCALE - GAME_HEIGHT / 2)
		{
			cameraX = GAME_WIDTH / 2;
		}
		if (cameraX <= GAME_WIDTH / 2 && cameraY >= GAME_HEIGHT / 2 && cameraY >= LEVEL1_TILE_HEIGHT * LEVEL1_TILE_SCALE - GAME_HEIGHT / 2)
		{
			cameraX = GAME_WIDTH / 2;
		}

		//Top Code
		if (cameraX <= GAME_WIDTH / 2 && cameraY <= GAME_HEIGHT / 2)
		{
			cameraY = GAME_HEIGHT / 2;
		}
		if (cameraX >= GAME_WIDTH / 2 && cameraY <= GAME_HEIGHT / 2)
		{
			cameraY = GAME_HEIGHT / 2;
		}
		//bottom code
		if (cameraX <= GAME_WIDTH / 2 && cameraY >= LEVEL1_TILE_HEIGHT * LEVEL1_TILE_SCALE - GAME_HEIGHT/2)
		{
			cameraY = LEVEL1_TILE_HEIGHT * LEVEL1_TILE_SCALE - GAME_HEIGHT / 2;
		}
		if (cameraX >= GAME_WIDTH / 2 && cameraY >= LEVEL1_TILE_HEIGHT * LEVEL1_TILE_SCALE - GAME_HEIGHT / 2)
		{
			cameraY = LEVEL1_TILE_HEIGHT * LEVEL1_TILE_SCALE - GAME_HEIGHT / 2;
		}
		////////////////////////////////////////////////////////////////////////////////////////////


	}

	this->viewMatrix = D3DXMATRIX(
		// product of transformation * scaleMatrix * rotationMatrix
		scaleFactors.x * cos(angle), scaleFactors.x *sin(angle), 0, 0,
		-scaleFactors.y *sin(angle), scaleFactors.y* cos(angle), 0, 0,
		0, 0, scaleFactors.z, 0,
		-cameraX * scaleFactors.x*cos(angle) + cameraY * scaleFactors.y*sin(angle), -cameraX * scaleFactors.y *sin(angle) - cameraY * scaleFactors.y*cos(angle), 0, 1
	);
}

void Camera::Follow(Player *mainPlayer) 
{
	this->mainPlayer = mainPlayer;
}

void Camera::UnFollow() {
	this->mainPlayer = nullptr;
}

bool Camera::isFollowing() const {
	return this->mainPlayer != nullptr;
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