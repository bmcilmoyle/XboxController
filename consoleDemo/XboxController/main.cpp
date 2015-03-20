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
		std::cout << " Delta time:  " << deltaTime * 100 << std::endl;
		timer.Start();

		//Update the controller with the delta time
		p_GameController_0->Update(deltaTime);


		//Test down functions
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

		//Test up functions
		if (p_GameController_0->buttonUp(ABUTTON))
			std::cout << " A button released" << std::endl;

		if (p_GameController_0->buttonUp(BBUTTON))
			std::cout << " B button released" << std::endl;

		if (p_GameController_0->buttonUp(XBUTTON))
			std::cout << " X button released" << std::endl;

		if (p_GameController_0->buttonUp(YBUTTON))
			std::cout << " Y button released" << std::endl;

		if (p_GameController_0->buttonUp(STARTBUTTON))
			std::cout << " START button released" << std::endl;

		if (p_GameController_0->buttonUp(BACKBUTTON))
			std::cout << " BACK button released" << std::endl;

		if (p_GameController_0->buttonUp(LEFTBUMPER))
			std::cout << " LEFT BUMPER released" << std::endl;

		if (p_GameController_0->buttonUp(LEFTTRIGGER))
			std::cout << " LEFT TRIGGER released" << std::endl;

		if (p_GameController_0->buttonUp(LEFTSTICKCLICK))
			std::cout << " LEFT STICK released" << std::endl;

		if (p_GameController_0->buttonUp(RIGHTBUMPER))
			std::cout << " RIGHT BUMPER released" << std::endl;

		if (p_GameController_0->buttonUp(RIGHTTRIGGER))
			std::cout << " RIGHT TRIGGER released" << std::endl;

		if (p_GameController_0->buttonUp(RIGHTSTICKCLICK))
			std::cout << " RIGHT STICK released" << std::endl;

		if (p_GameController_0->buttonUp(DPADLEFT))
			std::cout << " D PAD LEFT released" << std::endl;

		if (p_GameController_0->buttonUp(DPADRIGHT))
			std::cout << " D PAD RIGHT released" << std::endl;

		if (p_GameController_0->buttonUp(DPADUP))
			std::cout << " D PAD UP released" << std::endl;

		if (p_GameController_0->buttonUp(DPADDOWN))
			std::cout << " D PAD DOWN released" << std::endl;

		//test joysticks
		if(p_GameController_0->getLeftJoystickXAxis() != 0.0f)
			std::cout << " LEFT STICK X AXIS VALUE " << p_GameController_0->getLeftJoystickXAxis() << std::endl;

		if(p_GameController_0->getLeftJoystickYAxis() != 0.0f)
			std::cout << " LEFT STICK Y AXIS VALUE " << p_GameController_0->getLeftJoystickYAxis() << std::endl;

		if(p_GameController_0->getRightJoystickXAxis() != 0.0f)
			std::cout << " RIGHT STICK X AXIS VALUE " << p_GameController_0->getRightJoystickXAxis() << std::endl;

		if(p_GameController_0->getRightJoystickYAxis() != 0.0f)
			std::cout << " RIGHT STICK Y AXIS VALUE " << p_GameController_0->getRightJoystickYAxis() << std::endl;
	}
}