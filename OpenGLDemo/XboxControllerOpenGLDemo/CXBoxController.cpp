#include "CXBoxController.h"
#include <math.h>	// for sqrt() and fabs() in joystick thingy
#include <algorithm>

CXBoxController::CXBoxController( int playerNumber ) 
{
#ifdef _DEBUG
	if ( !CXBoxController::bGetProcCompleted )
	{
		CXBoxController::Init();
		CXBoxController::bGetProcCompleted = true;
	}
#endif

	this->m_playerNumber = playerNumber;
	ZeroMemory( &this->m_State, sizeof( this->m_State ) );
	this->m_deadZoneCalculation = SCALED_RADIAL_DEAD_ZONE;
	this->m_deadZone = CXBoxController::DEAD_ZONE_DEFAULT;
	this->leftTriggerThreshold = 0.9f;
	this->rightTriggerThreshold = 0.9f;
	return;
}

CXBoxController::~CXBoxController() 
{
	// Stop any vibration, just in case
	this->Vibrate(static_cast<unsigned int>(0), static_cast<unsigned int>(0));

	//Set all the buttons to false to start
	for (int i =0; 0 < 16; i++)
	{
		this->m_buttonPressed[i]=false;
		this->m_buttonPressedLastUpdate[i] = false;
	}

	return;
}

//typedef void (WINAPI *PGNSI)(LPSYSTEM_INFO);
//DWORD XInputGetState( _In_   DWORD dwUserIndex, _Out_  XINPUT_STATE *pState );
//typedef DWORD (WINAPI *XBoxInputGetState) ( DWORD dwUserIndex, XINPUT_STATE *pState );

//*********************************************************************************************************/
// We only need to do this in release mode since The Great Windows won't return the 
// process addresses for the controller, but ONLY in RELEASE mode. What'cha gonna do?
// If we are in DEBUG, we query the functions (so we don't have to have the DLL)
// If we are in RELEASE, we need to include the lib file, but DON'T need the DLL
#ifdef _DEBUG
//static
bool CXBoxController::Init(void)
{
	// Have we tried this before and it didn't work?
	if ( CXBoxController::bCantResolveProcAddresses_SoAllIsLost )
	{	// Yup, so let's not bother, OK? Are you OK with that? Good.
		return false;
	}
	// Have we already set this stuff up?
	if ( CXBoxController::bGetProcCompleted )
	{	// Yup. Done and done. Good job, computer.
		return true;
	}

	// Windows 8 (XInput 1.4), DirectX SDK (XInput 1.3), Windows Vista (XInput 9.1.0)
	//Xinput1_4.dll;
	//Xinput9_1_0.dll
	HINSTANCE dllHandle = NULL;
	dllHandle = LoadLibrary(L"Xinput9_1_0.dll");	
	if ( dllHandle == NULL )
	{	// All is lost
		CXBoxController::bCantResolveProcAddresses_SoAllIsLost = true;
		return false;
	}
	HMODULE han = GetModuleHandle(L"Xinput9_1_0.dll");
	if ( han == NULL )
	{	// All is lost
		CXBoxController::bCantResolveProcAddresses_SoAllIsLost = true;
		return false;
	}

	CXBoxController::pXboxInputGetState = (CXBoxController::XBoxInputGetState)::GetProcAddress( han, "XInputGetState" );
	CXBoxController::pXboxInputSetState = (CXBoxController::XBoxInputSetState)::GetProcAddress( han, "XInputSetState" );
	// See if we got the addresses for these functions
	if ( (pXboxInputGetState == NULL) || (pXboxInputSetState == NULL) )
	{	// All is lost
		CXBoxController::bCantResolveProcAddresses_SoAllIsLost = true;
		return false;
	}

	CXBoxController::bGetProcCompleted = true;

	return true;
}


/*static*/ 
CXBoxController::XBoxInputGetState CXBoxController::pXboxInputGetState = 0;
/*static*/ 
CXBoxController::XBoxInputSetState CXBoxController::pXboxInputSetState = 0;
/*static*/ 
bool CXBoxController::bGetProcCompleted = false;
bool CXBoxController::bCantResolveProcAddresses_SoAllIsLost = false;

#endif
//*********************************************************************************************************/

bool CXBoxController::buttonDown(int buttonID)
{
	return (!this->m_buttonPressedLastUpdate[buttonID]) && (this->m_buttonPressed[buttonID]);
}

bool CXBoxController::buttonPressed(int buttonID)
{
	return this->m_buttonPressed[buttonID] != 0; 
}

