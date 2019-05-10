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
// Task_DoubleJump.hpp
//
// Class: Task_DoubleJump
//
// Description: Bot helps human player (or other bot) to get somewhere.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef TASK_DOUBLE_JUMP_INCLUDED
#define TASK_DOUBLE_JUMP_INCLUDED

class Task_DoubleJump : public Task
{
	//
	// Group: Private members.
	//
	private:
		const Client *const m_client;				// Pointer to entity that request double jump.
		Math::Vector3D      m_origin;				// origin of double jump
		Waypoint           *m_travelStartWaypoint;	// travel start waypoint to double jump action
		bool                m_isReady;				// is double jump ready
		float               m_duckTime;				// is bot needed to duck for double jump
		float               m_timeToComplete;

	//
	// Group: (Con/De)structors.
	//
	public:
		inline Task_DoubleJump (YaPB *const bot, const Client *const client, const Math::Vector3D &origin) :
			Task (bot),

			m_client (client),
			m_origin (origin),
			m_travelStartWaypoint (NULL),
			m_isReady (false),
			m_duckTime (0.0f),
			m_timeToComplete (HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () + g_randomNumberGenerator.GetValueBetween (5.0f, 7.5f))
		{
			// going here

//			m_owner->m_aim->m_lookAt = m_client->GetOrigin () + Math::Vector3D (g_randomNumberGenerator.GetValueBetween (m_client->GetEdict ()->variables.boundingBox.mins.x, m_client->GetEdict ()->variables.boundingBox.maxs.x), g_randomNumberGenerator.GetValueBetween (m_client->GetEdict ()->variables.boundingBox.mins.y, m_client->GetEdict ()->variables.boundingBox.maxs.y), g_randomNumberGenerator.GetValueBetween (m_client->GetEdict ()->variables.boundingBox.mins.z, m_client->GetEdict ()->variables.boundingBox.maxs.z));
		}

	//
	// Group: Functions.
	//
	public:
		inline const ID_t GetID (void) const { return ID_PerformDoubleJump; }

		inline const Client *const GetClient (void) const { return m_client; }

	//
	// Group: Engine callbacks.
	//
	public:
		void Execute (void);

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

#endif	// ifndef TASK_DOUBLE_JUMP_INCLUDED