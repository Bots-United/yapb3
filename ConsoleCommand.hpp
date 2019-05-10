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
// ConsoleCommand.hpp
//
// Class: Command
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef CONSOLE_COMMANDS_INCLUDED
#define CONSOLE_COMMANDS_INCLUDED

//class CommandContainer;

class Command
{
	//
	// Group: Constants.
	//
	public:
		enum AccesLevel_t
		{
			CMD_ACCESS_NONE,

			CMD_ACCESS_WAYPOINT = BIT (0u),
			CMD_ACCESS_BOT      = BIT (1u),
			CMD_ACCESS_UTIL     = BIT (2u),
			CMD_ACCESS_CONFIG   = BIT (3u),
			CMD_ACCESS_DEBUG    = BIT (4u),
			CMD_ACCESS_USERS    = BIT (5u),

			CMD_ACCESS_ALL      = CMD_ACCESS_WAYPOINT | CMD_ACCESS_UTIL | CMD_ACCESS_BOT | CMD_ACCESS_CONFIG | CMD_ACCESS_DEBUG | CMD_ACCESS_USERS
		};

	//
	// Group: Private members.
	//
	private:
		typedef DynamicArray <DynamicString, unsigned char> PossibleNamesArray_t;

		PossibleNamesArray_t m_possibleNames;	// command names
		unsigned short       m_accessLevel;

	//
	// Group: (Con/De)structors.
	//
	public:
		inline          Command (const DynamicString &possibleNames, const unsigned short accessLevel) : m_possibleNames (possibleNames.Split <PossibleNamesArray_t::IndexType_t> ('/')), m_accessLevel (accessLevel) { /* VOID */ }
		virtual inline ~Command (void) { /* VOID */ }

	//
	// Group: Functions.
	//
	public:
		inline const DynamicString                          GetPossibleNames                    (void) const
		{
			// Reliability check.
			InternalAssert (!m_possibleNames.IsEmpty ());

			DynamicString possibleNames;

			// Format possible names to 'command1/command2/etc....'.
			for (PossibleNamesArray_t::IndexType_t index (0u); /* Empty */; possibleNames += '/')
			{
				possibleNames += m_possibleNames[index];

				if (++index >= m_possibleNames.GetElementNumber ())
					break;
			}

			return possibleNames;
		}

		// check command name
		virtual inline const bool                           IsCommand                           (const DynamicString &commandName) const
		{
			for (PossibleNamesArray_t::IndexType_t index (0u); index < m_possibleNames.GetElementNumber (); ++index)
				if (m_possibleNames[index].CompareWithoutCaseCheck (commandName) == 0)
					return true;

			return false;
		}

		// check human access level for this command
		inline const bool                                   IsHumanHasAccessLevel               (const Human *const human)                     const { return human->IsAdmin () || (human->GetPlayerPointer ()->GetCommandAccessLevel () & m_accessLevel) == m_accessLevel; }
		inline void                                         SetAccessLevel                      (const unsigned short accessLevel)                   { m_accessLevel = accessLevel; }
		inline void                                         AddAccessLevel                      (const AccesLevel_t accessLevel)                     { m_accessLevel |= accessLevel; }
		inline void                                         RemoveAccessLevel                   (const AccesLevel_t accessLevel)                     { m_accessLevel &= ~accessLevel; }

		// execute command
		virtual void                                        Execute                             (Human *const human, const DynamicString &commandName, const DynamicString &argument1, const DynamicString &argument2, const DynamicString &argument3, const DynamicString &argument4, const DynamicString &argument5) const =/*>*/ 0;	// function to execute this command

		virtual inline const bool                           IsCommandsContainer                 (void) const { return false; }
/*		inline const CommandContainer *const                GetCommandsContainerPointer         (void) const
		{
			// Reliability check.
			InternalAssert (IsCommandsContainer ());

			return static_cast <const CommandContainer *const> (this);
		}
*/
		virtual inline const Localizer::LocalizedStringID_t GetLocalizedHelpInformationStringID (void) const =/*>*/ 0;

		virtual inline const bool                           IsCanBeUsedOnDedicatedServer        (void) const =/*>*/ 0;
		virtual inline const bool                           IsShouldUsedInGame                  (void) const =/*>*/ 0;	// Whether we need to be in game or not. (Used only on listen server)
};

class Command_PrintMemoryUsage : public Command
{
	//
	// Group: (Con/De)structors.
	//
	public:
		inline Command_PrintMemoryUsage (void) : Command ("memory/memoryusage/memory_usage/printmemoryusage/print_memory_usage", CMD_ACCESS_BOT) { /* VOID */ }

	//
	// Group: Functions.
	//
	public:
		// execute command
		inline void                                 Execute                             (Human *const human, const DynamicString &/*commandName*/, const DynamicString &/*argument1*/, const DynamicString &/*argument2*/, const DynamicString &/*argument3*/, const DynamicString &/*argument4*/, const DynamicString &/*argument5*/) const
		{
			if (human != NULL)
				human->PrintFormat
				(
					HalfLifeEngine::SDK::Constants::PrintType_AtConsole,

					"Memory usage:\n"
					"   %u bytes host client\b"
					"   %u bytes per player\b"
					"   %u bytes per other bot\b"
					"   %u bytes per bot\b\n"

					"   %u Navigation Areas @ %u bytes each = %u bytes\n"
					"   %u Hiding Spots @ %u bytes each = %u bytes\n",
//					"   Encounter Spot data = %u bytes\n",

					sizeof (Host),
					sizeof (Player),
					sizeof (FakeClient_NotYaPB),
					sizeof (YaPB),

					g_server->GetNavigationMeshManager ().GetAreas ().GetElementNumber (),
					sizeof (NavigationMesh::NavigationArea),
					sizeof (NavigationMesh::NavigationArea) * g_server->GetNavigationMeshManager ().GetAreas ().GetElementNumber (),

					g_server->GetNavigationMeshManager ().GetHidingSpots ().GetElementNumber (),
					sizeof (NavigationMesh::NavigationArea::HidingSpot),
					sizeof (NavigationMesh::NavigationArea::HidingSpot) * g_server->GetNavigationMeshManager ().GetHidingSpots ().GetElementNumber ()
				);
			else
				HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat
				(
					"Memory usage:\n"
					"   %u bytes host client\b"
					"   %u bytes per player\b"
					"   %u bytes per other bot\b"
					"   %u bytes per bot\b\n"

					"   %u Navigation Areas @ %u bytes each = %u bytes\n"
					"   %u Hiding Spots @ %u bytes each = %u bytes\n",
//					"   Encounter Spot data = %u bytes\n",

					sizeof (Host),
					sizeof (Player),
					sizeof (FakeClient_NotYaPB),
					sizeof (YaPB),

					g_server->GetNavigationMeshManager ().GetAreas ().GetElementNumber (),
					sizeof (NavigationMesh::NavigationArea),
					sizeof (NavigationMesh::NavigationArea) * g_server->GetNavigationMeshManager ().GetAreas ().GetElementNumber (),

					g_server->GetNavigationMeshManager ().GetHidingSpots ().GetElementNumber (),
					sizeof (NavigationMesh::NavigationArea::HidingSpot),
					sizeof (NavigationMesh::NavigationArea::HidingSpot) * g_server->GetNavigationMeshManager ().GetHidingSpots ().GetElementNumber ()
				);
		}

		inline const Localizer::LocalizedStringID_t GetLocalizedHelpInformationStringID (void) const { return Localizer::LocalizedStringID_TODO; }

		inline const bool                           IsCanBeUsedOnDedicatedServer        (void) const { return true; }
		inline const bool                           IsShouldUsedInGame                  (void) const { return false; }	// Whether we need to be in game or not. (Used only on listen server)
};

//////////////////////////////////////////////////////////////////////
// add/addbot - adding one bot with random parameters to random team.
class Command_AddBot : public Command
{
	//
	// Group: (Con/De)structors.
	//
	public:
		inline Command_AddBot (void) : Command ("add/addbot/addone", CMD_ACCESS_BOT) { /* VOID */ }

	//
	// Group: Functions.
	//
	public:
		// execute command
		inline void                                 Execute                             (Human *const human, const DynamicString &commandName, const DynamicString &argument1, const DynamicString &argument2, const DynamicString &argument3, const DynamicString &argument4, const DynamicString &argument5) const
		{
			g_server->GetYaPBManager ()->AddBot (argument4, argument1, argument2, argument3, argument5);
		}

		inline const Localizer::LocalizedStringID_t GetLocalizedHelpInformationStringID (void) const { return Localizer::LocalizedStringID_CreatesBotInCurrentGame; }

		inline const bool                           IsCanBeUsedOnDedicatedServer        (void) const { return true; }
		inline const bool                           IsShouldUsedInGame                  (void) const { return true; }	// Whether we need to be in game or not. (Used only on listen server)
};

//////////////////////////////////////////////////////////////////////////
// kick/kickbot/kickone - kick off one random bot from the played server.
class Command_KickBot : public Command
{
	//
	// Group: (Con/De)structors.
	//
	public:
		inline Command_KickBot (void) : Command ("kick/kickbot/kickone", CMD_ACCESS_BOT) { /* VOID */ }

	//
	// Group: Functions.
	//
	public:
		// execute command
		inline void                                  Execute                             (Human *const human, const DynamicString &commandName, const DynamicString &argument1, const DynamicString &argument2, const DynamicString &argument3, const DynamicString &argument4, const DynamicString &argument5) const
		{
			g_server->GetYaPBManager ()->KickRandom ();
		}

		inline const Localizer::LocalizedStringID_t GetLocalizedHelpInformationStringID (void) const { return Localizer::LocalizedStringID_DisconnectsRandomBotFromGame; }

		inline const bool                           IsCanBeUsedOnDedicatedServer        (void) const { return true; }
		inline const bool                           IsShouldUsedInGame                  (void) const { return true; }	// Whether we need to be in game or not. (Used only on listen server)
};

class Command_Bot_PossessBot : public Command
{
	//
	// Group: (Con/De)structors.
	//
	public:
		inline Command_Bot_PossessBot (void) : Command ("possessBot/possess_bot", CMD_ACCESS_BOT) { /* VOID */ }

