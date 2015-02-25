#ifndef _CHRTimer_HG_
#define _CHRTimer_HG_

// Used to do high resulution timing
// Used QueryPerformanceTimer() 
// http://msdn.microsoft.com/en-us/library/ms644905(VS.85).aspx

// Written by Michael Feeney, Fanshawe College, 2010
// mfeeney@fanshawec.on.ca
// It may be distributed under the terms of the General Public License:
// http://www.fsf.org/licenses/gpl.html
// Use this code at your own risk. It is indented only as a learning aid.
//

class CHRTimer
{
public:
	CHRTimer();
	virtual ~CHRTimer();
	void Start(void);
	void Stop(void);
	void Reset(void);
	float GetElapsedSeconds(void);	// Can also use __int64 (win32 specific)
	unsigned long long GetFrequency(void);	
private:
	unsigned long long m_elapsedTime;	// Can also use __int64 (win32 specific)
	unsigned long long m_startTime;		// Can also use __int64 (win32 specific)
	unsigned long long m_stopTime;		// Can also use __int64 (win32 specific)
	unsigned long long m_frequency;		// Can also use __int64 (win32 specific)
};

#endif // _CHRTimer_HG_