bool CXBoxController::buttonUp(int buttonID)
{
	return (this->m_buttonPressedLastUpdate[buttonID]) && (!this->m_buttonPressed[buttonID]);
}

CXBoxController::enumDeadZoneCalculation CXBoxController::getDeadZoneCalculation(void)
{
	return this->m_deadZoneCalculation;
}

void CXBoxController::setDeadZoneCalculation(enumDeadZoneCalculation newDeadZoneSetting)
{
	this->m_deadZoneCalculation = newDeadZoneSetting;
	return;
}


std::string CXBoxController::getDeadZoneCalculationString(void)
{
	switch ( this->m_deadZoneCalculation )
	{
	case NO_DEAD_ZONE:
		return "No Dead Zone";
		break;
	case AXIAL_DEAD_ZONE:
		return "Axial Dead Zone";
		break;
	case RADIAL_DEAD_ZONE:
		return "Radial Dead Zone";
		break;
	case SCALED_RADIAL_DEAD_ZONE:
		return "Scaled Radial Dead Zone";
		break;
	}
	return "Unknown";
}

/*static*/ 
const float CXBoxController::DEAD_ZONE_DEFAULT = 0.1f;

void CXBoxController::setDeadZone(float newDeadZone)
{
	this->m_deadZone = newDeadZone;
	return;
}

float CXBoxController::getDeadZone(void)
{
	return this->m_deadZone;
}

void CXBoxController::updateInputMaps()
{
	//Save the last state of the buttons
	this->m_buttonPressedLastUpdate[ABUTTON] = this->m_buttonPressed[ABUTTON];
	this->m_buttonPressedLastUpdate[BBUTTON] = this->m_buttonPressed[BBUTTON];
	this->m_buttonPressedLastUpdate[XBUTTON] = this->m_buttonPressed[XBUTTON];
	this->m_buttonPressedLastUpdate[YBUTTON] = this->m_buttonPressed[YBUTTON];
	this->m_buttonPressedLastUpdate[LEFTBUMPER] = this->m_buttonPressed[LEFTBUMPER];
	this->m_buttonPressedLastUpdate[RIGHTBUMPER] = this->m_buttonPressed[RIGHTBUMPER];
	this->m_buttonPressedLastUpdate[LEFTTRIGGER] = this->m_buttonPressed[LEFTTRIGGER];
	this->m_buttonPressedLastUpdate[RIGHTTRIGGER] = this->m_buttonPressed[RIGHTTRIGGER];
	this->m_buttonPressedLastUpdate[BACKBUTTON] = this->m_buttonPressed[BACKBUTTON];
	this->m_buttonPressedLastUpdate[STARTBUTTON] = this->m_buttonPressed[STARTBUTTON];
	this->m_buttonPressedLastUpdate[LEFTSTICKCLICK] = this->m_buttonPressed[LEFTSTICKCLICK];
	this->m_buttonPressedLastUpdate[RIGHTSTICKCLICK] = this->m_buttonPressed[RIGHTSTICKCLICK];
	this->m_buttonPressedLastUpdate[DPADLEFT] = this->m_buttonPressed[DPADLEFT];
	this->m_buttonPressedLastUpdate[DPADRIGHT] = this->m_buttonPressed[DPADRIGHT];
	this->m_buttonPressedLastUpdate[DPADUP] = this->m_buttonPressed[DPADUP];
	this->m_buttonPressedLastUpdate[DPADDOWN] = this->m_buttonPressed[DPADDOWN];

	//Get the current status of the buttons
	this->m_buttonPressed[ABUTTON] = this->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_A;
	this->m_buttonPressed[BBUTTON] = this->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_B;
	this->m_buttonPressed[XBUTTON] = this->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_X;
	this->m_buttonPressed[YBUTTON] = this->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_Y;
	this->m_buttonPressed[LEFTBUMPER] = this->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER;
	this->m_buttonPressed[RIGHTBUMPER] = this->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER;
	this->m_buttonPressed[LEFTTRIGGER] = this->bIsLeftTriggerDown();
	this->m_buttonPressed[RIGHTTRIGGER] = this->bIsRightTriggerDown();
	this->m_buttonPressed[BACKBUTTON] = this->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_BACK;
	this->m_buttonPressed[STARTBUTTON] = this->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_START;
	this->m_buttonPressed[LEFTSTICKCLICK] = this->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB;
	this->m_buttonPressed[RIGHTSTICKCLICK] = this->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB;
	this->m_buttonPressed[DPADLEFT] = this->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT;
	this->m_buttonPressed[DPADRIGHT] = this->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT;
	this->m_buttonPressed[DPADUP] = this->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP;
	this->m_buttonPressed[DPADDOWN] = this->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN;
}

