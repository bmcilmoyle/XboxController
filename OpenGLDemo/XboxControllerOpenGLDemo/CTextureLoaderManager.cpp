#include "CTextureLoaderManager.h"
#include "CTextureFromBMP.h"

#include "CTriangle.h"		// also includes CVertex
#include <fstream>			// For loading the files ifstream
#include "Utils.h"
#include <sstream>			


CTextureFromBMP* g_p_Texture00 = 0;
CTextureFromBMP* g_p_Texture01 = 0;
CTextureFromBMP* g_p_Texture02 = 0;
CTextureFromBMP* g_p_Texture03 = 0;
CTextureFromBMP* g_p_Texture04 = 0;
CTextureFromBMP* g_p_Texture05 = 0;
CTextureFromBMP* g_p_Texture06 = 0;
CTextureFromBMP* g_p_Texture07 = 0;
CTextureFromBMP* g_p_Texture08 = 0;
CTextureFromBMP* g_p_Texture09 = 0;
CTextureFromBMP* g_p_Texture10 = 0;
CTextureFromBMP* g_p_Texture11 = 0;
CTextureFromBMP* g_p_Texture12 = 0;
CTextureFromBMP* g_p_Texture13 = 0;


CTextureLoaderManager::CTextureLoaderManager()
{
	//Set the default texture directory
	this->m_RootDirectoryName = "textures/";
	return;
}

CTextureLoaderManager::~CTextureLoaderManager()
{
	delete g_p_Texture00;
	delete g_p_Texture01;
	delete g_p_Texture02;
	delete g_p_Texture03;
	delete g_p_Texture04;
	delete g_p_Texture05;
	delete g_p_Texture06;
	delete g_p_Texture07;
	delete g_p_Texture08;
	delete g_p_Texture09;
	delete g_p_Texture10;
	delete g_p_Texture11;
	delete g_p_Texture12;
	delete g_p_Texture13;

	return;
}

