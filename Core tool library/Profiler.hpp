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
// Profiler.hpp
//
// Class: Profiler
//
// Version: $ID:$
//

#if defined _MSC_VER && _MSC_VER > 1000
#	pragma once
#endif	// if defined _MSC_VER && _MSC_VER > 1000

#ifndef PROFILER_INCLUDED
#define PROFILER_INCLUDED

class Profiler	// NOT WORKED AND INCLUDED YET!!!!
{
	//
	// Group: Private members.
	//
	private:
		DynamicString    m_functionName;

		unsigned __int64 m_startCycle;
		unsigned __int64 m_endCycle;

		unsigned __int64 m_deltaTime;

		unsigned __int64 m_min;
		unsigned __int64 m_max;

		unsigned __int64 m_average;

	//
	// Group: (Con/De)structors.
	//
	public:
		inline Profiler (const DynamicString &functionName) :
			m_functionName (functionName),

			m_startCycle (0u),
			m_endCycle (0u),

			m_deltaTime (0u),

			m_min (9999999999u),
			m_max (0u),

			m_average (2u)
		{ /*VOID*/ }

	//
	// Group: Functions.
	//
	public:
		inline void Start (void)
		{
			// "Begin" Timer i.e. update time

			QueryPerformanceCounter (reinterpret_cast <LARGE_INTEGER *> (&m_startCycle));
		}

		inline void Stop (void)
		{
			// Stop Timer, work out min/max values and set invoked

			QueryPerformanceCounter (reinterpret_cast <LARGE_INTEGER *> (&m_endCycle));

			m_deltaTime = m_endCycle - m_startCycle;
		}

		inline const unsigned __int64 GetDeltaTime (void) const { return m_deltaTime; }
};

#endif	// ifndef PROFILER_INCLUDED