// from: http://www.third-helix.com/2013/04/12/doing-thumbstick-dead-zones-right.html
void CXBoxController::m_adjustForDeadZone(void)
{
	this->m_UpdateLeftJoystickValues();
	this->m_UpdateRightJoystickValues();

	switch ( this->m_deadZoneCalculation )
	{
	case CXBoxController::AXIAL_DEAD_ZONE:
		if ( fabsf( this->m_LeftJoyX ) < this->m_deadZone )	{ this->m_LeftJoyX = 0.0f; }
		if ( fabsf( this->m_LeftJoyY ) < this->m_deadZone ) { this->m_LeftJoyY = 0.0f; }
		if ( fabsf( this->m_RightJoyX ) < this->m_deadZone ) { this->m_RightJoyX = 0.0f; }
		if ( fabsf( this->m_RightJoyY ) < this->m_deadZone ) { this->m_RightJoyY = 0.0f; }
		break;
	case CXBoxController::RADIAL_DEAD_ZONE:
		if ( this->m_LeftJoyMagnitude < this->m_deadZone ) { this->m_LeftJoyX = this->m_LeftJoyY = 0.0f; }
		if ( this->m_RightJoyMagnitude < this->m_deadZone ) { this->m_RightJoyX = this->m_RightJoyY = 0.0f; }
		break;
	case CXBoxController::SCALED_RADIAL_DEAD_ZONE:
		if ( this->m_LeftJoyMagnitude < this->m_deadZone ) { this->m_LeftJoyX = this->m_LeftJoyY = 0.0f; }
		else
		{
			float adjustLeft = (this->m_LeftJoyMagnitude - this->m_deadZone) / ( 1.0f - this->m_deadZone );
			this->m_LeftJoyX *= adjustLeft;
			this->m_LeftJoyY *= adjustLeft;
			float adjustRight = (this->m_RightJoyMagnitude - this->m_deadZone) / ( 1.0f - this->m_deadZone );
			this->m_RightJoyX *= adjustRight;
			this->m_RightJoyY *= adjustRight;
		}

		break;
		// else...
		// No dead zone
	}

	return;
}

XINPUT_STATE CXBoxController::GetState(void)
{
	ZeroMemory( &this->m_State, sizeof( this->m_State ) );

#ifdef _DEBUG
	// Call the pointer functions (from GetProcAddress)
	if ( !CXBoxController::bGetProcCompleted ) { return this->m_State; }
	CXBoxController::pXboxInputGetState( this->m_playerNumber, &this->m_State );
#else
	::XInputGetState( this->m_playerNumber, &this->m_State );
#endif

	return this->m_State;
}

int CXBoxController::GetPlayerNumber(void)
{
	return this->m_playerNumber;
}


bool CXBoxController::bIsConnected(void)
{
	ZeroMemory( &this->m_State, sizeof( this->m_State ) );

#ifdef _DEBUG
	// Call the pointer functions (from GetProcAddress)
	if ( !CXBoxController::bGetProcCompleted ) { return false; }
	DWORD hr = CXBoxController::pXboxInputGetState( this->m_playerNumber, &this->m_State );
#else
	DWORD hr = ::XInputGetState( this->m_playerNumber, &this->m_State );
#endif


	if ( hr == ERROR_SUCCESS )
	{
		return true;
	}
	else
	{
		return false;
	}
}

void CXBoxController::Vibrate( unsigned int leftVal, unsigned int rightVal )
{
	XINPUT_VIBRATION vibration;
	ZeroMemory( &vibration, sizeof( vibration ) );
	vibration.wLeftMotorSpeed = static_cast<WORD>(leftVal);
	vibration.wRightMotorSpeed = static_cast<WORD>(rightVal);

#ifdef _DEBUG
	// Call the pointer functions (from GetProcAddress)
	if ( !CXBoxController::bGetProcCompleted ) { return; }
	DWORD returnVal = CXBoxController::pXboxInputSetState( this->m_playerNumber, &vibration );
#else
	DWORD returnVal = XInputSetState( this->m_playerNumber, &vibration );
#endif
	return;
}