	//
	// Group: Functions.
	//
	public:
		// execute command
		inline void                                 Execute                             (Human *const human, const DynamicString &commandName, const DynamicString &argument1, const DynamicString &argument2, const DynamicString &argument3, const DynamicString &argument4, const DynamicString &argument5) const
		{
			// Without name/user ID?
			if (commandName.IsEmpty ())
				return;

			if (g_server->GetYaPBManager ()->GetBotsCount () == 0u)
			{
				human->Print (HalfLifeEngine::SDK::Constants::PrintType_AtConsole, human->GetLocalizer ()->GetLocalizedString (Localizer::LocalizedStringID_ThereAreNoYaPBOnTheServer) + '\n');

				return;
			}

			Client *const client (g_server->GetClientManager ()->GetClientByString (commandName));

			if (client == NULL)
			{
				human->Print (HalfLifeEngine::SDK::Constants::PrintType_AtConsole, human->GetLocalizer ()->GetLocalizedString (Localizer::LocalizedStringID_BotWithThatNameNotFound) + '\n');

				return;
			}

			if (!client->IsYaPB ())
			{
				human->Print (HalfLifeEngine::SDK::Constants::PrintType_AtConsole, human->GetLocalizer ()->GetLocalizedString (Localizer::LocalizedStringID_BotWithThatNameNotFound) + '\n');

				return;
			}
/*
			// Put the local player in control of this bot.
			if ( engine->Cmd_Argc() != 2 )
			{
				Warning( "PossessBot <client index>\n" );
				return;
			}

			int iBotClient = atoi( engine->Cmd_Argv(1) );
			int iBotEnt = iBotClient + 1;

			if ( iBotClient < 0 || iBotClient >= gpGlobals->maxClients || pPlayer->entindex() == iBotEnt )
			{
				Warning( "PossessBot <client index>\n" );
			}
			else*/
			{
				HalfLifeEngine::SDK::Classes::Edict *pPlayerData = human->GetEdict ();//pPlayer->m_variables;
				HalfLifeEngine::SDK::Classes::Edict *pBotData = client->GetEdict ();//engine->PEntityOfEntIndex( iBotEnt );

				if (pBotData != NULL && pBotData->privateData != NULL)
				{
					// SWAP EDICTS
/*
					// Backup things we don't want to swap.
					HalfLifeEngine::SDK::Classes::Edict oldPlayerData = *pPlayerData;
					HalfLifeEngine::SDK::Classes::Edict oldBotData = *pBotData;
*/
					// Swap edicts.
					SwapElements (*pPlayerData, *pBotData);

					// Restore things we didn't want to swap.
					//pPlayerData->m_EntitiesTouched = oldPlayerData.m_EntitiesTouched;
					//pBotData->m_EntitiesTouched = oldBotData.m_EntitiesTouched;

					HalfLifeEngine::SDK::Classes::BaseEntity *pPlayerBaseEnt = pPlayerData->privateData;
					HalfLifeEngine::SDK::Classes::BaseEntity *pBotBaseEnt = pBotData->privateData;

					// Make the other a bot and make the player not a bot.
//					pPlayerBaseEnt->RemoveFlag (FL_FAKECLIENT);
					pPlayerBaseEnt->m_variables->flags &= ~HalfLifeEngine::SDK::Constants::FL_FAKECLIENT;
//					pBotBaseEnt->AddFlag (FL_FAKECLIENT);
					pBotBaseEnt->m_variables->flags |= HalfLifeEngine::SDK::Constants::FL_FAKECLIENT;

					// Point the CBaseEntities at the right players.
					pPlayerBaseEnt->m_variables = &pPlayerData->variables;
					pBotBaseEnt->m_variables = &pBotData->variables;

					// Freeze the bot.
//					pBotBaseEnt->AddEFlags (EFL_BOT_FROZEN);
				}
			}
		}

		inline const Localizer::LocalizedStringID_t GetLocalizedHelpInformationStringID (void) const { return Localizer::LocalizedStringID_TODO; }

		inline const bool                           IsCanBeUsedOnDedicatedServer        (void) const { return false; }
		inline const bool                           IsShouldUsedInGame                  (void) const { return true; }	// Whether we need to be in game or not. (Used only on listen server)
};

#if defined _DEBUG
	class Command_SetDebugBot : public Command
	{
		//
		// Group: (Con/De)structors.
		//
		public:
			inline Command_SetDebugBot (void) : Command ("setDebugBot/set_debug_bot", CMD_ACCESS_BOT) { /* VOID */ }

		//
		// Group: Functions.
		//
		public:
			// execute command
			inline void                                  Execute                             (Human *const human, const DynamicString &commandName, const DynamicString &argument1, const DynamicString &argument2, const DynamicString &argument3, const DynamicString &argument4, const DynamicString &argument5) const
			{
				// Without name/user ID?
				if (commandName.IsEmpty ())
					return;

				if (g_server->GetYaPBManager ()->GetBotsCount () == 0u)
				{
					if (human != NULL)
						human->Print (HalfLifeEngine::SDK::Constants::PrintType_AtConsole, human->GetLocalizer ()->GetLocalizedString (Localizer::LocalizedStringID_ThereAreNoYaPBOnTheServer) + '\n');
					else
						HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrint (g_localizer.GetLocalizedString (Localizer::LocalizedStringID_ThereAreNoYaPBOnTheServer) + '\n');

					return;
				}

				Client *const client (g_server->GetClientManager ()->GetClientByString (commandName));

				if (client == NULL)
				{
					if (human != NULL)
						human->Print (HalfLifeEngine::SDK::Constants::PrintType_AtConsole, human->GetLocalizer ()->GetLocalizedString (Localizer::LocalizedStringID_BotWithThatNameNotFound) + '\n');
					else
						HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrint (g_localizer.GetLocalizedString (Localizer::LocalizedStringID_BotWithThatNameNotFound) + '\n');

					return;
				}

				if (!client->IsYaPB ())
				{
					if (human != NULL)
						human->Print (HalfLifeEngine::SDK::Constants::PrintType_AtConsole, human->GetLocalizer ()->GetLocalizedString (Localizer::LocalizedStringID_BotWithThatNameNotFound) + '\n');
					else
						HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrint (g_localizer.GetLocalizedString (Localizer::LocalizedStringID_BotWithThatNameNotFound) + '\n');

					return;
				}

				g_server->GetYaPBManager ()->SetDebugBot (client->GetYaPBPointer ());
			}

			inline const Localizer::LocalizedStringID_t GetLocalizedHelpInformationStringID (void) const { return Localizer::LocalizedStringID_TODO; }

			inline const bool                           IsCanBeUsedOnDedicatedServer        (void) const { return true; }
			inline const bool                           IsShouldUsedInGame                  (void) const { return true; }	// Whether we need to be in game or not. (Used only on listen server)
	};

	// Debugging changes the view entity to the specified index.
	class Command_SetViewEntity : public Command
	{
		//
		// Group: (Con/De)structors.
		//
		public:
			inline Command_SetViewEntity (void) : Command ("setViewEntity/set_view_entity", CMD_ACCESS_BOT) { /* VOID */ }

		//
		// Group: Functions.
		//
		public:
			// execute command
			inline void                                  Execute                             (Human *const human, const DynamicString &commandName, const DynamicString &argument1, const DynamicString &argument2, const DynamicString &argument3, const DynamicString &argument4, const DynamicString &argument5) const
			{
				InternalAssert (human != NULL);

				HalfLifeEngine::SDK::Structures::Client_t &engineClient (human->GetEngineClient ());

				// Without entity index?
				if (commandName.IsEmpty ())
				{
					human->PrintFormat (HalfLifeEngine::SDK::Constants::PrintType_AtConsole, "setViewEntity/set_view_entity entity index #\nCurrent \"%s\"\n", engineClient.pViewEntity == NULL ? "NULL" : engineClient.pViewEntity->GetClassName ().GetData ());

					return;
				}

				const unsigned short entityIndex (commandName.GetValue <unsigned short> ());

				if (entityIndex == 0u || entityIndex >= HalfLifeEngine::Globals::g_halfLifeEngine->GetNumberOfEntities ())
				{
					human->PrintFormat (HalfLifeEngine::SDK::Constants::PrintType_AtConsole, "setViewEntity/set_view_entity entity index %u is out of bounds!\n", entityIndex);

					return;
				}

				HalfLifeEngine::SDK::Classes::Edict *const entityEdict (HalfLifeEngine::Globals::g_halfLifeEngine->GetEdictOfIndex (entityIndex));

				HalfLifeEngine::Globals::g_halfLifeEngine->SetClientView (*human, entityEdict);
/*
				if (*human != entityEdict)
					HalfLifeEngine::Globals::g_halfLifeEngine->SetEntityModel (entityEdict, entityEdict->GetNonPlayerModel ());
*/
				human->PrintFormat (HalfLifeEngine::SDK::Constants::PrintType_AtConsole, "View entity set to %u\n", entityIndex);
			}

			inline const Localizer::LocalizedStringID_t GetLocalizedHelpInformationStringID (void) const { return Localizer::LocalizedStringID_TODO; }

			inline const bool                           IsCanBeUsedOnDedicatedServer        (void) const { return false; }
			inline const bool                           IsShouldUsedInGame                  (void) const { return true; }	// Whether we need to be in game or not. (Used only on listen server)
	};
#endif	// if defined _DEBUG

// Container of commands.
class CommandContainer : public Command
{
	//
	// Group: Type definitions.
	//
	public:
		typedef DynamicArray <Command *, unsigned char> CommandsArray_t;

	//
	// Group: Protected members.
	//
	protected:
		CommandsArray_t m_commands;

	//
	// Group: (Con/De)structors.
	//
	public:
		inline CommandContainer (const DynamicString &possibleNames, const unsigned short accessLevel) : Command (possibleNames, accessLevel) { /* VOID */ }
		virtual inline ~CommandContainer (void) { m_commands.DeleteAll (); }

	//
	// Group: Functions.
	//
	public:
		// execute command
		virtual void                               Execute                             (Human *const human, const DynamicString &commandName, const DynamicString &argument1, const DynamicString &argument2, const DynamicString &argument3, const DynamicString &argument4, const DynamicString &argument5) const;

