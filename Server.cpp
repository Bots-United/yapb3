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
// Server.cpp
//
// Class: Server
//
// Version: $ID:$
//

#include <Core.hpp>

/*static */void Server::SV_ParseVoiceData (HalfLifeEngine::SDK::Structures::Client_t *const client)
{
	if (client->fakeclient)
	{
		Label_JustCallOriginalFunctionAndReturn:
			// Just only call original function....
			g_server->CallOriginalSV_ParseVoiceDataFunction (client);

			return;
	}

	Human *const human (g_server->GetHumanManager ()->GetHuman (client->edict));

	// Reliability check.
	InternalAssert (human->IsValid ());

//	HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat ("SV_ParseVoiceData(): called for \"%s\".\n", human->GetName ().GetData ());

	human->SetSpeaking (true);

	YaPB *const bot (human->GetVoiceRecieverBot ());

	if (bot == NULL)
		goto Label_JustCallOriginalFunctionAndReturn;

	// Redirect bot instead of real client speaker :P

	HalfLifeEngine::SDK::Structures::Client_t &botEngineClient (bot->GetEngineClient ());
	const unsigned char clientIndex (static_cast <const unsigned char> (human->GetEdict ()->GetIndex ()) - 1u);
	unsigned long &originalValueReference (botEngineClient.m_VoiceStreams[clientIndex >> 5u]);
	const unsigned long originalValue (originalValueReference);

	switch (human->GetVoiceRecieverListenType ())
	{
		// No matter what send voice data packed back to real client....
		case Human::VoiceRecieverListenType_Loopback:
			originalValueReference |= BIT (clientIndex & (HalfLifeEngine::SDK::Constants::MaximumClients - 1u));

			break;

		// No matter what ignore voice data packed of the real client....
		case Human::VoiceRecieverListenType_Ignore:
			originalValueReference &= ~BIT (clientIndex & (HalfLifeEngine::SDK::Constants::MaximumClients - 1u));

			break;
	}

	// Substitute engine 'HalfLifeEngine::SDK::Structures::Client_t' pointers from a real client sender to ours bot and call original function....
	g_server->CallOriginalSV_ParseVoiceDataFunction (&botEngineClient);

	// Restore voice stream for real client....
	originalValueReference = originalValue;
}
static HalfLifeEngine::SDK::Structures::CLC_ParseFunction_t *const GetCLC_ParseFunctionsPointer (void)
{
	const int CLC_BAD (HalfLifeEngine::SDK::Constants::CLC_BAD);	// opcode (it's 'unsigned char', but compiled as 'int')
	const unsigned char signature_CLC_ParseFunctions[] = {"clc_bad"};
	void *address (MemoryUtilities::/*DecodeAnd*/FindPatternWithoutWildCard (HalfLifeEngine::Globals::g_halfLifeEngine->GetEngineModule (), signature_CLC_ParseFunctions, sizeof (signature_CLC_ParseFunctions)));

	// Reliability check.
	InternalAssert (address != NULL);

	address = MemoryUtilities::FindMemoryChunkReference32 (HalfLifeEngine::Globals::g_halfLifeEngine->GetEngineModule (), address, reinterpret_cast <const unsigned char *const> (&CLC_BAD), sizeof (CLC_BAD));

	// Reliability check.
	InternalAssert (address != NULL);

	return reinterpret_cast <HalfLifeEngine::SDK::Structures::CLC_ParseFunction_t *> (address);
}

unsigned short Server::ms_startsNumber (0u);

Server::Server (void) :
	m_server (MemoryUtilities::GetElementAddressFromOtherAddressAndOffset <HalfLifeEngine::SDK::Structures::Server_t> (MemoryUtilities::/*DecodeAnd*/FindPattern (HalfLifeEngine::Globals::g_halfLifeEngine->GetEngineModule (), HalfLifeEngine::SDK::Constants::Server::signature, GET_STATIC_STRING_LENGTH (HalfLifeEngine::SDK::Constants::Server::signature)), HalfLifeEngine::SDK::Constants::Server::Offset_Pointer)),
	m_serverStatic (MemoryUtilities::GetElementAddressFromOtherAddressAndOffset <HalfLifeEngine::SDK::Structures::ServerStatic_t> (MemoryUtilities::/*DecodeAnd*/FindPattern (HalfLifeEngine::Globals::g_halfLifeEngine->GetEngineModule (), HalfLifeEngine::SDK::Constants::Server::Static::signature, GET_STATIC_STRING_LENGTH (HalfLifeEngine::SDK::Constants::Server::Static::signature)), HalfLifeEngine::SDK::Constants::Server::Static::Offset_Pointer)),
	m_edictAreaNodes (&MemoryUtilities::GetElementAddressFromOtherAddressAndOffset <HalfLifeEngine::SDK::Structures::EdictAreaNode_t> (MemoryUtilities::/*DecodeAnd*/FindPattern (HalfLifeEngine::Globals::g_halfLifeEngine->GetEngineModule (), HalfLifeEngine::SDK::Constants::EdictAreaNode::signature, GET_STATIC_STRING_LENGTH (HalfLifeEngine::SDK::Constants::EdictAreaNode::signature)), HalfLifeEngine::SDK::Constants::EdictAreaNode::Offset_Pointer)),
	m_CLC_ParseFunctions (GetCLC_ParseFunctionsPointer ()),
	m_gameRules (MemoryUtilities::GetElementAddressFromOtherAddressAndOffset <HalfLifeEngine::SDK::Classes::HalfLifeMultiplay *const> (HalfLifeEngine::Globals::g_halfLifeEngine->GetGameDLLFunctionTable ().pfnServerDeactivate, HalfLifeEngine::SDK::Constants::GameRules::Offset_Pointer)),
	m_ZBotManager (MemoryUtilities::GetElementAddressFromOtherAddressAndOffset <HalfLifeEngine::SDK::Classes::CSBotManager *const> (HalfLifeEngine::Globals::g_halfLifeEngine->GetGameDLLFunctionTable ().pfnServerDeactivate, HalfLifeEngine::SDK::Constants::BotManager::Offset_Pointer)),
	m_mapCycle (MemoryUtilities::GetElementAddressFromOtherAddress <HalfLifeEngine::SDK::Structures::MapCycle_t> (MemoryUtilities::/*DecodeAnd*/FindPattern (*HalfLifeEngine::Globals::g_halfLifeEngine->GetGameLibrary (), HalfLifeEngine::SDK::Constants::signature_mapCycleStructure, sizeof (HalfLifeEngine::SDK::Constants::signature_mapCycleStructure)))),

	// Find the bot arguments addresses....
	m_UseBotArgs (MemoryUtilities::GetElementAddressFromOtherAddressAndOffset <bool> (HalfLifeEngine::Globals::g_halfLifeEngine->GetGameDLLFunctionTable ().pfnClientCommand, HalfLifeEngine::SDK::Constants::CS_CLICMD_OFFS_USEBOTARGS)),
	m_BotArgs (MemoryUtilities::GetPointerAddressFromOtherAddressAndOffset <const char *> (HalfLifeEngine::Globals::g_halfLifeEngine->GetGameDLLFunctionTable ().pfnClientCommand, HalfLifeEngine::SDK::Constants::CS_CLICMD_OFFS_BOTARGS)),

	m_originalSV_ParseVoiceDataFunction (NULL),
	m_originalUTIL_LogPrintfFunction (NULL),

	m_clientManager (new ClientManager ()),
	m_fakeClientManager (new FakeClientManager ()),
	m_otherBotManager (new FakeClientNotYaPBManager ()),
	m_humanManager (new HumanManager ()),
	m_YaPBManager (new YaPBManager ()),

	m_recastWrapper
	(
		#if defined MULTIPLE_DRAW_METHODS
			HalfLifeEngine::Globals::g_halfLifeEngine->IsSoftwareRenderingMode () == false ? new RecastWrapper_GL () :
		#endif	// if defined MULTIPLE_DRAW_METHODS
		new RecastWrapper ()
	),

	m_mapType (MapType_None)
{
	// This function gets called at the map start.

	g_server = this;

	++ms_startsNumber;	// Increase number of server runs.

	#if defined _DEBUG
/*		// Reliability check.
		if (m_server == NULL)
			AddLogEntry (false, LogLevel_Critical, true, "Failed to retrieve address of server object!");

		// Reliability check.
		if (m_edictAreaNodes == NULL)
			AddLogEntry (false, LogLevel_Critical, true, "Failed to retrieve address of edict area nodes object!");
*/
		// Reliability check.
		if (m_gameRules == NULL)
			AddLogEntry (false, LogLevel_Critical, true, "Failed to retrieve address of game rules object!");

		// Reliability check.
		if (m_ZBotManager == NULL)
			AddLogEntry (false, LogLevel_Critical, true, "Failed to retrieve address of Z-bot manager object!");

		// Reliability checks.
		InternalAssert (m_BotArgs != NULL);
		InternalAssert (m_CLC_ParseFunctions != NULL);

		// Reliability check.
		if (m_clientManager == NULL || m_fakeClientManager == NULL || m_otherBotManager == NULL || m_humanManager == NULL || m_YaPBManager == NULL)
			TerminateOnMalloc ();
	#endif	// if defined _DEBUG
/*
	AddLogEntry (true, LogLevel_Default, true, "Server::Server(): m_server.name: \"%s\".", m_server.name);
	for (unsigned char index (0u); index < HalfLifeEngine::SDK::Constants::EdictAreaNode_Total; ++index)
	for (HalfLifeEngine::SDK::Structures::Link_t *l (m_edictAreaNodes->solidEdicts.next), *next; l != &m_edictAreaNodes->solidEdicts; l = next)
	{
		InternalAssert (l != NULL);

		next = l->next;
		HalfLifeEngine::SDK::Classes::Edict *const touch (HalfLifeEngine::Utilities::GetEdictFromAreaLink (l));

		InternalAssert (touch != NULL);

		AddLogEntry (true, LogLevel_Default, true, "Server::Server(): Touch: \"%s\".", touch->GetClassName ().GetData ());
	}
*/
	DetectMapType ();	// Detect map type first!

	TryExecuteConfigurationFilesOnServerStart ();

	GetNavigationMeshManager ().Load ();	// Load the Navigation Mesh for this map.
	GetWaypointManager ().GetVisibilityTable ().Load ();

	GetRecastWrapper ()->Initialize ();

	RegisterAllForwardsAndHooks ();

	GetFakeClientManager ()->SetupLightStyles ();

	// Update sky light level value....
	g_skyLightLevel = static_cast <unsigned char> ((Math::GetClampedValueInRange (Console::Server::variableManager.GetVariable (Console::Server::VariableID_SkyColor_Red)->GetValue <int> (), 0, 255) + Math::GetClampedValueInRange (Console::Server::variableManager.GetVariable (Console::Server::VariableID_SkyColor_Green)->GetValue <int> (), 0, 255) + Math::GetClampedValueInRange (Console::Server::variableManager.GetVariable (Console::Server::VariableID_SkyColor_Blue)->GetValue <int> (), 0, 255)) / Color <>::ID_Total);
#if 0
	#if defined _DEBUG
	// Disable client kicking with message: "Connection to server lost during level change." - for step by step debugging....
	{
		#pragma message ("REMOVE ME!!!")

		unsigned char *const hwDLLBaseAddress (reinterpret_cast <unsigned char *const> (HalfLifeEngine::Globals::g_halfLifeEngine->GetEngineModule ().GetPointer ()));

		Trampolines::PatchMemoryData <unsigned char> (hwDLLBaseAddress + 0x581E6u/* (Address: 0x1D581E6) */, 0x52);	// Change "jz short loc_1D581F1" to "jz short loc_1D58239"
		Trampolines::PatchMemoryData <unsigned char> (hwDLLBaseAddress + 0x581EBu/* (Address: 0x1D581EB) */, 0x4D);	// Change "jz short loc_1D581F1" to "jz short loc_1D58239"
		Trampolines::PatchMemoryData <unsigned char> (hwDLLBaseAddress + 0x581EFu/* (Address: 0x1D581EF) */, Trampolines::Bytecode::JumpImmediately8);	// Change "jnz short loc_1D58239" to "jmp short loc_1D58239"
	}
	#endif	// if defined _DEBUG
#endif	// if 0
}

