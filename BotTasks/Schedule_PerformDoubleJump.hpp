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
// Schedule_PerformDoubleJump.hpp
//
// Class: Schedule_PerformDoubleJump
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef SCHEDULE_PERFORM_DOUBLE_JUMP_INCLUDED
#define SCHEDULE_PERFORM_DOUBLE_JUMP_INCLUDED

class Schedule_PerformDoubleJump : public Schedule
{
	//
	// Group: Private members.
	//
	private:
		const Client *const m_client;	// Pointer to entity that request double jump.

	//
	// Group: (Con/De)structors.
	//
	public:
		inline Schedule_PerformDoubleJump (YaPB *const bot, const Client *const client, const Math::Vector3D &origin) :
			Schedule (bot),

			m_client (client)
		{
			AddTask (new Task_MoveToPosition (bot, origin));	// first
			AddTask (new Task_DoubleJump (bot, client, origin));	// second
		}

	//
	// Group: Functions.
	//
	public:
		inline const ID_t GetID (void) const { return ID_PerformDoubleJump; }

	//
	// Group: Engine callbacks.
	//
	public:
		inline void OnClientKilled (Client *const victim, const HalfLifeEngine::SDK::Classes::Edict *const/* killer*/)
		{
			if (m_client == victim)
				CompleteAndDelete ();	// If so - delete this task....
		}
		inline void OnClientDisconnect (Client *const client)
		{
			// Is disconnecting client is double jump help requester?
			if (m_client == client)
				CompleteAndDelete ();	// If so - delete this task....
		}
};

#endif	// ifndef SCHEDULE_PERFORM_DOUBLE_JUMP_INCLUDED