void CTextureLoaderManager::LoadTextures(std::map<std::string, int>* textures)
{
	std::map<std::string,int>::iterator it =textures->begin();

	//Load Textures
	if (textures->size() >= 1)
	{
		g_p_Texture00 = new CTextureFromBMP();
		g_p_Texture00->SetDebug_cout_output( true );
		glActiveTexture( GL_TEXTURE0 );	// GL_TEXTURE0, GL_TEXTURE1, etc.
		if ( ! g_p_Texture00->CreateNewTextureFromBMPFile2(this->m_RootDirectoryName+it->first, GL_TEXTURE0, true) )
		{
			std::cout << "Failed to load a texture" << std::endl;
		}
		++it;
	}

	if (textures->size() >= 2)
	{
		g_p_Texture01 = new CTextureFromBMP();
		g_p_Texture01->SetDebug_cout_output( true );
		glActiveTexture( GL_TEXTURE1 );	
		if ( ! g_p_Texture01->CreateNewTextureFromBMPFile2(this->m_RootDirectoryName+it->first, GL_TEXTURE1, true) )
		{
			std::cout << "Didn't load the rock texture." << std::endl;
		}
		++it;
	}

	if (textures->size() >= 3)
	{
		g_p_Texture02 = new CTextureFromBMP();
		g_p_Texture02->SetDebug_cout_output( true );
		glActiveTexture( GL_TEXTURE2 );	
		if ( ! g_p_Texture02->CreateNewTextureFromBMPFile2(this->m_RootDirectoryName+it->first, GL_TEXTURE2, true) )
		{
			std::cout << "Didn't load the rock texture." << std::endl;
		}
		++it;
	}

	if (textures->size() >= 4)
	{
		g_p_Texture03 = new CTextureFromBMP();
		g_p_Texture03->SetDebug_cout_output( true );
		glActiveTexture( GL_TEXTURE3 );	
		if ( ! g_p_Texture03->CreateNewTextureFromBMPFile2(this->m_RootDirectoryName+it->first, GL_TEXTURE3, true) )
		{
			std::cout << "Didn't load the rock texture." << std::endl;
		}
		++it;
	}

	if (textures->size() >= 5)
	{
		g_p_Texture04 = new CTextureFromBMP();
		g_p_Texture04->SetDebug_cout_output( true );
		glActiveTexture( GL_TEXTURE4 );	
		if ( ! g_p_Texture04->CreateNewTextureFromBMPFile2(this->m_RootDirectoryName+it->first, GL_TEXTURE4, true) )
		{
			std::cout << "Didn't load the rock texture." << std::endl;
		}
		++it;
	}

	if (textures->size() >= 6)
	{
		g_p_Texture05 = new CTextureFromBMP();
		g_p_Texture05->SetDebug_cout_output( true );
		glActiveTexture( GL_TEXTURE5 );	
		if ( ! g_p_Texture05->CreateNewTextureFromBMPFile2(this->m_RootDirectoryName+it->first, GL_TEXTURE5, true) )
		{
			std::cout << "Didn't load the rock texture." << std::endl;
		}
		++it;
	}

	if (textures->size() >= 7)
	{
		g_p_Texture06 = new CTextureFromBMP();
		g_p_Texture06->SetDebug_cout_output( true );
		glActiveTexture( GL_TEXTURE6 );	
		if ( ! g_p_Texture06->CreateNewTextureFromBMPFile2(this->m_RootDirectoryName+it->first, GL_TEXTURE6, true) )
		{
			std::cout << "Didn't load the rock texture." << std::endl;
		}
		++it;
	}

	if (textures->size() >= 8)
	{
		g_p_Texture07 = new CTextureFromBMP();
		g_p_Texture07->SetDebug_cout_output( true );
		glActiveTexture( GL_TEXTURE7 );	
		if ( ! g_p_Texture07->CreateNewTextureFromBMPFile2(this->m_RootDirectoryName+it->first, GL_TEXTURE7, true) )
		{
			std::cout << "Didn't load the rock texture." << std::endl;
		}
		++it;
	}

	if (textures->size() >= 9)
	{
		g_p_Texture08 = new CTextureFromBMP();
		g_p_Texture08->SetDebug_cout_output( true );
		glActiveTexture( GL_TEXTURE8 );	
		if ( ! g_p_Texture08->CreateNewTextureFromBMPFile2(this->m_RootDirectoryName+it->first, GL_TEXTURE8, true) )
		{
			std::cout << "Didn't load the rock texture." << std::endl;
		}
		++it;
	}

	if (textures->size() >= 10)
	{
		g_p_Texture09 = new CTextureFromBMP();
		g_p_Texture09->SetDebug_cout_output( true );
		glActiveTexture( GL_TEXTURE9 );	
		if ( ! g_p_Texture09->CreateNewTextureFromBMPFile2(this->m_RootDirectoryName+it->first, GL_TEXTURE9, true) )
		{
			std::cout << "Didn't load the rock texture." << std::endl;
		}
		++it;
	}

	if (textures->size() >= 11)
	{
		g_p_Texture10 = new CTextureFromBMP();
		g_p_Texture10->SetDebug_cout_output( true );
		glActiveTexture( GL_TEXTURE10 );	// GL_TEXTURE0, GL_TEXTURE1, etc.
		if ( ! g_p_Texture10->CreateNewTextureFromBMPFile2(this->m_RootDirectoryName+it->first, GL_TEXTURE10, true) )
		{
			std::cout << "Didn't load brick brick texture." << std::endl;
		}
		++it;
	}

	if (textures->size() >= 12)
	{
		g_p_Texture11 = new CTextureFromBMP();
		g_p_Texture11->SetDebug_cout_output( true );
		glActiveTexture( GL_TEXTURE11 );	// GL_TEXTURE0, GL_TEXTURE1, etc.
		if ( ! g_p_Texture11->CreateNewTextureFromBMPFile2(this->m_RootDirectoryName+it->first, GL_TEXTURE11, true) )
		{
			std::cout << "Didn't load brick brick texture." << std::endl;
		}
		++it;
	}

	if (textures->size() >= 13)
	{
		g_p_Texture12 = new CTextureFromBMP();
		g_p_Texture12->SetDebug_cout_output( true );
		glActiveTexture( GL_TEXTURE12 );	// GL_TEXTURE0, GL_TEXTURE1, etc.
		if ( ! g_p_Texture12->CreateNewTextureFromBMPFile2(this->m_RootDirectoryName+it->first, GL_TEXTURE12, true) )
		{
			std::cout << "Didn't load brick brick texture." << std::endl;
		}
		++it;
	}

	if (textures->size() >= 14)
	{
		g_p_Texture12 = new CTextureFromBMP();
		g_p_Texture12->SetDebug_cout_output( true );
		glActiveTexture( GL_TEXTURE12 );	// GL_TEXTURE0, GL_TEXTURE1, etc.
		if ( ! g_p_Texture12->CreateNewTextureFromBMPFile2(this->m_RootDirectoryName+it->first, GL_TEXTURE12, true) )
		{
			std::cout << "Didn't load brick brick texture." << std::endl;
		}
		++it;
	}

	ExitOnGLError("ERROR: Could not Load Textures");


	return;
}

void CTextureLoaderManager::SetRootDirectory( std::string rootDirectory )
{
	this->m_RootDirectoryName = rootDirectory+"/";
	return;
}

std::string CTextureLoaderManager::GetRootDirectory(void)
{
	return this->m_RootDirectoryName;
}
