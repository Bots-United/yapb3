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
// BotActionManager.hpp
//
// Class: ActionManager
//
// Description: Bot action manager class.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef BOT_ACTION_MANAGER_INCLUDED
#define BOT_ACTION_MANAGER_INCLUDED

class ActionManager
{
	//
	// Group: Private members.
	//
	private:
		typedef Queue/*LinkedList/*DynamicArray*/ <Action *, unsigned char> ActionsContainer_t;

		ActionsContainer_t  m_actions;
//		Action             *m_actionsTable[Schedule::ID_Total];	// For easy searching by action ID....

		#if defined _DEBUG
			bool            m_someActionIsExecuted;
		#endif	// if defined _DEBUG

//		Action *m_currentAction;	// Actually current bot behaviour.

	//
	// Group: (Con/De)structors.
	//
	public:
		inline ActionManager (void)
			#if defined _DEBUG
				: m_someActionIsExecuted (false)
			#endif	// if defined _DEBUG
		{
			// Initialize the actions table....
//			for (unsigned char actionIndex (0u); actionIndex < Schedule::ID_Total; ++actionIndex)
//				m_actionsTable[actionIndex] = NULL;	// Null out the action pointer.
		}

		inline ~ActionManager (void)
		{
			// Delete all actions....
			FreeMemory ();
		}
/*
	//
	// Group: Private functions.
	//
	private:
		inline void Delete (const unsigned char actionIndex) { m_actions.Delete (actionIndex, 1u); }
*/
	//
	// Group: Functions.
	//
	public:
		inline Action *const GetAction (const Action::ID_t actionID)
		{
/*			for (unsigned char actionIndex (0u); actionIndex < m_actions.GetElementNumber (); ++actionIndex)
				if (m_actions[actionIndex]->GetID () == actionID)
					return m_actions[actionIndex];
*/
			for (ActionsContainer_t::LinkNodeType_t *tempNode (m_actions.GetHead ()); tempNode != NULL; tempNode = tempNode->GetNext ())
				if (tempNode->GetData ()->GetID () == actionID)
					return tempNode->GetData ();

			return NULL;
//			return m_actionsTable[actionID];
		}
		inline const Action *const GetAction (const Action::ID_t actionID) const
		{
/*			for (unsigned char actionIndex (0u); actionIndex < m_actions.GetElementNumber (); ++actionIndex)
				if (m_actions[actionIndex]->GetID () == actionID)
					return m_actions[actionIndex];
*/
			for (const ActionsContainer_t::LinkNodeType_t *tempNode (m_actions.GetHead ()); tempNode != NULL; tempNode = tempNode->GetNext ())
				if (tempNode->GetData ()->GetID () == actionID)
					return tempNode->GetData ();

			return NULL;
//			return m_actionsTable[actionID];
		}
		inline const bool HasAction (const Action *const action) const
		{
/*			for (unsigned char actionIndex (0u); actionIndex < m_actions.GetElementNumber (); ++actionIndex)
				if (m_actions[actionIndex] == action)
					return true;
*/
			for (const ActionsContainer_t::LinkNodeType_t *tempNode (m_actions.GetHead ()); tempNode != NULL; tempNode = tempNode->GetNext ())
				if (tempNode->GetData () == action)
					return true;

			return false;
//			return m_actionsTable[action->GetID ()] == action;
		}
		inline const bool HasAction (const Action::ID_t actionID) const
		{
/*			for (unsigned char actionIndex (0u); actionIndex < m_actions.GetElementNumber (); ++actionIndex)
				if (m_actions[actionIndex]->GetID () == actionID)
					return true;
*/
			for (const ActionsContainer_t::LinkNodeType_t *tempNode (m_actions.GetHead ()); tempNode != NULL; tempNode = tempNode->GetNext ())
				if (tempNode->GetData ()->GetID () == actionID)
					return true;

			return false;
//			return m_actionsTable[actionID] != NULL;
		}
		inline const bool HasActions (const Action::Bit_t actionsBits) const
		{
/*			for (unsigned char actionIndex (0u); actionIndex < m_actions.GetElementNumber (); ++actionIndex)
				if (BIT (m_actions[actionIndex]->GetID ()) & actionsBits)
					return true;
*/
			for (const ActionsContainer_t::LinkNodeType_t *tempNode (m_actions.GetHead ()); tempNode != NULL; tempNode = tempNode->GetNext ())
				if (BIT (tempNode->GetData ()->GetID ()) & actionsBits)
					return true;

			return false;
		}
		inline const bool IsCurrentAction (const Action::ID_t actionID) const
		{
			if (IsEmpty ())
				return false;

			return m_actions.GetFirstElement ()->GetID () == actionID;
		}
		inline const bool IsCurrentActions (const Action::Bit_t actionsBits) const
		{
			if (IsEmpty ())
				return false;

			return (BIT (m_actions.GetFirstElement ()->GetID ()) & actionsBits) > 0u;
		}

