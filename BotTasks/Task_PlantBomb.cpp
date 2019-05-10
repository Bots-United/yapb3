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
// Task_PlantBomb.cpp
//
// Class: Task_PlantBomb
//
// Description: Planting the bomb right now.
//
// Version: $ID:$
//

#include <Core.hpp>

void YaPB::Task_PlantBomb::Execute (void)
{
//	AddLogEntry (true, LogLevel_Default, false, "Task_PlantBomb::Execute() called for bot %s.", m_owner->GetName ().GetData ());

	m_owner->m_aim->Think ();
	m_owner->m_chatterManager.Think (m_owner);

	// fight the enemy if we found one
	if (m_owner->FindEnemy () || m_owner->m_enemy->IsValid ())
	{
		// Reliability check.
		InternalAssert (m_owner->m_enemy->IsValid ());

		AddLogEntry (true, LogLevel_Default, false, "\"%s\"::Task_PlantBomb::Execute(): Find enemy \"%s\" - taking pause....", m_owner->GetName ().GetData (), m_owner->m_enemy->pointer->GetName ().GetData ());

		m_owner->m_scheduleManager.AddFront (new Task_AttackEnemy (m_owner));

		return;
	}
/*
	m_owner->m_aimFlags |= AimPos_CampPoint;

	const Math::Vector3D &destination (m_owner->m_lastEnemyOrigin);

	// get a valid look direction
	GetCampDirection (&destination);
*/
	// we're still got the C4?
	if (m_owner->HasWeapon (HalfLifeEngine::SDK::Constants::WeaponID_C4))
	{
		if (m_owner->m_enemy->IsValid () || !m_owner->IsInBombPlace ())
		{
			Complete ();

			return;
		}

//		m_owner->m_moveToGoal = false;
//		m_owner->m_checkTerrain = false;

		m_owner->m_navigator->m_speed.move = m_owner->m_navigator->m_speed.strafe = 0.0f;

//		m_owner->m_navigationTimeSet = HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ();

		if (m_owner->m_currentWeapon->GetID () != HalfLifeEngine::SDK::Constants::WeaponID_C4)
			m_owner->SelectWeapon (HalfLifeEngine::SDK::Constants::WeaponID_C4);
		else
		{
/*		if (m_owner->m_currentWaypointFlags & WaypointFlag_Crouch)
			m_owner->m_edict->variables.buttons |= (HalfLifeEngine::SDK::Constants::IN_DUCK | HalfLifeEngine::SDK::Constants::IN_ATTACK);
		else*/
			m_owner->PressPrimaryAttack ();
		}

		return;
	}

	// done with planting
	Complete ();
/*
	if (m_owner->GetNearbyTeammatesNearPosition (GetOrigin (), 1200.0f) > 0)
		m_owner->RadioMessage (HalfLifeEngine::SDK::Constants::RadioCommand_NeedBackup);	// tell teammates to move over here...

	// delete all path finding nodes
	m_owner->DeleteSearchNodes ();

	if (m_owner->CanCamp ())
	{
		const short index (m_owner->FindDefendWaypoint (GetOrigin ()));

		StartTask (Task_Camp, TaskPriority_Camp, -1, HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () + ((Console::Server::variableManager.GetVariable (Console::Server::VariableID_C4Timer)->GetValue <float> () * 0.5f) + (Console::Server::variableManager.GetVariable (Console::Server::VariableID_C4Timer)->GetValue <float> () / 4.0f)), true);	// push camp task on to stack
		StartTask (Task_MoveToPosition, TaskPriority_MoveToPosition, index, HalfLifeEngine::Globals::g_halfLifeEngine->GetTime () + ((Console::Server::variableManager.GetVariable (Console::Server::VariableID_C4Timer)->GetValue <float> () * 0.5f) + (Console::Server::variableManager.GetVariable (Console::Server::VariableID_C4Timer)->GetValue <float> () / 4.0f)), true);	// push move command

		if (g_waypoint->GetPath (index)->visibility.crouch <= g_waypoint->GetPath (index)->visibility.stand)
			m_owner->m_campButtons |= HalfLifeEngine::SDK::Constants::IN_DUCK;
		else
			m_owner->m_campButtons = 0;
	}*/
}