void Server::RegisterClientToServerMessageForwards (void)
{
	// Replace some shit....
	m_originalSV_ParseVoiceDataFunction = m_CLC_ParseFunctions[HalfLifeEngine::SDK::Constants::CLC_VOICEDATA].pfnParse;

	m_CLC_ParseFunctions[HalfLifeEngine::SDK::Constants::CLC_VOICEDATA].pfnParse = SV_ParseVoiceData;
}
void Server::UnregisterClientToServerMessageForwards (void) const
{
	// Reliability check.
	InternalAssert (m_originalSV_ParseVoiceDataFunction != NULL);

	// Replace some shit back....
	m_CLC_ParseFunctions[HalfLifeEngine::SDK::Constants::CLC_VOICEDATA].pfnParse = m_originalSV_ParseVoiceDataFunction;
}

void Server::RegisterEngineAPIForwards (void) const
{
	HalfLifeEngine::Globals::g_halfLifeEngine->HookEngineAPISetModelFunction      ();
	HalfLifeEngine::Globals::g_halfLifeEngine->HookEngineAPIEmitSoundFunction     ();
	HalfLifeEngine::Globals::g_halfLifeEngine->HookEngineAPIClientCommandFunction ();
	HalfLifeEngine::Globals::g_halfLifeEngine->HookEngineAPILightStyleFunction    ();
	HalfLifeEngine::Globals::g_halfLifeEngine->HookEngineAPIClientPrintfFunction  ();
	HalfLifeEngine::Globals::g_halfLifeEngine->HookEngineAPISetViewFunction       ();
}
void Server::UnregisterEngineAPIForwards (void) const
{
	HalfLifeEngine::Globals::g_halfLifeEngine->UnhookEngineAPISetModelFunction      ();
	HalfLifeEngine::Globals::g_halfLifeEngine->UnhookEngineAPIEmitSoundFunction     ();
	HalfLifeEngine::Globals::g_halfLifeEngine->UnhookEngineAPIClientCommandFunction ();
	HalfLifeEngine::Globals::g_halfLifeEngine->UnhookEngineAPILightStyleFunction    ();
	HalfLifeEngine::Globals::g_halfLifeEngine->UnhookEngineAPIClientPrintfFunction  ();
	HalfLifeEngine::Globals::g_halfLifeEngine->UnhookEngineAPISetViewFunction       ();
}

void Server::RegisterGameDLLAPIForwards (void)
{
	HalfLifeEngine::Globals::g_halfLifeEngine->HookGameDLLAPITouchFunction                 ();
	HalfLifeEngine::Globals::g_halfLifeEngine->HookGameDLLAPIClientConnectFunction         ();
	HalfLifeEngine::Globals::g_halfLifeEngine->HookGameDLLAPIClientDisconnectFunction      ();
///	HalfLifeEngine::Globals::g_halfLifeEngine->HookGameDLLAPIClientCommandFunction         ();	/// @warning This crashes server, due to 'cstrike_amxx.DLL' in CtrlDetour_ClientCommand() patches our callback function! And one more: we directly hooking the ClientCommand() function from 'mp.DLL' so we don't need this callback....
	HalfLifeEngine::Globals::g_halfLifeEngine->HookGameDLLAPIClientUserInfoChangedFunction ();
	HalfLifeEngine::Globals::g_halfLifeEngine->HookGameDLLAPIServerDeactivateFunction      ();
	HalfLifeEngine::Globals::g_halfLifeEngine->HookGameDLLAPIStartFrameFunction            ();
	HalfLifeEngine::Globals::g_halfLifeEngine->HookGameDLLAPIUpdateClientDataFunction      ();

	GetClientManager ()->RegisterGameDLLAPIForwards ();
}
void Server::UnregisterGameDLLAPIForwards (void)
{
	HalfLifeEngine::Globals::g_halfLifeEngine->UnhookGameDLLAPITouchFunction                 ();
	HalfLifeEngine::Globals::g_halfLifeEngine->UnhookGameDLLAPIClientConnectFunction         ();
	HalfLifeEngine::Globals::g_halfLifeEngine->UnhookGameDLLAPIClientDisconnectFunction      ();
///	HalfLifeEngine::Globals::g_halfLifeEngine->UnhookGameDLLAPIClientCommandFunction         ();	/// @warning This crashes server, due to 'cstrike_amxx.DLL' in CtrlDetour_ClientCommand() patches our callback function! And one more: we directly hooking the ClientCommand() function from 'mp.DLL' so we don't need this callback....
	HalfLifeEngine::Globals::g_halfLifeEngine->UnhookGameDLLAPIClientUserInfoChangedFunction ();
	HalfLifeEngine::Globals::g_halfLifeEngine->UnhookGameDLLAPIServerDeactivateFunction      ();
	HalfLifeEngine::Globals::g_halfLifeEngine->UnhookGameDLLAPIStartFrameFunction            ();
	HalfLifeEngine::Globals::g_halfLifeEngine->UnhookGameDLLAPIUpdateClientDataFunction      ();

	GetClientManager ()->UnregisterGameDLLAPIForwards ();
}
#if 0
//typedef void (Trampolines::GenericClass::*CHalfLifeMultiplay__SendMOTDToClient_t) (HalfLifeEngine::SDK::Classes::Edict *client) const;
//typedef void (Trampolines::EmptyClass::*CHalfLifeMultiplay__SendMOTDToClient_t) (HalfLifeEngine::SDK::Classes::Edict *client) const;
typedef void (HalfLifeEngine::SDK::Classes::HalfLifeMultiplay::*CHalfLifeMultiplay__SendMOTDToClient_t) (HalfLifeEngine::SDK::Classes::Edict *client) const;
CHalfLifeMultiplay__SendMOTDToClient_t g_org (NULL);

//inline void __stdcall CHalfLifeMultiplay__SendMOTDToClient (HalfLifeEngine::SDK::Classes::Edict *client)
inline void __fastcall CHalfLifeMultiplay__SendMOTDToClient (HalfLifeEngine::SDK::Classes::HalfLifeMultiplay *const object, int /*dummy*/, HalfLifeEngine::SDK::Classes::Edict *client)
{
	HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "CHalfLifeMultiplay::SendMOTDToClient(): Called for client: \"%s\"!\n", client->GetNetName ().GetData ());
//	AddLogEntry (true, LogLevel_Default, true, "\nCHalfLifeMultiplay::SendMOTDToClient(): Called for client: \"%s\"!\n", client->GetNetName ().GetData ());
/*
	HalfLifeEngine::SDK::Classes::HalfLifeMultiplay *object (NULL);

	__asm
	{
		mov object, ecx;
	}
*/
	InternalAssert (object != NULL);
	InternalAssert (object == g_server->GetGameRules ());	// OCCURS!!!

//	reinterpret_cast <void (__fastcall *) (HalfLifeEngine::SDK::Classes::HalfLifeMultiplay *const /*this*/, int /*dummy*/, HalfLifeEngine::SDK::Classes::Edict *)> (reinterpret_cast <const void *&> (g_org)) (object, 0, client);
//	(reinterpret_cast <Trampolines::GenericClass *> (object)->*g_org) (client);
//	(reinterpret_cast <Trampolines::EmptyClass *> (object)->*g_org) (client);
	(object->*g_org) (client);
//	(g_server->GetGameRules ()->*g_org) (client);
}