		virtual inline void                        PrintHelp                           (const Human *const printTo) const
		{
			PrintPossibleNamesAndAppointment (printTo);
		}
		virtual inline void                        PrintPossibleNamesAndAppointment    (const Human *const printTo) const
		{
			// Is dedicated server or server is not running?
			if (printTo == NULL)
			{
				/// @todo REMOVE IT FROM HERE!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?
				if (HalfLifeEngine::Globals::g_halfLifeEngine->IsDedicatedServer ())
				{
					if (!IsCanBeUsedOnDedicatedServer ())
					{
						AddLogEntry (true, LogLevel_Error, false, g_localizer.GetLocalizedString (Localizer::LocalizedStringID_ConsoleCommandCanOnlyBeExecutedFromServerConsole), GetPossibleNames ().GetData ());

						return;
					}
				}
				else if (IsShouldUsedInGame () > g_server->IsRunning ())	// Make sure the game has started if we need.
				{
					AddLogEntry (true, LogLevel_Error, false, g_localizer.GetLocalizedString (Localizer::LocalizedStringID_ConsoleCommandCanOnlyBeExecutedWhenServerIsRunning), GetPossibleNames ().GetData ());

					return;
				}

				HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrint (g_localizer.GetLocalizedString (GetLocalizedHelpInformationStringID ()) + '\n');

				for (CommandsArray_t::IndexType_t index (0u); index < m_commands.GetElementNumber (); ++index)
					if (m_commands[index]->IsCanBeUsedOnDedicatedServer () == HalfLifeEngine::Globals::g_halfLifeEngine->IsDedicatedServer () && m_commands[index]->IsShouldUsedInGame () <= g_server->IsRunning ())
						HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrint ('\t' + m_commands[index]->GetPossibleNames () + " - " + g_localizer.GetLocalizedString (m_commands[index]->GetLocalizedHelpInformationStringID ()) + '\n');
			}
			else
			{
				/// @todo REMOVE IT FROM HERE!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?
				if (!IsHumanHasAccessLevel (printTo))
				{
					const DynamicString &possibleNames (GetPossibleNames ());

					AddLogEntry (false, LogLevel_Error, false, g_localizer.GetLocalizedString (Localizer::LocalizedStringID_ClientHasExecutedConsoleVariableCanBeOnlySetViaRCONAccess), printTo->GetName ().GetData (), possibleNames.GetData ());
					printTo->PrintFormat (HalfLifeEngine::SDK::Constants::PrintType_AtConsole, printTo->GetLocalizer ()->GetLocalizedString (Localizer::LocalizedStringID_ConsoleVariableCanBeOnlySetViaRCONAccess) + '\n', possibleNames.GetData ());

					return;
				}

				// Make sure the game has started if we need.
				if (IsShouldUsedInGame () > g_server->IsRunning ())
				{
					const DynamicString &possibleNames (GetPossibleNames ());

					if (printTo->IsHost ())
						AddLogEntry (true, LogLevel_Error, false, g_localizer.GetLocalizedString (Localizer::LocalizedStringID_ConsoleCommandCanOnlyBeExecutedWhenServerIsRunning), possibleNames.GetData ());
					else
					{
						AddLogEntry (false, LogLevel_Error, false, g_localizer.GetLocalizedString (Localizer::LocalizedStringID_ClientHasExecutedConsoleCommandThatCanOnlyBeExecutedWhenServerIsRunning), printTo->GetName ().GetData (), possibleNames.GetData ());
						printTo->PrintFormat (HalfLifeEngine::SDK::Constants::PrintType_AtConsole, printTo->GetLocalizer ()->GetLocalizedString (Localizer::LocalizedStringID_ConsoleCommandCanOnlyBeExecutedWhenServerIsRunning) + '\n', possibleNames.GetData ());
					}

					return;
				}

				printTo->Print (HalfLifeEngine::SDK::Constants::PrintType_AtConsole, printTo->GetLocalizer ()->GetLocalizedString (GetLocalizedHelpInformationStringID ()) + '\n');

				for (CommandsArray_t::IndexType_t index (0u); index < m_commands.GetElementNumber (); ++index)
				{
					// Make sure the game has started if we need.
					if (m_commands[index]->IsHumanHasAccessLevel (printTo) && m_commands[index]->IsShouldUsedInGame () <= g_server->IsRunning ())
						printTo->Print (HalfLifeEngine::SDK::Constants::PrintType_AtConsole, '\t' + m_commands[index]->GetPossibleNames () + " - " + printTo->GetLocalizer ()->GetLocalizedString (m_commands[index]->GetLocalizedHelpInformationStringID ()) + '\n');
				}
			}
		}

		inline const bool                          IsCommandsContainer                 (void) const { return true; }

		inline const CommandsArray_t              &GetCommands                         (void) const { return m_commands; }
};

class CommandContainer_Menu : public CommandContainer
{
class Command_Main : public Command
{
	//
	// Group: (Con/De)structors.
	//
	public:
		inline Command_Main (void) : Command ("main", CMD_ACCESS_BOT) { /* VOID */ }

	//
	// Group: Functions.
	//
	public:
		// execute command
		inline void                                 Execute                             (Human *const human, const DynamicString &commandName, const DynamicString &argument1, const DynamicString &argument2, const DynamicString &argument3, const DynamicString &argument4, const DynamicString &argument5) const
		{
			g_server->GetHumanManager ()->GetHostClient ()->DisplayMenu (MenuID_YaPBMain);
		}

		// check command name
		inline const bool                           IsCommand                           (const DynamicString &commandName) const
		{
			if (commandName.IsEmpty ())
				return true;

			// Call original function in base class....
			return Command::IsCommand (commandName);
		}

		inline const Localizer::LocalizedStringID_t GetLocalizedHelpInformationStringID (void) const { return Localizer::LocalizedStringID_TODO; }

		inline const bool                           IsCanBeUsedOnDedicatedServer        (void) const { return false; }
		inline const bool                           IsShouldUsedInGame                  (void) const { return true; }	// Whether we need to be in game or not. (Used only on listen server)
};
class Command_Control : public Command
{
	//
	// Group: (Con/De)structors.
	//
	public:
		inline Command_Control (void) : Command ("control", CMD_ACCESS_BOT) { /* VOID */ }

	//
	// Group: Functions.
	//
	public:
		// execute command
		inline void                                 Execute                             (Human *const human, const DynamicString &commandName, const DynamicString &argument1, const DynamicString &argument2, const DynamicString &argument3, const DynamicString &argument4, const DynamicString &argument5) const
		{
			g_server->GetHumanManager ()->GetHostClient ()->DisplayMenu (MenuID_BotControl);
		}

		inline const Localizer::LocalizedStringID_t GetLocalizedHelpInformationStringID (void) const { return Localizer::LocalizedStringID_TODO; }

		inline const bool                           IsCanBeUsedOnDedicatedServer        (void) const { return false; }
		inline const bool                           IsShouldUsedInGame                  (void) const { return true; }	// Whether we need to be in game or not. (Used only on listen server)
};

	//
	// Group: (Con/De)structors.
	//
	public:
		inline CommandContainer_Menu (void) : CommandContainer ("menu", CMD_ACCESS_BOT)
		{
			m_commands += new Command_Main ();
			m_commands += new Command_Control ();
		}

	//
	// Group: Functions.
	//
	public:
		inline const Localizer::LocalizedStringID_t GetLocalizedHelpInformationStringID (void) const { return Localizer::LocalizedStringID_TODO; }

		inline const bool                           IsCanBeUsedOnDedicatedServer        (void) const { return false; }
		inline const bool                           IsShouldUsedInGame                  (void) const { return true; }	// Whether we need to be in game or not. (Used only on listen server)
};

class CommandContainer_Localizer : public CommandContainer
{
class Command_Localizer_Reload : public Command
{
	//
	// Group: (Con/De)structors.
	//
	public:
		inline Command_Localizer_Reload (void) : Command ("reload", CMD_ACCESS_BOT) { /* VOID */ }

	//
	// Group: Functions.
	//
	public:
		// execute command
		inline void                                  Execute                             (Human *const human, const DynamicString &commandName, const DynamicString &argument1, const DynamicString &argument2, const DynamicString &argument3, const DynamicString &argument4, const DynamicString &argument5) const
		{
			g_localizer.Reload ();
		}

		inline const Localizer::LocalizedStringID_t GetLocalizedHelpInformationStringID (void) const { return Localizer::LocalizedStringID_ConsoleCommand_Localizer_Reload; }

		inline const bool                           IsCanBeUsedOnDedicatedServer        (void) const { return true; }
		inline const bool                           IsShouldUsedInGame                  (void) const { return false; }	// Whether we need to be in game or not. (Used only on listen server)
};
class Command_Localizer_LoadOriginalStrings : public Command
{
	//
	// Group: (Con/De)structors.
	//
	public:
		inline Command_Localizer_LoadOriginalStrings (void) : Command ("load_original_strings", CMD_ACCESS_BOT) { /* VOID */ }

	//
	// Group: Functions.
	//
	public:
		// execute command
		inline void                                  Execute                             (Human *const human, const DynamicString &commandName, const DynamicString &argument1, const DynamicString &argument2, const DynamicString &argument3, const DynamicString &argument4, const DynamicString &argument5) const
		{
			g_localizer.LoadOriginalStrings ();
		}

		inline const Localizer::LocalizedStringID_t GetLocalizedHelpInformationStringID (void) const { return Localizer::LocalizedStringID_ConsoleCommand_Localizer_LoadOriginalStrings; }

		inline const bool                           IsCanBeUsedOnDedicatedServer        (void) const { return true; }
		inline const bool                           IsShouldUsedInGame                  (void) const { return false; }	// Whether we need to be in game or not. (Used only on listen server)
};

	//
	// Group: (Con/De)structors.
	//
	public:
		inline CommandContainer_Localizer (void) : CommandContainer ("localizer", CMD_ACCESS_BOT)
		{
			m_commands += new Command_Localizer_Reload ();
			m_commands += new Command_Localizer_LoadOriginalStrings ();
		}

	//
	// Group: Functions.
	//
	public:
		inline const Localizer::LocalizedStringID_t GetLocalizedHelpInformationStringID (void) const { return Localizer::LocalizedStringID_ConsoleCommand_Localizer; }

