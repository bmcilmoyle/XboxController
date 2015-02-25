#ifndef _CTEXTURELOADERMANAGER_HG_
#define _CTEXTURELOADERMANAGER_HG_

#include <vector>
#include <string>
#include <map>

#include <iostream>
#include "CTriangle.h"		// Because the private methods need it

class CTextureLoaderManager
{
public:
	CTextureLoaderManager();
	~CTextureLoaderManager();

	void CTextureLoaderManager::LoadTextures(std::map<std::string, int>* textures);

	void SetRootDirectory( std::string rootDirectory );
	std::string GetRootDirectory(void);

private:
	std::string m_RootDirectoryName;

};

#endif