inline const unsigned int /*__declspec (naked) */SV_CalcPing (const HalfLifeEngine::SDK::Structures::Client_t *const client)
{
	// Reliability check.
	InternalAssert (client != NULL);

	const unsigned int result ((*HalfLifeEngine::Globals::g_halfLifeEngine->GetSV_CalcPingFunction ()) (client));

	HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat ("SV_CalcPing(): CALLED FOR CLIENT \"%s\", RESULT=%u!!!\n", client->datagram.name, result);

	return result;
}
inline void RedirectAllSV_CalcPingFunctionCalls (void)
{
/*	const unsigned char string[] = {"%4i %s\n"};	// From Host_Ping_f() function.
	unsigned char *address (static_cast <unsigned char *> (MemoryUtilities::FindPattern (HalfLifeEngine::Globals::g_halfLifeEngine->GetEngineModule ().GetPointer (), HalfLifeEngine::Globals::g_halfLifeEngine->GetEngineModule ().GetSize (), string, GET_STATIC_STRING_LENGTH (string))));

	if (address == NULL)
		return;

	address = static_cast <unsigned char *> (MemoryUtilities::FindMemoryChunkReference32 (HalfLifeEngine::Globals::g_halfLifeEngine->GetEngineModule (), address, Trampolines::Bytecode::Push32));

	if (address == NULL)
		return;

	address -= GET_STATIC_STRING_LENGTH ("\x4D\x93\x04\x00\x83\xC4\x04\x50");

	// Reliability check.
	InternalAssert ((address - sizeof (Trampolines::Bytecode::CallImmediately32))[0u] == Trampolines::Bytecode::CallImmediately32);

	Trampolines::RedirectFunctionCall32 (address, SV_CalcPing);
*/
	const unsigned int redirectionsNumber (Trampolines::RedirectFunctionCalls32 (HalfLifeEngine::Globals::g_halfLifeEngine->GetEngineModule ().GetPointer (), HalfLifeEngine::Globals::g_halfLifeEngine->GetEngineModule ().GetSize (), HalfLifeEngine::Globals::g_halfLifeEngine->GetSV_CalcPingFunction (), SV_CalcPing));

	AddLogEntry (false, LogLevel_Default, false, "RedirectAllSV_CalcPingFunctionCalls(): SV_CalcPing() function redirections number is %u.\n", redirectionsNumber);
}
inline void ClientPutInServer (HalfLifeEngine::SDK::Classes::Edict *edict)
{
	HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Center, "ClientPutInServer(): CALLED FOR \"%s\"!!!", edict->GetNetName ().GetData ());

	g_server->GetClientManager ()->GetClientPutInServerHooker ().CallOriginalFunction (edict);	// Let him actually join the game.
}
#endif	// if 0
void Server::RegisterEngineVirtualTableHooks (void)
{
	// Game rules specific hooks...
	m_engineVirtualTableHooksManager.RegisterEngineVirtualTableHookFromClassPointer (m_gameRules, HalfLifeEngine::SDK::Constants::GameRules::VirtualTableFunctionID_RestartRound, true, 0u, HalfLifeEngine::Export::ReplacedFunctions::GameRulesTable::RestartRound);

	// CS bot manager specific hooks...
	m_engineVirtualTableHooksManager.RegisterEngineVirtualTableHookFromClassPointer (m_ZBotManager, HalfLifeEngine::SDK::Constants::BotManager::VirtualTableFunctionID_OnEvent, false, 3u, HalfLifeEngine::Export::ReplacedFunctions::BotManagerTable::OnEvent);
/*
	Trampolines::RedirectFunctionCall32 (reinterpret_cast <void *> (reinterpret_cast <unsigned int> (HalfLifeEngine::Globals::g_halfLifeEngine->GetGameLibrary ()->GetPointer ()) + 0x991F2 + sizeof (Trampolines::Bytecode::CallImmediately32)), CHalfLifeMultiplay__SendMOTDToClient, reinterpret_cast <const void *&> (g_org));

	InternalAssert (g_org != NULL);
*/
	// hook "Round_Start" logging
//	Trampolines::RedirectFunctionCall32 (reinterpret_cast <void *> (reinterpret_cast <unsigned int> (HalfLifeEngine::Globals::g_halfLifeEngine->GetGameLibrary ()->GetPointer ()) + 0x97CEC + sizeof (Trampolines::Bytecode::CallImmediately32)), HalfLifeEngine::Export::ReplacedFunctions::Other::UTIL_LogPrintf_RoundStart, reinterpret_cast <const void *&> (m_originalUTIL_LogPrintfFunction));
	Trampolines::RedirectFunctionCall32 (MemoryUtilities::/*DecodeAnd*/FindPattern (*HalfLifeEngine::Globals::g_halfLifeEngine->GetGameLibrary (), HalfLifeEngine::SDK::Constants::signature_Calling_UTIL_LogPrintf_RoundStart, sizeof (HalfLifeEngine::SDK::Constants::signature_Calling_UTIL_LogPrintf_RoundStart)), HalfLifeEngine::Export::ReplacedFunctions::Other::UTIL_LogPrintf_RoundStart, reinterpret_cast <const void *&> (m_originalUTIL_LogPrintfFunction));
#if 0
	MemoryUtilities::ModuleInformation_t dynamicLibraryInformation;

	#if defined _DEBUG
		// Reliability check.
		InternalAssert (MemoryUtilities::GetModuleInformation (HalfLifeEngine::Globals::g_halfLifeEngine->GetGameDLLFunctionTable ().pfnServerDeactivate, dynamicLibraryInformation));
	#else	// if defined _DEBUG
		MemoryUtilities::GetModuleInformation (HalfLifeEngine::Globals::g_halfLifeEngine->GetGameDLLFunctionTable ().pfnServerDeactivate, dynamicLibraryInformation);
	#endif	// if !defined _DEBUG

	void *adr (HalfLifeEngine::Globals::g_halfLifeEngine->GetGameDLLFunctionTable ().pfnClientPutInServer);
	AddLogEntry (false, LogLevel_Error, true, "X=%X, adr=%X, baseAddress=%X.\n", (unsigned char *)adr - (unsigned char *)dynamicLibraryInformation.baseAddress, (unsigned char *)adr, (unsigned char *)dynamicLibraryInformation.baseAddress);

/*	void *adr (Trampolines::GetRealAddressOfRelativeAddress32 (MemoryUtilities::/*DecodeAnd*//*FindPattern (*HalfLifeEngine::Globals::g_halfLifeEngine->GetGameLibrary (), HalfLifeEngine::SDK::Constants::signature_Calling_UTIL_LogPrintf_RoundStart, sizeof (HalfLifeEngine::SDK::Constants::signature_Calling_UTIL_LogPrintf_RoundStart))));
	unsigned int c = Trampolines::RedirectFunctionCalls32 (dynamicLibraryInformation.baseAddress, dynamicLibraryInformation.memorySize, adr, HalfLifeEngine::Export::ReplacedFunctions::Other::UTIL_LogPrintf_RoundStart);
*/	unsigned int c = Trampolines::RedirectFunctionCalls32 (dynamicLibraryInformation.baseAddress, dynamicLibraryInformation.memorySize, adr, ClientPutInServer);

	AddLogEntry (false, LogLevel_Critical, true, "LOGPRINTF(): count = %u, X=%X, adr=%X, baseAddress=%X.\n", c, (unsigned char *)adr - (unsigned char *)dynamicLibraryInformation.baseAddress, (unsigned char *)adr, (unsigned char *)dynamicLibraryInformation.baseAddress);
#endif
//	RedirectAllSV_CalcPingFunctionCalls ();

	// Entity specific hooks...

	// Register virtual functions hooks for "player" entities....
	m_engineVirtualTableHooksManager.RegisterEngineVirtualTableHookByEntityClassName ("player", HalfLifeEngine::SDK::Constants::EntityVirtualTableFunctionID_Spawn,                    true, 0u, HalfLifeEngine::Export::ReplacedFunctions::EntityTable::Spawn);
	m_engineVirtualTableHooksManager.RegisterEngineVirtualTableHookByEntityClassName ("player", HalfLifeEngine::SDK::Constants::EntityVirtualTableFunctionID_TakeDamage,              false, 4u, HalfLifeEngine::Export::ReplacedFunctions::EntityTable::TakeDamage);
	m_engineVirtualTableHooksManager.RegisterEngineVirtualTableHookByEntityClassName ("player", HalfLifeEngine::SDK::Constants::EntityVirtualTableFunctionID_Killed,                   true, 2u, HalfLifeEngine::Export::ReplacedFunctions::EntityTable::Killed);
	m_engineVirtualTableHooksManager.RegisterEngineVirtualTableHookByEntityClassName ("player", HalfLifeEngine::SDK::Constants::EntityVirtualTableFunctionID_AddPlayerItem,           false, 1u, HalfLifeEngine::Export::ReplacedFunctions::EntityTable::AddPlayerItem);
	m_engineVirtualTableHooksManager.RegisterEngineVirtualTableHookByEntityClassName ("player", HalfLifeEngine::SDK::Constants::EntityVirtualTableFunctionID_RemovePlayerItem,        false, 1u, HalfLifeEngine::Export::ReplacedFunctions::EntityTable::RemovePlayerItem);
	m_engineVirtualTableHooksManager.RegisterEngineVirtualTableHookByEntityClassName ("player", HalfLifeEngine::SDK::Constants::EntityVirtualTableFunctionID_Player_ImpulseCommands,   true, 0u, HalfLifeEngine::Export::ReplacedFunctions::EntityTable::Player_ImpulseCommands);
	m_engineVirtualTableHooksManager.RegisterEngineVirtualTableHookByEntityClassName ("player", HalfLifeEngine::SDK::Constants::EntityVirtualTableFunctionID_Player_RoundRespawn,      true, 0u, HalfLifeEngine::Export::ReplacedFunctions::EntityTable::Player_RoundRespawn);
	m_engineVirtualTableHooksManager.RegisterEngineVirtualTableHookByEntityClassName ("player", HalfLifeEngine::SDK::Constants::EntityVirtualTableFunctionID_Player_OnTouchingWeapon,  true, 1u, HalfLifeEngine::Export::ReplacedFunctions::EntityTable::Player_OnTouchingWeapon);
	m_engineVirtualTableHooksManager.RegisterEngineVirtualTableHookByEntityClassName ("player", HalfLifeEngine::SDK::Constants::EntityVirtualTableFunctionID_Touch,                    true, 1u, HalfLifeEngine::Export::ReplacedFunctions::EntityTable::Player_Touch);

	m_engineVirtualTableHooksManager.RegisterEngineVirtualTableHookByEntityClassName ("hostage_entity", HalfLifeEngine::SDK::Constants::EntityVirtualTableFunctionID_Spawn,       true, 0u, HalfLifeEngine::Export::ReplacedFunctions::EntityTable::Hostage_Spawn);
	m_engineVirtualTableHooksManager.RegisterEngineVirtualTableHookByEntityClassName ("hostage_entity", HalfLifeEngine::SDK::Constants::EntityVirtualTableFunctionID_TakeDamage, false, 4u, HalfLifeEngine::Export::ReplacedFunctions::EntityTable::Hostage_TakeDamage);
	m_engineVirtualTableHooksManager.RegisterEngineVirtualTableHookByEntityClassName ("hostage_entity", HalfLifeEngine::SDK::Constants::EntityVirtualTableFunctionID_Use,         true, 4u, HalfLifeEngine::Export::ReplacedFunctions::EntityTable::Hostage_Use);

	// Bot code wants to evade engine seeing entities being touched.
//	m_engineVirtualTableHooksManager.RegisterEngineVirtualTableHookByEntityClassName ("item_kevlar",      HalfLifeEngine::SDK::Constants::EntityVirtualTableFunctionID_Touch, true, 1u, HalfLifeEngine::Export::ReplacedFunctions::EntityTable::Touch);
//	m_engineVirtualTableHooksManager.RegisterEngineVirtualTableHookByEntityClassName ("item_assaultsuit", HalfLifeEngine::SDK::Constants::EntityVirtualTableFunctionID_Touch, true, 1u, HalfLifeEngine::Export::ReplacedFunctions::EntityTable::Touch);
	m_engineVirtualTableHooksManager.RegisterEngineVirtualTableHookByEntityClassName ("item_thighpack",   HalfLifeEngine::SDK::Constants::EntityVirtualTableFunctionID_Touch, true, 1u, HalfLifeEngine::Export::ReplacedFunctions::EntityTable::Touch);
	m_engineVirtualTableHooksManager.RegisterEngineVirtualTableHookByEntityClassName ("weapon_shield",    HalfLifeEngine::SDK::Constants::EntityVirtualTableFunctionID_Touch, true, 1u, HalfLifeEngine::Export::ReplacedFunctions::EntityTable::Touch);
	m_engineVirtualTableHooksManager.RegisterEngineVirtualTableHookByEntityClassName ("weaponbox",        HalfLifeEngine::SDK::Constants::EntityVirtualTableFunctionID_Touch, true, 1u, HalfLifeEngine::Export::ReplacedFunctions::EntityTable::Touch);
	m_engineVirtualTableHooksManager.RegisterEngineVirtualTableHookByEntityClassName ("armoury_entity",   HalfLifeEngine::SDK::Constants::EntityVirtualTableFunctionID_Touch, true, 1u, HalfLifeEngine::Export::ReplacedFunctions::EntityTable::Touch);

	// Bot code wants to evade engine seeing entities being used.
	m_engineVirtualTableHooksManager.RegisterEngineVirtualTableHookByEntityClassName ("func_button",        HalfLifeEngine::SDK::Constants::EntityVirtualTableFunctionID_Use, true, 4u, HalfLifeEngine::Export::ReplacedFunctions::EntityTable::Use);
	m_engineVirtualTableHooksManager.RegisterEngineVirtualTableHookByEntityClassName ("func_door",          HalfLifeEngine::SDK::Constants::EntityVirtualTableFunctionID_Use, true, 4u, HalfLifeEngine::Export::ReplacedFunctions::EntityTable::Use);
	m_engineVirtualTableHooksManager.RegisterEngineVirtualTableHookByEntityClassName ("func_tank",          HalfLifeEngine::SDK::Constants::EntityVirtualTableFunctionID_Use, true, 4u, HalfLifeEngine::Export::ReplacedFunctions::EntityTable::Use);
	m_engineVirtualTableHooksManager.RegisterEngineVirtualTableHookByEntityClassName ("func_tankmortar",    HalfLifeEngine::SDK::Constants::EntityVirtualTableFunctionID_Use, true, 4u, HalfLifeEngine::Export::ReplacedFunctions::EntityTable::Use);
	m_engineVirtualTableHooksManager.RegisterEngineVirtualTableHookByEntityClassName ("func_tankrocket",    HalfLifeEngine::SDK::Constants::EntityVirtualTableFunctionID_Use, true, 4u, HalfLifeEngine::Export::ReplacedFunctions::EntityTable::Use);
	m_engineVirtualTableHooksManager.RegisterEngineVirtualTableHookByEntityClassName ("func_tanklaser",     HalfLifeEngine::SDK::Constants::EntityVirtualTableFunctionID_Use, true, 4u, HalfLifeEngine::Export::ReplacedFunctions::EntityTable::Use);
	m_engineVirtualTableHooksManager.RegisterEngineVirtualTableHookByEntityClassName ("func_pushable",      HalfLifeEngine::SDK::Constants::EntityVirtualTableFunctionID_Use, true, 4u, HalfLifeEngine::Export::ReplacedFunctions::EntityTable::Use);
	m_engineVirtualTableHooksManager.RegisterEngineVirtualTableHookByEntityClassName ("func_healthcharger", HalfLifeEngine::SDK::Constants::EntityVirtualTableFunctionID_Use, true, 4u, HalfLifeEngine::Export::ReplacedFunctions::EntityTable::Use);
	m_engineVirtualTableHooksManager.RegisterEngineVirtualTableHookByEntityClassName ("func_recharge",      HalfLifeEngine::SDK::Constants::EntityVirtualTableFunctionID_Use, true, 4u, HalfLifeEngine::Export::ReplacedFunctions::EntityTable::Use);

	// Item::Deploy()
	m_engineVirtualTableHooksManager.RegisterEngineVirtualTableHookByEntityClassName ("weapon_p228",         HalfLifeEngine::SDK::Constants::EntityVirtualTableFunctionID_Item_Deploy, false, 0u, HalfLifeEngine::Export::ReplacedFunctions::EntityTable::Item_Deploy);
	m_engineVirtualTableHooksManager.RegisterEngineVirtualTableHookByEntityClassName ("weapon_scout",        HalfLifeEngine::SDK::Constants::EntityVirtualTableFunctionID_Item_Deploy, false, 0u, HalfLifeEngine::Export::ReplacedFunctions::EntityTable::Item_Deploy);
	m_engineVirtualTableHooksManager.RegisterEngineVirtualTableHookByEntityClassName ("weapon_hegrenade",    HalfLifeEngine::SDK::Constants::EntityVirtualTableFunctionID_Item_Deploy, false, 0u, HalfLifeEngine::Export::ReplacedFunctions::EntityTable::Item_Deploy);
	m_engineVirtualTableHooksManager.RegisterEngineVirtualTableHookByEntityClassName ("weapon_xm1014",       HalfLifeEngine::SDK::Constants::EntityVirtualTableFunctionID_Item_Deploy, false, 0u, HalfLifeEngine::Export::ReplacedFunctions::EntityTable::Item_Deploy);
	m_engineVirtualTableHooksManager.RegisterEngineVirtualTableHookByEntityClassName ("weapon_c4",           HalfLifeEngine::SDK::Constants::EntityVirtualTableFunctionID_Item_Deploy, false, 0u, HalfLifeEngine::Export::ReplacedFunctions::EntityTable::Item_Deploy);
	m_engineVirtualTableHooksManager.RegisterEngineVirtualTableHookByEntityClassName ("weapon_mac10",        HalfLifeEngine::SDK::Constants::EntityVirtualTableFunctionID_Item_Deploy, false, 0u, HalfLifeEngine::Export::ReplacedFunctions::EntityTable::Item_Deploy);
	m_engineVirtualTableHooksManager.RegisterEngineVirtualTableHookByEntityClassName ("weapon_aug",          HalfLifeEngine::SDK::Constants::EntityVirtualTableFunctionID_Item_Deploy, false, 0u, HalfLifeEngine::Export::ReplacedFunctions::EntityTable::Item_Deploy);
	m_engineVirtualTableHooksManager.RegisterEngineVirtualTableHookByEntityClassName ("weapon_smokegrenade", HalfLifeEngine::SDK::Constants::EntityVirtualTableFunctionID_Item_Deploy, false, 0u, HalfLifeEngine::Export::ReplacedFunctions::EntityTable::Item_Deploy);
	m_engineVirtualTableHooksManager.RegisterEngineVirtualTableHookByEntityClassName ("weapon_elite",        HalfLifeEngine::SDK::Constants::EntityVirtualTableFunctionID_Item_Deploy, false, 0u, HalfLifeEngine::Export::ReplacedFunctions::EntityTable::Item_Deploy);
	m_engineVirtualTableHooksManager.RegisterEngineVirtualTableHookByEntityClassName ("weapon_fiveseven",    HalfLifeEngine::SDK::Constants::EntityVirtualTableFunctionID_Item_Deploy, false, 0u, HalfLifeEngine::Export::ReplacedFunctions::EntityTable::Item_Deploy);
	m_engineVirtualTableHooksManager.RegisterEngineVirtualTableHookByEntityClassName ("weapon_ump45",        HalfLifeEngine::SDK::Constants::EntityVirtualTableFunctionID_Item_Deploy, false, 0u, HalfLifeEngine::Export::ReplacedFunctions::EntityTable::Item_Deploy);
	m_engineVirtualTableHooksManager.RegisterEngineVirtualTableHookByEntityClassName ("weapon_sg550",        HalfLifeEngine::SDK::Constants::EntityVirtualTableFunctionID_Item_Deploy, false, 0u, HalfLifeEngine::Export::ReplacedFunctions::EntityTable::Item_Deploy);
	m_engineVirtualTableHooksManager.RegisterEngineVirtualTableHookByEntityClassName ("weapon_galil",        HalfLifeEngine::SDK::Constants::EntityVirtualTableFunctionID_Item_Deploy, false, 0u, HalfLifeEngine::Export::ReplacedFunctions::EntityTable::Item_Deploy);
	m_engineVirtualTableHooksManager.RegisterEngineVirtualTableHookByEntityClassName ("weapon_famas",        HalfLifeEngine::SDK::Constants::EntityVirtualTableFunctionID_Item_Deploy, false, 0u, HalfLifeEngine::Export::ReplacedFunctions::EntityTable::Item_Deploy);
	m_engineVirtualTableHooksManager.RegisterEngineVirtualTableHookByEntityClassName ("weapon_usp",          HalfLifeEngine::SDK::Constants::EntityVirtualTableFunctionID_Item_Deploy, false, 0u, HalfLifeEngine::Export::ReplacedFunctions::EntityTable::Item_Deploy);
	m_engineVirtualTableHooksManager.RegisterEngineVirtualTableHookByEntityClassName ("weapon_glock18",      HalfLifeEngine::SDK::Constants::EntityVirtualTableFunctionID_Item_Deploy, false, 0u, HalfLifeEngine::Export::ReplacedFunctions::EntityTable::Item_Deploy);
	m_engineVirtualTableHooksManager.RegisterEngineVirtualTableHookByEntityClassName ("weapon_awp",          HalfLifeEngine::SDK::Constants::EntityVirtualTableFunctionID_Item_Deploy, false, 0u, HalfLifeEngine::Export::ReplacedFunctions::EntityTable::Item_Deploy);
	m_engineVirtualTableHooksManager.RegisterEngineVirtualTableHookByEntityClassName ("weapon_mp5navy",      HalfLifeEngine::SDK::Constants::EntityVirtualTableFunctionID_Item_Deploy, false, 0u, HalfLifeEngine::Export::ReplacedFunctions::EntityTable::Item_Deploy);
	m_engineVirtualTableHooksManager.RegisterEngineVirtualTableHookByEntityClassName ("weapon_m249",         HalfLifeEngine::SDK::Constants::EntityVirtualTableFunctionID_Item_Deploy, false, 0u, HalfLifeEngine::Export::ReplacedFunctions::EntityTable::Item_Deploy);
	m_engineVirtualTableHooksManager.RegisterEngineVirtualTableHookByEntityClassName ("weapon_m3",           HalfLifeEngine::SDK::Constants::EntityVirtualTableFunctionID_Item_Deploy, false, 0u, HalfLifeEngine::Export::ReplacedFunctions::EntityTable::Item_Deploy);
	m_engineVirtualTableHooksManager.RegisterEngineVirtualTableHookByEntityClassName ("weapon_m4a1",         HalfLifeEngine::SDK::Constants::EntityVirtualTableFunctionID_Item_Deploy, false, 0u, HalfLifeEngine::Export::ReplacedFunctions::EntityTable::Item_Deploy);
	m_engineVirtualTableHooksManager.RegisterEngineVirtualTableHookByEntityClassName ("weapon_tmp",          HalfLifeEngine::SDK::Constants::EntityVirtualTableFunctionID_Item_Deploy, false, 0u, HalfLifeEngine::Export::ReplacedFunctions::EntityTable::Item_Deploy);
	m_engineVirtualTableHooksManager.RegisterEngineVirtualTableHookByEntityClassName ("weapon_g3sg1",        HalfLifeEngine::SDK::Constants::EntityVirtualTableFunctionID_Item_Deploy, false, 0u, HalfLifeEngine::Export::ReplacedFunctions::EntityTable::Item_Deploy);
	m_engineVirtualTableHooksManager.RegisterEngineVirtualTableHookByEntityClassName ("weapon_flashbang",    HalfLifeEngine::SDK::Constants::EntityVirtualTableFunctionID_Item_Deploy, false, 0u, HalfLifeEngine::Export::ReplacedFunctions::EntityTable::Item_Deploy);
	m_engineVirtualTableHooksManager.RegisterEngineVirtualTableHookByEntityClassName ("weapon_deagle",       HalfLifeEngine::SDK::Constants::EntityVirtualTableFunctionID_Item_Deploy, false, 0u, HalfLifeEngine::Export::ReplacedFunctions::EntityTable::Item_Deploy);
	m_engineVirtualTableHooksManager.RegisterEngineVirtualTableHookByEntityClassName ("weapon_sg552",        HalfLifeEngine::SDK::Constants::EntityVirtualTableFunctionID_Item_Deploy, false, 0u, HalfLifeEngine::Export::ReplacedFunctions::EntityTable::Item_Deploy);
	m_engineVirtualTableHooksManager.RegisterEngineVirtualTableHookByEntityClassName ("weapon_ak47",         HalfLifeEngine::SDK::Constants::EntityVirtualTableFunctionID_Item_Deploy, false, 0u, HalfLifeEngine::Export::ReplacedFunctions::EntityTable::Item_Deploy);
	m_engineVirtualTableHooksManager.RegisterEngineVirtualTableHookByEntityClassName ("weapon_knife",        HalfLifeEngine::SDK::Constants::EntityVirtualTableFunctionID_Item_Deploy, false, 0u, HalfLifeEngine::Export::ReplacedFunctions::EntityTable::Item_Deploy);
	m_engineVirtualTableHooksManager.RegisterEngineVirtualTableHookByEntityClassName ("weapon_p90",          HalfLifeEngine::SDK::Constants::EntityVirtualTableFunctionID_Item_Deploy, false, 0u, HalfLifeEngine::Export::ReplacedFunctions::EntityTable::Item_Deploy);

	// Weapon::Reload()
	m_engineVirtualTableHooksManager.RegisterEngineVirtualTableHookByEntityClassName ("weapon_p228",      HalfLifeEngine::SDK::Constants::EntityVirtualTableFunctionID_Weapon_Reload, true, 0u, HalfLifeEngine::Export::ReplacedFunctions::EntityTable::Weapon_Reload);
	m_engineVirtualTableHooksManager.RegisterEngineVirtualTableHookByEntityClassName ("weapon_scout",     HalfLifeEngine::SDK::Constants::EntityVirtualTableFunctionID_Weapon_Reload, true, 0u, HalfLifeEngine::Export::ReplacedFunctions::EntityTable::Weapon_Reload);
	m_engineVirtualTableHooksManager.RegisterEngineVirtualTableHookByEntityClassName ("weapon_xm1014",    HalfLifeEngine::SDK::Constants::EntityVirtualTableFunctionID_Weapon_Reload, true, 0u, HalfLifeEngine::Export::ReplacedFunctions::EntityTable::Weapon_Shotgun_Reload);	// Register shotguns, we need different function for additional checks.
	m_engineVirtualTableHooksManager.RegisterEngineVirtualTableHookByEntityClassName ("weapon_mac10",     HalfLifeEngine::SDK::Constants::EntityVirtualTableFunctionID_Weapon_Reload, true, 0u, HalfLifeEngine::Export::ReplacedFunctions::EntityTable::Weapon_Reload);
	m_engineVirtualTableHooksManager.RegisterEngineVirtualTableHookByEntityClassName ("weapon_aug",       HalfLifeEngine::SDK::Constants::EntityVirtualTableFunctionID_Weapon_Reload, true, 0u, HalfLifeEngine::Export::ReplacedFunctions::EntityTable::Weapon_Reload);
	m_engineVirtualTableHooksManager.RegisterEngineVirtualTableHookByEntityClassName ("weapon_elite",     HalfLifeEngine::SDK::Constants::EntityVirtualTableFunctionID_Weapon_Reload, true, 0u, HalfLifeEngine::Export::ReplacedFunctions::EntityTable::Weapon_Reload);
	m_engineVirtualTableHooksManager.RegisterEngineVirtualTableHookByEntityClassName ("weapon_fiveseven", HalfLifeEngine::SDK::Constants::EntityVirtualTableFunctionID_Weapon_Reload, true, 0u, HalfLifeEngine::Export::ReplacedFunctions::EntityTable::Weapon_Reload);
	m_engineVirtualTableHooksManager.RegisterEngineVirtualTableHookByEntityClassName ("weapon_ump45",     HalfLifeEngine::SDK::Constants::EntityVirtualTableFunctionID_Weapon_Reload, true, 0u, HalfLifeEngine::Export::ReplacedFunctions::EntityTable::Weapon_Reload);
	m_engineVirtualTableHooksManager.RegisterEngineVirtualTableHookByEntityClassName ("weapon_sg550",     HalfLifeEngine::SDK::Constants::EntityVirtualTableFunctionID_Weapon_Reload, true, 0u, HalfLifeEngine::Export::ReplacedFunctions::EntityTable::Weapon_Reload);
	m_engineVirtualTableHooksManager.RegisterEngineVirtualTableHookByEntityClassName ("weapon_galil",     HalfLifeEngine::SDK::Constants::EntityVirtualTableFunctionID_Weapon_Reload, true, 0u, HalfLifeEngine::Export::ReplacedFunctions::EntityTable::Weapon_Reload);
	m_engineVirtualTableHooksManager.RegisterEngineVirtualTableHookByEntityClassName ("weapon_famas",     HalfLifeEngine::SDK::Constants::EntityVirtualTableFunctionID_Weapon_Reload, true, 0u, HalfLifeEngine::Export::ReplacedFunctions::EntityTable::Weapon_Reload);
	m_engineVirtualTableHooksManager.RegisterEngineVirtualTableHookByEntityClassName ("weapon_usp",       HalfLifeEngine::SDK::Constants::EntityVirtualTableFunctionID_Weapon_Reload, true, 0u, HalfLifeEngine::Export::ReplacedFunctions::EntityTable::Weapon_Reload);
	m_engineVirtualTableHooksManager.RegisterEngineVirtualTableHookByEntityClassName ("weapon_glock18",   HalfLifeEngine::SDK::Constants::EntityVirtualTableFunctionID_Weapon_Reload, true, 0u, HalfLifeEngine::Export::ReplacedFunctions::EntityTable::Weapon_Reload);
	m_engineVirtualTableHooksManager.RegisterEngineVirtualTableHookByEntityClassName ("weapon_awp",       HalfLifeEngine::SDK::Constants::EntityVirtualTableFunctionID_Weapon_Reload, true, 0u, HalfLifeEngine::Export::ReplacedFunctions::EntityTable::Weapon_Reload);
	m_engineVirtualTableHooksManager.RegisterEngineVirtualTableHookByEntityClassName ("weapon_mp5navy",   HalfLifeEngine::SDK::Constants::EntityVirtualTableFunctionID_Weapon_Reload, true, 0u, HalfLifeEngine::Export::ReplacedFunctions::EntityTable::Weapon_Reload);
	m_engineVirtualTableHooksManager.RegisterEngineVirtualTableHookByEntityClassName ("weapon_m249",      HalfLifeEngine::SDK::Constants::EntityVirtualTableFunctionID_Weapon_Reload, true, 0u, HalfLifeEngine::Export::ReplacedFunctions::EntityTable::Weapon_Reload);
	m_engineVirtualTableHooksManager.RegisterEngineVirtualTableHookByEntityClassName ("weapon_m3",        HalfLifeEngine::SDK::Constants::EntityVirtualTableFunctionID_Weapon_Reload, true, 0u, HalfLifeEngine::Export::ReplacedFunctions::EntityTable::Weapon_Shotgun_Reload);	// Register shotguns, we need different function for additional checks.
	m_engineVirtualTableHooksManager.RegisterEngineVirtualTableHookByEntityClassName ("weapon_m4a1",      HalfLifeEngine::SDK::Constants::EntityVirtualTableFunctionID_Weapon_Reload, true, 0u, HalfLifeEngine::Export::ReplacedFunctions::EntityTable::Weapon_Reload);
	m_engineVirtualTableHooksManager.RegisterEngineVirtualTableHookByEntityClassName ("weapon_tmp",       HalfLifeEngine::SDK::Constants::EntityVirtualTableFunctionID_Weapon_Reload, true, 0u, HalfLifeEngine::Export::ReplacedFunctions::EntityTable::Weapon_Reload);
	m_engineVirtualTableHooksManager.RegisterEngineVirtualTableHookByEntityClassName ("weapon_g3sg1",     HalfLifeEngine::SDK::Constants::EntityVirtualTableFunctionID_Weapon_Reload, true, 0u, HalfLifeEngine::Export::ReplacedFunctions::EntityTable::Weapon_Reload);
	m_engineVirtualTableHooksManager.RegisterEngineVirtualTableHookByEntityClassName ("weapon_deagle",    HalfLifeEngine::SDK::Constants::EntityVirtualTableFunctionID_Weapon_Reload, true, 0u, HalfLifeEngine::Export::ReplacedFunctions::EntityTable::Weapon_Reload);
	m_engineVirtualTableHooksManager.RegisterEngineVirtualTableHookByEntityClassName ("weapon_sg552",     HalfLifeEngine::SDK::Constants::EntityVirtualTableFunctionID_Weapon_Reload, true, 0u, HalfLifeEngine::Export::ReplacedFunctions::EntityTable::Weapon_Reload);
	m_engineVirtualTableHooksManager.RegisterEngineVirtualTableHookByEntityClassName ("weapon_ak47",      HalfLifeEngine::SDK::Constants::EntityVirtualTableFunctionID_Weapon_Reload, true, 0u, HalfLifeEngine::Export::ReplacedFunctions::EntityTable::Weapon_Reload);
	m_engineVirtualTableHooksManager.RegisterEngineVirtualTableHookByEntityClassName ("weapon_p90",       HalfLifeEngine::SDK::Constants::EntityVirtualTableFunctionID_Weapon_Reload, true, 0u, HalfLifeEngine::Export::ReplacedFunctions::EntityTable::Weapon_Reload);
/*
	// Weapon::PrimaryAttack()
	m_engineVirtualTableHooksManager.RegisterEngineVirtualTableHookByEntityClassName ("weapon_p228",      HalfLifeEngine::SDK::Constants::EntityVirtualTableFunctionID_Weapon_PrimaryAttack, true, 0u, HalfLifeEngine::Export::ReplacedFunctions::EntityTable::Weapon_PrimaryAttack);
	m_engineVirtualTableHooksManager.RegisterEngineVirtualTableHookByEntityClassName ("weapon_scout",     HalfLifeEngine::SDK::Constants::EntityVirtualTableFunctionID_Weapon_PrimaryAttack, true, 0u, HalfLifeEngine::Export::ReplacedFunctions::EntityTable::Weapon_PrimaryAttack);
//	m_engineVirtualTableHooksManager.RegisterEngineVirtualTableHookByEntityClassName ("weapon_xm1014",    HalfLifeEngine::SDK::Constants::EntityVirtualTableFunctionID_Weapon_PrimaryAttack, true, 0u, HalfLifeEngine::Export::ReplacedFunctions::EntityTable::Weapon_Shotgun_PrimaryAttack);	// Register shotguns, we need different function for additional checks.
	m_engineVirtualTableHooksManager.RegisterEngineVirtualTableHookByEntityClassName ("weapon_mac10",     HalfLifeEngine::SDK::Constants::EntityVirtualTableFunctionID_Weapon_PrimaryAttack, true, 0u, HalfLifeEngine::Export::ReplacedFunctions::EntityTable::Weapon_PrimaryAttack);
	m_engineVirtualTableHooksManager.RegisterEngineVirtualTableHookByEntityClassName ("weapon_aug",       HalfLifeEngine::SDK::Constants::EntityVirtualTableFunctionID_Weapon_PrimaryAttack, true, 0u, HalfLifeEngine::Export::ReplacedFunctions::EntityTable::Weapon_PrimaryAttack);
	m_engineVirtualTableHooksManager.RegisterEngineVirtualTableHookByEntityClassName ("weapon_elite",     HalfLifeEngine::SDK::Constants::EntityVirtualTableFunctionID_Weapon_PrimaryAttack, true, 0u, HalfLifeEngine::Export::ReplacedFunctions::EntityTable::Weapon_PrimaryAttack);
	m_engineVirtualTableHooksManager.RegisterEngineVirtualTableHookByEntityClassName ("weapon_fiveseven", HalfLifeEngine::SDK::Constants::EntityVirtualTableFunctionID_Weapon_PrimaryAttack, true, 0u, HalfLifeEngine::Export::ReplacedFunctions::EntityTable::Weapon_PrimaryAttack);
	m_engineVirtualTableHooksManager.RegisterEngineVirtualTableHookByEntityClassName ("weapon_ump45",     HalfLifeEngine::SDK::Constants::EntityVirtualTableFunctionID_Weapon_PrimaryAttack, true, 0u, HalfLifeEngine::Export::ReplacedFunctions::EntityTable::Weapon_PrimaryAttack);
	m_engineVirtualTableHooksManager.RegisterEngineVirtualTableHookByEntityClassName ("weapon_sg550",     HalfLifeEngine::SDK::Constants::EntityVirtualTableFunctionID_Weapon_PrimaryAttack, true, 0u, HalfLifeEngine::Export::ReplacedFunctions::EntityTable::Weapon_PrimaryAttack);
	m_engineVirtualTableHooksManager.RegisterEngineVirtualTableHookByEntityClassName ("weapon_galil",     HalfLifeEngine::SDK::Constants::EntityVirtualTableFunctionID_Weapon_PrimaryAttack, true, 0u, HalfLifeEngine::Export::ReplacedFunctions::EntityTable::Weapon_PrimaryAttack);
	m_engineVirtualTableHooksManager.RegisterEngineVirtualTableHookByEntityClassName ("weapon_famas",     HalfLifeEngine::SDK::Constants::EntityVirtualTableFunctionID_Weapon_PrimaryAttack, true, 0u, HalfLifeEngine::Export::ReplacedFunctions::EntityTable::Weapon_PrimaryAttack);
	m_engineVirtualTableHooksManager.RegisterEngineVirtualTableHookByEntityClassName ("weapon_usp",       HalfLifeEngine::SDK::Constants::EntityVirtualTableFunctionID_Weapon_PrimaryAttack, true, 0u, HalfLifeEngine::Export::ReplacedFunctions::EntityTable::Weapon_PrimaryAttack);
	m_engineVirtualTableHooksManager.RegisterEngineVirtualTableHookByEntityClassName ("weapon_glock18",   HalfLifeEngine::SDK::Constants::EntityVirtualTableFunctionID_Weapon_PrimaryAttack, true, 0u, HalfLifeEngine::Export::ReplacedFunctions::EntityTable::Weapon_PrimaryAttack);
	m_engineVirtualTableHooksManager.RegisterEngineVirtualTableHookByEntityClassName ("weapon_awp",       HalfLifeEngine::SDK::Constants::EntityVirtualTableFunctionID_Weapon_PrimaryAttack, true, 0u, HalfLifeEngine::Export::ReplacedFunctions::EntityTable::Weapon_PrimaryAttack);
	m_engineVirtualTableHooksManager.RegisterEngineVirtualTableHookByEntityClassName ("weapon_mp5navy",   HalfLifeEngine::SDK::Constants::EntityVirtualTableFunctionID_Weapon_PrimaryAttack, true, 0u, HalfLifeEngine::Export::ReplacedFunctions::EntityTable::Weapon_PrimaryAttack);
	m_engineVirtualTableHooksManager.RegisterEngineVirtualTableHookByEntityClassName ("weapon_m249",      HalfLifeEngine::SDK::Constants::EntityVirtualTableFunctionID_Weapon_PrimaryAttack, true, 0u, HalfLifeEngine::Export::ReplacedFunctions::EntityTable::Weapon_PrimaryAttack);
//	m_engineVirtualTableHooksManager.RegisterEngineVirtualTableHookByEntityClassName ("weapon_m3",        HalfLifeEngine::SDK::Constants::EntityVirtualTableFunctionID_Weapon_PrimaryAttack, true, 0u, HalfLifeEngine::Export::ReplacedFunctions::EntityTable::Weapon_Shotgun_PrimaryAttack);	// Register shotguns, we need different function for additional checks.
	m_engineVirtualTableHooksManager.RegisterEngineVirtualTableHookByEntityClassName ("weapon_m4a1",      HalfLifeEngine::SDK::Constants::EntityVirtualTableFunctionID_Weapon_PrimaryAttack, true, 0u, HalfLifeEngine::Export::ReplacedFunctions::EntityTable::Weapon_PrimaryAttack);
	m_engineVirtualTableHooksManager.RegisterEngineVirtualTableHookByEntityClassName ("weapon_tmp",       HalfLifeEngine::SDK::Constants::EntityVirtualTableFunctionID_Weapon_PrimaryAttack, true, 0u, HalfLifeEngine::Export::ReplacedFunctions::EntityTable::Weapon_PrimaryAttack);
	m_engineVirtualTableHooksManager.RegisterEngineVirtualTableHookByEntityClassName ("weapon_g3sg1",     HalfLifeEngine::SDK::Constants::EntityVirtualTableFunctionID_Weapon_PrimaryAttack, true, 0u, HalfLifeEngine::Export::ReplacedFunctions::EntityTable::Weapon_PrimaryAttack);
	m_engineVirtualTableHooksManager.RegisterEngineVirtualTableHookByEntityClassName ("weapon_deagle",    HalfLifeEngine::SDK::Constants::EntityVirtualTableFunctionID_Weapon_PrimaryAttack, true, 0u, HalfLifeEngine::Export::ReplacedFunctions::EntityTable::Weapon_PrimaryAttack);
	m_engineVirtualTableHooksManager.RegisterEngineVirtualTableHookByEntityClassName ("weapon_sg552",     HalfLifeEngine::SDK::Constants::EntityVirtualTableFunctionID_Weapon_PrimaryAttack, true, 0u, HalfLifeEngine::Export::ReplacedFunctions::EntityTable::Weapon_PrimaryAttack);
	m_engineVirtualTableHooksManager.RegisterEngineVirtualTableHookByEntityClassName ("weapon_ak47",      HalfLifeEngine::SDK::Constants::EntityVirtualTableFunctionID_Weapon_PrimaryAttack, true, 0u, HalfLifeEngine::Export::ReplacedFunctions::EntityTable::Weapon_PrimaryAttack);
	m_engineVirtualTableHooksManager.RegisterEngineVirtualTableHookByEntityClassName ("weapon_p90",       HalfLifeEngine::SDK::Constants::EntityVirtualTableFunctionID_Weapon_PrimaryAttack, true, 0u, HalfLifeEngine::Export::ReplacedFunctions::EntityTable::Weapon_PrimaryAttack);
*/}
void Server::UnregisterEngineVirtualTableHooks (void)
{
	// unhook "Round_Start" logging
//	Trampolines::RedirectFunctionCall32 (reinterpret_cast <void *> (reinterpret_cast <unsigned int> (HalfLifeEngine::Globals::g_halfLifeEngine->GetGameLibrary ()->GetPointer ()) + 0x97CEC + sizeof (Trampolines::Bytecode::CallImmediately32)), reinterpret_cast <const void *&> (m_originalUTIL_LogPrintfFunction));
	Trampolines::RedirectFunctionCall32 (MemoryUtilities::/*DecodeAnd*/FindPattern (*HalfLifeEngine::Globals::g_halfLifeEngine->GetGameLibrary (), HalfLifeEngine::SDK::Constants::signature_Calling_UTIL_LogPrintf_RoundStart, sizeof (HalfLifeEngine::SDK::Constants::signature_Calling_UTIL_LogPrintf_RoundStart)), m_originalUTIL_LogPrintfFunction);

	m_engineVirtualTableHooksManager.DeleteAllHooks ();
}