		inline void RemoveAction (const Action::ID_t/* actionID*/)
		{
/*			for (unsigned char actionIndex (0u); actionIndex < m_actions.GetElementNumber (); ++actionIndex)
				if (m_actions[actionIndex]->GetID () == actionID)
				{
					Delete (actionIndex);

					return;
				}
*/
			/// @todo !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

			// Error.
			#if defined _DEBUG
				AddLogEntry (false, LogLevel_Critical, false, "ActionManager::RemoveAction(): Action is not found!");
			#endif	// if defined _DEBUG

//			RemoveAction (m_actionsTable[actionID]);
		}
		inline void RemoveAction (Action *const action)
		{
			m_actions.FindAndDeleteElement (action);
/*
			m_actions -= action;

			delete m_actionsTable[action->GetID ()];

			m_actionsTable[action->GetID ()] = NULL;*/
		}

		inline void RemoveTop (void)
		{
			// Reliability check.
			InternalAssert (!IsEmpty ());

			m_actions.DeleteFront ();
		}

		inline void FreeMemory (void)
		{
			// Delete all actions....
			m_actions.DeleteAndPurgeAll/*DeleteAll*/ ();
		}

		inline void AddFrontFromExecutingAction (Action *const newAction)
		{
			// Reliability checks.
			InternalAssert (newAction != NULL);
			InternalAssert (!HasAction (newAction->GetID ()));

			#if defined _DEBUG
				// Reliability check.
				InternalAssert (m_someActionIsExecuted);
			#endif	// if defined _DEBUG

			// add
/// @todo !!!			m_actions.Insert (1u, newAction);
		}
		inline void AddFront (Action *const newAction)
		{
			// Reliability checks.
			InternalAssert (newAction != NULL);
//			InternalAssert (!HasAction (newAction->GetID ()));	// OCCURS!!!
			{
				Action *const existsAction (GetAction (newAction->GetID ()));

				if (existsAction != NULL)	// OCCURS!!!
				{
//CHECK ME!!! CRASH!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!					AddLogEntry (true, LogLevel_Error, true, "\"%s\"::AddFront(): HasAction (%s), current task: %s (state: %s), exits task state: %s!", newAction->GetOwner ()->GetName ().GetData (), newAction->GetName (), IsEmpty () ? "NONE" : FormatBuffer ("%s, state: %s", GetCurrentTask ()->GetName (), GetCurrentTask ()->GetStateString (), existsAction->GetStateString ()));

					return;
				}
			}
/*
			#if defined _DEBUG
				// Reliability check.
				InternalAssert (!m_someActionIsExecuted);
			#endif	// if defined _DEBUG
*/
			// add
/*			if (m_someActionIsExecuted)
				m_actions.Insert (1u, newAction);
			else*/
			m_actions.PushFront (newAction);
		}

