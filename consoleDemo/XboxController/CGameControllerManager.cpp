#include "CGameControllerManager.h"

// Sigleton method
/* static */
CGameControllerManager* CGameControllerManager::getGameControllerManager(void)
{
	if ( CGameControllerManager::m_sp_theOneAndOnlyGameControllerManager == 0 )
	{
		CGameControllerManager::m_sp_theOneAndOnlyGameControllerManager = new CGameControllerManager();
	}

	return CGameControllerManager::m_sp_theOneAndOnlyGameControllerManager;
}

/* static */
CGameControllerManager* CGameControllerManager::m_sp_theOneAndOnlyGameControllerManager = 0;


CGameControllerManager::CGameControllerManager()
{
	for ( int index = CGameControllerManager::MINCONTROLLERINDEX; index != CGameControllerManager::MAXNUMCONTROLLERS; index++ )
	{
		this->m_GameControllers[index] = 0;
	}
	return;
}

CGameControllerManager::~CGameControllerManager()
{
	// Get rid of all the controllers
	for ( int index = CGameControllerManager::MINCONTROLLERINDEX; index != CGameControllerManager::MAXNUMCONTROLLERS; index++ )
	{
		if ( this->m_GameControllers[index] != 0 )
		{
			delete this->m_GameControllers[index];
		}
	}
	return;
}

//You can have 4 controllers max, values 0 - 3
//From: http://msdn.microsoft.com/en-us/library/windows/desktop/microsoft.directx_sdk.reference.xinputgetstate(v=vs.85).aspx
CXBoxController* CGameControllerManager::getController(int playerNumber)
{
	if ( ( playerNumber < CGameControllerManager::MINCONTROLLERINDEX ) || 
		( playerNumber >= CGameControllerManager::MAXNUMCONTROLLERS ) )
	{
		return 0;	// Invalid
	}
	if ( this->m_GameControllers[playerNumber] == 0 )
	{
		this->m_GameControllers[playerNumber] = new CXBoxController(playerNumber);
	}
	return this->m_GameControllers[playerNumber];
}

CXBoxController* CGameControllerManager::getNextController(void)
{
	// Got any controllers? 
	std::vector<unsigned int> vecControllerIDs = this->getNumberOfActiveControllers();

	// Any there? 
	if ( vecControllerIDs.empty() )
	{	// Nope, so create one and return it
		return this->getController( CGameControllerManager::MINCONTROLLERINDEX );
	}
	// or return the first one we found
	return this->getController( *( vecControllerIDs.begin() ) );
}

std::vector<unsigned int> CGameControllerManager::getNumberOfActiveControllers(void)
{
	std::vector<unsigned int> vecReturnIDs;
	for ( int index = CGameControllerManager::MINCONTROLLERINDEX; index != CGameControllerManager::MAXNUMCONTROLLERS; index++ )
	{
		if ( this->m_GameControllers[index] != 0 )
		{
			vecReturnIDs.push_back( index );
		}
	}

	return vecReturnIDs;
}

bool CGameControllerManager::deleteController(int playerNumber)
{
	if ( ( playerNumber < CGameControllerManager::MINCONTROLLERINDEX ) || 
		( playerNumber >= CGameControllerManager::MAXNUMCONTROLLERS ) )
	{
		return false;
	}
	delete this->m_GameControllers[playerNumber];
	return true;
}