void Server::DetectMapType (void)
{
	m_mapType = MapType_None;	// Reset map type.

	// the following maps doesn't have map-specific entities, so determine it by map name
//	{
		const DynamicString mapName (GetMapName ());

		if (mapName.CompareWithoutCaseCheck ("ka_", GET_STATIC_STRING_LENGTH ("ka_")) == 0)
			m_mapType |= MapType_Ka;	// knife arena map
		else if (mapName.CompareWithoutCaseCheck ("fy_", GET_STATIC_STRING_LENGTH ("fy_")) == 0)
			m_mapType |= MapType_Fy;	// fun map
		else if (mapName.CompareWithoutCaseCheck ("gg_", GET_STATIC_STRING_LENGTH ("gg_")) == 0)
			m_mapType |= MapType_Gg;	// map for gun game mod
		else if (mapName.CompareWithoutCaseCheck ("aim_", GET_STATIC_STRING_LENGTH ("aim_")) == 0)
			m_mapType |= MapType_Aim;	// aim map
		else if (mapName.CompareWithoutCaseCheck ("dm_", GET_STATIC_STRING_LENGTH ("dm_")) == 0)
			m_mapType |= MapType_Dm;	// deathmatch map
		else if (mapName.CompareWithoutCaseCheck ("kz_", GET_STATIC_STRING_LENGTH ("kz_")) == 0)
			m_mapType |= MapType_Kz;	// kreedz jumping map
		else if (mapName.CompareWithoutCaseCheck ("zm_", GET_STATIC_STRING_LENGTH ("zm_")) == 0)
			m_mapType |= MapType_Zm;	// map for zombie mod
		else if (mapName.CompareWithoutCaseCheck ("awp_", GET_STATIC_STRING_LENGTH ("awp_")) == 0)
			m_mapType |= MapType_Awp;	// awp map
		else if (mapName.CompareWithoutCaseCheck ("he_", GET_STATIC_STRING_LENGTH ("he_")) == 0)
			m_mapType |= MapType_He;	// he map
//	}

	// Find the VIP safety zone entities....
	if (HalfLifeEngine::Globals::g_halfLifeEngine->FIND_ENTITY_BY_CLASSNAME (NULL, "func_vip_safetyzone")->IsValid () || HalfLifeEngine::Globals::g_halfLifeEngine->FIND_ENTITY_BY_CLASSNAME (NULL, "info_vip_safetyzone")->IsValid ())
		m_mapType |= MapType_As;	// assassination map

	// Find the hostage entitities....
	if (m_hostageManager.GetHostagesNumber () > 0u)
		m_mapType |= MapType_Cs;	// rescue map

	// Find the c4 target entitities....
	if (HalfLifeEngine::Globals::g_halfLifeEngine->FIND_ENTITY_BY_CLASSNAME (NULL, "func_bomb_target")->IsValid () || HalfLifeEngine::Globals::g_halfLifeEngine->FIND_ENTITY_BY_CLASSNAME (NULL, "info_bomb_target")->IsValid ())
		m_mapType |= MapType_De;	// defusion map

	// Find the escape zone entitities....
	if (HalfLifeEngine::Globals::g_halfLifeEngine->FIND_ENTITY_BY_CLASSNAME (NULL, "func_escapezone")->IsValid ())
		m_mapType |= MapType_Es;	// escape map

	#if defined _DEBUG
		DynamicString mapType;
		const DynamicString types[MapTypeID_Total] =
		{
			"as",
			"cs",
			"de",
			"es",
			"ka",
			"fy",
			"gg",
			"aim",
			"dm",
			"kz",
			"zm",
			"awp",
			"he"
		};

		STORE_FLAGS_TO_STRING (m_mapType, mapType, MapTypeID_As, MapTypeID_Total, types, "+");

		HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat ("Server::DetectMapType(): Map \"%s\" type is: \"%s\".\n", mapName.GetData (), mapType.IsEmpty () ? "NONE" : mapType.GetData ());
	#endif	// if defined _DEBUG
}