		inline void AddAction (Action *const newAction)
		{
			// Reliability checks.
			InternalAssert (newAction != NULL);
			InternalAssert (!HasAction (newAction->GetID ()));	// OCCURS!!!
//			if (HasAction (newAction->GetID ()))	// OCCURS!!!
//				AddLogEntry (false, LogLevel_Critical, true, "\"%s\"::AddAction(): HasAction (%s), current task: %s!", newAction->GetOwner ()->GetName ().GetData (), newAction->GetName (), IsEmpty () ? "NONE" : GetCurrentTask ()->GetName ());

			// add
			m_actions += newAction;
		}
/*! @todo !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		inline void ChangeAction (const unsigned char actionIndex, Action *const newAction)
		{
			// Reliability checks.
			InternalAssert (newAction != NULL);
			InternalAssert (!HasAction (newAction->GetID ()));

			delete m_actions[actionIndex];

			m_actions[actionIndex] = newAction;
		}*/
		inline void ChangeAction (Action *const oldAction, Action *const newAction)
		{
			// Reliability checks.
			InternalAssert (!m_someActionIsExecuted);
			InternalAssert (oldAction != NULL);
			InternalAssert (newAction != NULL);
			InternalAssert (HasAction (oldAction));
			InternalAssert (!HasAction (newAction->GetID ()));

			for (ActionsContainer_t::LinkNodeType_t *tempNode (m_actions.GetHead ()); tempNode != NULL; tempNode = tempNode->GetNext ())
			{
				if (tempNode->GetData () != oldAction)
					continue;

				#if defined _DEBUG
					AddLogEntry (true, LogLevel_Default, false, "ActionManager::ChangeAction(): Bot \"%s\" changed old action %s to new %s!", oldAction->GetOwner ()->GetName ().GetData (), oldAction->GetName (), newAction->GetName ());
				#endif	// if defined _DEBUG

				delete tempNode->GetData ();

				tempNode->SetData (newAction);

				return;
			}
		}

		inline const bool IsEmpty (void) const { return m_actions.IsEmpty (); }

		inline Action *const GetCurrentAction (void)
		{
			// Reliability check.
			InternalAssert (!IsEmpty ());

			return m_actions.GetFirstElement ();
		}
		inline const Action *const GetCurrentAction (void) const
		{
			// Reliability check.
			InternalAssert (!IsEmpty ());

			return m_actions.GetFirstElement ();
		}

		inline Task *const GetCurrentTask (void)
		{
			// Reliability check.
			InternalAssert (!IsEmpty ());

			return m_actions.GetFirstElement ()->GetCurrentTask ();
		}
		inline const Task *const GetCurrentTask (void) const
		{
			// Reliability check.
			InternalAssert (!IsEmpty ());

			return m_actions.GetFirstElement ()->GetCurrentTask ();
		}
		inline void RemoveCurrentTask (void)
		{
			// Reliability check.
			InternalAssert (!IsEmpty ());

			if (m_actions.GetFirstElement ()->IsTask ())
				RemoveTop ();
			else
				m_actions.GetFirstElement ()->GetSchedulePointer ()->RemoveTop ();
		}

	//
	// Group: Engine callbacks.
	//
	public:
		inline void Think (void)
		{
			if (IsEmpty ())
				return;

			Action *const currentAction (m_actions.GetFirstElement ());

			InternalAssert (currentAction != NULL);

			// Is current task was idling? (just added)
			if (currentAction->IsIdling ())
			{
				#if defined _DEBUG
					AddLogEntry (true, LogLevel_Default, false, "ActionManager::Think(): Bot \"%s\" started new action %s!", currentAction->GetOwner ()->GetName ().GetData (), currentAction->GetName ());
				#endif	// if defined _DEBUG

				currentAction->Run ();	// Change task state.

				InternalAssert (!currentAction->IsIdling ());
			}

			if (!currentAction->IsRunning ())
			{
				#if defined _DEBUG
					if (currentAction->IsFailed ())
						AddLogEntry (true, LogLevel_Warning, false, "ActionManager::Think(): Bot \"%s\" current action %s is failed!", currentAction->GetOwner ()->GetName ().GetData (), currentAction->GetName ());
				#endif	// if defined _DEBUG

				RemoveTop ();

				return;
			}

			#if defined _DEBUG
				m_someActionIsExecuted = true;
			#endif	// if defined _DEBUG

			currentAction->Execute ();

			#if defined _DEBUG
				m_someActionIsExecuted = false;
			#endif	// if defined _DEBUG

			// Reliability check.
//			InternalAssert (m_actions.GetFirstElement () == currentAction);

			// Still running?
			if (currentAction->IsRunning ())
				return;

			#if defined _DEBUG
				if (currentAction->IsFailed ())
					AddLogEntry (true, LogLevel_Warning, true, "ActionManager::Think(): Bot \"%s\" current action %s is failed!", currentAction->GetOwner ()->GetName ().GetData (), currentAction->GetName ());
				else if (currentAction->IsCompleted ())
					AddLogEntry (true, LogLevel_Default, true, "ActionManager::Think(): Bot \"%s\" current action %s is completed!", currentAction->GetOwner ()->GetName ().GetData (), currentAction->GetName ());
				else
					InternalAssert (false);
			#endif	// if defined _DEBUG

			RemoveTop ();
		}

