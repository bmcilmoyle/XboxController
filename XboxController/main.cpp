#include <iostream>

#include "CGameControllerManager.h"
#include "CHRTimer.h"

int main ()
{
	CGameControllerManager*		p_GameControllerManager = 0;
	CXBoxController*			p_GameController_0 = 0;

	//Set up controller manager
	p_GameControllerManager = CGameControllerManager::getGameControllerManager();
	p_GameController_0 = p_GameControllerManager->getController(0);

	//Controller will vibrate when program starts to confirm game works
	if ( !(( p_GameController_0 != 0 ) && ( p_GameController_0->bIsConnected() )) )
	{
		std::cout << "Didn't get an ID for the game controller; is there one plugged in?" << std::endl;
		return 1;
	}

	//Create a high resolution timer to get time between updates.
	//This is used to update the controller for functions like vibrate.
	CHRTimer timer;

	//Start the timer
	timer.Reset();
	timer.Start();		

	//loop
	for (;;)
	{
		//Get the time that has passd since the last call to the timer
		timer.Stop();
		float deltaTime = timer.GetElapsedSeconds();
		timer.Start();

		//Update the controller with the delta time
		p_GameController_0->Update(deltaTime);


		//Test controller input
		if (p_GameController_0->buttonDown(ABUTTON))
			std::cout << " A button pressed" << std::endl;

		if (p_GameController_0->buttonDown(BBUTTON))
			std::cout << " B button pressed" << std::endl;

		if (p_GameController_0->buttonDown(XBUTTON))
			std::cout << " X button pressed" << std::endl;

		if (p_GameController_0->buttonDown(YBUTTON))
			std::cout << " Y button pressed" << std::endl;

		if (p_GameController_0->buttonDown(STARTBUTTON))
			std::cout << " START button pressed" << std::endl;

		if (p_GameController_0->buttonDown(BACKBUTTON))
			std::cout << " BACK button pressed" << std::endl;

		if (p_GameController_0->buttonDown(LEFTBUMPER))
			std::cout << " LEFT BUMPER pressed" << std::endl;

		if (p_GameController_0->buttonDown(LEFTTRIGGER))
			std::cout << " LEFT TRIGGER pressed" << std::endl;

		if (p_GameController_0->buttonDown(LEFTSTICKCLICK))
			std::cout << " LEFT STICK pressed" << std::endl;

		if (p_GameController_0->buttonDown(RIGHTBUMPER))
			std::cout << " RIGHT BUMPER pressed" << std::endl;

		if (p_GameController_0->buttonDown(RIGHTTRIGGER))
			std::cout << " RIGHT TRIGGER pressed" << std::endl;

		if (p_GameController_0->buttonDown(RIGHTSTICKCLICK))
			std::cout << " RIGHT STICK pressed" << std::endl;

		if (p_GameController_0->buttonDown(DPADLEFT))
			std::cout << " D PAD LEFT pressed" << std::endl;

		if (p_GameController_0->buttonDown(DPADRIGHT))
			std::cout << " D PAD RIGHT pressed" << std::endl;

		if (p_GameController_0->buttonDown(DPADUP))
			std::cout << " D PAD UP pressed" << std::endl;

		if (p_GameController_0->buttonDown(DPADDOWN))
			std::cout << " D PAD DOWN pressed" << std::endl;


	}
}