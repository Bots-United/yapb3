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
// CountdownTimer.hpp
//
// Class: CountdownTimer
//
// Description: Simple class for counting down a short interval of time.
//				Upon creation, the timer is invalidated. Invalidated countdown timers are considered to have elapsed.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef CORE_TOOL_LIBRARY_COUNTDOWN_TIMER_INCLUDED
#define CORE_TOOL_LIBRARY_COUNTDOWN_TIMER_INCLUDED

template <const float (*const GetTimeFunction) (void)> class CountdownTimer
{
	//
	// Group: Private members.
	//
	private:
		/// @warning DO NOT CHANGE VARIABLES AND DO NOT ADD VIRTUAL FUNCTIONS!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		float m_duration;
		float m_timeStamp;

	//
	// Group: (Con/De)structors.
	//
	public:
		inline CountdownTimer (void) : m_timeStamp (-1.0f), m_duration (0.0f) { /* VOID */ }
		inline CountdownTimer (const float duration) { Start (duration) }

	//
	// Group: Functions.
	//
	public:
		inline void Reset (void) { m_timeStamp = GetTimeFunction () + m_duration; }

		inline void Start (const float duration)
		{
			m_duration = duration;

			Reset ();
		}

		inline void Invalidate (void) { m_timeStamp = -1.0f; }

		inline const bool HasStarted (void) const { return m_timeStamp > 0.0f; }

		inline const bool IsElapsed (void) const { return m_timeStamp < GetTimeFunction (); }

		inline const float GetElapsedTime (void) const { return GetTimeFunction () - m_timeStamp + m_duration; }

		inline const float GetTimeStamp (void) const { return m_timeStamp; }

		inline const float GetRemainingTime (void) const { return m_timeStamp - GetTimeFunction (); }

		/// return original countdown time
		inline const float GetCountdownDuration (void) const { return HasStarted () ? m_duration : 0.0f; }
};

#endif	// ifndef CORE_TOOL_LIBRARY_COUNTDOWN_TIMER_INCLUDED