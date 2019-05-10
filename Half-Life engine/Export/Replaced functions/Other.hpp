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
// Other.hpp
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef HALF_LIFE_ENGINE_EXPORT_REPLACED_FUNCTIONS_OTHER_INCLUDED
#define HALF_LIFE_ENGINE_EXPORT_REPLACED_FUNCTIONS_OTHER_INCLUDED

inline void UTIL_LogPrintf_RoundStart (const char *const format/*, ...*/)
{
	// Argument = "World triggered \"Round_Start\"\n".

	// Print to server console (call the original function....)
//	g_server->CallOriginalUTIL_LogPrintfFunction (format);
	Globals::g_halfLifeEngine->AlertMessage (SDK::Constants::AlertType_AtLogged, format);
//	Globals::g_halfLifeEngine->AlertMessage (SDK::Constants::AlertType_AtLogged, "World triggered \"Round_Start\"\n");

	g_server->FreezePeriodExpired ();
}

inline const unsigned int SV_CalcPing_IfFakeClient (const SDK::Structures::Client_t *const engineFakeClient)
{
	// Reliability checks.
	InternalAssert (engineFakeClient != NULL);
	InternalAssert (engineFakeClient->edict->IsValid ());

	Client *const fakeClient (g_server->GetClientManager ()->GetClient (engineFakeClient->edict));

	// Reliability check.
	InternalAssert (fakeClient->IsValid ());

	// Skip other bots....
	if (!fakeClient->IsYaPB ())
		return 0u;	// Zero is original result for fake clients in SV_CalcPing() function.

	YaPB *const bot (fakeClient->GetYaPBPointer ());
	const unsigned short botPing (bot->GetCalculatedPing ());

//	HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Center, "SV_CalcPing_IfFakeClient(): CALLED FOR FAKE CLIENT \"%s\", RESULT=%u!!!", bot->GetName ().GetData (), botPing);
//	HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat ("SV_CalcPing_IfFakeClient(): CALLED FOR FAKE CLIENT \"%s\", RESULT=%u!!!\n", bot->GetName ().GetData (), botPing);

//	return 999u;
	return botPing;
}

#endif	// ifndef HALF_LIFE_ENGINE_EXPORT_REPLACED_FUNCTIONS_OTHER_INCLUDED