void CXBoxController::Vibrate( float fLeftValNormalized, float fRightValNormalized )
{
	if ( fLeftValNormalized > 1.0f ) { fLeftValNormalized = 1.0f; }
	if ( fLeftValNormalized < 0.0f ) { fLeftValNormalized = 0.0f; }
	if ( fRightValNormalized > 1.0f )	{ fRightValNormalized = 1.0f; }
	if ( fLeftValNormalized < 0.0f )	{ fLeftValNormalized = 0.0f; }

	unsigned int iLeftVal = static_cast<unsigned int>( fLeftValNormalized * ( CXBoxController::MAXVIBRATEMOTORSPEED - CXBoxController::MINVIBRATEMOTORSPEED ) );
	unsigned int iRightVal = static_cast<unsigned int>( fRightValNormalized * ( CXBoxController::MAXVIBRATEMOTORSPEED - CXBoxController::MINVIBRATEMOTORSPEED ) );

	this->Vibrate( iLeftVal, iRightVal );
	return;
}

// This is really only used for vibration effects
void CXBoxController::AddVibrationSequence( const std::vector<CVibStep> &vecVibSequence )
{
	// Append the next sequence to current one...
	for ( std::vector<CVibStep>::const_iterator itVib = vecVibSequence.begin(); 
		itVib != vecVibSequence.end(); itVib++ )
	{
		// Add the appropriate steps to the appropriate motors
		switch ( itVib->motor )
		{
		case CVibStep::LEFT:
			this->m_VibSequenceLeft.insert( this->m_VibSequenceLeft.end(), *itVib );
			break;
		case CVibStep::RIGHT:
			this->m_VibSequenceRight.insert( this->m_VibSequenceRight.end(), *itVib );
			break;
		}
	}
	return;
}

void CXBoxController::AddVibrationSequence( const CVibStep &VibStep )
{
	// Add the appropriate steps to the appropriate motors
	switch ( VibStep.motor )
	{
	case CVibStep::LEFT:
		this->m_VibSequenceLeft.insert( this->m_VibSequenceLeft.end(), VibStep );
		break;
	case CVibStep::RIGHT:
		this->m_VibSequenceRight.insert( this->m_VibSequenceRight.end(), VibStep );
		break;
	}
	return;
}

void CXBoxController::AddVibrationSequence( CVibStep::enumMotor motor_, float speed_, float duration_ )
{
	CXBoxController::CVibStep vibStep( motor_, speed_, duration_ );
	this->AddVibrationSequence( vibStep );
	return;
}


void CXBoxController::ClearVibrationSequence(void)
{
	this->m_VibSequenceLeft.clear();
	this->m_VibSequenceRight.clear();
	this->Vibrate( static_cast<unsigned int>(0), static_cast<unsigned int>(0) );
	return;
}

static float lg_lastLeftVibSpeed;
static float lg_lastRightVibSpeed;

// This is used for the vibration sequence
bool CXBoxController::Update( float deltaSeconds )
{
	//button down
	this->updateInputMaps();

	float lVib = 0.0f;
	float rVib = 0.0f;
	// Go through each list, seeing if it's time for a change...
	if ( !this->m_VibSequenceLeft.empty() )
	{	
		lVib = this->m_VibSequenceLeft.begin()->fSpeed;
		// Decrement the time
		this->m_VibSequenceLeft.begin()->durationSeconds -= deltaSeconds;
		// Still time? 
		if ( this->m_VibSequenceLeft.begin()->durationSeconds <= 0.0f )
		{	// nope, so delete this node
			this->m_VibSequenceLeft.erase( this->m_VibSequenceLeft.begin() );
		}
	}

	if ( !this->m_VibSequenceRight.empty() )
	{	
		rVib = this->m_VibSequenceRight.begin()->fSpeed;
		// Decrement the time
		this->m_VibSequenceRight.begin()->durationSeconds -= deltaSeconds;
		// Still time? 
		if ( this->m_VibSequenceRight.begin()->durationSeconds <= 0.0f )
		{	// nope, so delete this node
			this->m_VibSequenceRight.erase( this->m_VibSequenceRight.begin() );
		}
	}

	// Have the vibration speeds changed since the last call?
	if ( ( lg_lastLeftVibSpeed != lVib ) || ( lg_lastRightVibSpeed != rVib ) )
	{
		// Save the current speed (replacing the old speed)
		lg_lastLeftVibSpeed = lVib;
		lg_lastRightVibSpeed = rVib;
		// Yes, so update the vibration
		this->Vibrate( lVib, rVib );
	}

	return true;
}

float CXBoxController::getLeftJoystickXAxis(void)
{
	this->m_UpdateLeftJoystickValues();
	return this->m_LeftJoyX;
}

float CXBoxController::getLeftJoystickYAxis(void)
{
	this->m_adjustForDeadZone();
	return this->m_LeftJoyY;
}