		inline void                         OnOwnerSpawn           (void)
		{
			if (!IsEmpty ())
				m_actions.GetFirstElement ()->OnOwnerSpawn ();
		}
		inline void                         OnTouched              (const HalfLifeEngine::SDK::Classes::Edict *const entity)
		{
			if (!IsEmpty ())
				m_actions.GetFirstElement ()->OnTouched (entity);
		}
		inline void                         OnReactOnRadio         (const RadioCommandsManager::RadioCommand *const radioMessage)
		{
			if (!IsEmpty ())
				m_actions.GetFirstElement ()->OnReactOnRadio (radioMessage);
		}
		inline void                         OnTakeDamage           (const HalfLifeEngine::SDK::Classes::Edict *const inflictor, const unsigned short damage, const unsigned char armor, const HalfLifeEngine::SDK::Constants::DamageType_t bits)
		{
			if (!IsEmpty ())
				m_actions.GetFirstElement ()->OnTakeDamage (inflictor, damage, armor, bits);
		}
		inline void                         OnTakeBlinded          (const float duration, const Client *const flasher)
		{
			if (!IsEmpty ())
				m_actions.GetFirstElement ()->OnTakeBlinded (duration, flasher);
		}
		inline void                         OnKilled               (const HalfLifeEngine::SDK::Classes::Edict *const killer)
		{
			if (!IsEmpty ())
				m_actions.GetFirstElement ()->OnKilled (killer);
		}
		inline void                         OnKill                 (const Client *const victim)
		{
			if (!IsEmpty ())
				m_actions.GetFirstElement ()->OnKill (victim);
		}
		inline void                         OnClientKilled         (Client *const victim, const HalfLifeEngine::SDK::Classes::Edict *const killer)
		{
			if (!IsEmpty ())
				m_actions.GetFirstElement ()->OnClientKilled (victim, killer);
		}
		inline void                         OnClientDisconnect     (Client *const client)
		{
			if (!IsEmpty ())
				m_actions.GetFirstElement ()->OnClientDisconnect (client);
		}
		inline void                         OnEnteredBombPlace     (void)
		{
			if (!IsEmpty ())
				m_actions.GetFirstElement ()->OnEnteredBombPlace ();
		}
		inline void                         OnLeaveBombPlace       (void)
		{
			if (!IsEmpty ())
				m_actions.GetFirstElement ()->OnLeaveBombPlace ();
		}
		inline void                         OnEnteredRescueZone    (void)
		{
			if (!IsEmpty ())
				m_actions.GetFirstElement ()->OnEnteredRescueZone ();
		}
		inline void                         OnLeaveEscapeZone      (void)
		{
			if (!IsEmpty ())
				m_actions.GetFirstElement ()->OnLeaveEscapeZone ();
		}
		inline void                         OnEnteredEscapeZone    (void)
		{
			if (!IsEmpty ())
				m_actions.GetFirstElement ()->OnEnteredEscapeZone ();
		}
		inline void                         OnLeaveRescueZone      (void)
		{
			if (!IsEmpty ())
				m_actions.GetFirstElement ()->OnLeaveRescueZone ();
		}
		inline void                         OnEnteredVIPSafetyZone (void)
		{
			if (!IsEmpty ())
				m_actions.GetFirstElement ()->OnEnteredVIPSafetyZone ();
		}
		inline void                         OnLeaveVIPSafetyZone   (void)
		{
			if (!IsEmpty ())
				m_actions.GetFirstElement ()->OnLeaveVIPSafetyZone ();
		}
		inline void                         OnEnteredBuyZone       (void)
		{
			if (!IsEmpty ())
				m_actions.GetFirstElement ()->OnEnteredBuyZone ();
		}
		inline void                         OnLeaveBuyZone         (void)
		{
			if (!IsEmpty ())
				m_actions.GetFirstElement ()->OnLeaveBuyZone ();
		}
		inline void                         OnAmmoAmountChanged    (const HalfLifeEngine::SDK::Constants::WeaponID_t ammoID, const unsigned char newAmount)
		{
			if (!IsEmpty ())
				m_actions.GetFirstElement ()->OnAmmoAmountChanged (ammoID, newAmount);
		}
		inline void                         OnRoundStarted         (void)
		{
			if (!IsEmpty ())
				m_actions.GetFirstElement ()->OnRoundStarted ();
		}
		inline void                         OnRoundEnded           (void)
		{
			if (!IsEmpty ())
				m_actions.GetFirstElement ()->OnRoundEnded ();
		}
		inline void                         OnBombPlanted          (void)
		{
			if (!IsEmpty ())
				m_actions.GetFirstElement ()->OnBombPlanted ();
		}
		inline void                         OnBombDetonated        (void)
		{
			if (!IsEmpty ())
				m_actions.GetFirstElement ()->OnBombDetonated ();
		}
		inline void                         OnBombDefused          (void)
		{
			if (!IsEmpty ())
				m_actions.GetFirstElement ()->OnBombDefused ();
		}
		inline void                         OnItemDeployPost       (const HalfLifeEngine::SDK::Classes::Edict *const weapon)
		{
			if (!IsEmpty ())
				m_actions.GetFirstElement ()->OnItemDeployPost (weapon);
		}
		inline void                         OnWeaponReloadPost     (const HalfLifeEngine::SDK::Classes::Edict *const weapon)
		{
			if (!IsEmpty ())
				m_actions.GetFirstElement ()->OnWeaponReloadPost (weapon);
		}
		inline void                         OnAddWeapon            (const HalfLifeEngine::SDK::Classes::Edict *const weapon)
		{
			if (!IsEmpty ())
				m_actions.GetFirstElement ()->OnAddWeapon (weapon);
		}
		inline void                         OnRemoveWeapon         (const HalfLifeEngine::SDK::Classes::Edict *const weapon)
		{
			if (!IsEmpty ())
				m_actions.GetFirstElement ()->OnRemoveWeapon (weapon);
		}
		inline void                         OnTouchingWeapon       (const HalfLifeEngine::SDK::Classes::Edict *const weapon)
		{
			if (!IsEmpty ())
				m_actions.GetFirstElement ()->OnTouchingWeapon (weapon);
		}
		inline void                         OnUse                  (const HalfLifeEngine::SDK::Classes::Edict *const entity, const HalfLifeEngine::SDK::Classes::Edict *const caller, const HalfLifeEngine::SDK::Constants::UseType_t type, const float value)
		{
			if (!IsEmpty ())
				m_actions.GetFirstElement ()->OnUse (entity, caller, type, value);
		}
		inline void                         OnTouchPost            (const HalfLifeEngine::SDK::Classes::Edict *const entity)
		{
			if (!IsEmpty ())
				m_actions.GetFirstElement ()->OnTouchPost (entity);
		}
		inline void                         OnHostageKilled        (const HalfLifeEngine::SDK::Classes::Edict *const hostage, const Client *const killer)
		{
			if (!IsEmpty ())
				m_actions.GetFirstElement ()->OnHostageKilled (hostage, killer);
		}
		inline void                         OnHostageRescued       (const HalfLifeEngine::SDK::Classes::Edict *const hostage)
		{
			if (!IsEmpty ())
				m_actions.GetFirstElement ()->OnHostageRescued (hostage);
		}
		inline void                         OnHostageUnusePost     (const HalfLifeEngine::SDK::Classes::Edict *const hostage, const Client *const currentRescuer)
		{
			if (!IsEmpty ())
				m_actions.GetFirstElement ()->OnHostageUnusePost (hostage, currentRescuer);
		}
		inline void                         OnHostageUsePost       (const HalfLifeEngine::SDK::Classes::Edict *const hostage, const Client *const previousRescuer)
		{
			if (!IsEmpty ())
				m_actions.GetFirstElement ()->OnHostageUsePost (hostage, previousRescuer);
		}
		inline void                         OnAllHostagesRescued   (void)
		{
			if (!IsEmpty ())
				m_actions.GetFirstElement ()->OnAllHostagesRescued ();
		}
		inline void                         OnImpulseCommand       (const HalfLifeEngine::SDK::Constants::PlayerImpulseID_t impulseCommand)
		{
			if (!IsEmpty ())
				m_actions.GetFirstElement ()->OnImpulseCommand (impulseCommand);
		}
		inline void                         OnHearNoisedSound      (const Client *const from, const NoisedSound_t &noisedSound)
		{
			if (!IsEmpty ())
				m_actions.GetFirstElement ()->OnHearNoisedSound (from, noisedSound);
		}
};

#endif	// ifndef BOT_ACTION_MANAGER_INCLUDED