		inline const bool                           IsCanBeUsedOnDedicatedServer        (void) const { return true; }
		inline const bool                           IsShouldUsedInGame                  (void) const { return false; }	// Whether we need to be in game or not. (Used only on listen server)
};

class CommandContainer_Profiles : public CommandContainer
{
class Command_Profiles_Print : public Command
{
	//
	// Group: (Con/De)structors.
	//
	public:
		inline Command_Profiles_Print (void) : Command ("print/view/show", CMD_ACCESS_BOT) { /* VOID */ }

	//
	// Group: Functions.
	//
	public:
		// execute command
		inline void                                  Execute                             (Human *const human, const DynamicString &commandName, const DynamicString &argument1, const DynamicString &argument2, const DynamicString &argument3, const DynamicString &argument4, const DynamicString &argument5) const
		{
			g_server->GetYaPBManager ()->GetProfileManager ().PrintBotProfiles ();
		}

		inline const Localizer::LocalizedStringID_t GetLocalizedHelpInformationStringID (void) const { return Localizer::LocalizedStringID_TODO; }

		inline const bool                           IsCanBeUsedOnDedicatedServer        (void) const { return true; }
		inline const bool                           IsShouldUsedInGame                  (void) const { return true; }	// Whether we need to be in game or not. (Used only on listen server)
};

	//
	// Group: (Con/De)structors.
	//
	public:
		inline CommandContainer_Profiles (void) : CommandContainer ("profiles", CMD_ACCESS_BOT)
		{
			m_commands += new Command_Profiles_Print ();
		}

	//
	// Group: Functions.
	//
	public:
		inline const Localizer::LocalizedStringID_t GetLocalizedHelpInformationStringID (void) const { return Localizer::LocalizedStringID_TODO; }

		inline const bool                           IsCanBeUsedOnDedicatedServer        (void) const { return true; }
		inline const bool                           IsShouldUsedInGame                  (void) const { return false; }	// Whether we need to be in game or not. (Used only on listen server)
};

class CommandContainer_Waypoint : public CommandContainer
{
class Command_Waypoint_Draw : public Command
{
	//
	// Group: (Con/De)structors.
	//
	public:
		inline Command_Waypoint_Draw (void) : Command ("draw/show", CMD_ACCESS_WAYPOINT) { /* VOID */ }

	//
	// Group: Functions.
	//
	public:
		// execute command
		inline void                                 Execute                             (Human *const human, const DynamicString &commandName, const DynamicString &argument1, const DynamicString &argument2, const DynamicString &argument3, const DynamicString &argument4, const DynamicString &argument5) const
		{
			if (commandName.IsEmpty () || commandName.CompareWithoutCaseCheck ("toggle") == 0)
			{
				g_server->GetWaypointManager ().ToggleDraw ();

				HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat ("Waypoint Editing %s.\n", g_server->GetWaypointManager ().IsEditModeActivated () ? "Enabled" : "Disabled");
			}
			else if (commandName == "1" || commandName.CompareWithoutCaseCheck ("on") == 0 || commandName.CompareWithoutCaseCheck ("enable") == 0)
			{
				g_server->GetWaypointManager ().ToggleDraw (true);

				HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrint ("Waypoint Editing Enabled.\n");
			}
			else if (commandName == "0" || commandName.CompareWithoutCaseCheck ("off") == 0 || commandName.CompareWithoutCaseCheck ("disable") == 0)
			{
				g_server->GetWaypointManager ().ToggleDraw (false);

				HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrint ("Waypoint Editing Disabled.\n");
			}
			else
				human->Print (HalfLifeEngine::SDK::Constants::PrintType_AtConsole, "Error: Unknown state!");
		}

		inline const Localizer::LocalizedStringID_t GetLocalizedHelpInformationStringID (void) const { return Localizer::LocalizedStringID_TODO; }

		inline const bool                           IsCanBeUsedOnDedicatedServer        (void) const { return false; }
		inline const bool                           IsShouldUsedInGame                  (void) const { return true; }	// Whether we need to be in game or not. (Used only on listen server)
};
class Command_Waypoint_Add : public Command
{
	//
	// Group: (Con/De)structors.
	//
	public:
		inline Command_Waypoint_Add (void) : Command ("add/build", CMD_ACCESS_WAYPOINT) { /* VOID */ }

	//
	// Group: Functions.
	//
	public:
		// execute command
		inline void                                 Execute                             (Human *const human, const DynamicString &commandName, const DynamicString &argument1, const DynamicString &argument2, const DynamicString &argument3, const DynamicString &argument4, const DynamicString &argument5) const
		{
			g_server->GetWaypointManager ().ToggleDraw (true);

			g_server->GetWaypointManager ().AddWaypoint (human->GetOrigin (), Waypoint::Flag_None, human->GetIllumination ());
		}

		inline const Localizer::LocalizedStringID_t GetLocalizedHelpInformationStringID (void) const { return Localizer::LocalizedStringID_TODO; }

		inline const bool                           IsCanBeUsedOnDedicatedServer        (void) const { return false; }
		inline const bool                           IsShouldUsedInGame                  (void) const { return true; }	// Whether we need to be in game or not. (Used only on listen server)
};
class Command_Waypoint_ToggleAutowppointing : public Command
{
	//
	// Group: (Con/De)structors.
	//
	public:
		inline Command_Waypoint_ToggleAutowppointing (void) : Command ("autoWP/autoWaypoint", CMD_ACCESS_WAYPOINT) { /* VOID */ }

	//
	// Group: Functions.
	//
	public:
		// execute command
		inline void                                 Execute                             (Human *const human, const DynamicString &commandName, const DynamicString &argument1, const DynamicString &argument2, const DynamicString &argument3, const DynamicString &argument4, const DynamicString &argument5) const
		{
//			g_server->GetWaypointManager ().ToggleAutowppointing ();
		}

		inline const Localizer::LocalizedStringID_t GetLocalizedHelpInformationStringID (void) const { return Localizer::LocalizedStringID_TogglesAutowppointing; }

		inline const bool                           IsCanBeUsedOnDedicatedServer        (void) const { return false; }
		inline const bool                           IsShouldUsedInGame                  (void) const { return true; }	// Whether we need to be in game or not. (Used only on listen server)
};

	//
	// Group: (Con/De)structors.
	//
	public:
		inline CommandContainer_Waypoint (void) : CommandContainer ("WP/Waypoint", CMD_ACCESS_WAYPOINT)
		{
			m_commands += new Command_Waypoint_Draw ();
			m_commands += new Command_Waypoint_Add ();
			m_commands += new Command_Waypoint_ToggleAutowppointing ();
		}

	//
	// Group: Functions.
	//
	public:
		inline const Localizer::LocalizedStringID_t GetLocalizedHelpInformationStringID (void) const { return Localizer::LocalizedStringID_DisplaysWaypointConsoleCommandsContainer; }

		inline const bool                           IsCanBeUsedOnDedicatedServer        (void) const { return false; }
		inline const bool                           IsShouldUsedInGame                  (void) const { return true; }	// Whether we need to be in game or not. (Used only on listen server)
};

class CommandContainer_NavigationMesh : public CommandContainer
{
class Command_NavigationMesh_Generate : public Command
{
	//
	// Group: (Con/De)structors.
	//
	public:
		inline Command_NavigationMesh_Generate (void) : Command ("gen/build/generate", CMD_ACCESS_WAYPOINT) { /* VOID */ }

	//
	// Group: Functions.
	//
	public:
		// execute command
		inline void                                 Execute                             (Human *const human, const DynamicString &commandName, const DynamicString &argument1, const DynamicString &argument2, const DynamicString &argument3, const DynamicString &argument4, const DynamicString &argument5) const
		{
			g_server->GetNavigationMeshManager ().BeginGeneration ();
		}

		inline const Localizer::LocalizedStringID_t GetLocalizedHelpInformationStringID (void) const { return Localizer::LocalizedStringID_NavigationMesh_Generate; }

		inline const bool                           IsCanBeUsedOnDedicatedServer        (void) const { return true; }
		inline const bool                           IsShouldUsedInGame                  (void) const { return true; }	// Whether we need to be in game or not. (Used only on listen server)
};
class Command_NavigationMesh_GenerateIncremental : public Command
{
	//
	// Group: (Con/De)structors.
	//
	public:
		inline Command_NavigationMesh_GenerateIncremental (void) : Command ("generate_incremental", CMD_ACCESS_WAYPOINT) { /* VOID */ }

	//
	// Group: Functions.
	//
	public:
		// execute command
		inline void                                 Execute                             (Human *const human, const DynamicString &commandName, const DynamicString &argument1, const DynamicString &argument2, const DynamicString &argument3, const DynamicString &argument4, const DynamicString &argument5) const
		{
			g_server->GetNavigationMeshManager ().BeginGeneration (true);
		}

		inline const Localizer::LocalizedStringID_t GetLocalizedHelpInformationStringID (void) const { return Localizer::LocalizedStringID_TODO; }

		inline const bool                           IsCanBeUsedOnDedicatedServer        (void) const { return true; }
		inline const bool                           IsShouldUsedInGame                  (void) const { return true; }	// Whether we need to be in game or not. (Used only on listen server)
};
class Command_NavigationMesh_SaveAsOBJ : public Command
{
	//
	// Group: (Con/De)structors.
	//
	public:
		inline Command_NavigationMesh_SaveAsOBJ (void) : Command ("saveAsOBJ/toOBJ", CMD_ACCESS_WAYPOINT) { /* VOID */ }

	//
	// Group: Functions.
	//
	public:
		// execute command
		inline void                                 Execute                             (Human *const human, const DynamicString &commandName, const DynamicString &argument1, const DynamicString &argument2, const DynamicString &argument3, const DynamicString &argument4, const DynamicString &argument5) const
		{
			const DynamicString filename (HalfLifeEngine::Globals::g_halfLifeEngine->GetGameModName () + "/Addons/YaPB/Navigation mesh/OBJs/" + HalfLifeEngine::Globals::g_halfLifeEngine->GetMapName () + ".OBJ");

			if (g_server->GetNavigationMeshManager ().SaveToOBJFile ())
				AddLogEntry (true, LogLevel_Default, false, "Navigation map '%s' saved.", filename.GetData ());
			else
				AddLogEntry (true, LogLevel_Error, false, "Cannot save navigation map '%s'.", filename.GetData ());
		}

