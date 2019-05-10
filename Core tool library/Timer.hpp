//
// Copyright © 2003-2010, by YaPB Development Team. All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
// Timer.hpp
//
// Class: Timer
//
// Description: Simple timer. Code from OGRE engine.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef TIMER_INCLUDED
#define TIMER_INCLUDED

class CommonTimer
{
	public:
		long long m_frequency;
		long long m_lastCount;

	public:
		inline CommonTimer (void) : m_frequency (1LL)
		{
			QueryPerformanceFrequency (reinterpret_cast <LARGE_INTEGER *> (&m_frequency));	// Get the constant frequency
		}

	public:
		inline const long long GetFrequency (void) const { return m_frequency; }

		template <typename elementType> inline const elementType GetSeconds (void);
		template </* float */> inline const float GetSeconds (void)
		{
			QueryPerformanceCounter (reinterpret_cast <LARGE_INTEGER *> (&m_lastCount));

			return static_cast <float> (m_lastCount) / GetFrequency ();
		}
		template </* double */> inline const double GetSeconds (void)
		{
			QueryPerformanceCounter (reinterpret_cast <LARGE_INTEGER *> (&m_lastCount));

			return static_cast <double> (m_lastCount) / GetFrequency ();
		}

		template <typename elementType> inline const elementType GetMilliseconds (void);
		template </* float */> inline const float GetMilliseconds (void)
		{
			return GetSeconds <float> () * 1000.0f;
		}
		template </* double */> inline const double GetMilliseconds (void)
		{
			return GetSeconds <double> () * 1000.0;
		}

		template <typename elementType> inline const elementType GetMicroseconds (void);
		template </* float */> inline const float GetMicroseconds (void)
		{
			return GetSeconds <float> () * 1000000.0f;
		}
		template </* double */> inline const double GetMicroseconds (void)
		{
			return GetSeconds <double> () * 1000000.0;
		}
};

extern DLL_GLOBAL CommonTimer g_commonTimer;

class Timer
{
	//
	// Group: Private members.
	//
	private:
		clock_t      m_zeroClock;

		unsigned int m_startTick;
		long long    m_lastTime;
		long long    m_startTime;

		unsigned int m_timerMask;

	//
	// Group: (Con/De)structors.
	//
	public:
		/** Timer constructor. MUST be called on same thread that calls GetMilliseconds() */
		inline Timer (void) : m_zeroClock (0l), m_startTick (0u), m_lastTime (0ll), m_startTime (0ll), m_timerMask (0u)
		{
			Reset ();
		}

	//
	// Group: Functions.
	//
	public:
		/** Resets timer */
		inline void Reset (void)
		{
			// Find the lowest core that this process uses
			if (m_timerMask == 0u)
			{
				// Get the current process core mask
				unsigned long procMask, sysMask;

				GetProcessAffinityMask (GetCurrentProcess (), &procMask, &sysMask);

				// If procMask is 0, consider there is only one core available
				// (using 0 as procMask will cause an infinite loop below)
				if (procMask == 0ul)
					procMask = 1ul;

				m_timerMask = 1u;

				while ((m_timerMask & procMask) == 0u)
					m_timerMask <<= 1u;
			}

			const HANDLE thread (GetCurrentThread ());
			const unsigned int oldMask (SetThreadAffinityMask (thread, m_timerMask));	// Set affinity to the first core

			QueryPerformanceCounter (reinterpret_cast <LARGE_INTEGER *> (&m_startTime));	// Query the timer

			m_startTick = GetTickCount ();

			// Reset affinity
			SetThreadAffinityMask (thread, oldMask);

			m_lastTime = 0ll;
			m_zeroClock = clock ();
		}

		/** Returns milliseconds since initialisation or last reset */
		inline const unsigned int GetMilliseconds (void)
		{
			long long currentTime (0ll);
			const HANDLE thread (GetCurrentThread ());
			const unsigned int oldMask (SetThreadAffinityMask (thread, m_timerMask));	// Set affinity to the first core

			QueryPerformanceCounter (reinterpret_cast <LARGE_INTEGER *> (&currentTime));	// Query the timer
			SetThreadAffinityMask (thread, oldMask);	// Reset affinity

			long long newTime (currentTime - m_startTime);
			unsigned int newTicks (static_cast <unsigned int> (1000ll * newTime / g_commonTimer.GetFrequency ()));	// get milliseconds to check against GetTickCount()
			const int msecOff (static_cast <int> (newTicks - (GetTickCount () - m_startTick)));

			// detect and compensate for performance counter leaps (surprisingly common, see Microsoft KB: Q274323)
			if (msecOff < -100l || msecOff > 100l)
			{
				// We must keep the timer running forward :)
				const long long adjust (Math::GetMinimumValueBetween (msecOff * g_commonTimer.GetFrequency () / 1000ll, newTime - m_lastTime));

				m_startTime += adjust;
				newTime -= adjust;

				// Re-calculate milliseconds
				newTicks = static_cast <unsigned int> (1000ll * newTime / g_commonTimer.GetFrequency ());	// scale by 1000 for milliseconds
			}

			// Record last time for adjust
			m_lastTime = newTime;

			return newTicks;
		}

		/** Returns microseconds since initialisation or last reset */
		inline const unsigned int GetMicroseconds (void)
		{
			long long currentTime (0ll);
			const HANDLE thread (GetCurrentThread ());
			const unsigned int oldMask (SetThreadAffinityMask (thread, m_timerMask));	// Set affinity to the first core

			QueryPerformanceCounter (reinterpret_cast <LARGE_INTEGER *> (&currentTime));	// Query the timer
			SetThreadAffinityMask (thread, oldMask);	// Reset affinity

			long long newTime (currentTime - m_startTime);
			unsigned int newTicks (static_cast <unsigned int> (1000ll * newTime / g_commonTimer.GetFrequency ()));	// get milliseconds to check against GetTickCount()
			const int msecOff (static_cast <int> (newTicks - (GetTickCount () - m_startTick)));

			// detect and compensate for performance counter leaps (surprisingly common, see Microsoft KB: Q274323)
			if (msecOff < -100l || msecOff > 100l)
			{
				// We must keep the timer running forward :)
				const long long adjust (Math::GetMinimumValueBetween (msecOff * g_commonTimer.GetFrequency () / 1000ll, newTime - m_lastTime));

				m_startTime += adjust;
				newTime -= adjust;
			}

			// Record last time for adjust
			m_lastTime = newTime;

			// scale by 1000000 for microseconds
			return static_cast <unsigned int> (1000000ll * newTime / g_commonTimer.GetFrequency ());
		}

		/** Returns milliseconds since initialisation or last reset, only CPU time measured */
		inline const double GetMillisecondsCPU (void) const
		{
			return static_cast <float> (clock () - m_zeroClock) / (CLOCKS_PER_SEC / 1000.0);
		}

		/** Returns microseconds since initialisation or last reset, only CPU time measured */
		inline const double GetMicrosecondsCPU (void) const
		{
			return static_cast <float> (clock () - m_zeroClock) / (CLOCKS_PER_SEC / 1000000.0);
		}
};

#endif	// ifndef TIMER_INCLUDED