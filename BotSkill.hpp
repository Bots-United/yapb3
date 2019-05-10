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
// BotSkill.hpp
//
// Class: Skill
//
// Description: Simple skill class which is used in a YaPB profile.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef BOT_SKILL_INCLUDED
#define BOT_SKILL_INCLUDED

class Skill
{
	//
	// Group: Private members.
	//
	private:
		unsigned char m_value;				// skill of this bot (MinimumBotSkillValue = unskilled - MaximumBotSkillValue = skilled)
		float         m_reducingOffset;		// reducing offset to bots skill (this is automaticly calculated in the constructor & ProfileManager::LoadBotProfiles() function during assigning a 'skill' value)
		float         m_increasingOffset;	// increasing offset to bots skill (this is automaticly calculated in the constructor & ProfileManager::LoadBotProfiles() function during assigning a 'skill' value)

	//
	// Group: (Con/De)structors.
	//
	public:
		inline Skill (const unsigned char value) :
			m_value (value),
			m_reducingOffset ((BotSkillValue_Maximum - value) / static_cast <float> (BotSkillValue_Maximum - BotSkillValue_Minimum)),
			m_increasingOffset (value / static_cast <float> (BotSkillValue_Maximum))
		{ /*VOID*/ }

		inline Skill (const Skill &right) :
			m_value (right.m_value),
			m_reducingOffset (right.m_reducingOffset),
			m_increasingOffset (right.m_increasingOffset)
		{ /*VOID*/ }

	//
	// Group: Functions.
	//
	public:
		inline const unsigned char GetValue            (void) const { return m_value; }
		inline const float         GetReducingOffset   (void) const { return m_reducingOffset; }
		inline const float         GetIncreasingOffset (void) const { return m_increasingOffset; }

	//
	// Group: Operators.
	//
	public:
		//
		// Function: operator =
		//
		// Description: Assigns the value to skill.
		//
		// Parameters:
		//	value - Value that should be assigned.
		//
		// Returns: Reassigned skill.
		//
		inline Skill &operator = (const unsigned char value)
		{
			m_value            = value;
			m_reducingOffset   = (BotSkillValue_Maximum - value) / static_cast <float> (BotSkillValue_Maximum - BotSkillValue_Minimum);
			m_increasingOffset = value / static_cast <float> (BotSkillValue_Maximum);

			return *this;
		}
		inline Skill &operator = (const Skill &right)
		{
			m_value            = right.m_value;
			m_reducingOffset   = right.m_reducingOffset;
			m_increasingOffset = right.m_increasingOffset;

			return *this;
		}

		// casting operators
		inline operator unsigned char (void) const { return m_value; }	// 'Skill' will now automatically convert to 'unsigned char' when needed.
};

#endif	// ifndef BOT_SKILL_INCLUDED