		inline const Localizer::LocalizedStringID_t GetLocalizedHelpInformationStringID (void) const { return Localizer::LocalizedStringID_NavigationMesh_Save; }

		inline const bool                           IsCanBeUsedOnDedicatedServer        (void) const { return true; }
		inline const bool                           IsShouldUsedInGame                  (void) const { return true; }	// Whether we need to be in game or not. (Used only on listen server)
};
class Command_NavigationMesh_Save : public Command
{
	//
	// Group: (Con/De)structors.
	//
	public:
		inline Command_NavigationMesh_Save (void) : Command ("save", CMD_ACCESS_WAYPOINT) { /* VOID */ }

	//
	// Group: Functions.
	//
	public:
		// execute command
		inline void                                 Execute                             (Human *const human, const DynamicString &commandName, const DynamicString &argument1, const DynamicString &argument2, const DynamicString &argument3, const DynamicString &argument4, const DynamicString &argument5) const
		{
			if (g_server->GetNavigationMeshManager ().Save ())
				AddLogEntry (true, LogLevel_Default, false, "Navigation map '%s' saved.", g_server->GetNavigationMeshManager ().GetFilename ().GetData ());
			else
				AddLogEntry (true, LogLevel_Error, false, "Cannot save navigation map '%s'.", g_server->GetNavigationMeshManager ().GetFilename ().GetData ());
		}

		inline const Localizer::LocalizedStringID_t GetLocalizedHelpInformationStringID (void) const { return Localizer::LocalizedStringID_NavigationMesh_Save; }

		inline const bool                           IsCanBeUsedOnDedicatedServer        (void) const { return true; }
		inline const bool                           IsShouldUsedInGame                  (void) const { return true; }	// Whether we need to be in game or not. (Used only on listen server)
};
class Command_NavigationMesh_Load : public Command
{
	//
	// Group: (Con/De)structors.
	//
	public:
		inline Command_NavigationMesh_Load (void) : Command ("load", CMD_ACCESS_WAYPOINT) { /* VOID */ }

	//
	// Group: Functions.
	//
	public:
		// execute command
		inline void                                 Execute                             (Human *const human, const DynamicString &commandName, const DynamicString &argument1, const DynamicString &argument2, const DynamicString &argument3, const DynamicString &argument4, const DynamicString &argument5) const
		{
			if (g_server->GetNavigationMeshManager ().Load () != NavigationMesh::NAV_OK)
				AddLogEntry (true, LogLevel_Error, false, "Navigation Mesh load failed.");
		}

		inline const Localizer::LocalizedStringID_t GetLocalizedHelpInformationStringID (void) const { return Localizer::LocalizedStringID_NavigationMesh_Load; }

		inline const bool                           IsCanBeUsedOnDedicatedServer        (void) const { return true; }
		inline const bool                           IsShouldUsedInGame                  (void) const { return true; }	// Whether we need to be in game or not. (Used only on listen server)
};
class Command_NavigationMesh_ReloadPlaceDatabase : public Command
{
	//
	// Group: (Con/De)structors.
	//
	public:
		inline Command_NavigationMesh_ReloadPlaceDatabase (void) : Command ("reload_place_database", CMD_ACCESS_WAYPOINT) { /* VOID */ }

	//
	// Group: Functions.
	//
	public:
		// execute command
		inline void                                 Execute                             (Human *const human, const DynamicString &commandName, const DynamicString &argument1, const DynamicString &argument2, const DynamicString &argument3, const DynamicString &argument4, const DynamicString &argument5) const
		{
			g_server->GetNavigationMeshManager ().GetPlaceContainer ().ReloadPlaceDatabase ();
		}

		inline const Localizer::LocalizedStringID_t GetLocalizedHelpInformationStringID (void) const { return Localizer::LocalizedStringID_NavigationMesh_ReloadPlaceDatabase; }

		inline const bool                           IsCanBeUsedOnDedicatedServer        (void) const { return true; }
		inline const bool                           IsShouldUsedInGame                  (void) const { return true; }	// Whether we need to be in game or not. (Used only on listen server)
};
class Command_NavigationMesh_PrintPlaceList : public Command
{
	//
	// Group: (Con/De)structors.
	//
	public:
		inline Command_NavigationMesh_PrintPlaceList (void) : Command ("print_place_list", CMD_ACCESS_WAYPOINT) { /* VOID */ }

	//
	// Group: Functions.
	//
	public:
		// execute command
		inline void                                 Execute                             (Human *const human, const DynamicString &commandName, const DynamicString &argument1, const DynamicString &argument2, const DynamicString &argument3, const DynamicString &argument4, const DynamicString &argument5) const
		{
			DynamicArray <NavigationMesh::Place_t> placeDirectory;

			for (NavigationMesh::NavAreaArray::IndexType_t index (0u); index < g_server->GetNavigationMeshManager ().GetAreas ().GetElementNumber (); ++index)
			{
				const NavigationMesh::Place_t place (g_server->GetNavigationMeshManager ().GetAreas ()[index]->GetPlace ());

				if (place > NavigationMesh::UNDEFINED_PLACE && !placeDirectory.IsMember (place))
					placeDirectory += place;
			}

			AddLogEntry (true, LogLevel_Default, false, "Map uses %u place names:", placeDirectory.GetElementNumber ());

			for (unsigned short index (0u); index < placeDirectory.GetElementNumber (); ++index)
				AddLogEntry (true, LogLevel_Default, false, "\t%s.", g_server->GetNavigationMeshManager ().GetPlaceContainer ().PlaceToName (placeDirectory[index]));
		}

		inline const Localizer::LocalizedStringID_t GetLocalizedHelpInformationStringID (void) const { return Localizer::LocalizedStringID_NavigationMesh_PrintPlaceList; }

		inline const bool                           IsCanBeUsedOnDedicatedServer        (void) const { return true; }
		inline const bool                           IsShouldUsedInGame                  (void) const { return true; }	// Whether we need to be in game or not. (Used only on listen server)
};
class Command_NavigationMesh_TogglePlaceMode : public Command
{
	//
	// Group: (Con/De)structors.
	//
	public:
		inline Command_NavigationMesh_TogglePlaceMode (void) : Command ("toggle_place_mode", CMD_ACCESS_WAYPOINT) { /* VOID */ }

	//
	// Group: Functions.
	//
	public:
		// execute command
		inline void                                 Execute                             (Human *const human, const DynamicString &commandName, const DynamicString &argument1, const DynamicString &argument2, const DynamicString &argument3, const DynamicString &argument4, const DynamicString &argument5) const
		{
			g_server->GetNavigationMeshManager ().CommandNavTogglePlaceMode ();
		}

		inline const Localizer::LocalizedStringID_t GetLocalizedHelpInformationStringID (void) const { return Localizer::LocalizedStringID_NavigationMesh_TogglePlaceMode; }

		inline const bool                           IsCanBeUsedOnDedicatedServer        (void) const { return false; }
		inline const bool                           IsShouldUsedInGame                  (void) const { return true; }	// Whether we need to be in game or not. (Used only on listen server)
};
class Command_NavigationMesh_MakeSniperSpots : public Command
{
	//
	// Group: (Con/De)structors.
	//
	public:
		inline Command_NavigationMesh_MakeSniperSpots (void) : Command ("make_sniper_spots", CMD_ACCESS_WAYPOINT) { /* VOID */ }

	//
	// Group: Functions.
	//
	public:
		// execute command
		inline void                                 Execute                             (Human *const human, const DynamicString &commandName, const DynamicString &argument1, const DynamicString &argument2, const DynamicString &argument3, const DynamicString &argument4, const DynamicString &argument5) const
		{
			g_server->GetNavigationMeshManager ().CommandNavMakeSniperSpots ();
		}

		inline const Localizer::LocalizedStringID_t GetLocalizedHelpInformationStringID (void) const { return Localizer::LocalizedStringID_NavigationMesh_MakeSniperSpots; }

		inline const bool                           IsCanBeUsedOnDedicatedServer        (void) const { return true; }
		inline const bool                           IsShouldUsedInGame                  (void) const { return true; }	// Whether we need to be in game or not. (Used only on listen server)
};
class Command_NavigationMesh_Transient : public Command
{
	//
	// Group: (Con/De)structors.
	//
	public:
		inline Command_NavigationMesh_Transient (void) : Command ("transient", CMD_ACCESS_WAYPOINT) { /* VOID */ }

	//
	// Group: Functions.
	//
	public:
		// execute command
		inline void                                 Execute                             (Human *const human, const DynamicString &commandName, const DynamicString &argument1, const DynamicString &argument2, const DynamicString &argument3, const DynamicString &argument4, const DynamicString &argument5) const
		{
			g_server->GetNavigationMeshManager ().CommandNavToggleAttribute (NavigationMesh::NAV_MESH_TRANSIENT);
		}

		inline const Localizer::LocalizedStringID_t GetLocalizedHelpInformationStringID (void) const { return Localizer::LocalizedStringID_NavigationMesh_Transient; }

		inline const bool                           IsCanBeUsedOnDedicatedServer        (void) const { return false; }
		inline const bool                           IsShouldUsedInGame                  (void) const { return true; }	// Whether we need to be in game or not. (Used only on listen server)
};
class Command_NavigationMesh_Delete : public Command
{
	//
	// Group: (Con/De)structors.
	//
	public:
		inline Command_NavigationMesh_Delete (void) : Command ("delete", CMD_ACCESS_WAYPOINT) { /* VOID */ }

	//
	// Group: Functions.
	//
	public:
		// execute command
		inline void                                 Execute                             (Human *const human, const DynamicString &commandName, const DynamicString &argument1, const DynamicString &argument2, const DynamicString &argument3, const DynamicString &argument4, const DynamicString &argument5) const
		{
			g_server->GetNavigationMeshManager ().CommandNavDelete ();
		}

		inline const Localizer::LocalizedStringID_t GetLocalizedHelpInformationStringID (void) const { return Localizer::LocalizedStringID_NavigationMesh_Delete; }

