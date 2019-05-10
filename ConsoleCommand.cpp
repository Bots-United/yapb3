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
// ConsoleCommand.cpp
//
// Class: Command
//
// Version: $ID:$
//

#include <Core.hpp>

const ConsoleCommandsManager g_consoleCommandsManager;

void CommandContainer::Execute (Human *const human, const DynamicString &commandName, const DynamicString &argument1, const DynamicString &argument2, const DynamicString &argument3, const DynamicString &argument4, const DynamicString &argument5) const
{
	// Reliability check.
	if (commandName.IsEmpty () || commandName[0u] == '\?' || commandName.CompareWithoutCaseCheck ("help") == 0)
	{
		PrintHelp (human);

		return;
	}

	for (CommandsArray_t::IndexType_t index (0u); index < m_commands.GetElementNumber (); ++index)
	{
		const Command *const command (m_commands[index]);

		if (!command->IsCommand (commandName))
			continue;

		// Is dedicated server or server is not running?
		if (human == NULL)
		{
			if (HalfLifeEngine::Globals::g_halfLifeEngine->IsDedicatedServer ())
			{
				if (!IsCanBeUsedOnDedicatedServer ())
				{
					AddLogEntry (true, LogLevel_Error, false, g_localizer.GetLocalizedString (Localizer::LocalizedStringID_ConsoleCommandCanOnlyBeExecutedFromServerConsole), commandName.GetData ());

					return;
				}
			}
			else if (command->IsShouldUsedInGame () > g_server->IsRunning ())	// Make sure the game has started if we need.
			{
				AddLogEntry (true, LogLevel_Error, false, g_localizer.GetLocalizedString (Localizer::LocalizedStringID_ConsoleCommandCanOnlyBeExecutedWhenServerIsRunning), commandName.GetData ());

				return;
			}
		}
		else
		{
			if (!command->IsHumanHasAccessLevel (human))
			{
				AddLogEntry (false, LogLevel_Error, false, g_localizer.GetLocalizedString (Localizer::LocalizedStringID_ClientHasExecutedConsoleVariableCanBeOnlySetViaRCONAccess), human->GetName ().GetData (), argument1.GetData ());
				human->PrintFormat (HalfLifeEngine::SDK::Constants::PrintType_AtConsole, human->GetLocalizer ()->GetLocalizedString (Localizer::LocalizedStringID_ConsoleVariableCanBeOnlySetViaRCONAccess) + '\n', argument1.GetData ());

				return;
			}

			// Make sure the game has started if we need.
			if (command->IsShouldUsedInGame () > g_server->IsRunning ())
			{
				if (human->IsHost ())
					AddLogEntry (true, LogLevel_Error, false, g_localizer.GetLocalizedString (Localizer::LocalizedStringID_ConsoleCommandCanOnlyBeExecutedWhenServerIsRunning), commandName.GetData ());
				else
				{
					AddLogEntry (false, LogLevel_Error, false, g_localizer.GetLocalizedString (Localizer::LocalizedStringID_ClientHasExecutedConsoleCommandThatCanOnlyBeExecutedWhenServerIsRunning), human->GetName ().GetData (), commandName.GetData ());
					human->PrintFormat (HalfLifeEngine::SDK::Constants::PrintType_AtConsole, human->GetLocalizer ()->GetLocalizedString (Localizer::LocalizedStringID_ConsoleCommandCanOnlyBeExecutedWhenServerIsRunning) + '\n', commandName.GetData ());
				}

				return;
			}
		}

		// move arguments
		command->Execute (human, argument1, argument2, argument3, argument4, argument5, "");

		return;
	}

	PrintPossibleNamesAndAppointment (human);

	if (human == NULL)
		AddLogEntry (true, LogLevel_Error, false, g_localizer.GetLocalizedString (Localizer::LocalizedStringID_UnknownConsoleCommand), commandName.GetData ());
	else
	{
		if (human->IsHost ())
			AddLogEntry (true, LogLevel_Error, false, g_localizer.GetLocalizedString (Localizer::LocalizedStringID_UnknownConsoleCommand), commandName.GetData ());
		else
		{
			AddLogEntry (false, LogLevel_Error, false, g_localizer.GetLocalizedString (Localizer::LocalizedStringID_ClientHasExecutedUnknownConsoleCommand), human->GetName ().GetData (), commandName.GetData ());
			human->PrintFormat (HalfLifeEngine::SDK::Constants::PrintType_AtConsole, human->GetLocalizer ()->GetLocalizedString (Localizer::LocalizedStringID_UnknownConsoleCommand) + '\n', commandName.GetData ());
		}
	}
}