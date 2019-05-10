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
// TaskIdle.hpp
//
// Class: Task_Idle
//
// Description: Bot is pausing.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef TASK_IDLE_INCLUDED
#define TASK_IDLE_INCLUDED

class Task_Idle : public Task
{
	//
	// Group: Private members.
	//
	private:
		float m_duration;

	//
	// Group: (Con/De)structors.
	//
	public:
		inline Task_Idle (YaPB *const bot, const float duration) :
			Task (bot),

			m_duration (duration)
		{ /*VOID*/ }	// going here

	//
	// Group: Functions.
	//
	public:
		//
		// Function: GetID
		//
		// Description: Gets a action ID.
		//
		// Returns: Action ID.
		//
		inline const ID_t GetID                  (void) const { return ID_Idle; }

	//
	// Group: Engine callbacks.
	//
	public:
		inline void       Execute                (void)
		{
			// stop camping if time over or gets hurt by something else than bullets
			if (m_duration <= HalfLifeEngine::Globals::g_halfLifeEngine->GetTime ()/* || m_owner->m_lastDamageType > HalfLifeEngine::SDK::Constants::DamageType_Generic*/)
				Complete ();

			/* Do nothing.... */
		}

		inline void       OnOwnerSpawn           (void) { /* Do nothing.... */ }
		inline void       OnTouched              (const HalfLifeEngine::SDK::Classes::Edict *const/* entity*/) { /* Do nothing.... */ }
		inline void       OnReactOnRadio         (const RadioCommandsManager::RadioCommand *const/* radioMessage*/) { /* Do nothing.... */ }
		inline void       OnTakeDamage           (const HalfLifeEngine::SDK::Classes::Edict *const/* inflictor*/, const unsigned short/* damage*/, const unsigned char/* armor*/, const HalfLifeEngine::SDK::Constants::DamageType_t/* bits*/) { /* Do nothing.... */ }
		inline void       OnTakeBlinded          (const float/* duration*/, const Client *const/* flasher*/) { /* Do nothing.... */ }
		inline void       OnKilled               (const HalfLifeEngine::SDK::Classes::Edict *const/* killer*/) { /* Do nothing.... */ }
		inline void       OnKill                 (const Client *const/* victim*/) { /* Do nothing.... */ }
		inline void       OnClientKilled         (Client *const/* victim*/, const HalfLifeEngine::SDK::Classes::Edict *const/* killer*/) { /* Do nothing.... */ }
		inline void       OnClientDisconnect     (Client *const/* client*/) { /* Do nothing.... */ }
		inline void       OnEnteredBombPlace     (void) { /* Do nothing.... */ }
		inline void       OnLeaveBombPlace       (void) { /* Do nothing.... */ }
		inline void       OnEnteredRescueZone    (void) { /* Do nothing.... */ }
		inline void       OnLeaveEscapeZone      (void) { /* Do nothing.... */ }
		inline void       OnEnteredEscapeZone    (void) { /* Do nothing.... */ }
		inline void       OnLeaveRescueZone      (void) { /* Do nothing.... */ }
		inline void       OnEnteredVIPSafetyZone (void) { /* Do nothing.... */ }
		inline void       OnLeaveVIPSafetyZone   (void) { /* Do nothing.... */ }
		inline void       OnEnteredBuyZone       (void) { /* Do nothing.... */ }
		inline void       OnLeaveBuyZone         (void) { /* Do nothing.... */ }
		inline void       OnAmmoAmountChanged    (const HalfLifeEngine::SDK::Constants::WeaponID_t/* ammoID*/, const unsigned char/* newAmount*/) { /* Do nothing.... */ }
		inline void       OnRoundStarted         (void) { /* Do nothing.... */ }
		inline void       OnRoundEnded           (void) { /* Do nothing.... */ }
		inline void       OnBombPlanted          (void) { /* Do nothing.... */ }
		inline void       OnBombDetonated        (void) { /* Do nothing.... */ }
		inline void       OnBombDefused          (void) { /* Do nothing.... */ }
		inline void       OnItemDeployPost       (const HalfLifeEngine::SDK::Classes::Edict *const/* weapon*/) { /* Do nothing.... */ }
		inline void       OnWeaponReloadPost     (const HalfLifeEngine::SDK::Classes::Edict *const/* weapon*/) { /* Do nothing.... */ }
		inline void       OnAddWeapon            (const HalfLifeEngine::SDK::Classes::Edict *const/* weapon*/) { /* Do nothing.... */ }
		inline void       OnRemoveWeapon         (const HalfLifeEngine::SDK::Classes::Edict *const/* weapon*/) { /* Do nothing.... */ }
		inline void       OnUse                  (const HalfLifeEngine::SDK::Classes::Edict *const/* entity*/, const HalfLifeEngine::SDK::Classes::Edict *const/* caller*/, const HalfLifeEngine::SDK::Constants::UseType_t/* type*/, const float/* value*/) { /* Do nothing.... */ }
		inline void       OnHostageKilled        (const HalfLifeEngine::SDK::Classes::Edict *const/* hostage*/, const Client *const/* killer*/) { /* Do nothing.... */ }
		inline void       OnHostageRescued       (const HalfLifeEngine::SDK::Classes::Edict *const/* hostage*/) { /* Do nothing.... */ }
		inline void       OnHostageUnusePost     (const HalfLifeEngine::SDK::Classes::Edict *const/* hostage*/, const Client *const/* currentRescuer*/) { /* Do nothing.... */ }
		inline void       OnHostageUsePost       (const HalfLifeEngine::SDK::Classes::Edict *const/* hostage*/, const Client *const/* previousRescuer*/) { /* Do nothing.... */ }
		inline void       OnAllHostagesRescued   (void) { /* Do nothing.... */ }
		inline void       OnImpulseCommand       (const HalfLifeEngine::SDK::Constants::PlayerImpulseID_t/* impulseCommand*/) { /* Do nothing.... */ }
		inline void       OnHearNoisedSound      (const Client *const/* from*/, const NoisedSound_t &/*noisedSound*/) { /* Do nothing.... */ }
};

#endif	// ifndef TASK_IDLE_INCLUDED