		inline const bool                           IsCanBeUsedOnDedicatedServer        (void) const { return false; }
		inline const bool                           IsShouldUsedInGame                  (void) const { return true; }	// Whether we need to be in game or not. (Used only on listen server)
};
class Command_NavigationMesh_TogglePlacePainting : public Command
{
	//
	// Group: (Con/De)structors.
	//
	public:
		inline Command_NavigationMesh_TogglePlacePainting (void) : Command ("toggle_place_painting", CMD_ACCESS_WAYPOINT) { /* VOID */ }

	//
	// Group: Functions.
	//
	public:
		// execute command
		inline void                                 Execute                             (Human *const human, const DynamicString &commandName, const DynamicString &argument1, const DynamicString &argument2, const DynamicString &argument3, const DynamicString &argument4, const DynamicString &argument5) const
		{
			g_server->GetNavigationMeshManager ().CommandNavTogglePlacePainting ();
		}

		inline const Localizer::LocalizedStringID_t GetLocalizedHelpInformationStringID (void) const { return Localizer::LocalizedStringID_NavigationMesh_TogglePlacePainting; }

		inline const bool                           IsCanBeUsedOnDedicatedServer        (void) const { return false; }
		inline const bool                           IsShouldUsedInGame                  (void) const { return true; }	// Whether we need to be in game or not. (Used only on listen server)
};
class Command_NavigationMesh_PrintAllPlaces : public Command
{
	//
	// Group: (Con/De)structors.
	//
	public:
		inline Command_NavigationMesh_PrintAllPlaces (void) : Command ("print_all_places", CMD_ACCESS_WAYPOINT) { /* VOID */ }

	//
	// Group: Functions.
	//
	public:
		// execute command
		inline void                                 Execute                             (Human *const human, const DynamicString &commandName, const DynamicString &argument1, const DynamicString &argument2, const DynamicString &argument3, const DynamicString &argument4, const DynamicString &argument5) const
		{
			g_server->GetNavigationMeshManager ().GetPlaceContainer ().PrintAllPlaces ();
		}

		inline const Localizer::LocalizedStringID_t GetLocalizedHelpInformationStringID (void) const { return Localizer::LocalizedStringID_NavigationMesh_PrintAllPlaces; }

		inline const bool                           IsCanBeUsedOnDedicatedServer        (void) const { return true; }
		inline const bool                           IsShouldUsedInGame                  (void) const { return true; }	// Whether we need to be in game or not. (Used only on listen server)
};
class Command_NavigationMesh_UsePlace : public Command
{
	//
	// Group: (Con/De)structors.
	//
	public:
		inline Command_NavigationMesh_UsePlace (void) : Command ("use_place", CMD_ACCESS_WAYPOINT) { /* VOID */ }

	//
	// Group: Functions.
	//
	public:
		// execute command
		inline void                                 Execute                             (Human *const human, const DynamicString &commandName, const DynamicString &argument1, const DynamicString &argument2, const DynamicString &argument3, const DynamicString &argument4, const DynamicString &argument5) const
		{
			if (commandName.IsEmpty ())
			{
				// no arguments = list all available places
				g_server->GetNavigationMeshManager ().GetPlaceContainer ().PrintAllPlaces ();
			}
			else
			{
				// single argument = set current place
				const NavigationMesh::Place_t place (g_server->GetNavigationMeshManager ().GetPlaceContainer ().PartialNameToPlace (commandName));

				if (place == NavigationMesh::UNDEFINED_PLACE)
					AddLogEntry (true, LogLevel_Default, false, "Ambiguous.");
				else
				{
					AddLogEntry (true, LogLevel_Default, false, "Current place set to '%s'.", g_server->GetNavigationMeshManager ().GetPlaceContainer ().PlaceToName (place).GetData ());

					g_server->GetNavigationMeshManager ().SetNavPlace (place);
				}
			}
		}

		inline const Localizer::LocalizedStringID_t GetLocalizedHelpInformationStringID (void) const { return Localizer::LocalizedStringID_NavigationMesh_UsePlace; }

		inline const bool                           IsCanBeUsedOnDedicatedServer        (void) const { return false; }
		inline const bool                           IsShouldUsedInGame                  (void) const { return true; }	// Whether we need to be in game or not. (Used only on listen server)
};
class Command_NavigationMesh_PlaceReplace : public Command
{
	//
	// Group: (Con/De)structors.
	//
	public:
		inline Command_NavigationMesh_PlaceReplace (void) : Command ("place_replace", CMD_ACCESS_WAYPOINT) { /* VOID */ }

	//
	// Group: Functions.
	//
	public:
		// execute command
		inline void                                 Execute                             (Human *const human, const DynamicString &commandName, const DynamicString &argument1, const DynamicString &argument2, const DynamicString &argument3, const DynamicString &argument4, const DynamicString &argument5) const
		{
			if (argument1.IsEmpty ())
			{
				// no arguments
				AddLogEntry (true, LogLevel_Default, false, "Usage: yb navmesh place_replace <OldPlace> <NewPlace>.");
			}
			else
			{
				// two arguments - replace the first place with the second
				const NavigationMesh::Place_t oldPlace (g_server->GetNavigationMeshManager ().GetPlaceContainer ().PartialNameToPlace (commandName));
				const NavigationMesh::Place_t newPlace (g_server->GetNavigationMeshManager ().GetPlaceContainer ().PartialNameToPlace (argument1));

				if (oldPlace == NavigationMesh::UNDEFINED_PLACE || newPlace == NavigationMesh::UNDEFINED_PLACE)
					AddLogEntry (true, LogLevel_Default, false, "Ambiguous.");
				else for (NavigationMesh::NavAreaArray::IndexType_t index (0u); index < g_server->GetNavigationMeshManager ().GetAreas ().GetElementNumber (); ++index)
				{
					NavigationMesh::NavigationArea *const area (g_server->GetNavigationMeshManager ().GetAreas ()[index]);

					if (area->GetPlace () == oldPlace)
						area->SetPlace (newPlace);
				}
			}
		}

		inline const Localizer::LocalizedStringID_t GetLocalizedHelpInformationStringID (void) const { return Localizer::LocalizedStringID_NavigationMesh_PlaceReplace; }

		inline const bool                           IsCanBeUsedOnDedicatedServer        (void) const { return true; }
		inline const bool                           IsShouldUsedInGame                  (void) const { return true; }	// Whether we need to be in game or not. (Used only on listen server)
};
class Command_NavigationMesh_Analyze : public Command
{
	//
	// Group: (Con/De)structors.
	//
	public:
		inline Command_NavigationMesh_Analyze (void) : Command ("analyze", CMD_ACCESS_WAYPOINT) { /* VOID */ }

	//
	// Group: Functions.
	//
	public:
		// execute command
		inline void                                 Execute                             (Human *const human, const DynamicString &commandName, const DynamicString &argument1, const DynamicString &argument2, const DynamicString &argument3, const DynamicString &argument4, const DynamicString &argument5) const
		{
			g_server->GetNavigationMeshManager ().BeginAnalysis ();
		}

		inline const Localizer::LocalizedStringID_t GetLocalizedHelpInformationStringID (void) const { return Localizer::LocalizedStringID_NavigationMesh_Analyze; }

		inline const bool                           IsCanBeUsedOnDedicatedServer        (void) const { return true; }
		inline const bool                           IsShouldUsedInGame                  (void) const { return true; }	// Whether we need to be in game or not. (Used only on listen server)
};
class Command_NavigationMesh_Mark : public Command
{
	//
	// Group: (Con/De)structors.
	//
	public:
		inline Command_NavigationMesh_Mark (void) : Command ("mark/cache", CMD_ACCESS_WAYPOINT) { /* VOID */ }

	//
	// Group: Functions.
	//
	public:
		// execute command
		inline void                                 Execute                             (Human *const human, const DynamicString &commandName, const DynamicString &argument1, const DynamicString &argument2, const DynamicString &argument3, const DynamicString &argument4, const DynamicString &argument5) const
		{
			g_server->GetNavigationMeshManager ().CommandNavMark ();
		}

		inline const Localizer::LocalizedStringID_t GetLocalizedHelpInformationStringID (void) const { return Localizer::LocalizedStringID_NavigationMesh_Mark; }

		inline const bool                           IsCanBeUsedOnDedicatedServer        (void) const { return false; }
		inline const bool                           IsShouldUsedInGame                  (void) const { return true; }	// Whether we need to be in game or not. (Used only on listen server)
};
class Command_NavigationMesh_Draw : public Command
{
	//
	// Group: (Con/De)structors.
	//
	public:
		inline Command_NavigationMesh_Draw (void) : Command ("draw/show", CMD_ACCESS_WAYPOINT) { /* VOID */ }

	//
	// Group: Functions.
	//
	public:
		// execute command
		inline void                                 Execute                             (Human *const human, const DynamicString &commandName, const DynamicString &argument1, const DynamicString &argument2, const DynamicString &argument3, const DynamicString &argument4, const DynamicString &argument5) const
		{
			if (commandName.IsEmpty () || commandName.CompareWithoutCaseCheck ("toggle") == 0)
			{
				Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_nav_edit)->SetValue <bool> (Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_nav_edit)->GetValue <bool> () ^ true);

				HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat ("Navigation Mesh Editing %s.\n", Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_nav_edit)->GetValue <bool> () ? "Enabled" : "Disabled");
			}
			else if (commandName == "1" || commandName.CompareWithoutCaseCheck ("on") == 0 || commandName.CompareWithoutCaseCheck ("enable") == 0)
			{
				Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_nav_edit)->SetValue <bool> (true);

				HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrint ("Navigation Mesh Editing Enabled.\n");
			}
			else if (commandName == "0" || commandName.CompareWithoutCaseCheck ("off") == 0 || commandName.CompareWithoutCaseCheck ("disable") == 0)
			{
				Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_nav_edit)->SetValue <bool> (false);

				HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrint ("Navigation Mesh Editing Disabled.\n");
			}
			else
				human->Print (HalfLifeEngine::SDK::Constants::PrintType_AtConsole, "Error: Unknown state!");
		}

		inline const Localizer::LocalizedStringID_t GetLocalizedHelpInformationStringID (void) const { return Localizer::LocalizedStringID_NavigationMesh_Draw; }

		inline const bool                           IsCanBeUsedOnDedicatedServer        (void) const { return false; }
		inline const bool                           IsShouldUsedInGame                  (void) const { return true; }	// Whether we need to be in game or not. (Used only on listen server)
};
class Command_NavigationMesh_PrintBSPEntities : public Command
{
	//
	// Group: (Con/De)structors.
	//
	public:
		inline Command_NavigationMesh_PrintBSPEntities (void) : Command ("print_bsp_entities", CMD_ACCESS_WAYPOINT) { /* VOID */ }

