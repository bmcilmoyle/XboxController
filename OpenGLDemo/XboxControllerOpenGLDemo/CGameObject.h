#ifndef _CGameObject_HG_
#define _CGameObject_HG_

#include <string>

class CGameObject
{
public:
	CGameObject();
	~CGameObject();
	float x,y,z;
	float rotX, rotY, rotZ;
	float scale;
	std::string name;
	bool visible;
	int textureID;
};

#endif
