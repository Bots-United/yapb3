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
// Task_PlantBomb.hpp
//
// Class: Task_PlantBomb
//
// Description: Planting the bomb right now.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef TASK_PLANT_BOMB_INCLUDED
#define TASK_PLANT_BOMB_INCLUDED

class Task_PlantBomb : public Task
{
	//
	// Group: Private members.
	//
	private:
		

	//
	// Group: (Con/De)structors.
	//
	public:
		inline Task_PlantBomb (YaPB *const bot) : Task (bot) { /*VOID*/ }	// going here

	//
	// Group: Functions.
	//
	public:
		inline const ID_t GetID (void) const { return ID_PlantBomb; }

		inline void       Run   (void)
		{
			// Call original function in base class....
			Task::Run ();

			// play info about that
			if (g_randomNumberGenerator.GetValueBetween <unsigned char> (1u, 100u) < 95u)
				switch (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Communication)->GetValue <Console::Bot::VariableValue_Communication_t> ())
				{
					case Console::Bot::VariableValue_Communication_Radio:
						m_owner->RadioMessage (m_owner->GetNearbyTeammatesNearPosition (m_owner->GetOrigin (), 400.0f) > 0u ? HalfLifeEngine::SDK::Constants::RadioCommand_CoverMe : HalfLifeEngine::SDK::Constants::RadioCommand_NeedBackup);

						break;

					case Console::Bot::VariableValue_Communication_Voice:
						m_owner->ChatterMessage (::ChatterManager::ChatterMessage_PlantingC4);

						break;
				}
		}

	//
	// Group: Engine callbacks.
	//
	public:
		void Execute (void);
};

class Schedule_PlantBomb : public Schedule
{
	//
	// Group: (Con/De)structors.
	//
	public:
		inline Schedule_PlantBomb (YaPB *const bot) : Schedule (bot) { /* VOID */ }

	//
	// Group: Functions.
	//
	public:
		inline const ID_t GetID (void) const { return ID_PlantBomb; }

		inline void       Run   (void)
		{
			InternalAssert (m_owner->HasWeapon (HalfLifeEngine::SDK::Constants::WeaponID_C4));
			InternalAssert (!s_bombPlaceAreaMapZones.IsEmpty ());

			AddTask (new Task_MoveToPosition (m_owner, GetRandomPositionInAreaZone (s_bombPlaceAreaMapZones.GetRandomElement ())));	// first
			AddTask (new Task_PlantBomb (m_owner));	// second
			AddTask (new Task_Camp (m_owner, Console::Server::variableManager.GetVariable (Console::Server::VariableID_C4Timer)->GetValue <float> () * 0.6f));	// third

			// Call this function in base class....
			Schedule::Run ();
		}

		inline void OnEnteredBuyZone (void)
		{
			// Don't buy if we're the VIP or not in buy zone or bot variable deathmatch is on, OR bot variable buy is off, buying is off then no need to buy....
			if (m_owner->IsCanBuy ())
				m_owner->m_scheduleManager.AddFront (new Task_AutoBuy (m_owner));	// buy weapons first
		}
};

#endif	// ifndef TASK_PLANT_BOMB_INCLUDED