void Server::RoundStarted (void)
{
	// This function is called at the start of each round.

//	HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "Server::RoundStarted() called! Teams scores: T's=%u, CT's=%u, can buy: T's: %s, CT's: %s.\n", GetTeamScore (TeamArrayID_Terrorist), GetTeamScore (TeamArrayID_CounterTerrorist), IsTeamCanBuy (TeamArrayID_Terrorist) ? "true" : "false", IsTeamCanBuy (TeamArrayID_CounterTerrorist) ? "true" : "false");

	m_round.Start ();

	m_navigationMeshManager.RemovePlayers ();

	m_hostageManager.RoundStarted ();

	m_clientManager->RoundStarted ();
	m_YaPBManager->RoundStarted ();
/*
//	g_waypoint->SetBombPosition (true);	// reset bomb position
	g_waypoint->ClearGoalScore ();

	g_lastRadioTime[TeamArrayID_Terrorist] = 0.0f;
	g_lastRadioTime[TeamArrayID_CounterTerrorist] = 0.0f;

	g_bombSayString = false;
	g_timeNextBombUpdate = 0.0f;
	g_botsCanPause = false;

	g_leaderChoosen[TeamArrayID_Terrorist] = false;
	g_leaderChoosen[TeamArrayID_CounterTerrorist] = false;

	// update experience data on round start
	UpdateGlobalExperienceData ();*/
}

