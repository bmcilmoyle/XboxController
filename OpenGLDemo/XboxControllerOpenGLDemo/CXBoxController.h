#ifndef _CXBoxController_HG_
#define _CXBoxController_HG_

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <XInput.h>		// for the actual controller

#include <list>
#include <vector>
#include <string>
#include <map>


#define ABUTTON 0
#define BBUTTON 1
#define XBUTTON 2
#define YBUTTON 3
#define LEFTBUMPER 4
#define RIGHTBUMPER 5
#define LEFTTRIGGER 6
#define RIGHTTRIGGER 7
#define BACKBUTTON 8
#define STARTBUTTON 9
#define LEFTSTICKCLICK 10
#define RIGHTSTICKCLICK 11
#define DPADLEFT 12
#define DPADRIGHT 13
#define DPADUP 14
#define DPADDOWN 15

// The darn includes and dlls are different between the various versions of Windows... Grrrr.
// See: https://katyscode.wordpress.com/2013/08/30/xinput-tutorial-part-1-adding-gamepad-support-to-your-windows-game/
#ifndef _DEBUG
#pragma comment(lib, "XINPUT9_1_0.LIB")
#endif


class CXBoxController
{
public:

	//C'tor
	CXBoxController( int playerNumber );

	//D'tor
	~CXBoxController();

	//When the button is first pushed in
	bool buttonDown(int buttonID);

	//When the button is being held down
	bool buttonPressed(int buttonID);

	//When the button is released
	bool buttonUp(int buttonID);

	enum enumDeadZoneCalculation
	{
		NO_DEAD_ZONE,
		AXIAL_DEAD_ZONE,
		RADIAL_DEAD_ZONE,
		SCALED_RADIAL_DEAD_ZONE		// Default
	};

	struct CVibStep
	{
	public:
		enum enumMotor { LEFT, RIGHT };
		enumMotor motor;
		float durationSeconds;
		float fSpeed;
		CVibStep( enumMotor motor_, float fSpeed_, float duration_ ) : 
			motor( motor_), fSpeed( fSpeed_ ), durationSeconds( duration_ ) {};
		CVibStep() {};
	};

	enumDeadZoneCalculation getDeadZoneCalculation(void);
	void setDeadZoneCalculation(enumDeadZoneCalculation newDeadZoneCalculation);

	std::string getDeadZoneCalculationString(void);
	void setDeadZone(float newDeadZone);
	float getDeadZone(void);

	XINPUT_STATE GetState(void);
	bool bIsConnected(void);
	// Values from 0 to 65,535 (XBox controller)
	void Vibrate( unsigned int intLeftVal, unsigned int intRightVal );
	// Values scaled from 0.0 to 1.0f
	void Vibrate( float fLeftValNormalized, float fRightValNormalized );

	// Update has to be called for this to work
	void AddVibrationSequence( const CVibStep &VibStep );
	void AddVibrationSequence( CVibStep::enumMotor motor_, float speed_, float duration_ );
	void AddVibrationSequence( const std::vector<CVibStep> &vecVibSequence );
	void ClearVibrationSequence(void);

	// This is used for the vibration sequence
	bool Update( float deltaSeconds );

	int GetPlayerNumber(void);

	float getLeftJoystickXAxis(void);
	float getLeftJoystickYAxis(void);
	float getLeftJoystickMagnitudeFromCentre(void);
	float getRightJoystickXAxis(void);
	float getRightJoystickYAxis(void);
	float getRightJoystickMagnitudeFromCentre(void);
	float getLeftTrigger(void);
	float getRightTrigger(void);

	bool bIsLeftTriggerDown(void);	// Depends on threshold
	bool bIsRightTriggerDown(void);	// Depends on threshold

private:
	CXBoxController() {};	// Can't call

	XINPUT_STATE m_State;
	int m_playerNumber;

	// These are the massaged values for the joysticks.
	void m_UpdateLeftJoystickValues(void);
	void m_UpdateRightJoystickValues(void);
	float m_LeftJoyX;
	float m_LeftJoyY;
	float m_LeftJoyMagnitude;
	float m_RightJoyX;
	float m_RightJoyY;
	float m_RightJoyMagnitude;

	void updateInputMaps();

	enumDeadZoneCalculation m_deadZoneCalculation;
	void m_adjustForDeadZone(void);
	float m_deadZone;
	static const float DEAD_ZONE_DEFAULT;

	float leftTriggerThreshold;
	float rightTriggerThreshold;

	static const unsigned int MINVIBRATEMOTORSPEED = 0;
	static const unsigned int MAXVIBRATEMOTORSPEED = 65535;

	std::list<CVibStep> m_VibSequenceLeft;
	std::list<CVibStep> m_VibSequenceRight;
	float m_elapsedSeconds;

	std::map<int,int> m_buttonDown;
	std::map<int,int> m_buttonPressed;
	std::map<int,int> m_buttonUp;
	std::map<int,int> m_buttonPressedLastUpdate;

#ifdef _DEBUG
	// These are to get the process addresses so we don't have to link to the libraries any more
	// Gets the process addresses
	static bool Init(void);	 
	// Will be true once one controller is created.
	static bool bGetProcCompleted;
	static bool bCantResolveProcAddresses_SoAllIsLost;		// If this is false, then the controller won't work

	//      DWORD XInputGetState              ( _In_ DWORD dwUserIndex, _Out_ XINPUT_STATE *pState );
	typedef DWORD (WINAPI *XBoxInputGetState) ( DWORD dwUserIndex, XINPUT_STATE *pState );
	//      DWORD XInputSetState              ( _In_ DWORD dwUserIndex, _Inout_ XINPUT_VIBRATION *pVibration );
	typedef DWORD (WINAPI *XBoxInputSetState) ( DWORD dwUserIndex, XINPUT_VIBRATION *pVibration );

	static XBoxInputGetState pXboxInputGetState;
	static XBoxInputSetState pXboxInputSetState;
#endif

};

#endif