	//
	// Group: Functions.
	//
	public:
		// execute command
		inline void                                 Execute                             (Human *const human, const DynamicString &commandName, const DynamicString &argument1, const DynamicString &argument2, const DynamicString &argument3, const DynamicString &argument4, const DynamicString &argument5) const
		{
			g_server->GetNavigationMeshManager ().GetWorldMap ().PrintEntities ();
		}

		inline const Localizer::LocalizedStringID_t GetLocalizedHelpInformationStringID (void) const { return Localizer::LocalizedStringID_NavigationMesh_PrintBSPEntities; }

		inline const bool                           IsCanBeUsedOnDedicatedServer        (void) const { return true; }
		inline const bool                           IsShouldUsedInGame                  (void) const { return true; }	// Whether we need to be in game or not. (Used only on listen server)
};
class Command_NavigationMesh_CreateWalkFacesBMPFile : public Command
{
	//
	// Group: (Con/De)structors.
	//
	public:
		inline Command_NavigationMesh_CreateWalkFacesBMPFile (void) : Command ("create_walk_faces_BMP_file", CMD_ACCESS_WAYPOINT) { /* VOID */ }

	//
	// Group: Functions.
	//
	public:
		// execute command
		inline void                                 Execute                             (Human *const human, const DynamicString &commandName, const DynamicString &argument1, const DynamicString &argument2, const DynamicString &argument3, const DynamicString &argument4, const DynamicString &argument5) const
		{
			g_server->GetNavigationMeshManager ().GetWorldMap ().WriteDebugBitmap ();
		}

		inline const Localizer::LocalizedStringID_t GetLocalizedHelpInformationStringID (void) const { return Localizer::LocalizedStringID_NavigationMesh_CreateWalkFacesBMPFile; }

		inline const bool                           IsCanBeUsedOnDedicatedServer        (void) const { return true; }
		inline const bool                           IsShouldUsedInGame                  (void) const { return true; }	// Whether we need to be in game or not. (Used only on listen server)
};
class Command_NavigationMesh_CreateNavigationAreasBMPFile : public Command
{
	//
	// Group: (Con/De)structors.
	//
	public:
		inline Command_NavigationMesh_CreateNavigationAreasBMPFile (void) : Command ("create_navigation_areas_BMP_file", CMD_ACCESS_WAYPOINT) { /* VOID */ }

	//
	// Group: Functions.
	//
	public:
		// execute command
		inline void                                 Execute                             (Human *const human, const DynamicString &commandName, const DynamicString &argument1, const DynamicString &argument2, const DynamicString &argument3, const DynamicString &argument4, const DynamicString &argument5) const
		{
			g_server->GetNavigationMeshManager ().WriteNavigationAreasBitmap (commandName.IsEmpty () ? HalfLifeEngine::Globals::g_halfLifeEngine->GetGameModName () + "/Addons/YaPB/Navigation mesh/" + HalfLifeEngine::Globals::g_halfLifeEngine->GetMapName () + "-Navigation areas.BMP" : commandName);
		}

		inline const Localizer::LocalizedStringID_t GetLocalizedHelpInformationStringID (void) const { return Localizer::LocalizedStringID_NavigationMesh_CreateNavigationAreasBMPFile; }

		inline const bool                           IsCanBeUsedOnDedicatedServer        (void) const { return true; }
		inline const bool                           IsShouldUsedInGame                  (void) const { return true; }	// Whether we need to be in game or not. (Used only on listen server)
};

	//
	// Group: (Con/De)structors.
	//
	public:
		inline CommandContainer_NavigationMesh (void) : CommandContainer ("navmesh/navigationmesh/navigation_mesh", CMD_ACCESS_WAYPOINT)
		{
			m_commands += new Command_NavigationMesh_Generate ();
			m_commands += new Command_NavigationMesh_GenerateIncremental ();
			m_commands += new Command_NavigationMesh_SaveAsOBJ ();
			m_commands += new Command_NavigationMesh_Save ();
			m_commands += new Command_NavigationMesh_Load ();
			m_commands += new Command_NavigationMesh_ReloadPlaceDatabase ();
			m_commands += new Command_NavigationMesh_PrintPlaceList ();
			m_commands += new Command_NavigationMesh_TogglePlaceMode ();
			m_commands += new Command_NavigationMesh_MakeSniperSpots ();
			m_commands += new Command_NavigationMesh_Transient ();
			m_commands += new Command_NavigationMesh_Delete ();
			m_commands += new Command_NavigationMesh_TogglePlacePainting ();
			m_commands += new Command_NavigationMesh_PrintAllPlaces ();
			m_commands += new Command_NavigationMesh_UsePlace ();
			m_commands += new Command_NavigationMesh_PlaceReplace ();
			m_commands += new Command_NavigationMesh_Analyze ();
			m_commands += new Command_NavigationMesh_Mark ();
			m_commands += new Command_NavigationMesh_Draw ();
			m_commands += new Command_NavigationMesh_PrintBSPEntities ();
			m_commands += new Command_NavigationMesh_CreateWalkFacesBMPFile ();
			m_commands += new Command_NavigationMesh_CreateNavigationAreasBMPFile ();
		}

	//
	// Group: Functions.
	//
	public:
		inline const Localizer::LocalizedStringID_t GetLocalizedHelpInformationStringID (void) const { return Localizer::LocalizedStringID_NavigationMesh_DisplaysConsoleCommandsContainer; }

		inline const bool                           IsCanBeUsedOnDedicatedServer        (void) const { return true; }
		inline const bool                           IsShouldUsedInGame                  (void) const { return true; }	// Whether we need to be in game or not. (Used only on listen server)
};

class CommandContainer_Utilities : public CommandContainer
{
class Command_Utilities_SetHealth : public Command
{
	//
	// Group: (Con/De)structors.
	//
	public:
		inline Command_Utilities_SetHealth (void) : Command ("set_hp/set_health", CMD_ACCESS_UTIL) { /* VOID */ }

	//
	// Group: Functions.
	//
	public:
		// execute command
		inline void                                 Execute                             (Human *const human, const DynamicString &commandName, const DynamicString &argument1, const DynamicString &argument2, const DynamicString &argument3, const DynamicString &argument4, const DynamicString &argument5) const
		{
			if (commandName.IsEmpty ())
			{
				if (human != NULL)
					human->Print (HalfLifeEngine::SDK::Constants::PrintType_AtConsole, human->GetLocalizer ()->GetLocalizedString (Localizer::LocalizedStringID_PleaseSpecifyHealth) + '\n');
				else
					HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrint (g_localizer.GetLocalizedString (Localizer::LocalizedStringID_PleaseSpecifyHealth) + '\n');

				return;
			}

			if (g_server->GetYaPBManager ()->GetBotsCount () == 0u)
			{
				if (human != NULL)
					human->Print (HalfLifeEngine::SDK::Constants::PrintType_AtConsole, human->GetLocalizer ()->GetLocalizedString (Localizer::LocalizedStringID_ThereAreNoYaPBOnTheServer) + '\n');
				else
					HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrint (g_localizer.GetLocalizedString (Localizer::LocalizedStringID_ThereAreNoYaPBOnTheServer) + '\n');

				return;
			}

			// Without name?
			if (argument1.IsEmpty ())
			{
				g_server->GetYaPBManager ()->SetHealth (static_cast <unsigned short> (abs (commandName.GetValue <int> ())));

				return;
			}

			/// @TODO: For specified bot.
		}

		inline const Localizer::LocalizedStringID_t GetLocalizedHelpInformationStringID (void) const { return Localizer::LocalizedStringID_TODO; }

		inline const bool                           IsCanBeUsedOnDedicatedServer        (void) const { return true; }
		inline const bool                           IsShouldUsedInGame                  (void) const { return true; }	// Whether we need to be in game or not. (Used only on listen server)
};
class Command_Utilities_SetMoney : public Command
{
	//
	// Group: (Con/De)structors.
	//
	public:
		inline Command_Utilities_SetMoney (void) : Command ("setmoney/set_money", CMD_ACCESS_UTIL) { /* VOID */ }

	//
	// Group: Functions.
	//
	public:
		// execute command
		inline void                                 Execute                             (Human *const human, const DynamicString &commandName, const DynamicString &argument1, const DynamicString &argument2, const DynamicString &argument3, const DynamicString &argument4, const DynamicString &argument5) const
		{
			if (commandName.IsEmpty ())
			{
				if (human != NULL)
					human->Print (HalfLifeEngine::SDK::Constants::PrintType_AtConsole, human->GetLocalizer ()->GetLocalizedString (Localizer::LocalizedStringID_PleaseSpecifyHealth) + '\n');
				else
					HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrint (g_localizer.GetLocalizedString (Localizer::LocalizedStringID_PleaseSpecifyHealth) + '\n');

				return;
			}

			if (g_server->GetYaPBManager ()->GetBotsCount () == 0u)
			{
				if (human != NULL)
					human->Print (HalfLifeEngine::SDK::Constants::PrintType_AtConsole, human->GetLocalizer ()->GetLocalizedString (Localizer::LocalizedStringID_ThereAreNoYaPBOnTheServer) + '\n');
				else
					HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrint (g_localizer.GetLocalizedString (Localizer::LocalizedStringID_ThereAreNoYaPBOnTheServer) + '\n');

				return;
			}

			// Without name?
			if (argument1.IsEmpty ())
			{
				g_server->GetYaPBManager ()->SetMoney (static_cast <const unsigned short> (Math::GetClampedValueInRange <int> (abs (commandName.GetValue <int> ()), 0, HalfLifeEngine::SDK::Constants::PlayerMaximumMoneyAmount)));

				return;
			}

			/// @TODO: For specified bot.
		}

		inline const Localizer::LocalizedStringID_t GetLocalizedHelpInformationStringID (void) const { return Localizer::LocalizedStringID_TODO; }

