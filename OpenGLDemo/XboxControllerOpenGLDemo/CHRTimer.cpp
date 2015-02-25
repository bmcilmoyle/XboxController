#include "CHRTimer.h"
#include "windows.h"	// For high freq

// Written by Michael Feeney, Fanshawe College, 2010
// mfeeney@fanshawec.on.ca
// It may be distributed under the terms of the General Public License:
// http://www.fsf.org/licenses/gpl.html
// Use this code at your own risk. It is indented only as a learning aid.
//

//__int64 start_count;
//__int64 end_count;
//__int64 freq;
//
//// Get the frequency and save it, it shouldn't change
//QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
//QueryPerformanceCounter((LARGE_INTEGER*)&start_count);
//// do some stuff that takes up time
//QueryPerformanceCounter((LARGE_INTEGER*)&end_count);
////find the time
//float time = (float)(end_count - start_count) / (float)freq;


CHRTimer::CHRTimer()
{
	this->m_elapsedTime = 0;
	this->m_startTime = 0;
	this->m_stopTime = 0;

	LARGE_INTEGER tempTime;
	
	QueryPerformanceFrequency( &tempTime ); 

	this->m_frequency = static_cast<unsigned long long>( tempTime.QuadPart );
}

CHRTimer::~CHRTimer()
{
}

void CHRTimer::Start(void)
{
	LARGE_INTEGER tempTime;
	QueryPerformanceCounter( &tempTime );
	this->m_startTime = static_cast<unsigned long long>( tempTime.QuadPart ) ;
}

void CHRTimer::Stop(void)
{
	LARGE_INTEGER tempTime;
	QueryPerformanceCounter( &tempTime );
	this->m_stopTime = static_cast<unsigned long long>( tempTime.QuadPart ) ;
}

void CHRTimer::Reset(void)
{
	this->m_elapsedTime = 0;
	this->m_startTime = 0;
	this->m_stopTime = 0;
}

float CHRTimer::GetElapsedSeconds(void)
{
	// Update current time
	LARGE_INTEGER tempTime;
	QueryPerformanceCounter( &tempTime );
	this->m_stopTime = static_cast<unsigned long long>( tempTime.QuadPart ) ;

	// Calculate elapsed time
	this->m_elapsedTime = this->m_stopTime - this->m_startTime;
	float elapsedTimeSec = static_cast<float>( this->m_elapsedTime ) / this->m_frequency;
	return elapsedTimeSec;
}

unsigned long long CHRTimer::GetFrequency(void)
{
	return this->m_frequency;
}