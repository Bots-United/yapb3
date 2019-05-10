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
// Task_RescueHostages.hpp
//
// Class: Task_RescueHostages
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef BOT_TASKS_TASK_RESCUE_HOSTAGES_INCLUDED
#define BOT_TASKS_TASK_RESCUE_HOSTAGES_INCLUDED

class Task_RescueHostages : public Task
{
	//
	// Group: Private members.
	//
	private:
		HalfLifeEngine::SDK::Classes::Edict *m_currentHostageToRescue;
		const AreaMapZone_t                 *m_hostagesZone;

	//
	// Group: (Con/De)structors.
	//
	public:
		inline Task_RescueHostages (YaPB *const bot, const AreaMapZone_t &hostagesZone) :
			Task (bot),

			m_hostagesZone (&hostagesZone)
		{ /*VOID*/ }	// going here

	//
	// Group: Functions.
	//
	public:
		inline const ID_t GetID (void) const { return ID_RescueHostages; }

		inline void       Run   (void)
		{
			// Call original function in base class....
			Task::Run ();

			m_owner->m_areaGameState.visitedHostageZones += m_hostagesZone;

			// play info about that
			if (g_randomNumberGenerator.GetValueBetween <unsigned char> (1u, 100u) < 95u)
				switch (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Communication)->GetValue <Console::Bot::VariableValue_Communication_t> ())
				{
					case Console::Bot::VariableValue_Communication_Radio:
						m_owner->RadioMessage (m_owner->GetNearbyTeammatesNearPosition (m_owner->GetOrigin (), 400.0f) > 0u ? HalfLifeEngine::SDK::Constants::RadioCommand_CoverMe : HalfLifeEngine::SDK::Constants::RadioCommand_NeedBackup);

						break;

					case Console::Bot::VariableValue_Communication_Voice:
						m_owner->ChatterMessage (::ChatterManager::ChatterMessage_RescuingHostages);

						break;
				}
		}

	//
	// Group: Engine callbacks.
	//
	public:
		void Execute (void);

		inline void OnHostageUsePost (const HalfLifeEngine::SDK::Classes::Edict *const hostage, const Client *const/* previousRescuer*/)
		{
			if (hostage == m_currentHostageToRescue)
				m_currentHostageToRescue = NULL;
		}
};

class Schedule_RescueHostages : public Schedule
{
	//
	// Group: (Con/De)structors.
	//
	public:
		inline Schedule_RescueHostages (YaPB *const bot) : Schedule (bot) { /* VOID */ }

	//
	// Group: Functions.
	//
	public:
		inline const ID_t GetID (void) const { return ID_RescueHostages; }

		inline void       Run   (void)
		{
			/// @warning ADDING TASKS IN Run() FUNCTION CAUSES TO InternalAssert(currnetActrion->(SHEDULE HAS NO TASKS!!!))

			InternalAssert (!s_hostageAreaMapZones.IsEmpty ());
			InternalAssert (!s_hostageRescueAreaMapZones.IsEmpty ());

			const AreaMapZone_t &hostagesZone (s_hostageAreaMapZones.GetRandomElement ());

			AddTask (new Task_MoveToPosition (m_owner, GetRandomPositionInAreaZone (hostagesZone)));	// first
			AddTask (new Task_RescueHostages (m_owner, hostagesZone));	// second
			AddTask (new Task_MoveToRescueZoneWithHostages (m_owner));	// third

			// Call this function in base class....
			Schedule::Run ();
		}
};

#endif	// ifndef BOT_TASKS_TASK_RESCUE_HOSTAGES_INCLUDED