void Server::RoundEnded (void)
{
	// This function is called at the end of each round (i.e. after next messages: Terrorists Win!, Counter-Terrorists Win! or something else....).

	m_round.End ();

	m_YaPBManager->RoundEnded ();

	m_c4Manager.RoundEnded ();
}

void Server::OnGameEvent (const HalfLifeEngine::SDK::Constants::GameEventType_t gameEvent, const HalfLifeEngine::SDK::Classes::BaseEntity *const argument1, const void *const argument2)
{
/*	#if defined _DEBUG
		AddLogEntry (false, LogLevel_Default, true, "Server::OnGameEvent(): called for game event = %u.", gameEvent);
//		AddLogEntry (false, LogLevel_Default, true, "Server::OnGameEvent(): argument1: \"%s\", argument2: \"%s\".", argument1 == NULL ? "NULL" : argument1->IsPlayer () ? argument1->GetNetName ().GetData () : argument1->GetClassName ().GetData (), argument2 == NULL ? "NULL" : argument2->IsPlayer () ? argument2->GetNetName ().GetData () : argument2->GetClassName ().GetData ());
//		HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "Server::OnGameEvent(): gameEvent=%u, argument1: \"%s\", argument2: \"%s\".\n", gameEvent, argument1 == NULL ? "NULL" : argument1->IsPlayer () ? argument1->GetNetName ().GetData () : argument1->GetClassName ().GetData (), argument2 == NULL ? "NULL" : gameEvent == HalfLifeEngine::SDK::Constants::GameEventType_BULLET_IMPACT ? static_cast <const Math::Vector3D *const> (argument2)->ToString () : argument2->IsPlayer () ? argument2->GetNetName ().GetData () : argument2->GetClassName ().GetData ());
	#endif	// if defined _DEBUG
*/
	switch (gameEvent)
	{
		case HalfLifeEngine::SDK::Constants::GameEventType_WEAPON_FIRED:				// tell bots the player is attack
		case HalfLifeEngine::SDK::Constants::GameEventType_WEAPON_FIRED_ON_EMPTY:		// tell bots the player is attack without clip ammo
		case HalfLifeEngine::SDK::Constants::GameEventType_WEAPON_RELOADED:				// tell bots the player is reloading his weapon
		case HalfLifeEngine::SDK::Constants::GameEventType_HE_GRENADE_EXPLODED:			// tell bots the HE grenade is exploded
			//! @todo MAKE SOME CALLBACKS HERE!

			break;

		case HalfLifeEngine::SDK::Constants::GameEventType_FLASHBANG_GRENADE_EXPLODED:	// tell bots the flashbang grenade is exploded
		{
			// Reliability checks.
			InternalAssert (argument1 != NULL);
			InternalAssert (argument2 != NULL);

			const HalfLifeEngine::SDK::Classes::Edict *const grenadeThrower (argument1->GetEdict ());
//			const Math::Vector3D &explosionOrigin (*static_cast <const Math::Vector3D *const> (argument2));

			// Reliability checks.
			InternalAssert (grenadeThrower->IsValid ());
			InternalAssert (grenadeThrower->IsNotWorldspawnPlayer ());
/*
			#if defined _DEBUG
				HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "Server::OnGameEvent(): gameEvent: FLASHBANG_EXPLODED, grenade thrower: \"%s\", explosion origin: %s.\n", grenadeThrower->GetNetName ().GetData (), explosionOrigin.ToString ());
			#endif	// if defined _DEBUG
*/
			//! @todo MAKE SOME CALLBACKS HERE!

			break;
		}

		case HalfLifeEngine::SDK::Constants::GameEventType_SMOKE_GRENADE_EXPLODED:		// tell bots the smoke grenade is exploded

		case HalfLifeEngine::SDK::Constants::GameEventType_PLAYER_BLINDED_BY_FLASHBANG:	// tell bots the player is flashed
		case HalfLifeEngine::SDK::Constants::GameEventType_PLAYER_FOOTSTEP:				// tell bots the player is running
		case HalfLifeEngine::SDK::Constants::GameEventType_PLAYER_JUMPED:				// tell bots the player is jumped
		case HalfLifeEngine::SDK::Constants::GameEventType_PLAYER_DIED:					// tell bots the player is killed
		case HalfLifeEngine::SDK::Constants::GameEventType_PLAYER_LANDED_FROM_HEIGHT:	// tell bots the player is fell with some damage
		case HalfLifeEngine::SDK::Constants::GameEventType_PLAYER_TOOK_DAMAGE:			// tell bots the player is take damage
		case HalfLifeEngine::SDK::Constants::GameEventType_HOSTAGE_DAMAGED:				// tell bots the player has injured a hostage
		case HalfLifeEngine::SDK::Constants::GameEventType_HOSTAGE_KILLED:				// tell bots the player has killed a hostage

		case HalfLifeEngine::SDK::Constants::GameEventType_DOOR:					// tell bots the door is moving

		case HalfLifeEngine::SDK::Constants::GameEventType_BREAK_GLASS:					// tell bots the glass has break
		case HalfLifeEngine::SDK::Constants::GameEventType_BREAK_WOOD:					// tell bots the wood has break
		case HalfLifeEngine::SDK::Constants::GameEventType_BREAK_METAL:					// tell bots the metal/computer has break
		case HalfLifeEngine::SDK::Constants::GameEventType_BREAK_FLESH:					// tell bots the flesh has break
		case HalfLifeEngine::SDK::Constants::GameEventType_BREAK_CONCRETE:				// tell bots the concrete has break

		case HalfLifeEngine::SDK::Constants::GameEventType_BOMB_PLANTED:				// tell bots the bomb has been planted
		case HalfLifeEngine::SDK::Constants::GameEventType_BOMB_DROPPED:				// tell bots the bomb has been dropped
		case HalfLifeEngine::SDK::Constants::GameEventType_BOMB_PICKED_UP:				// let the bots hear the bomb beeping
		case HalfLifeEngine::SDK::Constants::GameEventType_BOMB_BEEP:					// let the bots hear the bomb beeping
		case HalfLifeEngine::SDK::Constants::GameEventType_BOMB_DEFUSING:				// tell the bots someone has started defusing
		case HalfLifeEngine::SDK::Constants::GameEventType_BOMB_DEFUSE_ABORTED:			// tell the bots someone has aborted defusing
		case HalfLifeEngine::SDK::Constants::GameEventType_BOMB_DEFUSED:				// tell the bots the bomb is defused
		case HalfLifeEngine::SDK::Constants::GameEventType_BOMB_EXPLODED:				// let the bots hear the bomb exploding

		case HalfLifeEngine::SDK::Constants::GameEventType_HOSTAGE_USED:				// tell bots the hostage is used
			//! @todo MAKE SOME CALLBACKS HERE!

			break;

		case HalfLifeEngine::SDK::Constants::GameEventType_HOSTAGE_RESCUED:				// tell bots the hostage is rescued
			// Reliability checks.
			InternalAssert (argument2 != NULL);

			if (argument1 == NULL)
				m_hostageManager.HostageEscaped (static_cast <const HalfLifeEngine::SDK::Classes::BaseEntity *const> (argument2)->GetEdict ());
			else
				m_hostageManager.HostageRescued (static_cast <const HalfLifeEngine::SDK::Classes::BaseEntity *const> (argument2)->GetEdict (), m_clientManager->GetClient (argument1->GetEdict ()));

			break;

		case HalfLifeEngine::SDK::Constants::GameEventType_ALL_HOSTAGES_RESCUED:		// tell bots the all hostages are rescued

			m_hostageManager.AllHostagesRescued ();

			break;

		case HalfLifeEngine::SDK::Constants::GameEventType_VIP_ESCAPED:					// tell bots the VIP is escaped
		case HalfLifeEngine::SDK::Constants::GameEventType_VIP_ASSASSINATED:			// tell bots the VIP is assassinated

		case HalfLifeEngine::SDK::Constants::GameEventType_TERRORISTS_WIN:				// tell bots the terrorists won the round
		case HalfLifeEngine::SDK::Constants::GameEventType_CTS_WIN:						// tell bots the CTs won the round
			//! @todo MAKE SOME CALLBACKS HERE!

			break;

		case HalfLifeEngine::SDK::Constants::GameEventType_ROUND_DRAW:					// tell bots the round was a draw
			// Reliability checks.
			InternalAssert (argument1 == NULL);
			InternalAssert (argument2 == NULL);
/*
			#if defined _DEBUG
				HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "Server::OnGameEvent(): gameEvent: ROUND_DRAW!\n");
			#endif	// if defined _DEBUG
*/
			//! @todo MAKE SOME CALLBACKS HERE!

			break;

		case HalfLifeEngine::SDK::Constants::GameEventType_ROUND_START:				// tell bots the round was started (when freeze period is expired)
		case HalfLifeEngine::SDK::Constants::GameEventType_PLAYER_SPAWNED:			// tell bots the player is spawned
		case HalfLifeEngine::SDK::Constants::GameEventType_BUY_TIME_START:			// tell bots the round was restarted
		case HalfLifeEngine::SDK::Constants::GameEventType_DEATH_CAMERA_START:		// tell bots the player is sart observer
		case HalfLifeEngine::SDK::Constants::GameEventType_TUTOR_BUY_MENU_OPENNED:	// tell bots the buy menu is opened

		// Radio commands....
			case HalfLifeEngine::SDK::Constants::GameEventType_RADIO_1_Cover_me:					// 
			case HalfLifeEngine::SDK::Constants::GameEventType_RADIO_1_You_take_the_point:			// 
			case HalfLifeEngine::SDK::Constants::GameEventType_RADIO_1_Hold_this_position:			// 
			case HalfLifeEngine::SDK::Constants::GameEventType_RADIO_1_Regroup_team:				// 
			case HalfLifeEngine::SDK::Constants::GameEventType_RADIO_1_Follow_me:					// 
			case HalfLifeEngine::SDK::Constants::GameEventType_RADIO_1_Taking_fire:					// 

			case HalfLifeEngine::SDK::Constants::GameEventType_RADIO_2_Go_go_go:					// 
			case HalfLifeEngine::SDK::Constants::GameEventType_RADIO_2_Team_fall_back:				// 
			case HalfLifeEngine::SDK::Constants::GameEventType_RADIO_2_Stick_together_team:			// 
			case HalfLifeEngine::SDK::Constants::GameEventType_RADIO_2_Get_in_position_and_wait:	// 
			case HalfLifeEngine::SDK::Constants::GameEventType_RADIO_2_Storm_the_front:				// 
			case HalfLifeEngine::SDK::Constants::GameEventType_RADIO_2_Report_in_team:				// 

			case HalfLifeEngine::SDK::Constants::GameEventType_RADIO_3_Affirmative:					// 
			case HalfLifeEngine::SDK::Constants::GameEventType_RADIO_3_Enemy_spotted:				// 
			case HalfLifeEngine::SDK::Constants::GameEventType_RADIO_3_Need_backup:					// 
			case HalfLifeEngine::SDK::Constants::GameEventType_RADIO_3_Sector_clear:				// 
			case HalfLifeEngine::SDK::Constants::GameEventType_RADIO_3_In_position:					// 
			case HalfLifeEngine::SDK::Constants::GameEventType_RADIO_3_Reporting_in:				// 
			case HalfLifeEngine::SDK::Constants::GameEventType_RADIO_3_Get_out_of_there:			// 
			case HalfLifeEngine::SDK::Constants::GameEventType_RADIO_3_Negative:					// 
			case HalfLifeEngine::SDK::Constants::GameEventType_RADIO_3_Enemy_down:					// 

		case HalfLifeEngine::SDK::Constants::GameEventType_NEW_MATCH:					// tell bots the game is reset
			//! @todo MAKE SOME CALLBACKS HERE!

			break;

		/// DON'T USE ME!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		case HalfLifeEngine::SDK::Constants::GameEventType_PLAYER_CHANGED_TEAM:			// tell bots the player is switch his team
		{
/*			// Reliability checks.
			InternalAssert (argument1 != NULL);

			const HalfLifeEngine::SDK::Classes::Edict *const player (argument1->GetEdict ());

			// Reliability checks.
			InternalAssert (player->IsValid ());
			InternalAssert (player->IsNotWorldspawnPlayer ());

			#if defined _DEBUG
				HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "Server::OnGameEvent(): gameEvent: PLAYER_SWITCH_TEAM, player: \"%s\", new team = %u.\n", player->GetNetName ().GetData (), player->GetPlayerTeam ());
			#endif	// if defined _DEBUG

			m_clientManager->ClientTeamSwitched (player);
*/
			break;
		}

		case HalfLifeEngine::SDK::Constants::GameEventType_BULLET_IMPACT:			// tell bots the player is shoot at wall
		{
			// Reliability checks.
			InternalAssert (argument1 != NULL);
			InternalAssert (argument2 != NULL);

			const HalfLifeEngine::SDK::Classes::Edict *const player (argument1->GetEdict ());
//			const Math::Vector3D &shootTraceEndPosition (*static_cast <const Math::Vector3D *const> (argument2));

			// Reliability checks.
			InternalAssert (player->IsValid ());
			InternalAssert (player->IsNotWorldspawnPlayer ());
/*
			#if defined _DEBUG
				HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "Server::OnGameEvent(): gameEvent: PLAYER_SHOOT_AT_WALL, player: \"%s\", shoot trace end position: %s.\n", player->GetNetName ().GetData (), shootTraceEndPosition.ToString ());
			#endif	// if defined _DEBUG
*/
			//! @todo MAKE SOME CALLBACKS HERE!

			break;
		}

		case HalfLifeEngine::SDK::Constants::GameEventType_GAME_COMMENCE:				// tell bots the game is commencing
		case HalfLifeEngine::SDK::Constants::GameEventType_WEAPON_ZOOMED:				// tell bots the player is switch weapon zoom
		case HalfLifeEngine::SDK::Constants::GameEventType_HOSTAGE_CALLED_FOR_HELP:		// tell bots the hostage is talking
			//! @todo MAKE SOME CALLBACKS HERE!

			break;

		#if defined _DEBUG
			// Error!
			default:
				AddLogEntry (false, LogLevel_Critical, true, "Server::OnGameEvent(): Function called for unrecognized game event type (%u).", gameEvent);
		#endif	// if defined _DEBUG
	}
}