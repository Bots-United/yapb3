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
// IntervalTimer.hpp
//
// Class: IntervalTimer
//
// Description: Simple class for tracking intervals of game time.
//				Upon creation, the timer is invalidated. To measure time intervals, start the timer via Start().
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef CORE_TOOL_LIBRARY_INTERVAL_TIMER_INCLUDED
#define CORE_TOOL_LIBRARY_INTERVAL_TIMER_INCLUDED

template <const float (*const GetTimeFunction) (void)> class IntervalTimer
{
	//
	// Group: Private members.
	//
	private:
		float m_timeStamp;

	//
	// Group: (Con/De)structors.
	//
	public:
		inline IntervalTimer (void) : m_timeStamp (-1.0f) { /* VOID */ }

	//
	// Group: Functions.
	//
	public:
		inline void Reset (void) { m_timeStamp = GetTimeFunction (); }

		inline void Start (void) { m_timeStamp = GetTimeFunction (); }

		inline void Invalidate (void) { m_timeStamp = -1.0f; }

		inline const bool HasStarted (void) const { return m_timeStamp > 0.0f; }

		/// if not started, elapsed time is very large
		inline const float GetElapsedTime (void) const { return HasStarted () ? GetTimeFunction () - m_timeStamp : Math::MaximumFloatValue; }

		inline const bool IsLessThen (const float duration) const { return GetTimeFunction () - m_timeStamp < duration; }

		inline const bool IsGreaterThen (const float duration) const { return GetTimeFunction () - m_timeStamp > duration; }
};

#endif	// ifndef CORE_TOOL_LIBRARY_INTERVAL_TIMER_INCLUDED