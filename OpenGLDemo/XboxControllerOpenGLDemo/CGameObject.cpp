#include "CGameObject.h"

CGameObject::CGameObject()
{
	this->x = this->y = this->z = 0.0f;
	this->rotX = this->rotY = this->rotZ = 0.0f;
	this->scale = 1.0f;
	this->name = "Unknown";
	this->visible = true;
	this->textureID = 0;
	return;
}
CGameObject::~CGameObject()
{
}