		inline const bool                           IsCanBeUsedOnDedicatedServer        (void) const { return true; }
		inline const bool                           IsShouldUsedInGame                  (void) const { return true; }	// Whether we need to be in game or not. (Used only on listen server)
};
class Command_Utilities_SetTeam : public Command
{
	//
	// Group: (Con/De)structors.
	//
	public:
		inline Command_Utilities_SetTeam (void) : Command ("set_team", CMD_ACCESS_UTIL) { /* VOID */ }

	//
	// Group: Functions.
	//
	public:
		// execute command
		inline void                                 Execute                             (Human *const human, const DynamicString &commandName, const DynamicString &argument1, const DynamicString &argument2, const DynamicString &argument3, const DynamicString &argument4, const DynamicString &argument5) const
		{
			const HalfLifeEngine::SDK::Constants::TeamID_t newTeam (argument1.GetValue <HalfLifeEngine::SDK::Constants::TeamID_t> ());

			// Without name?
			if (commandName.IsEmpty ())
			{
				g_server->GetClientManager ()->SetTeam (newTeam);

				return;
			}

			Client *const client (g_server->GetClientManager ()->GetClientByString (commandName));

			if (client == NULL)
			{
				if (human != NULL)
					human->Print (HalfLifeEngine::SDK::Constants::PrintType_AtConsole, human->GetLocalizer ()->GetLocalizedString (Localizer::LocalizedStringID_BotWithThatNameNotFound) + '\n');
				else
					HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrint (g_localizer.GetLocalizedString (Localizer::LocalizedStringID_BotWithThatNameNotFound) + '\n');

				return;
			}

			client->SetTeam (newTeam);

//			PrintFormat (HalfLifeEngine::SDK::Constants::PrintType_AtConsole, g_localizer.GetLocalizedString (Localizer::LocalizedStringID_BotHasExecutedCommand) + '\n', client->GetName ().GetData (), newTeam);
		}

		inline const Localizer::LocalizedStringID_t GetLocalizedHelpInformationStringID (void) const { return Localizer::LocalizedStringID_TODO; }

		inline const bool                           IsCanBeUsedOnDedicatedServer        (void) const { return true; }
		inline const bool                           IsShouldUsedInGame                  (void) const { return true; }	// Whether we need to be in game or not. (Used only on listen server)
};
class Command_Utilities_PrintCurrentTime : public Command
{
	//
	// Group: (Con/De)structors.
	//
	public:
		inline Command_Utilities_PrintCurrentTime (void) : Command ("time/ctime/printtime/print_current_time", CMD_ACCESS_UTIL) { /* VOID */ }

	//
	// Group: Functions.
	//
	public:
		// execute command
		inline void                                 Execute                             (Human *const human, const DynamicString &commandName, const DynamicString &argument1, const DynamicString &argument2, const DynamicString &argument3, const DynamicString &argument4, const DynamicString &argument5) const
		{
			const tm *const localTime (GetLocalTime ());

			if (human != NULL)
				human->PrintFormat (HalfLifeEngine::SDK::Constants::PrintType_AtConsole, human->GetLocalizer ()->GetLocalizedString (Localizer::LocalizedStringID_CurrentTimeIs) + '\n', localTime->tm_hour, localTime->tm_min, localTime->tm_sec);
			else
				HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetLocalizedString (Localizer::LocalizedStringID_CurrentTimeIs) + '\n', localTime->tm_hour, localTime->tm_min, localTime->tm_sec);
		}

		inline const Localizer::LocalizedStringID_t GetLocalizedHelpInformationStringID (void) const { return Localizer::LocalizedStringID_TODO; }

		inline const bool                           IsCanBeUsedOnDedicatedServer        (void) const { return true; }
		inline const bool                           IsShouldUsedInGame                  (void) const { return true; }	// Whether we need to be in game or not. (Used only on listen server)
};
class Command_Utilities_PrintCurrentDate : public Command
{
	//
	// Group: (Con/De)structors.
	//
	public:
		inline Command_Utilities_PrintCurrentDate (void) : Command ("date/cdate/printdate/print_current_date", CMD_ACCESS_UTIL) { /* VOID */ }

	//
	// Group: Functions.
	//
	public:
		// execute command
		inline void                                 Execute                             (Human *const human, const DynamicString &commandName, const DynamicString &argument1, const DynamicString &argument2, const DynamicString &argument3, const DynamicString &argument4, const DynamicString &argument5) const
		{
			const tm *const localTime (GetLocalTime ());

			if (human != NULL)
				human->PrintFormat (HalfLifeEngine::SDK::Constants::PrintType_AtConsole, human->GetLocalizer ()->GetLocalizedString (Localizer::LocalizedStringID_CurrentDateIs) + '\n', localTime->tm_mday, localTime->tm_mon + 1u, localTime->tm_year + 1900u);
			else
				HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat (g_localizer.GetLocalizedString (Localizer::LocalizedStringID_CurrentDateIs) + '\n', localTime->tm_mday, localTime->tm_mon + 1u, localTime->tm_year + 1900u);
		}

		inline const Localizer::LocalizedStringID_t GetLocalizedHelpInformationStringID (void) const { return Localizer::LocalizedStringID_TODO; }

		inline const bool                           IsCanBeUsedOnDedicatedServer        (void) const { return true; }
		inline const bool                           IsShouldUsedInGame                  (void) const { return true; }	// Whether we need to be in game or not. (Used only on listen server)
};
class Command_Utilities_ToggleAdvancedHostageAI : public Command
{
	//
	// Group: (Con/De)structors.
	//
	public:
		inline Command_Utilities_ToggleAdvancedHostageAI (void) : Command ("toggle_advanced_hostage_AI", CMD_ACCESS_UTIL) { /* VOID */ }

	//
	// Group: Functions.
	//
	public:
		// execute command
		inline void                                 Execute                             (Human *const human, const DynamicString &commandName, const DynamicString &argument1, const DynamicString &argument2, const DynamicString &argument3, const DynamicString &argument4, const DynamicString &argument5) const
		{
			HalfLifeEngine::Utilities::ZBotInGameEnabler::ToggleAdvancedHostageAI ();
		}

		inline const Localizer::LocalizedStringID_t GetLocalizedHelpInformationStringID (void) const { return Localizer::LocalizedStringID_TODO; }

		inline const bool                           IsCanBeUsedOnDedicatedServer        (void) const { return true; }
		inline const bool                           IsShouldUsedInGame                  (void) const { return true; }	// Whether we need to be in game or not. (Used only on listen server)
};

	//
	// Group: (Con/De)structors.
	//
	public:
		inline CommandContainer_Utilities (void) : CommandContainer ("utils/utilities", CMD_ACCESS_UTIL)
		{
			m_commands += new Command_Utilities_SetHealth ();	// sets health for all available bots
			m_commands += new Command_Utilities_SetMoney ();	// sets money for all available bots
			m_commands += new Command_Utilities_SetTeam ();
			m_commands += new Command_Utilities_PrintCurrentTime ();
			m_commands += new Command_Utilities_PrintCurrentDate ();
			m_commands += new Command_Utilities_ToggleAdvancedHostageAI ();
		}

	//
	// Group: Functions.
	//
	public:
		inline const Localizer::LocalizedStringID_t GetLocalizedHelpInformationStringID (void) const { return Localizer::LocalizedStringID_TODO; }

		inline const bool                           IsCanBeUsedOnDedicatedServer        (void) const { return true; }
		inline const bool                           IsShouldUsedInGame                  (void) const { return true; }	// Whether we need to be in game or not. (Used only on listen server)
};

class CommandContainer_YaPB : public CommandContainer
{
	//
	// Group: (Con/De)structors.
	//
	public:
		inline CommandContainer_YaPB (void) : CommandContainer (PRODUCT_CONSOLE_TAG "/" PRODUCT_CONSOLE_TAG_OTHER, CMD_ACCESS_ALL)
		{
			m_commands += new CommandContainer_Menu ();

			m_commands += new Command_PrintMemoryUsage ();

			m_commands += new Command_AddBot ();
			m_commands += new Command_KickBot ();
//			m_commands += new Command_Bot_PossessBot ();	/// NOT WORK!!!

			#if defined _DEBUG
				m_commands += new Command_SetDebugBot ();
				m_commands += new Command_SetViewEntity ();
			#endif	// if defined _DEBUG

			m_commands += new CommandContainer_Profiles ();
			m_commands += new CommandContainer_Localizer ();

			m_commands += new CommandContainer_NavigationMesh ();
			m_commands += new CommandContainer_Waypoint ();
/*			m_commands += new Command_PathWaypoint ();
			m_commands += new Command_Debug ();
			m_commands += new PrintCommands ();
			m_commands += new Command_Config ();
			m_commands += new Command_Users ();*/
			m_commands += new CommandContainer_Utilities ();
		}

	//
	// Group: Functions.
	//
	public:
		inline const Localizer::LocalizedStringID_t GetLocalizedHelpInformationStringID (void) const { return Localizer::LocalizedStringID_DisplaysBotConsoleCommandsContainer; }

		inline const bool                           IsCanBeUsedOnDedicatedServer        (void) const { return true; }
		inline const bool                           IsShouldUsedInGame                  (void) const { return false; }	// Whether we need to be in game or not. (Used only on listen server)
};

class ConsoleCommandsManager
{
	//
	// Group: Private members.
	//
	private:
		CommandContainer_YaPB m_YaPBConsoleCommandsContainer;

	//
	// Group: Functions.
	//
	public:
		inline const bool                   IsYaPBCommand            (const DynamicString &commandName) const { return m_YaPBConsoleCommandsContainer.IsCommand (commandName); }
		inline const CommandContainer_YaPB &GetYaPBCommandsContainer (void)                             const { return m_YaPBConsoleCommandsContainer; }

	//
	// Group: Engine callbacks.
	//
	public:
		inline void HandleExecutedCommand (Human *const human, const DynamicString &commandName, const DynamicString &argument1, const DynamicString &argument2, const DynamicString &argument3, const DynamicString &argument4, const DynamicString &argument5) const
		{
			m_YaPBConsoleCommandsContainer.Execute (human, commandName, argument1, argument2, argument3, argument4, argument5);
		}
};

extern const ConsoleCommandsManager g_consoleCommandsManager;

#endif	// ifndef CONSOLE_COMMANDS_INCLUDED