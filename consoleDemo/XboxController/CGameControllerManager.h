#ifndef _CGameControllerManager_HG_
#define _CGameControllerManager_HG_

#include "CXboxController.h"
#include <vector>


// This is a singleton
class CGameControllerManager
{
public:
	static CGameControllerManager* getGameControllerManager(void);
	~CGameControllerManager();
	
	// Can pass zero (0) to get whatever next controller there is
	// Returns null (0) if can't get one
	CXBoxController* getController(int playerNumber);

	// This one, you don't pass a number; it just picks the next one
	CXBoxController* getNextController(void);
	std::vector<unsigned int> getNumberOfActiveControllers(void);
	// This will remove a controller (why would you do this??)
	bool deleteController(int playerNumber);

private: 
	// Can't be called directly (it's a singleton)
	CGameControllerManager();
	static CGameControllerManager* m_sp_theOneAndOnlyGameControllerManager;

	static const unsigned int MINCONTROLLERINDEX = 0;
	static const unsigned int MAXNUMCONTROLLERS = 3;
	static unsigned int m_nextControllerID;
	CXBoxController* m_GameControllers[MAXNUMCONTROLLERS];
};

#endif

	