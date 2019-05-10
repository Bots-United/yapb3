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
// Task_DefuseBomb.hpp
//
// Class: Task_DefuseBomb
//
// Description: Bomb defusing behaviour.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef TASK_DEFUSE_BOMB_INCLUDED
#define TASK_DEFUSE_BOMB_INCLUDED

class Task_DefuseBomb : public Task
{
	//
	// Group: Private members.
	//
	private:
		const float m_timeToBlowUp;
		float       m_defuseEndTime;
		float       m_duckDefuseCheckTime;
		bool        m_isDuckDefuse;

	//
	// Group: (Con/De)structors.
	//
	public:
		inline Task_DefuseBomb (YaPB *const bot) :
			Task (bot),

			m_timeToBlowUp (g_server->GetC4Manager ().GetC4 ()->GetExplosionTime ()),
			m_defuseEndTime (0.0f),
			m_duckDefuseCheckTime (0.0f),
			m_isDuckDefuse (false)
		{ /*VOID*/ }	// going here

	//
	// Group: Functions.
	//
	public:
		inline const ID_t GetID (void) const { return ID_DefuseBomb; }

	//
	// Group: Engine callbacks.
	//
	public:
		void Execute (void);
};

class Schedule_DefuseBomb : public Schedule
{
	//
	// Group: (Con/De)structors.
	//
	public:
		inline Schedule_DefuseBomb (YaPB *const bot) :
			Schedule (bot)
		{
			AddTask (new Task_MoveToPosition (bot, g_server->GetC4Manager ().GetC4 ()->GetOrigin ()));	// first
			AddTask (new Task_DefuseBomb (bot));	// second
		}

	//
	// Group: Functions.
	//
	public:
		inline const ID_t GetID (void) const { return ID_DefuseBomb; }

	//
	// Group: Engine callbacks.
	//
	public:
		inline void OnRoundEnded (void) { CompleteAndDelete (); }
};

#endif	// ifndef TASK_DEFUSE_BOMB_INCLUDED