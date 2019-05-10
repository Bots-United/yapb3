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
// Task_SelectTeam.hpp
//
// Class: Task_SelectTeam
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef TASK_SELECT_TEAM_INCLUDED
#define TASK_SELECT_TEAM_INCLUDED

class Task_SelectTeam : public Task
{
	//
	// Group: Private members.
	//
	private:
		const float m_delayTime;	// Time bot "choosing" his team.... (thinking about it)

	//
	// Group: (Con/De)structors.
	//
	public:
		inline Task_SelectTeam (YaPB *const bot, const float delay = 0.5f) :
			Task (bot),

			m_delayTime (HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () + bot->m_profile->baseReactTime * g_randomNumberGenerator.GetValueBetween (0.2f, 1.0f) + delay)
		{ /* VOID */ }	// going here

	//
	// Group: Functions.
	//
	public:
		inline const ID_t GetID (void) const { return ID_SelectTeam; }

	//
	// Group: Engine callbacks.
	//
	public:
		void Execute (void);
};

#endif	// ifndef TASK_SELECT_TEAM_INCLUDED