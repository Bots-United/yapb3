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
// Task_FollowUser.hpp
//
// Class: Task_FollowUser
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef BOT_TASKS_TASK_FOLLOW_USER_INCLUDED
#define BOT_TASKS_TASK_FOLLOW_USER_INCLUDED

class Task_FollowUser : public Task
{
	//
	// Group: Private members.
	//
	private:
		const Client *const m_client;
		const float         m_completeTime;

	//
	// Group: (Con/De)structors.
	//
	public:
		inline Task_FollowUser (YaPB *const bot, const Client *const client, const float completeTime) :
			Task (bot),

			m_client (client),
			m_completeTime (HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () + completeTime)
		{ /*VOID*/ }	// going here

	//
	// Group: Functions.
	//
	public:
		inline const ID_t GetID (void) const { return ID_FollowUser; }

		inline const Client *const GetClient (void) const { return m_client; }

	//
	// Group: Engine callbacks.
	//
	public:
		inline void Execute (void)
		{
			if (m_completeTime <= HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ())
			{
				Complete ();

				return;
			}

			m_owner->m_aim->Think ();
			m_owner->m_chatterManager.Think (m_owner);

			m_owner->m_navigator->SetDestinationOrigin (m_client->GetOrigin ());

			if (m_owner->m_reloadState == ReloadState_None && m_owner->m_currentWeapon->GetAmmo () > 0u)
				m_owner->m_reloadState = ReloadState_Primary;

			m_owner->TryCheckReload ();

			if (m_client->GetOrigin ().GetDistanceSquared2D (m_owner->GetOrigin ()) <= MATH_GET_SQUARED (100.0f))
				return;

			m_owner->m_edict->variables.buttons = HalfLifeEngine::SDK::Constants::IN_FORWARD;

			m_owner->m_navigator->Think ();
		}

		inline void OnClientKilled (Client *const victim, const HalfLifeEngine::SDK::Classes::Edict *const/* killer*/)
		{
			if (m_client == victim)
				CompleteAndDelete ();	// If so - delete this task....
		}
		inline void OnClientDisconnect (Client *const client)
		{
			// Is disconnecting client is user client?
			if (m_client == client)
				CompleteAndDelete ();	// If so - delete this task....
		}
};

#endif	// ifndef BOT_TASKS_TASK_FOLLOW_USER_INCLUDED