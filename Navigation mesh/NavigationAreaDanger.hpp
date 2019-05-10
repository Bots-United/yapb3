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
// NavigationAreaDanger.hpp
//
// Class: Danger
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef NAVIGATION_AREA_DANGER_INCLUDED
#define NAVIGATION_AREA_DANGER_INCLUDED

class Danger
{
	//
	// Group: Private members.
	//
	private:
		float m_value;		///< high value is allowing bots to avoid areas where they died in the past - zero is no danger
		float m_timestamp;	///< time when danger value was set - used for decaying

	//
	// Group: (Con/De)structors.
	//
	public:
		inline Danger (void) : m_value (0.0f), m_timestamp (0.0f) { /*VOID*/ }

	//
	// Group: Operators.
	//
	public:
		inline operator const float (void)	///< Return the danger of this area (decays over time)
		{
			Decay ();

			return m_value;
		}

	//
	// Group: Private functions.
	//
	private:
		/**
		* Decay the danger value.
		*/
		inline void Decay (const float time = 2.0f * 60.0f)
		{
			const float decayRate (1.0f / time);	// one kill == 1.0, which we will forget about in two minutes
			const float deltaTime (HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () - m_timestamp);
			const float decayAmount (decayRate * deltaTime);

			m_value -= decayAmount;

			if (m_value < 0.0f)
				m_value = 0.0f;

			// update timestamp
			m_timestamp = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ();
		}

	//
	// Group: Functions.
	//
	public:
		inline void Increase (const float amount)	///< Increase the danger of this area for the given team
		{
			// before we add the new value, decay what's there
			Decay ();

			m_value += amount;
		}
};

#endif	// ifndef NAVIGATION_AREA_DANGER_INCLUDED