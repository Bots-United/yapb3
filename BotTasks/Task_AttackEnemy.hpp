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
// Task_AttackEnemy.hpp
//
// Class: Task_AttackEnemy
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef TASK_ATTACK_ENEMY_INCLUDED
#define TASK_ATTACK_ENEMY_INCLUDED

class Task_AttackEnemy : public Task
{
	//
	// Group: Private members.
	//
	private:
		

	//
	// Group: (Con/De)structors.
	//
	public:
		inline Task_AttackEnemy (YaPB *const bot) : Task (bot) { /*VOID*/ }	// going here

		virtual inline ~Task_AttackEnemy (void)
		{
			// null out the enemy pointer as it's no longer valid
			m_owner->DeleteEnemy ();	// So remove him from my enemies list....
		}

	//
	// Group: Private functions.
	//
	private:
		void FireWeapon (void);

	//
	// Group: Functions.
	//
	public:
		inline const ID_t GetID (void) const { return ID_AttackEnemy; }

		inline void       Complete   (void)
		{
			// Call this function in base class....
			Task::Complete ();

			m_owner->m_aim->SetLookTask (new Aim::Task ());
		}
		inline void       Run   (void)
		{
			if (!m_owner->m_enemy->IsValid ())
			{
				Complete ();

				return;
			}

			// Call this function in base class....
			Task::Run ();

			m_owner->m_aim->SetLookTask (new Aim::Task_Enemy ());
		}

	//
	// Group: Engine callbacks.
	//
	public:
		void Execute (void);

		inline void OnKilled (const HalfLifeEngine::SDK::Classes::Edict *const/* killer*/)
		{
//			Complete ();
			CompleteAndDelete ();
		}
		inline void OnKill (const Client *const victim)
		{
			InternalAssert (m_owner->m_enemy->IsValid ());

			if (m_owner->m_enemy->pointer != victim)
				return;

			HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "\"%s\"::Task_AttackEnemy::OnKill(): Victim: \"%s\".\n", m_owner->GetName ().GetData (), m_owner->m_enemy->pointer->GetName ().GetData ());

			AddLogEntry (true, LogLevel_Default, false, "\"%s\"::Task_AttackEnemy::OnKill(): Task completed!, m_enemy: \"%s\".", m_owner->GetName ().GetData (), m_owner->m_enemy->pointer->GetName ().GetData ());

//			Complete ();
			CompleteAndDelete ();
		}
		inline void OnClientKilled (Client *const victim, const HalfLifeEngine::SDK::Classes::Edict *const/* killer*/)
		{
			InternalAssert (m_owner->m_enemy->IsValid ());

			// Is dead man my enemy?
			if (m_owner->m_enemy->pointer != victim)
				return;

			HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "\"%s\"::Task_AttackEnemy::OnClientKilled(): Victim: \"%s\".\n", m_owner->GetName ().GetData (), m_owner->m_enemy->pointer->GetName ().GetData ());

			AddLogEntry (true, LogLevel_Default, false, "\"%s\"::Task_AttackEnemy::OnClientKilled(): Task completed!, m_enemy: \"%s\".", m_owner->GetName ().GetData (), m_owner->m_enemy->pointer->GetName ().GetData ());

//			Complete ();
			CompleteAndDelete ();
		}

		inline void OnClientDisconnect (Client *const client)
		{
			// This function called when somebody disconnects from server.

			InternalAssert (m_owner->m_enemy->IsValid ());

			if (m_owner->m_enemy->pointer != client)
				return;

			HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "\"%s\"::Task_AttackEnemy::OnClientDisconnect(): Client: \"%s\".\n", m_owner->GetName ().GetData (), m_owner->m_enemy->pointer->GetName ().GetData ());

			AddLogEntry (true, LogLevel_Default, false, "\"%s\"::Task_AttackEnemy::Execute(): Task completed!, m_enemy: \"%s\".", m_owner->GetName ().GetData (), m_owner->m_enemy->pointer->GetName ().GetData ());

//			Complete ();
			CompleteAndDelete ();
		}

		inline void OnTakeBlinded (const float/* duration*/, const Client *const/* flasher*/)
		{
			InternalAssert (m_owner->m_enemy->IsValid ());

			HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat ("\"%s\"::Task_AttackEnemy::OnTakeBlinded(): Called, m_enemy: \"%s\".\n", m_owner->GetName ().GetData (), m_owner->m_enemy->pointer->GetName ().GetData ());

//			Complete ();
			CompleteAndDelete ();
		}

		inline void OnRoundStarted (void)
		{
			InternalAssert (m_owner->m_enemy->IsValid ());

			HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "\"%s\"::Task_AttackEnemy::OnRoundStarted(): Called, m_enemy: \"%s\".\n", m_owner->GetName ().GetData (), m_owner->m_enemy->pointer->GetName ().GetData ());

//			Complete ();
			CompleteAndDelete ();
		}
};

#endif	// ifndef TASK_ATTACK_ENEMY_INCLUDED