float CXBoxController::getRightJoystickXAxis(void)
{
	this->m_adjustForDeadZone();
	return this->m_RightJoyX;
}

float CXBoxController::getRightJoystickYAxis(void)
{
	this->m_adjustForDeadZone();
	return this->m_RightJoyY;
}

float CXBoxController::getLeftTrigger(void)
{
	int trigVal = this->GetState().Gamepad.bLeftTrigger;
	if ( trigVal > XINPUT_GAMEPAD_TRIGGER_THRESHOLD )
	{
		return static_cast<float>(trigVal) / static_cast<float>(255);
	}
	return 0.0f;
}

float CXBoxController::getRightTrigger(void)
{
	int trigVal = this->GetState().Gamepad.bRightTrigger;
	if ( trigVal > XINPUT_GAMEPAD_TRIGGER_THRESHOLD )
	{
		return static_cast<float>(trigVal) / static_cast<float>(255);
	}
	return 0.0f;
}

bool CXBoxController::bIsLeftTriggerDown(void)
{
	if ( this->getLeftTrigger() > leftTriggerThreshold )
	{
		return true;
	}
	return false;
}

bool CXBoxController::bIsRightTriggerDown(void)
{
	if ( this->getRightTrigger() > rightTriggerThreshold )
	{
		return true;
	}
	return false;
}

float CXBoxController::getLeftJoystickMagnitudeFromCentre(void)
{
	this->m_UpdateLeftJoystickValues();
	return this->m_LeftJoyMagnitude;
}

float CXBoxController::getRightJoystickMagnitudeFromCentre(void)
{
	this->m_UpdateRightJoystickValues();
	return this->m_RightJoyMagnitude;
}

float getLeftJoystickMagnitudeFromCentre(void);

void CXBoxController::m_UpdateLeftJoystickValues(void)
{	// From: http://msdn.microsoft.com/en-us/library/windows/desktop/ee417001(v=vs.85).aspx
	XINPUT_STATE state = this->GetState();

	float LX = state.Gamepad.sThumbLX;
	float LY = state.Gamepad.sThumbLY;

	//determine how far the controller is pushed
	float magnitude = sqrt(LX*LX + LY*LY);

	//determine the direction the controller is pushed
	this->m_LeftJoyX = LX / magnitude;
	this->m_LeftJoyY = LY / magnitude;

	this->m_LeftJoyMagnitude = 0.0f;

	//check if the controller is outside a circular dead zone
	if (magnitude > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE )
	{
		//clip the magnitude at its expected maximum value
		if (magnitude > 32767) magnitude = 32767;

		//adjust magnitude relative to the end of the dead zone
		magnitude -= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;

		//optionally normalize the magnitude with respect to its expected range
		//giving a magnitude value of 0.0 to 1.0
		this->m_LeftJoyMagnitude = magnitude / (32767 - XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE );
	}
	else //if the controller is in the deadzone zero out the magnitude
	{
		magnitude = 0.0;
		this->m_LeftJoyMagnitude = 0.0;
		//
		this->m_LeftJoyX = 0.0f;
		this->m_LeftJoyY = 0.0f;
	}

	return;
}

void CXBoxController::m_UpdateRightJoystickValues(void)
{	// From: http://msdn.microsoft.com/en-us/library/windows/desktop/ee417001(v=vs.85).aspx
	XINPUT_STATE state = this->GetState();

	float RX = state.Gamepad.sThumbRX;
	float RY = state.Gamepad.sThumbRY;

	//determine how far the controller is pushed
	float magnitude = sqrt(RX*RX + RY*RY);

	//determine the direction the controller is pushed
	this->m_RightJoyX = RX / magnitude;
	this->m_RightJoyY = RY / magnitude;

	this->m_RightJoyMagnitude = 0.0f;

	//check if the controller is outside a circular dead zone
	if (magnitude > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE )
	{
		//clip the magnitude at its expected maximum value
		if (magnitude > 32767) magnitude = 32767;

		//adjust magnitude relative to the end of the dead zone
		magnitude -= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;

		//optionally normalize the magnitude with respect to its expected range
		//giving a magnitude value of 0.0 to 1.0
		this->m_RightJoyMagnitude = magnitude / (32767 - XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE );
	}
	else //if the controller is in the deadzone zero out the magnitude
	{
		magnitude = 0.0;
		this->m_RightJoyMagnitude = 0.0;
		this->m_RightJoyX = 0.0f;
		this->m_RightJoyY = 0.0f;
	}

	return;
}
