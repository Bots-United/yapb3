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
// Engine.hpp
//
// Class: Engine
//
// Description: Class for convenient work with Half-Life engine.
//
// Version: $ID:$
//

#include <Core.hpp>

#include <assert.h>

// "ScreenShake", "ScreenFade"
const unsigned char signature_ClientEngineFunctionTable[] = {0x83, 0xC4, 0x04, 0x68, 0x2A, 0x2A, 0x2A, 0x2A, 0x68, 0x2A, 0x2A, 0x2A, 0x2A, 0xE8, 0x2A, 0x2A, 0x2A, 0x2A, 0x68, 0x2A, 0x2A, 0x2A, 0x2A, 0x68, 0x2A, 0x2A, 0x2A, 0x2A, 0xE8, 0x2A, 0x2A, 0x2A, 0x2A, 0x6A, 0x07, 0x68, 0x2A, 0x2A, 0x2A, 0x2A};
enum { Offset_ClientEngineFunctionTable_Pointer = 36u };

// "qconsole.log" in Con_Printf() function
const unsigned char signature_RealTime[] = {0x68, 0x00, 0x01, 0x00, 0x00, 0x51, 0x68, 0x2A, 0x2A, 0x2A, 0x2A, 0xE8, 0x2A, 0x2A, 0x2A, 0x2A, 0xDD, 0x05};
enum { Offset_RealTime_Pointer = sizeof (signature_RealTime) };

// "VFileSystem009" in FileSystem_LoadDLL() function
const unsigned char signature_FileSystem[] = {0x6A, 0x00, 0x68, 0x2A, 0x2A, 0x2A, 0x2A, 0xFF, 0xD0, 0x83, 0xC4, 0x08, 0xA3, 0x2A, 0x2A, 0x2A, 0x2A, 0x85, 0xC0, 0x0F, 0x95, 0xC0, 0x5E};
enum { Offset_FileSystem_Pointer = GET_STATIC_STRING_LENGTH ("\x6A\x00\x68\x2A\x2A\x2A\x2A\xFF\xD0\x83\xC4\x08\xA3") };

// "Dropping fakeclient on level change" in SV_InactivateClients() function
const unsigned char signature_SV_InactivateClients[] = {0xA1, 0x2A, 0x2A, 0x2A, 0x2A, 0x53, 0x56, 0x8B, 0x35, 0x2A, 0x2A, 0x2A, 0x2A, 0x57, 0x33, 0xFF, 0x33, 0xDB, 0x3B, 0xC7, 0x7E, 0x7D};

const unsigned char signature_GL_DisableMultitexture[] = {0xA1, 0x2A, 0x2A, 0x2A, 0x2A, 0x85, 0xC0, 0x74, 0x23, 0x68, 0xE1, 0x0D, 0x00, 0x00};

inline HalfLifeEngine::SDK::TypeDefinitions::SV_CalcPingFunction_t GetSV_CalcPingFunction (const LibraryHandleWithSize &engineLibrary)
{
	const unsigned char string[] = {"%4i %s\n"};	// From Host_Ping_f() function.
	unsigned char *address (static_cast <unsigned char *> (MemoryUtilities::FindPatternWithoutWildCard (engineLibrary, string, GET_STATIC_STRING_LENGTH (string))));

	if (address == NULL)
		return NULL;

	address = static_cast <unsigned char *> (MemoryUtilities::FindMemoryChunkReference32 (engineLibrary, address, Trampolines::Bytecode::Push32));

	if (address == NULL)
		return NULL;

	address -= GET_STATIC_STRING_LENGTH ("\x4D\x93\x04\x00\x83\xC4\x04\x50");

	return static_cast <HalfLifeEngine::SDK::TypeDefinitions::SV_CalcPingFunction_t> (Trampolines::GetRealAddressOfRelativeAddress32 (address));
}
inline HalfLifeEngine::SDK::Classes::Interface::Engine *const GetInterface_Engine (const LibraryHandleWithSize &engineLibrary)
{
	const unsigned char signature[] =
	{
		Trampolines::Bytecode::CallImmediately32, Trampolines::Bytecode::WildCard, Trampolines::Bytecode::WildCard, Trampolines::Bytecode::WildCard, Trampolines::Bytecode::WildCard,													// call    Sys_InitArgv     ; Call Procedure
		Trampolines::Bytecode::MoveValue32ToECX[0u], Trampolines::Bytecode::MoveValue32ToECX[1u], Trampolines::Bytecode::WildCard, Trampolines::Bytecode::WildCard, Trampolines::Bytecode::WildCard, Trampolines::Bytecode::WildCard,	// mov     ecx, 'eng'
		Trampolines::Bytecode::AddESP8[0u], Trampolines::Bytecode::AddESP8[1u], 0x10,																																					// add     esp, 10h        ; Add
		0x8B, 0x01,																																																						// mov     eax, [ecx]
		Trampolines::Bytecode::Push8, 0x00,																																																// push    0
		0xFF, 0x50, 0x40																																																				// call    dword ptr [eax+40h] ; Indirect Call Near Procedure
	};	// Below "Sys_InitArgv( OrigCmd )" string from RunListenServer() function.
	unsigned char *address (static_cast <unsigned char *> (MemoryUtilities::FindPattern (engineLibrary, signature, sizeof (signature))));

	if (address == NULL)
		return NULL;

	address += Trampolines::Bytecode::CallImmediately32Size;		// call    Sys_InitArgv     ; Call Procedure
	address += sizeof (Trampolines::Bytecode::MoveValue32ToECX);	// mov  ecx, ....

	return MemoryUtilities::GetElementAddressFromOtherAddress <HalfLifeEngine::SDK::Classes::Interface::Engine *const> (address);
}
inline HalfLifeEngine::SDK::Classes::Interface::CDAudio *const GetInterface_CDAudio (const HalfLifeEngine::SDK::Classes::Interface::Engine *const engine)
{
	if (engine == NULL)
		return NULL;

	const VoidBaseHookedClassWithVirtualFunctionTable *const eng (reinterpret_cast <const VoidBaseHookedClassWithVirtualFunctionTable *> (engine));
	const unsigned char CEngine__Frame_FunctionIndex (7u);

	const unsigned char *const CEngine__Frame_FunctionAddress (static_cast <const unsigned char *const> (eng->GetPseudoVirtualFunctionTable ()[CEngine__Frame_FunctionIndex]));
	const unsigned char offset
	(
		GET_STATIC_STRING_LENGTH
		(
			"\x83\xEC\x08"	// sub     esp, 8           ; Integer Subtraction
			"\x56"			// push    esi
			"\x8B\xF1"		// mov     esi, ecx
			"\x8B\x0D"		// mov     ecx, off_1E6656C ; (only part)
		)
	);

	// Reliability check.
	assert (reinterpret_cast <const unsigned char *const> (CEngine__Frame_FunctionAddress)[0u] == 0x83);

	// (Address: 0x1E6656C)
	return MemoryUtilities::GetElementAddressFromOtherAddress <HalfLifeEngine::SDK::Classes::Interface::CDAudio *> (CEngine__Frame_FunctionAddress + offset);
}
#if 0
typedef HMODULE (WINAPI *LoadLibraryA_t) (const char *const name);
LoadLibraryA_t g_originalLoadLibraryA (NULL);
HMODULE WINAPI LoadLibraryA_MY (const char *const name)
{
	const DynamicString nameString (name);
	const bool isVoiceCodec (nameString.CompareWithoutCaseCheck ("voice_", GET_STATIC_STRING_LENGTH ("voice_")) == 0);
/*
	if (isVoiceCodec && g_server->GetYaPBManager ()->GetVoiceCodecDLLWrapper ().Initialized ())
		return g_server->GetYaPBManager ()->GetVoiceCodecDLLWrapper ().GetVoiceCodecDLL ();
*/
	HMODULE result (g_originalLoadLibraryA (name));

	if (isVoiceCodec)
		InternalAssert (result == g_server->GetYaPBManager ()->GetVoiceCodecDLLWrapper ().GetVoiceCodecDLL ());

	HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat ("LoadLibraryA_MY(): called for \"%s\", isVoiceCodec=%u, result: \"%s\"!\n", name, isVoiceCodec, result == NULL ? "NULL" : MemoryUtilities::GetModuleName (result).GetData ());

	return result;
}
typedef BOOL (WINAPI *FreeLibrary_t) (HMODULE module);
FreeLibrary_t g_originalFreeLibrary (NULL);
BOOL WINAPI FreeLibrary_MY (HMODULE module)
{
	const DynamicString nameString (module == NULL ? "NULL" : MemoryUtilities::GetModuleName (module).GetData ());
	const bool isVoiceCodec (nameString.CompareWithoutCaseCheck ("voice_", GET_STATIC_STRING_LENGTH ("voice_")) == 0);
	BOOL result (g_originalFreeLibrary (module));

	HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat ("FreeLibrary_MY(): called for \"%s\", isVoiceCodec=%u, result = %u!\n", nameString.GetData (), isVoiceCodec, result);

	return result;
}
#endif	// if 0
typedef void (WINAPI *OutputDebugStringA_t) (const char *const outputString);
OutputDebugStringA_t g_originalOutputDebugStringA (NULL);
void WINAPI OutputDebugStringA_MY (const char *const outputString)
{
	MessageBox (NULL, FormatBuffer ("OutputDebugStringA_MY() called for \"%s\"!", outputString), "YaPB Critical Error", MB_ICONASTERISK | MB_TOPMOST);

//DON'T WORK	HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat ("OutputDebugStringA_MY(): called for \"%s\"!\n", outputString);

	g_originalOutputDebugStringA (outputString);
}

typedef void (*SV_DropClient_DroppingFakeclientOnLevelChange_t) (HalfLifeEngine::SDK::Structures::Client_t *engineFakeClient, const bool crash, const char *const reason);
SV_DropClient_DroppingFakeclientOnLevelChange_t g_originalSV_DropClient_DroppingFakeclientOnLevelChange (NULL);
void *g_SV_DropClient_DroppingFakeclientOnLevelChangeCallAddress (NULL);
inline void SV_DropClient_DroppingFakeclientOnLevelChange (HalfLifeEngine::SDK::Structures::Client_t *engineFakeClient, const bool crash, const char *const reason)
{
//	MessageBox (NULL, FormatBuffer ("SV_DropClient_DroppingFakeclientOnLevelChange() called for \"%s\"!", engineFakeClient->name), "YaPB Critical Error", MB_ICONASTERISK);

	// Reliability checks.
	InternalAssert (g_server->IsRunning ());
	InternalAssert (engineFakeClient->edict->IsValid ());

	FakeClient *const fakeClient (g_server->GetClientManager ()->GetClient (engineFakeClient->edict)->GetFakeClientPointer ());

	// Reliability check.
	InternalAssert (fakeClient->IsValid ());

	if (fakeClient->IsYaPB ())
	{
		// Save this bot profile....
		g_server->GetYaPBManager ()->GetCreationTable () += *fakeClient->GetYaPBPointer ()->GetProfile ();
/*
		g_connectedYaPBs += ConnectedYaPB_t (fakeClient->GetYaPBPointer (), &fakeClient->GetEngineClient ());

		// Call part of original function for real client....
		{
			const unsigned char pattern[] =
			{
				0xA1, 0x48, 0xD0, 0x4C, 0x02,		// mov     eax, dword_24CD048
				0x53,								// push    ebx
				0x56,								// push    esi
				0x8B, 0x35, 0x44, 0xD0, 0x4C, 0x02,	// mov     esi, dword_24CD044
				0x57,								// push    edi
				0x33, 0xFF,							// xor     edi, edi        ; Logical Exclusive OR
				0x33, 0xDB,							// xor     ebx, ebx        ; Logical Exclusive OR
				0x3B, 0xC7,							// cmp     eax, edi        ; Compare Two Operands
				0x7E, 0x7D,							// jle     short loc_1DB6FE3 ; Jump if Less or Equal (ZF=1 | SF!=OF)

				// loc_1DB6F66:
				0x39, 0x3E,							// cmp     [esi], edi      ; Compare Two Operands
				0x75, 0x0A,							// jnz     short loc_1DB6F74 ; Jump if Not Zero (ZF=0)
				0x39, 0x7E, 0x0C,					// cmp     [esi+0Ch], edi  ; Compare Two Operands
				0x75, 0x05,							// jnz     short loc_1DB6F74 ; Jump if Not Zero (ZF=0)
				0x39, 0x7E, 0x04,					// cmp     [esi+4], edi    ; Compare Two Operands
				0x74, 0x5F,							// jz      short loc_1DB6FD3 ; Jump if Zero (ZF=1)

				// loc_1DB6F74:
				0x39, 0xBE, 0x48, 0x25, 0x00, 0x00,	// cmp     [esi+2548h], edi ; Compare Two Operands
				0x74, 0x11,							// jz      short loc_1DB6F8D ; Jump if Zero (ZF=1)
				0x68, 0x5C, 0x99, 0xE8, 0x01,		// push    offset aDroppingFakecl ; "Dropping fakeclient on level change"
				0x57,								// push    edi             ; int
				0x56,								// push    esi             ; int
				0xE8, 0x48, 0xAE, 0xFA, 0xFF,		// call    sub_1D61DD0     ; Call Procedure
				0x83, 0xC4, 0x0C,					// add     esp, 0Ch
				0xEB, 0x46							// jmp     short loc_1DB6FD3 ; Jump
			};
			const unsigned char *const jumpToAddress (reinterpret_cast <const unsigned char *const> (HalfLifeEngine::Globals::g_halfLifeEngine->GetSV_InactivateClientsFunction ()) + sizeof (pattern));

			// Don't call original function - don't kick YaPB!!!
			__asm
			{
				jmp jumpToAddress;
			}

#error "TODO: MAKE CALL OF ORIGINAL CLIENT CODE FOR BOT!!!!!!!!!!!!!!!"
		}
*/	}

	g_originalSV_DropClient_DroppingFakeclientOnLevelChange (engineFakeClient, crash, reason);
}
inline void RedirectCallFromSV_InactivateClientsFunction (const LibraryHandleWithSize &engineLibrary)
{
	const unsigned char string[] = {"Dropping fakeclient on level change"};	// From SV_InactivateClients() function.
	unsigned char *address (static_cast <unsigned char *> (MemoryUtilities::FindPatternWithoutWildCard (engineLibrary, string, GET_STATIC_STRING_LENGTH (string))));

	if (address == NULL)
		return;

	address = static_cast <unsigned char *> (MemoryUtilities::FindMemoryChunkReference32 (engineLibrary, address, Trampolines::Bytecode::Push32));

	if (address == NULL)
		return;

	address += Trampolines::Bytecode::Push32Size;
	address += sizeof (Trampolines::Bytecode::PushEDI);
	address += sizeof (Trampolines::Bytecode::PushESI);

	assert (*address == Trampolines::Bytecode::CallImmediately32);

	address += sizeof (Trampolines::Bytecode::CallImmediately32);

	g_SV_DropClient_DroppingFakeclientOnLevelChangeCallAddress = reinterpret_cast <void *> (address);

	Trampolines::RedirectFunctionCall32 (address, &SV_DropClient_DroppingFakeclientOnLevelChange, reinterpret_cast <const void *&> (g_originalSV_DropClient_DroppingFakeclientOnLevelChange));
}
inline void RestoreSV_InactivateClientsFunction (void)
{
	// Reliability check.
	InternalAssert (g_SV_DropClient_DroppingFakeclientOnLevelChangeCallAddress != NULL);

	Trampolines::RedirectFunctionCall32 (g_SV_DropClient_DroppingFakeclientOnLevelChangeCallAddress, g_originalSV_DropClient_DroppingFakeclientOnLevelChange);
}
/*inline void PatchSV_InactivateClientsFunction (HalfLifeEngine::SDK::TypeDefinitions::SV_InactivateClientsFunction_t function)
{
	// Disable fakeclient kicking.... (Comment 'SV_DropClient (fakeclient, false, "Dropping fakeclient on level change");' line)

#pragma warning (push)
#pragma warning (disable: 4189)	// A variable is declared and initialized but not used.
	unsigned char *const address (reinterpret_cast <unsigned char *> (function));
	const unsigned char sig1[] =
	{
		0xA1, 0x48, 0xD0, 0x4C, 0x02,		// mov     eax, dword_24CD048
		0x53,								// push    ebx
		0x56,								// push    esi
		0x8B, 0x35, 0x44, 0xD0, 0x4C, 0x02,	// mov     esi, dword_24CD044
		0x57,								// push    edi
		0x33, 0xFF,							// xor     edi, edi        ; Logical Exclusive OR
		0x33, 0xDB,							// xor     ebx, ebx        ; Logical Exclusive OR
		0x3B, 0xC7,							// cmp     eax, edi        ; Compare Two Operands
		0x7E, 0x7D,							// jle     short loc_1DB6FE3 ; Jump if Less or Equal (ZF=1 | SF!=OF)

		// loc_1DB6F66:
		0x39, 0x3E							// cmp     [esi], edi      ; Compare Two Operands
	};
	const unsigned char sig2[] =
	{
		0x75, 0x0A,							// jnz     short loc_1DB6F74 ; Jump if Not Zero (ZF=0)
		0x39, 0x7E, 0x0C					// cmp     [esi+0Ch], edi  ; Compare Two Operands
	};
	const unsigned char sig3[] =
	{
		0x75, 0x05,							// jnz     short loc_1DB6F74 ; Jump if Not Zero (ZF=0)
		0x39, 0x7E, 0x04,					// cmp     [esi+4], edi    ; Compare Two Operands
		0x74, 0x5F,							// jz      short loc_1DB6FD3 ; Jump if Zero (ZF=1)

		// loc_1DB6F74:
		0x39, 0xBE, 0x48, 0x25, 0x00, 0x00,	// cmp     [esi+2548h], edi ; Compare Two Operands
		0x74, 0x11,							// jz      short loc_1DB6F8D ; Jump if Zero (ZF=1)
		0x68, 0x5C, 0x99, 0xE8, 0x01,		// push    offset aDroppingFakecl ; "Dropping fakeclient on level change"
		0x57,								// push    edi             ; int
		0x56,								// push    esi             ; int
		0xE8, 0x48, 0xAE, 0xFA, 0xFF,		// call    sub_1D61DD0     ; Call Procedure
		0x83, 0xC4, 0x0C,					// add     esp, 0Ch
		0xEB, 0x46							// jmp     short loc_1DB6FD3 ; Jump
	};

	assert (address[sizeof (sig1)] == 0x75);
	assert (address[sizeof (sig1) + sizeof (sig2)] == 0x75);
	assert (address[sizeof (sig1) + sizeof (sig2) + sizeof (sig3)] == 0x8D);

	const unsigned char *const jumpToAddress (address + sizeof (sig1) + sizeof (sig2) + sizeof (sig3));

	Trampolines::PatchMemoryData <unsigned char> (address + sizeof (sig1) + Trampolines::Bytecode::Pointer8Size,                 reinterpret_cast <unsigned char> (Trampolines::GetRelativeAddressOfRealAddress16 (address + sizeof (sig1), jumpToAddress)));
	Trampolines::PatchMemoryData <unsigned char> (address + sizeof (sig1) + sizeof (sig2) + Trampolines::Bytecode::Pointer8Size, reinterpret_cast <unsigned char> (Trampolines::GetRelativeAddressOfRealAddress16 (address + sizeof (sig1) + sizeof (sig2), jumpToAddress)));

	assert (address[sizeof (sig1) + Trampolines::Bytecode::Pointer8Size] == 0x23);
	assert (address[sizeof (sig1) + sizeof (sig2) + Trampolines::Bytecode::Pointer8Size] == 0x1E);
#pragma warning (pop)
}
*/
namespace HalfLifeEngine
{
namespace CHEAT
{
//=====================================================================
// OffsetCStudioModelRenderer
// Returns a pointer to CStudioModelRenderer class in hl memory
//=====================================================================
SDK::Structures::StudioModelRenderer_t* OffsetCStudioModelRenderer(void)
{
	void *clBase = GetModuleHandle("client.DLL");
	unsigned long clSize = (unsigned long )0x00159000u;
/*
//==============================
// guidance offset pattern
//==============================
const unsigned char ModelrString[] = "cl_himodels";

	void *const address = MemoryUtilities::FindPatternWithoutWildCard(clBase, clSize, ModelrString, GET_STATIC_STRING_LENGTH (ModelrString));
	if(address == NULL)
		return NULL;

	void* pInit = (void*)MemoryUtilities::FindMemoryChunkReference32(clBase, clSize, address, Trampolines::Bytecode::Push32);
	if(pInit == NULL)
		return NULL;

	for(unsigned char* b = (unsigned char*)pInit; (DWORD)b > (DWORD)pInit - 0x10; --b)
		if (*(b-1) == 0x90 && *(b-2) == 0x90 )
			return (SDK::Structures::StudioModelRenderer_t*)((DWORD)MemoryUtilities::FindPattern(clBase, clSize, reinterpret_cast <unsigned char *> (&b), sizeof(DWORD)) - sizeof(DWORD));

	return NULL;
*/
	// off_19CD8E0
	const unsigned char signature[] = "\x56\x8B\xF1\xE8\x2A\x2A\x2A\x2A\xC7\x06\x2A\x2A\x2A\x2A\xC6\x86\x2A\x2A\x2A\x2A\x2A\x8B\xC6\x5E\xC3";

	return (SDK::Structures::StudioModelRenderer_t*)*(DWORD*)((unsigned int)MemoryUtilities::FindPattern(clBase, clSize, signature, sizeof(signature)-sizeof ('\0')) + GET_STATIC_STRING_LENGTH ("\x56\x8B\xF1\xE8\x2A\x2A\x2A\x2A\xC7\x06"));
}

//==============================
// OffsetEngineFunc
// Returns a pointer to ClientEngineFunction_t struct
//==============================
SDK::Structures::ClientEngineFunction_t* OffsetEngineFunc(const LibraryHandleWithSize &engineLibrary)
{
//==============================
// guidance offset pattern
//==============================
const unsigned char EngineString[] = "ScreenFade";

	void *const address = MemoryUtilities::FindPatternWithoutWildCard(engineLibrary, EngineString, GET_STATIC_STRING_LENGTH (EngineString));
	if(address == NULL)
		return NULL;

	unsigned long* ppEngfunc = (unsigned long*)((DWORD)MemoryUtilities::FindMemoryChunkReference32(engineLibrary, address, Trampolines::Bytecode::Push32)+0xD);
	if(ppEngfunc)
		return (SDK::Structures::ClientEngineFunction_t*)*ppEngfunc;

	return NULL;
}

//==============================
// OffsetEngineStudio
// Returns a pointer to SDK::Structures::EngineStudioAPI_t struct
//==============================
SDK::Structures::EngineStudioAPI_t* OffsetEngineStudio(const LibraryHandleWithSize &engineLibrary)
{
//==============================
// guidance offset pattern
//==============================
const unsigned char StudioString[] = "Couldn't get client .dll studio model rendering interface.";

	void *const address = MemoryUtilities::FindPatternWithoutWildCard(engineLibrary, StudioString, GET_STATIC_STRING_LENGTH (StudioString));
	if(address == NULL)
		return NULL;

	return (SDK::Structures::EngineStudioAPI_t*)*(DWORD*)((DWORD)MemoryUtilities::FindMemoryChunkReference32(engineLibrary, address, Trampolines::Bytecode::Push32)-20);
}

//==============================
// OffsetExportTable
// Returns a pointer to cdll's exporttable
//==============================
SDK::Structures::ClientDLLExportTable_t *OffsetExportTable(const LibraryHandleWithSize &engineLibrary)
{
//==============================
// guidance offset pattern
//==============================
const unsigned char StudioString[] = "Couldn't get client .dll studio model rendering interface.";

	void *const address = MemoryUtilities::FindPatternWithoutWildCard(engineLibrary, StudioString, GET_STATIC_STRING_LENGTH (StudioString));
	if(address == NULL)
		return NULL;

	return (SDK::Structures::ClientDLLExportTable_t*)(*(DWORD*)((DWORD)MemoryUtilities::FindMemoryChunkReference32(engineLibrary, address, Trampolines::Bytecode::Push32)-29)-0x9C);
}

//==============================
// OffsetPreSDynSound
// Returns a pointer to PreS_Dynamicsound function
//==============================
void* OffsetPreSDynSound(const LibraryHandleWithSize &engineLibrary)
{
//==============================
// guidance offset pattern
//==============================
const unsigned char SoundsString[] = "S_StartDynamicSound: %s volume > 255";

	void *const address = MemoryUtilities::FindPatternWithoutWildCard(engineLibrary, SoundsString, GET_STATIC_STRING_LENGTH (SoundsString));
	if(address == NULL)
		return NULL;

	return (void*)((DWORD)MemoryUtilities::FindMemoryChunkReference32(engineLibrary, address, Trampolines::Bytecode::Push32)-0x9C);
}

//==============================
// OffsetAddHudElem
// Returns a pointer to cHUD::AddHudElem function
//==============================
void* OffsetAddHudElem(const LibraryHandleWithSize &engineLibrary)
{
//==============================
// guidance offset pattern
//==============================
const unsigned char WeaponString[] = "CurWeapon";

	void *const address = MemoryUtilities::FindPatternWithoutWildCard(engineLibrary, WeaponString, GET_STATIC_STRING_LENGTH (WeaponString));
	if(address == NULL)
		return NULL;

	DWORD Address = (DWORD)MemoryUtilities::FindMemoryChunkReference32(engineLibrary, address, Trampolines::Bytecode::Push32) - 0x09;
	return (void*) (*(DWORD*)Address + Address + 0x04);
}

//==============================
// AddressOfEventPtr
//==============================
void* AddressOfEventPtr (const LibraryHandleWithSize &engineLibrary)
{
//==============================
// guidance offset pattern
//==============================
const unsigned char EventString[] = "CL_HookEvent:  Called on existing hook, updating event hook\n";

	void *const address = MemoryUtilities::FindPatternWithoutWildCard (engineLibrary, EventString, GET_STATIC_STRING_LENGTH (EventString));
	if(address == NULL)
		return NULL;

	DWORD Address = (DWORD)MemoryUtilities::FindMemoryChunkReference32 (engineLibrary, address, Trampolines::Bytecode::Push32) - 0x0D;
	Address = (*(DWORD*)Address + Address + 0x04);
	return (void*) (*(unsigned long**)(Address + 0x03));
}

//==============================
// AddressOfCommandPtr
//==============================
void* AddressOfCommandPtr (const LibraryHandleWithSize &engineLibrary)
{
//==============================
// guidance offset pattern
//==============================
const unsigned char CommandString[] = "Cmd_AddCommand: %s already defined\n";

	void *const address = MemoryUtilities::FindPatternWithoutWildCard (engineLibrary, CommandString, GET_STATIC_STRING_LENGTH (CommandString));
	if(address == NULL)
		return NULL;

	DWORD Address = (DWORD)MemoryUtilities::FindMemoryChunkReference32 (engineLibrary, address, Trampolines::Bytecode::Push32) - 0x40;
	return (void*) (**(unsigned long***)Address);
}

void* AddressOfWeaponBaseFnc()
{
//==============================
// direct offset pattern
//==============================
unsigned char WeaponBasePattern[] =
{
	0xB8, 0x2A, 0x2A, 0x2A, 0x2A, 0xC3,
	0xB8, 0x2A, 0x2A, 0x2A, 0x2A, 0xC3,
	0xB8, 0x2A, 0x2A, 0x2A, 0x2A, 0xC3,
	0xB8, 0x2A, 0x2A, 0x2A, 0x2A, 0xC3,
	0xB8, 0x2A, 0x2A, 0x2A, 0x2A, 0xC3,
	0xB8, 0x2A, 0x2A, 0x2A, 0x2A, 0xC3,
	0xB8, 0x2A, 0x2A, 0x2A, 0x2A, 0xC3,
	0xB8, 0x2A, 0x2A, 0x2A, 0x2A, 0xC3,
	0xB8, 0x2A, 0x2A, 0x2A, 0x2A, 0xC3,
	0xB8, 0x2A, 0x2A, 0x2A, 0x2A, 0xC3
};

	const unsigned long clSize = (unsigned long)0x00159000u;
	void *clBase = GetModuleHandle("client.DLL");

	return (void*)((unsigned long)MemoryUtilities::FindPattern(clBase, clSize, WeaponBasePattern, sizeof(WeaponBasePattern)/sizeof(unsigned char)-1) - 0x15);
}

void* AddressOfRefParamsPtr(const LibraryHandleWithSize &engineLibrary)
{
//==============================
// direct offset pattern
//==============================
const unsigned char RefPtrSig[] = "\x53\x8B\x5C\x24\x2A\x56\x57\xB9\x2A\x2A\x2A\x2A\x8B\xF3\xBF\x2A\x2A\x2A\x2A\x68";

	return (void*)(((unsigned long)MemoryUtilities::FindPattern(engineLibrary.GetPointer (), engineLibrary.GetSize (), RefPtrSig, sizeof(RefPtrSig)/sizeof(unsigned char)-1)) + 0x0F);
}

void* AddressOfPlayerMove(const LibraryHandleWithSize &engineLibrary)
{
//==============================
// direct offset pattern
//==============================
const unsigned char PMovePtrSig[] = "\xA1\x2A\x2A\x2A\x2A\x8B\x0D\x2A\x2A\x2A\x2A\x8B\x95\x2A\x2A\x2A\x2A\x8D\x74\x24\x2A";

	return (void*)**(unsigned long**)((unsigned long)MemoryUtilities::FindPattern(engineLibrary.GetPointer (), engineLibrary.GetSize (), PMovePtrSig, sizeof(PMovePtrSig)/sizeof(unsigned char)-1) + 0x01);
}

void* AddressOfGlobalTime(const LibraryHandleWithSize &engineLibrary)
{
//==============================
// direct offset pattern
//==============================
const unsigned char GlobalTimeSig[] = {0x74, 0x47, 0xDD, 0x05, 0x2A, 0x2A, 0x2A, 0x2A, 0x51, 0x8D, 0x4C, 0x24, 0x08, 0xD9, 0x5C, 0x24, 0x04, 0xD9, 0x44, 0x24, 0x04, 0xD9, 0x1C, 0x24, 0x51, 0x2A, 0xD0, 0x83, 0xC4, 0x08, 0x85, 0xC0, 0x74, 0x27};

	return (void*)*(unsigned long*)((unsigned long)MemoryUtilities::FindPattern(engineLibrary.GetPointer (), engineLibrary.GetSize (), GlobalTimeSig, sizeof(GlobalTimeSig)/sizeof(unsigned char)) + 0x04);
}
bool CalcScreen(const Math::Vector3D &origin,Math::Vector2D &vecScreen) // Credits Azorbix
{
	int result = Globals::g_halfLifeEngine->GetClientEngineFunctionTable ().triangleAPI->WorldToScreen(origin, vecScreen);
	if(vecScreen.x < 1 && vecScreen.y < 1 && vecScreen.x > -1 && vecScreen.y > -1 && !result)
	{
		vecScreen.x	= (vecScreen.x * 1280 / 2)	+ 1280 / 2;
		vecScreen.y	= (-vecScreen.y * 1024 / 2) + 1024 / 2;
		return true;
	}
	return false;
}
void gDrawFilledBoxAtLocation(Human * /*h*/, const Math::Vector3D &origin, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha, int radius)
{
	Math::Vector2D vecScreen;
//	if( !h->GetScreenPosition(origin, vecScreen) ) { return; }
	if( !CalcScreen(origin, vecScreen) ) { return; }

	int radius2 = radius<<1;

	Globals::g_halfLifeEngine->GetClientEngineFunctionTable ().pfnFillRGBA(int(vecScreen.x-radius),int(vecScreen.y-radius),radius2,radius2,red,green,blue,alpha);
}
void HUD_DrawLine(int x1, int y1, int x2, int y2, unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	Globals::g_halfLifeEngine->GetClientEngineFunctionTable ().triangleAPI->RenderMode( SDK::Constants::kRenderNormal );
	Globals::g_halfLifeEngine->GetClientEngineFunctionTable ().triangleAPI->CullFace( SDK::Structures::TriangleAPI_t::CullStyle_None );
	Globals::g_halfLifeEngine->GetClientEngineFunctionTable ().triangleAPI->Begin( SDK::Structures::TriangleAPI_t::TRI_LINES );

	Globals::g_halfLifeEngine->GetClientEngineFunctionTable ().triangleAPI->Color4f( r/255.0f, g/255.0f, b/255.0f, a/255.0f );

	Globals::g_halfLifeEngine->GetClientEngineFunctionTable ().triangleAPI->Brightness(1);
	Globals::g_halfLifeEngine->GetClientEngineFunctionTable ().triangleAPI->Vertex3f((float)x1, (float)y1, 0);
	Globals::g_halfLifeEngine->GetClientEngineFunctionTable ().triangleAPI->Vertex3f((float)x2, (float)y2, 0);
	Globals::g_halfLifeEngine->GetClientEngineFunctionTable ().triangleAPI->End();
}
void HUD_DrawLine2(const Math::Vector3D &source, const Math::Vector3D &destination, unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	Globals::g_halfLifeEngine->GetClientEngineFunctionTable ().triangleAPI->RenderMode( SDK::Constants::kRenderNormal );
	Globals::g_halfLifeEngine->GetClientEngineFunctionTable ().triangleAPI->CullFace( SDK::Structures::TriangleAPI_t::CullStyle_None );
	Globals::g_halfLifeEngine->GetClientEngineFunctionTable ().triangleAPI->Begin( SDK::Structures::TriangleAPI_t::TRI_LINES );

//	Globals::g_halfLifeEngine->GetClientEngineFunctionTable ().triangleAPI->Color4f( r/255.0f, g/255.0f, b/255.0f, a/255.0f );
	Globals::g_halfLifeEngine->GetClientEngineFunctionTable ().triangleAPI->Color4ub( r, g, b, a);

//	Globals::g_halfLifeEngine->GetClientEngineFunctionTable ().triangleAPI->Brightness (1);
	Globals::g_halfLifeEngine->GetClientEngineFunctionTable ().triangleAPI->Vertex3fv (source);
	Globals::g_halfLifeEngine->GetClientEngineFunctionTable ().triangleAPI->Vertex3fv (destination);
	Globals::g_halfLifeEngine->GetClientEngineFunctionTable ().triangleAPI->End ();
}
#if defined MULTIPLE_DRAW_METHODS
//=============================
// DrawingBegin
// Initializes our drawing matrix
//=============================
void DrawingBegin(void)
{
//	g_drawInterface->glPushMatrix();
//	g_drawInterface->glLoadIdentity();
	g_drawInterface->glDisable(GL_TEXTURE_2D);
	g_drawInterface->glEnable(GL_BLEND);
	g_drawInterface->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

//=============================
// DrawingEnd
// disables our drawing matrix
//=============================
void DrawingEnd(void)
{
	g_drawInterface->glDisable(GL_BLEND);
	g_drawInterface->glEnable(GL_TEXTURE_2D);
//	g_drawInterface->glPopMatrix();
}

//=============================
// DrawingSetColor
// Set r,g,b,(a) color for drawing
//=============================
void __fastcall DrawingSetColor(unsigned char r, unsigned char g, unsigned char b)
{
	g_drawInterface->glColor3ub(r, g, b);
}

void __fastcall DrawingSetColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	g_drawInterface->glColor4ub(r, g, b, a);
}

//=============================
// DrawingSetLineWidth
// Set the width of lines to draw
//=============================
void __fastcall DrawingSetLineWidth(float w)
{
	g_drawInterface->glLineWidth(w);
}

//=============================
// DrawingDrawRectangle
// Draws a solid or nonsolid rectangle
//=============================
void DrawingDrawRectangle(int x, int y, int w, int h, bool solid)
{
	g_drawInterface->glBegin(solid ? GL_QUADS : GL_LINE_LOOP);
		g_drawInterface->glVertex2i(x-w/2, y-h/2);
		g_drawInterface->glVertex2i(x+w/2+1, y-h/2);
		g_drawInterface->glVertex2i(x+w/2, y+h/2);
		g_drawInterface->glVertex2i(x-w/2, y+h/2);
	g_drawInterface->glEnd();
}
//=============================
// DrawingDrawLine
// Draws a line from A to B
//=============================
void __fastcall DrawingDrawLine(int xs, int ys, int xe, int ye)
{
	g_drawInterface->glBegin(GL_LINES);
		g_drawInterface->glVertex2i(xs, ys);
		g_drawInterface->glVertex2i(xe, ye);
	g_drawInterface->glEnd();
}
void __fastcall DrawingDrawLine3D(const Math::Vector3D &src, const Math::Vector3D &dst)
{
	g_drawInterface->glBegin(GL_LINES);
		g_drawInterface->glVertex3fv(src);
		g_drawInterface->glVertex3fv(dst);
	g_drawInterface->glEnd();
}

//=============================
// DrawingDrawCircle
// Draws a circle with given radius
//=============================
void __fastcall DrawingDrawCircle(int x, int y, int r, bool/* solid*/)
{
	float vectorX, vectorY1 = (float)y + r;
	float vectorY, vectorX1 = (float)x;
	float cosine, sine;
	g_drawInterface->glBegin(GL_LINE_STRIP);
		for (float angle = 0.0f; angle <= Math::TwoPi; angle += Math::OnEpsilon)
		{
			Math::GetCosineSine (angle, cosine, sine);

			vectorX = (float)x + (float)r * sine;
			vectorY = (float)y + (float)r * cosine;
			g_drawInterface->glVertex2f(vectorX1, vectorY1);
			vectorY1 = vectorY;
			vectorX1 = vectorX;
		}
	g_drawInterface->glEnd();
}

//================================================================
// DrawingDrawText
// Draws a text to the screen
// Very bad performance quality. You should better write your own.
//================================================================
void DrawingDrawText(const char* Text, int x, int y, unsigned char r, unsigned char g, unsigned char b, unsigned char a, const char* FontName, int w, int h, bool Centered = true)
{
	HDC hDC = g_drawInterface->wglGetCurrentDC();
	GLuint Lists = g_drawInterface->glGenLists(256);
	g_drawInterface->wglUseFontBitmapsA(hDC, 0, 256, Lists);

	HFONT hFont = CreateFont(w, 0, 0, 0, h, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, FF_DONTCARE | DEFAULT_PITCH, FontName);
	HFONT hOldFont = (HFONT) SelectObject(hDC, hFont);

	g_drawInterface->glShadeModel(GL_SMOOTH);
	g_drawInterface->glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	SIZE s;
	char width[256];
	char height;
	for(unsigned short i=0; i < 256; ++i)
	{
		char temp[2] = {(char)i, '\0'};
		GetTextExtentPoint32(hDC, temp, 1, &s);
		width[i] = (char)s.cx;
		height = (char)s.cy;
	}

	SelectObject(hDC, hOldFont);
	DeleteObject(hFont);

	GLfloat curcolor[4], position[4];
	g_drawInterface->glPushAttrib(GL_LIST_BIT);
	g_drawInterface->glGetFloatv(GL_CURRENT_COLOR, curcolor);
	g_drawInterface->glGetFloatv(GL_CURRENT_RASTER_POSITION, position);
	g_drawInterface->glDisable(GL_TEXTURE_2D);

	g_drawInterface->glColor4ub(r, g, b, a);

	int length (0u);

	if (Centered)
	{
		int drawlen (0u);

		while (Text[length] != '\0')
		{
			drawlen += width[Text[length]];
			++length;
		}

		drawlen /= 2;

		if (x >= drawlen)
			x -= drawlen;
		else
			x = 0;
	}
	else while (Text[length] != '\0')
		++length;

	g_drawInterface->glRasterPos2i(x, y);

	g_drawInterface->glListBase(Lists);
	g_drawInterface->glCallLists(length, GL_UNSIGNED_BYTE, Text);

	g_drawInterface->glPopAttrib();
	g_drawInterface->glColor4fv(curcolor);
	g_drawInterface->glRasterPos2f(position[0],position[1]);
	g_drawInterface->glEnable(GL_TEXTURE_2D);

	g_drawInterface->glDeleteLists(Lists, 256);
}
#endif	// if defined MULTIPLE_DRAW_METHODS
/*
void HUD_PlayerMove (SDK::Structures::PlayerMove_t *ppmove, BOOL server)
{
	for (int index (1u); index < ppmove->numberPhysEntities; ++index)
	{
		g_server->GetHumanManager ()->GetHostClient ()->DrawLine (ppmove->origin, HalfLifeEngine::Globals::g_halfLifeEngine->GetEdictOfIndex (ppmove->physEntities[index].info)->GetOrigin (), HalfLifeEngine::Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (HalfLifeEngine::Engine::PrecachedSpriteIndex_Laser), 1u, 0u, Color <> (255u, 0u, 0u), 250u, 5u, 1u, false);
		HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "\"%s\": ppmove->physEntities[%i].name: \"%s\", origin=[%.2f, %.2f, %.2f].", g_server->GetClientManager ()->GetClient (ppmove->playerIndex)->GetName ().GetData (), index, ppmove->physEntities[index].name, ppmove->physEntities[index].origin.x, ppmove->physEntities[index].origin.y, ppmove->physEntities[index].origin.z);
	}

	Globals::g_halfLifeEngine->GetClientDLLExportTable ().HUD_PlayerMove (ppmove, server);
}*/
int HUD_Redraw( float time, int intermission )
{
	const int result (Globals::g_halfLifeEngine->GetClientDLLExportTable ().HUD_Redraw (time, intermission));
#if defined MULTIPLE_DRAW_METHODS
#if 0
	// Set 3D mode....
	{
		g_drawInterface->GL_Set3D ();
	}

	// Draw something in 3D mode....
	{
	SDK::Structures::ClientEntity_t *pLocal = Globals::g_halfLifeEngine->GetClientEngineFunctionTable ().GetLocalPlayer();
	for(int i=0; i < SDK::Constants::MaximumClients; ++i)
	{
		SDK::Structures::ClientEntity_t* ent = Globals::g_halfLifeEngine->GetClientEngineFunctionTable ().GetEntityByIndex (i);

		if(!ent || ent == pLocal || !ent->player || !ent->curstate.solid)
			continue;

		Math::Vector3D source (pLocal->origin/* / 1000.0f*/);
		Math::Vector3D destination (ent->origin/* / 1000.0f*/);
/*
		DrawingBegin();
			DrawingSetLineWidth(1.0f);
			DrawingSetColor(0, 255, 0, 255);

			g_drawInterface->glBegin(GL_LINES);
				g_drawInterface->glVertex3fv(source);
				g_drawInterface->glVertex3fv(destination);
			g_drawInterface->glEnd();
		DrawingEnd();
*/
		HUD_DrawLine2 (pLocal->origin, ent->origin, 0, 255, 0, 255);
	}
	}

	// Back to 2D mode....
	{
		g_drawInterface->GL_Set2D ();
	}
#endif	// if 0
	#if defined MULTIPLE_DRAW_METHODS
		if (g_server->IsRunning () && g_server->GetRecastWrapper ()->IsActive ())
		{
			DrawingBegin ();

			g_server->GetRecastWrapper ()->GetRecastWrapper_GLPointer ()->Think2D ();

			DrawingEnd ();
		}
	#endif	// if defined MULTIPLE_DRAW_METHODS
/*
	GLint matrixMode (0);
	g_drawInterface->glGetIntegerv (GL_MATRIX_MODE, &matrixMode);

	const SDK::Structures::con_nprint_t printInfo =
	{
		15u & 31u,
		0.1f,
		Color <float> (0.0f, 1.0f, 0.0f)
	};
	HalfLifeEngine::Globals::g_halfLifeEngine->GetClientEngineFunctionTable ().Con_NXPrintf (&printInfo, "matrixMode=%s", matrixMode == GL_MODELVIEW ? "MODELVIEW" : matrixMode == GL_PROJECTION ? "PROJECTION" : matrixMode == GL_TEXTURE ? "TEXTURE" : "UNKNOWN!");*/
#if 0
//	HUD_DrawLine (640, 512, 640, 512 + 10, 0, 255, 0, 255);

	SDK::Structures::ClientEntity_t *pLocal = Globals::g_halfLifeEngine->GetClientEngineFunctionTable ().GetLocalPlayer();
/*	SDK::Structures::ClientEntity_t *const currentEntity (Globals::g_halfLifeEngine->GetClientEngineFunctionTable ().GetCurrentEntity ());
	if(ent && ent != pLocal && ent->player && ent->curstate.solid)
	{
		//
	}
	Math::Vector2D vecScreen;
	if(g_server->GetHumanManager ()->GetHostClient ()->GetScreenPosition(bot->GetOrigin (), vecScreen) )
	{
	int radius2 = 1<<1;

	Globals::g_halfLifeEngine->GetClientEngineFunctionTable ().pfnFillRGBA(vecScreen[0]-1,vecScreen[1]-1,radius2,radius2,255,0,0,255);
			}
*/
	for (int i=0; i < SDK::Constants::MaximumClients; ++i)
	{
		SDK::Structures::ClientEntity_t* ent = Globals::g_halfLifeEngine->GetClientEngineFunctionTable ().GetEntityByIndex (i);

		if(!ent || ent == pLocal || !ent->player || !ent->curstate.solid)
			continue;
/*
		float Screen1[2], Screen2[2];

		if(CalcScreen(pLocal->origin, Screen1) && CalcScreen(ent->origin, Screen2))
			HUD_DrawLine (Screen1[0], Screen1[1], Screen2[0], Screen2[1], 0, 255, 0, 255);
		HUD_DrawLine2 (pLocal->origin, ent->origin, 0, 255, 0, 255);
*/
		// draw history:
		{
			int historyIndex = ent->current_position;
			for(int i=0;i<HISTORY_MAX;++i)
			{
				gDrawFilledBoxAtLocation(g_server->GetHumanManager ()->GetHostClient (), ent->ph[historyIndex].origin, 0, 255, 0, 255, 1);
//				Globals::g_halfLifeEngine->DrawLine (ent->ph[historyIndex].origin - Math::Vector3D (0.0f, 0.0f, 36.0f), ent->ph[historyIndex].origin + Math::Vector3D (0.0f, 0.0f, 36.0f + 10.0f), Globals::g_halfLifeEngine->GetPrecachedSpriteIndex (Engine::PrecachedSpriteIndex_Laser), 1u, 0u, Color <> (0u, 255u, 0u), 250u, 5u, 1u);

				// step back
				if (--historyIndex < 0)
					historyIndex = HISTORY_MAX-1;
			}
		}

		{
		float Screen[2];

		if(!CalcScreen(ent->origin, Screen))
			continue;

	DrawingBegin();
	DrawingSetLineWidth(1.0f);
	DrawingSetColor(0, 255, 0, 255);

		DrawingDrawRectangle ((int)Screen[0], (int)Screen[1], 20, 20, false);

//	DrawingDrawText ("THIS IS SELF TEST", (int)Screen[0], (int)Screen[1], 255, 0, 0, 255, "UNNAMED FONT", 11, FW_DONTCARE);

	DrawingEnd();
		}
	}
#endif	// if 0
#endif	// if defined MULTIPLE_DRAW_METHODS
#if 0
	// RELOAD-O_METER
	{
	int Cstrike_SequenceInfo[] = {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 0..9
		0, 1, 2, 0, 1, 2, 0, 1, 2, 0, // 10..19
		1, 2, 0, 1, 1, 2, 0, 1, 1, 2, // 20..29
		0, 1, 2, 0, 1, 2, 0, 1, 2, 0, // 30..39
		1, 2, 0, 1, 2, 0, 1, 2, 0, 1, // 40..49
		2, 0, 1, 2, 0, 0, 0, 4, 0, 4, // 50..59
		0, 5, 0, 5, 0, 0, 1, 1, 2, 0, // 60..69
		1, 1, 2, 0, 1, 0, 1, 0, 1, 2, // 70..79
		0, 1, 2, 3, 3, 3, 3, 3, 3, 3, // 80..89
		3, 3, 3, 3, 3, 3, 3, 3, 3, 3, // 90..99
		3
	};
	for (unsigned char index (0u); index < g_server->GetClientManager ()->GetClientsCount (); ++index)
	{
		const Client *const client (g_server->GetClientManager ()->GetClient (index));

	//SEQUENCE_RELOAD=2, SEQUENCE_DIE=3, SEQUENCE_THROW=4, SEQUENCE_ARM_C4=5
	int atype = Cstrike_SequenceInfo[client->GetEdict ()->variables.sequence];
	if(atype == 2 || atype == 4 || atype == 5)
	{
//		float vecScreen[2];

//		if (!CalcScreen (client->GetOrigin (), vecScreen))
//			continue;

		const int tempy (0);

		//    "%1.2f%",(client->GetEdict ()->variables.frame/255)*100.0f);
		//max is 255, so divide by 5 to get 0-51 with 25 on either side and 26 in the middle
		//int x, int y, int width, int height, int r, int g, int b, int a
//		oglSubtractive=true;
		Globals::g_halfLifeEngine->GetClientEngineFunctionTable ().pfnFillRGBA(/*vecScreen[0]*/1280/2 - 26,/*vecScreen[1]+tempy*/1024/2 + 1,53,12,0,0,0,250);
//		oglSubtractive=false;
		Globals::g_halfLifeEngine->GetClientEngineFunctionTable ().pfnFillRGBA(/*vecScreen[0]*/1280/2 - 25,/*vecScreen[1]+tempy*/1024/2 + 2,(client->GetEdict ()->variables.frame/255)*50,10,0,200,0,255);
//		tempy+=16;
	}
	}
	}
#endif	// if 0
	return result;
}

void IN_MouseEvent (int mstate)
{
	#if defined MULTIPLE_DRAW_METHODS
		if (g_server->IsRunning () && g_server->GetRecastWrapper ()->IsActive () && g_server->GetRecastWrapper ()->GetRecastWrapper_GLPointer ()->MouseEvent (mstate, true))
			return;
	#endif	// if defined MULTIPLE_DRAW_METHODS

	Globals::g_halfLifeEngine->GetClientDLLExportTable ().IN_MouseEvent (mstate);
}
BOOL HUD_Key_Event (int eventcode, int keynum, const char *pszCurrentBinding)
{
	#if defined MULTIPLE_DRAW_METHODS
		if (g_server->IsRunning () && g_server->GetRecastWrapper ()->IsActive () && g_server->GetRecastWrapper ()->GetRecastWrapper_GLPointer ()->KeyEvent (eventcode, keynum, pszCurrentBinding))
			return TRUE;
	#endif	// if defined MULTIPLE_DRAW_METHODS

	return Globals::g_halfLifeEngine->GetClientDLLExportTable ().HUD_Key_Event (eventcode, keynum, pszCurrentBinding);
}

	#define CVARwiggle Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_BotBuy)->GetValue <float> ()
	#define CVARwiggleangle Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_Chat)->GetValue <float> ()
//	const float CVARwiggle (120.0f);
//	const float CVARwiggleangle (12.0f);
	float g_wiggleangle (12.0f);
	bool g_wiggleleft (true);
	bool g_doWiggle (false);

inline void CL_CreateMove (float frameTime, SDK::Structures::UserCommand_t *command, BOOL active)
{
	Globals::g_halfLifeEngine->GetClientDLLExportTable ().CL_CreateMove (frameTime, command, active);

	if (CVARwiggle > 0.0f && (command->buttons & HalfLifeEngine::SDK::Constants::IN_USE) && !g_server->GetHumanManager ()->GetHostClient ()->IsOnFloor ())
	{
		g_doWiggle = true;

		if (g_wiggleleft)
			command->sideMove -= 250.0f;
		else
			command->sideMove += 250.0f;
	}
	else
		g_doWiggle = false;
}
inline void V_CalcRefdef (SDK::Structures::ref_params_t *parameters)
{
	if (g_doWiggle)
	{
		const float speed (g_server->GetHumanManager ()->GetHostClient ()->GetEdict ()->GetSpeed2D ());

		if (speed != 0.0f)
		{
			float wiggy (CVARwiggle / speed);
			const float halfWiggleAngle (CVARwiggleangle * 0.5f);

			if (wiggy > halfWiggleAngle)
				wiggy = halfWiggleAngle;

			if (g_wiggleleft)
			{
				if (g_wiggleangle + wiggy > CVARwiggleangle)
					wiggy = CVARwiggleangle - g_wiggleangle;

				g_wiggleangle += wiggy;

				parameters->cl_viewangles.yaw += wiggy;
			}
			else
			{
				if (g_wiggleangle - wiggy < -CVARwiggleangle)
					wiggy = CVARwiggleangle + g_wiggleangle;

				g_wiggleangle -= wiggy;

				parameters->cl_viewangles.yaw -= wiggy;
			}

			if (g_wiggleangle >= CVARwiggleangle)
				g_wiggleleft = false;
			else if (g_wiggleangle <= -CVARwiggleangle)
				g_wiggleleft = true;
		}
	}
	else if (g_wiggleangle != 0.0f)
	{
		g_wiggleangle = 0.0f;

		parameters->cl_viewangles.yaw -= g_wiggleangle;
	}

	Globals::g_halfLifeEngine->GetClientDLLExportTable ().V_CalcRefdef (parameters);
}

void RenderWireFrame (void)
{
	if (!g_server->IsRunning ())
		return;

	// all valid model_s' stored in the engine start at 1 by this method, not 0,
	// and the world model is always the first model. Thus, it's at index 1.
	SDK::Structures::Model_t *const pModel (g_server->GetServer ().worldmodel/*Globals::g_halfLifeEngine->GetClientEngineStudioAPI ().GetModelByIndex (1)*/);

	// make sure it's a valid Model_t pointer and the model type is ModelType_Brush
	if (pModel == NULL || pModel->type != SDK::Constants::ModelType_Brush)
		return;
/*
	#if defined MULTIPLE_DRAW_METHODS
	{
		g_drawInterface->glDisable (GL_DEPTH_TEST);
		g_drawInterface->glDisable (GL_TEXTURE_2D);
		g_drawInterface->glEnable (GL_BLEND);
		g_drawInterface->glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		g_drawInterface->glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		Globals::g_halfLifeEngine->GetClientEngineFunctionTable ().triangleAPI->Color4ub (255, 255, 255, 255);	// white lines
//		Globals::g_halfLifeEngine->GetClientEngineFunctionTable ().triangleAPI->Color4ub (255, 0, 0, 255);	// red lines
	}
	#endif	// if defined MULTIPLE_DRAW_METHODS
*/
	// now it shouldn't crash if we do this
	// we're simply drawing every single edge in the world
	Globals::g_halfLifeEngine->GetClientEngineFunctionTable ().triangleAPI->Begin (SDK::Structures::TriangleAPI_t::TRI_LINES);
	Globals::g_halfLifeEngine->GetClientEngineFunctionTable ().triangleAPI->CullFace (SDK::Structures::TriangleAPI_t::CullStyle_None);	// don't cull
	Globals::g_halfLifeEngine->GetClientEngineFunctionTable ().triangleAPI->RenderMode (SDK::Constants::kRenderNormal);	// normal render mode
	Globals::g_halfLifeEngine->GetClientEngineFunctionTable ().triangleAPI->Color4ub (255, 255, 255, 255);	// white lines

	// for all edges in the map
	for (int i (0); i < pModel->numedges; ++i)
	{
		// get the current edge at index i
		const SDK::Structures::medge_t &currentEdge (pModel->edges[i]);
		// the medge_t's simply store indexes into a master vertex (DVertex_t) list, so get those two vertexes
		const SDK::Structures::DVertex_t currentVertexes[2] =
		{
			pModel->vertexes[currentEdge.v[0]],
			pModel->vertexes[currentEdge.v[1]]
		};

		// now render this edge/line
		Globals::g_halfLifeEngine->GetClientEngineFunctionTable ().triangleAPI->Vertex3fv (currentVertexes[0].point);
		Globals::g_halfLifeEngine->GetClientEngineFunctionTable ().triangleAPI->Vertex3fv (currentVertexes[1].point);
	}

	Globals::g_halfLifeEngine->GetClientEngineFunctionTable ().triangleAPI->End ();	// we're done rendering
/*
	#if defined MULTIPLE_DRAW_METHODS
	{
		g_drawInterface->glColor3f (1.0f, 1.0f, 1.0f);

		g_drawInterface->glDisable (GL_BLEND);
		g_drawInterface->glEnable (GL_TEXTURE_2D);
		g_drawInterface->glEnable (GL_DEPTH_TEST);
	}
	#endif	// if defined MULTIPLE_DRAW_METHODS*/
}

void HUD_DrawNormalTriangles (void)
{
	// Note: This function called even variable "r_drawentities" value is ZERO !!!
	// Draw something in 3D mode....

	Globals::g_halfLifeEngine->GetClientDLLExportTable ().HUD_DrawNormalTriangles ();	// Draw free/chase overwiew modes for spectator.

	#if defined MULTIPLE_DRAW_METHODS
	if (!g_server->IsRunning ())
		return;	/// @todo DYNAMIC REDIRECTIONS OF THIS FUNCTION IN Server(Activate/Deactivate)!!!

	// Disable color flicking....
	Globals::g_halfLifeEngine->CallGL_DisableMultitextureFunction ();

	{
		g_drawInterface->glDisable (GL_DEPTH_TEST);
		g_drawInterface->glDisable (GL_TEXTURE_2D);
		g_drawInterface->glEnable (GL_BLEND);
		g_drawInterface->glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		g_drawInterface->glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

//	RenderWireFrame ();
	g_drawInterface->DrawAllObjects ();

	#if defined MULTIPLE_DRAW_METHODS
		if (g_server->GetRecastWrapper ()->IsActive ())
			g_server->GetRecastWrapper ()->GetRecastWrapper_GLPointer ()->Think ();
	#endif	// if defined MULTIPLE_DRAW_METHODS

	{
		g_drawInterface->glDisable (GL_BLEND);
		g_drawInterface->glEnable (GL_TEXTURE_2D);
		g_drawInterface->glEnable (GL_DEPTH_TEST);
	}

//	g_drawInterface->glPopAttrib ();
/*
	float &gl_wireframeValue (Globals::g_halfLifeEngine->GetEngineModule ().GetMemory <float> (0x176E14));

	gl_wireframeValue = 1.0f;

	float &iga_areacheckValue (Globals::g_halfLifeEngine->GetEngineModule ().GetMemory <float> (0x165BB4));

	iga_areacheckValue = 2.0f;*/
//	Globals::g_halfLifeEngine->GetClientEngineFunctionTable ().Cvar_SetValue ("gl_wireframe", 1.0f);
	#endif	// if defined MULTIPLE_DRAW_METHODS
}
inline void CL_CreateMove_DUMMY (float frameTime, SDK::Structures::UserCommand_t *command, BOOL active)
{
	Globals::g_halfLifeEngine->GetClientDLLExportTable ().CL_CreateMove (frameTime, command, active);
}
void V_CalcRefdef_DUMMY (SDK::Structures::ref_params_t *parameters)
{
	Globals::g_halfLifeEngine->GetClientDLLExportTable ().V_CalcRefdef (parameters);

	if (!g_server->IsRunning () || g_server->GetYaPBManager ()->GetBotsCount () == 0u)
		return;

	// external variables
const bool camera = true;
const int camera_x = 40;
const int camera_y = 80;
const int camera_width = 200;
const int camera_height = 150;

// internal variables
YaPB *const bot (g_server->GetYaPBManager ()->GetRandomBot ());	// on the nearest enemy/player
Host *const host (g_server->GetHumanManager ()->GetHostClient ());

	if (!camera || !host->IsAlive ())
		return;

	if (parameters->nextView == 0)
	{
		//prevent collision with V_CalcSpectatorRefdef
		parameters->nextView = 1;
	}
	else if (parameters->nextView == 1)
	{
		// prevent collision with V_CalcSpectatorRefdef
		parameters->nextView = 0;
		parameters->onlyClientDraw = false;

		parameters->viewport[0] = camera_x;
		parameters->viewport[1] = camera_y;
		parameters->viewport[2] = camera_width;
		parameters->viewport[3] = camera_height;
		parameters->vieworg = bot->GetEyePosition ();
		parameters->viewangles = bot->GetEdict ()->variables.modelAngles;
	}
}

typedef void (SDK::Classes::Interface::Engine::*Engine__TrapKey_Event_t) (int key, bool down);
Engine__TrapKey_Event_t g_originalEngine__TrapKey_Event (NULL);
void __fastcall Engine__TrapKey_Event (SDK::Classes::Interface::Engine *engine, int /*dummy*/, int key, bool down)
{
	if (g_server->IsRunning ())
	{
//		g_server->GetHumanManager ()->GetHostClient ()->OnKeyEvent (static_cast <HalfLifeEngine::SDK::Constants::FROM_SDL::SDLKey> (key), down);

		#if defined MULTIPLE_DRAW_METHODS
			if (g_server->GetRecastWrapper ()->GetRecastWrapper_GLPointer ()->KeyEvent (down, key, NULL))
				return;
		#endif	// if defined MULTIPLE_DRAW_METHODS
	}

	(engine->*g_originalEngine__TrapKey_Event) (key, down);
}
typedef void (SDK::Classes::Interface::Engine::*Engine__TrapMouse_Event_t) (int key, bool down);
Engine__TrapMouse_Event_t g_originalEngine__TrapMouse_Event (NULL);
void __fastcall Engine__TrapMouse_Event (SDK::Classes::Interface::Engine *engine, int /*dummy*/, int key, bool down)
{
	#if defined MULTIPLE_DRAW_METHODS
		if (g_server->IsRunning () && g_server->GetRecastWrapper ()->IsActive () && g_server->GetRecastWrapper ()->GetRecastWrapper_GLPointer ()->MouseEvent (key, down))
			return;
	#endif	// if defined MULTIPLE_DRAW_METHODS

	(engine->*g_originalEngine__TrapMouse_Event) (key, down);
}

void __fastcall StudioRenderModel (void *object, int /*dummy*/)
{
	reinterpret_cast <void (__fastcall *) (void *const /*this*/, int /*dummy*/)> (Globals::g_halfLifeEngine->GetClientStudioModelRenderer ().StudioRenderModel) (object, 0);

	RenderWireFrame ();
}

typedef void (*S_StartDynamicSound_t) (const int entnum, const int entchannel, const char *const sample, const Math::Vector3D &origin, const float volume, const float attenuation, const int flags, const int pitch);
S_StartDynamicSound_t S_StartDynamicSound (NULL);
unsigned char S_StartDynamicSound_originalBytes[Trampolines::Bytecode::JumpImmediately32Size];
unsigned char S_StartDynamicSound_patchedBytes[Trampolines::Bytecode::JumpImmediately32Size];
void MY_S_StartDynamicSound (const int entnum, const int entchannel, const char *const sample, const Math::Vector3D &origin, const float volume, const float attenuation, const int flags, const int pitch)
{
	HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "MY_S_StartDynamicSound(): entnum=%i, entchannel=%i, sample: \"%s\", vol=%.1f, attn=%.2f, flags=%i, pitch=%i.", entnum, entchannel, sample, volume, attenuation, flags, pitch);

	memcpy (S_StartDynamicSound, S_StartDynamicSound_originalBytes, Trampolines::Bytecode::JumpImmediately32Size);

		(*S_StartDynamicSound) (entnum, entchannel, sample, origin, volume, attenuation, flags, pitch);

	if (Trampolines::VirtualProtect (S_StartDynamicSound, Trampolines::Bytecode::JumpImmediately32Size, PAGE_EXECUTE_READWRITE))
		memcpy (S_StartDynamicSound, S_StartDynamicSound_patchedBytes, Trampolines::Bytecode::JumpImmediately32Size);
}

typedef void (__fastcall *CBaseMonster__RadiusDamage_t) (SDK::Classes::BaseMonster *pThis, unsigned int /* dummy */, SDK::Structures::EntityVariables_t *pevInflictor, SDK::Structures::EntityVariables_t *pevAttacker, float flDamage, int iClassIgnore, SDK::Constants::DamageType_t bitsDamageType);
CBaseMonster__RadiusDamage_t CBaseMonster__RadiusDamage (NULL);
unsigned char CBaseMonster__RadiusDamage_originalBytes[Trampolines::Bytecode::JumpImmediately32Size];
unsigned char CBaseMonster__RadiusDamage_patchedBytes[Trampolines::Bytecode::JumpImmediately32Size];
void __fastcall MY_CBaseMonster__RadiusDamage (SDK::Classes::BaseMonster *pThis, unsigned int /* dummy */, SDK::Structures::EntityVariables_t *pevInflictor, SDK::Structures::EntityVariables_t *pevAttacker, float flDamage, int iClassIgnore, SDK::Constants::DamageType_t bitsDamageType)
{
	HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "MY_CBaseMonster__RadiusDamage(): pevInflictor=%s, pevAttacker=%s, flDamage=%.1f, bitsDamageType=%i.", pevInflictor == NULL ? "NULL" : pevInflictor->containingEntity->IsPlayer () ? pevInflictor->containingEntity->GetNetName ().GetData () : pevInflictor->containingEntity->GetClassName ().GetData (), pevAttacker == NULL ? "NULL" : pevAttacker->containingEntity->IsPlayer () ? pevAttacker->containingEntity->GetNetName ().GetData () : pevAttacker->containingEntity->GetClassName ().GetData (), flDamage, bitsDamageType);

	memcpy (CBaseMonster__RadiusDamage, CBaseMonster__RadiusDamage_originalBytes, Trampolines::Bytecode::JumpImmediately32Size);

		(*CBaseMonster__RadiusDamage) (pThis, 0u, pevInflictor, pevAttacker, flDamage, iClassIgnore, bitsDamageType);

	if (Trampolines::VirtualProtect (CBaseMonster__RadiusDamage, Trampolines::Bytecode::JumpImmediately32Size, PAGE_EXECUTE_READWRITE))
		memcpy (CBaseMonster__RadiusDamage, CBaseMonster__RadiusDamage_patchedBytes, Trampolines::Bytecode::JumpImmediately32Size);
}
typedef void (*RadiusDamage2_t) (Math::Vector3D vecSrc, SDK::Structures::EntityVariables_t *pevInflictor, SDK::Structures::EntityVariables_t *pevAttacker, float flDamage, float flRadius, int iClassIgnore, SDK::Constants::DamageType_t bitsDamageType);
RadiusDamage2_t RadiusDamage2 (NULL);
unsigned char RadiusDamage2_originalBytes[Trampolines::Bytecode::JumpImmediately32Size];
unsigned char RadiusDamage2_patchedBytes[Trampolines::Bytecode::JumpImmediately32Size];
void MY_RadiusDamage2 (Math::Vector3D vecSrc, SDK::Structures::EntityVariables_t *pevInflictor, SDK::Structures::EntityVariables_t *pevAttacker, float flDamage, float flRadius, int iClassIgnore, SDK::Constants::DamageType_t bitsDamageType)
{
	HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "MY_RadiusDamage2(): pevInflictor=%s, pevAttacker=%s, flDamage=%.1f, flRadius=%.2f, bitsDamageType=%i.", pevInflictor == NULL ? "NULL" : pevInflictor->containingEntity->IsPlayer () ? pevInflictor->containingEntity->GetNetName ().GetData () : pevInflictor->containingEntity->GetClassName ().GetData (), pevAttacker == NULL ? "NULL" : pevAttacker->containingEntity->IsPlayer () ? pevAttacker->containingEntity->GetNetName ().GetData () : pevAttacker->containingEntity->GetClassName ().GetData (), flDamage, flRadius, bitsDamageType);

	memcpy (RadiusDamage2, RadiusDamage2_originalBytes, Trampolines::Bytecode::JumpImmediately32Size);

		(*RadiusDamage2) (vecSrc, pevInflictor, pevAttacker, flDamage, flRadius, iClassIgnore, bitsDamageType);

	if (Trampolines::VirtualProtect (RadiusDamage2, Trampolines::Bytecode::JumpImmediately32Size, PAGE_EXECUTE_READWRITE))
		memcpy (RadiusDamage2, RadiusDamage2_patchedBytes, Trampolines::Bytecode::JumpImmediately32Size);
}
typedef void (*RadiusDamage_t) (Math::Vector3D vecSrc, SDK::Structures::EntityVariables_t *pevInflictor, SDK::Structures::EntityVariables_t *pevAttacker, float flDamage, float flRadius, int iClassIgnore, SDK::Constants::DamageType_t bitsDamageType);
RadiusDamage_t RadiusDamage (NULL);
unsigned char RadiusDamage_originalBytes[Trampolines::Bytecode::JumpImmediately32Size];
unsigned char RadiusDamage_patchedBytes[Trampolines::Bytecode::JumpImmediately32Size];
void MY_RadiusDamage (Math::Vector3D vecSrc, SDK::Structures::EntityVariables_t *pevInflictor, SDK::Structures::EntityVariables_t *pevAttacker, float flDamage, float flRadius, int iClassIgnore, SDK::Constants::DamageType_t bitsDamageType)
{
	HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "MY_RadiusDamage(): pevInflictor=%s, pevAttacker=%s, flDamage=%.1f, flRadius=%.2f, bitsDamageType=%i.", pevInflictor == NULL ? "NULL" : pevInflictor->containingEntity->IsPlayer () ? pevInflictor->containingEntity->GetNetName ().GetData () : pevInflictor->containingEntity->GetClassName ().GetData (), pevAttacker == NULL ? "NULL" : pevAttacker->containingEntity->IsPlayer () ? pevAttacker->containingEntity->GetNetName ().GetData () : pevAttacker->containingEntity->GetClassName ().GetData (), flDamage, flRadius, bitsDamageType);

	memcpy (RadiusDamage, RadiusDamage_originalBytes, Trampolines::Bytecode::JumpImmediately32Size);

		(*RadiusDamage) (vecSrc, pevInflictor, pevAttacker, flDamage, flRadius, iClassIgnore, bitsDamageType);

	if (Trampolines::VirtualProtect (RadiusDamage, Trampolines::Bytecode::JumpImmediately32Size, PAGE_EXECUTE_READWRITE))
		memcpy (RadiusDamage, RadiusDamage_patchedBytes, Trampolines::Bytecode::JumpImmediately32Size);
}

typedef Math::Vector3D &(__fastcall *BaseEntity__FireBullets3_t) (SDK::Classes::BaseEntity *entity, int /*dummy*/, Math::Vector3D &result, Math::Vector3D vecSrc, Math::Vector3D vecDirShooting, float vecSpread, float flDistance, int iPenetration, int iBulletType, int iDamage, float flRangeModifier, SDK::Structures::EntityVariables_t *pevAttacker, bool isUNKNOWN/* is single shot (pistols, awp) ???? */, int shared_rand);
BaseEntity__FireBullets3_t BaseEntity__FireBullets3 (NULL);
unsigned char BaseEntity__FireBullets3_originalBytes[Trampolines::Bytecode::JumpImmediately32Size];
unsigned char BaseEntity__FireBullets3_patchedBytes[Trampolines::Bytecode::JumpImmediately32Size];
Math::Vector3D &__fastcall MY_BaseEntity__FireBullets3 (SDK::Classes::BaseEntity *entity, int /*dummy*/, Math::Vector3D &result, Math::Vector3D vecSrc, Math::Vector3D vecDirShooting, float vecSpread, float flDistance, int iPenetration, int iBulletType, int iDamage, float flRangeModifier, SDK::Structures::EntityVariables_t *pevAttacker, bool isUNKNOWN/* is single shot (pistols, awp) ???? */, int shared_rand)
{
	if (!Trampolines::VirtualProtect (BaseEntity__FireBullets3, Trampolines::Bytecode::JumpImmediately32Size, PAGE_EXECUTE_READWRITE))
	{
		result = Math::Vector3D::ZeroValue;
		return result;
	}
/*
	const Client *const client (g_server->GetClientManager ()->GetClient (entity->GetEdict ()));
	const SDK::Constants::WeaponID_t currentWeaponID (client->GetCurrentWeapon ()->GetID ());
	const float MY_vecSpread (client->GetCurrentWeapon ()->GetSpreadOffset ());
	const float MY_flDistance (HalfLifeEngine::SDK::Constants::weaponProperties[currentWeaponID].maximumShootDistance1);
	const int MY_iPenetration (HalfLifeEngine::SDK::Constants::weaponProperties[currentWeaponID].penetrationPower2);
	const int MY_iBulletType (HalfLifeEngine::SDK::Constants::weaponProperties[currentWeaponID].bulletType);
	const int MY_iDamage (HalfLifeEngine::SDK::Constants::weaponProperties[currentWeaponID].damage1);
	const float MY_flRangeModifier (HalfLifeEngine::SDK::Constants::weaponProperties[currentWeaponID].obstaclePierce1);
	const Math::Vector2D MY_result (client->GetCurrentWeapon ()->GetSpreadOffset2D ());
	HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat ("%s::FireBullets3(): MY_vecSpread=%.5f, MY_flDistance=%.2f, MY_iPenetration=%i, MY_iBulletType=%i, MY_iDamage=%i, MY_flRangeModifier=%.2f, MY_shared_rand=%u, MY_result=[%.10f, %.10f].\n", entity->GetEdict ()->GetNetName ().GetData (), MY_vecSpread, MY_flDistance, MY_iPenetration, MY_iBulletType, MY_iDamage, MY_flRangeModifier, entity->GetBasePlayerPointer ()->random_seed, MY_result.x, MY_result.y);
*/
	memcpy (BaseEntity__FireBullets3, BaseEntity__FireBullets3_originalBytes, Trampolines::Bytecode::JumpImmediately32Size);

	(*BaseEntity__FireBullets3) (entity, 0, result, vecSrc, vecDirShooting, vecSpread/*/Console::Bot::variableManager.GetVariable (Console::Bot::VariableID_KnifeMode)->GetValue <float> ()*/, flDistance, iPenetration, iBulletType, iDamage, flRangeModifier, pevAttacker, isUNKNOWN, shared_rand);

	memcpy (BaseEntity__FireBullets3, BaseEntity__FireBullets3_patchedBytes, Trampolines::Bytecode::JumpImmediately32Size);

	const Math::Vector3D vecDir = vecDirShooting +
	      result.x * Globals::g_halfLifeEngine->GetGlobalVariables ()->right +
		  result.y * Globals::g_halfLifeEngine->GetGlobalVariables ()->upward;
	const Math::Vector3D vecEnd = vecSrc + vecDir * flDistance;

	#if defined MULTIPLE_DRAW_METHODS
	g_drawInterface->DrawLineTimed (entity->m_variables->origin + entity->m_variables->viewOffset, vecEnd, Color <> (255u, 0u, 0u), 0.5f, 255u, 1u, false);
	#endif	// if defined MULTIPLE_DRAW_METHODS

	HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat ("%s::FireBullets3(): vecSpread=%.5f, flDistance=%.2f, iPenetration=%i, iBulletType=%i, iDamage=%i, flRangeModifier=%.2f, pevAttacker=%s, isUNKNOWN(single shot?)=%u, shared_rand=%u, result=[%.10f, %.10f].\n", entity->GetEdict ()->GetNetName ().GetData (), vecSpread, flDistance, iPenetration, iBulletType, iDamage, flRangeModifier, pevAttacker == NULL ? "NULL" : pevAttacker->containingEntity->IsPlayer () ? pevAttacker->containingEntity->GetNetName ().GetData () : pevAttacker->containingEntity->GetClassName ().GetData (), isUNKNOWN, shared_rand, result.x, result.y);
//	HalfLifeEngine::Globals::g_halfLifeEngine->PrintFormat (HalfLifeEngine::SDK::Constants::HUDPrint_Talk, "%s::FireBullets3(): vecSpread=%.2f, flDistance=%.2f, iPenetration=%i, iBulletType=%i, iDamage=%i, flRangeModifier=%.2f, pevAttacker=%s, isUNKNOWN=%u, shared_rand=%u.", entity->GetEdict ()->GetNetName ().GetData (), vecSpread, flDistance, iPenetration, iBulletType, iDamage, flRangeModifier, pevAttacker == NULL ? "NULL" : pevAttacker->containingEntity->IsPlayer () ? pevAttacker->containingEntity->GetNetName ().GetData () : pevAttacker->containingEntity->GetClassName ().GetData (), isUNKNOWN, shared_rand);

//	HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat ("EQUAL: [vecSpread=%u, result eq=%u]\n", MY_vecSpread == vecSpread, MY_result == result);

	return result;
}

typedef void (SDK::Classes::NonCZCSBot::*CSBot__Upkeep_t) (void);
CSBot__Upkeep_t g_originalCSBot__Upkeep (NULL);
void __fastcall CSBot__Upkeep (SDK::Classes::NonCZCSBot *bot, int /*dummy*/)
{
	(bot->*g_originalCSBot__Upkeep) ();

	g_server->GetHumanManager ()->GetHostClient ()->DrawCross3D (bot->m_enemyShootPosition, 15.0f, Color <> (255u, 0u, 0u), 1u, 255u, 1u, false);
	g_server->GetHumanManager ()->GetHostClient ()->DrawCross3D (bot->m_enemyShootPosition - bot->m_aimOffset, 15.0f, Color <> (0u, 0u, 255u), 1u, 255u, 1u, false);
}

typedef void (*InsertLinkBefore_t) (SDK::Structures::Link_t *const link, SDK::Structures::Link_t *const before);
InsertLinkBefore_t g_originalInsertLinkBefore (NULL);
void InsertLinkBefore (SDK::Structures::Link_t *const link, SDK::Structures::Link_t *const before)
{
	SDK::Classes::Edict *const edict ((SDK::Classes::Edict *)((unsigned int)link - 8u));

	InternalAssert (edict->IsValid ());

//	if (origin changed)
		HalfLifeEngine::Globals::g_halfLifeEngine->ServerPrintFormat ("InsertLinkBefore(): Entity: \"%s\".\n", edict->IsPlayer () ? edict->GetNetName ().GetData () : edict->GetClassName ().GetData ());

	(*g_originalInsertLinkBefore) (link, before);
}
}

static void Host_InitProcessor( void )
{
	const CPUInformation& pi = GetCPUInformation();

	// Compute Frequency in Mhz: 
	char* szFrequencyDenomination = "Mhz";
	double fFrequency = pi.m_Speed / 1000000.0;

	// Adjust to Ghz if nessecary:
	if( fFrequency > 1000.0 )
	{
		fFrequency /= 1000.0;
		szFrequencyDenomination = "Ghz";
	}

	char szFeatureString[256];
	strcpy( szFeatureString, pi.m_szProcessorID );
	strcat( szFeatureString, " " );
	
	if( pi.m_bSSE )
	{
		strcat(szFeatureString, "SSE " );
	}
	
	if( pi.m_bSSE2 )
	{
		strcat(szFeatureString, "SSE2 " );
	}
	
	if( pi.m_bMMX )
	{
		strcat(szFeatureString, "MMX " );
	}

	if( pi.m_b3DNow )
	{
		strcat(szFeatureString, "3DNow " );
	}

	if( pi.m_bRDTSC )	strcat(szFeatureString, "RDTSC " );
	if( pi.m_bCMOV )	strcat(szFeatureString, "CMOV " );
	if( pi.m_bFCMOV )	strcat(szFeatureString, "FCMOV " );

	// Remove the trailing space.  There will always be one.
	szFeatureString[strlen(szFeatureString)-1] = '\0';

	// Dump CPU information:
	if( pi.m_nLogicalProcessors == 1 )
	{
		Globals::g_halfLifeEngine->ServerPrintFormat( "1 CPU, Frequency: %.01f %s,  Features: %s\n", 
			fFrequency,
			szFrequencyDenomination,
			szFeatureString
			);
	} else
	{
		char buffer[256] = "";
		if( pi.m_nPhysicalProcessors != pi.m_nLogicalProcessors )
		{
			sprintf(buffer, " (%i physical)", (int) pi.m_nPhysicalProcessors );
		}

		Globals::g_halfLifeEngine->ServerPrintFormat( "%i CPUs%s, Frequency: %.01f %s,  Features: %s\n", 
			(int)pi.m_nLogicalProcessors,
			buffer,
			fFrequency,
			szFrequencyDenomination,
			szFeatureString
			);
	}
}

	/*static */const char *Engine::sm_precachedStrings[PrecachedStringID_Total] =
	{
		// Sounds:
		"weapons/xbow_hit1.wav",		// PrecachedStringID_weapons_slash_xbow_hit1_dot_wav
		"weapons/mine_activate.wav",	// PrecachedStringID_weapons_slash_mine_activate_dot_wav
		"debris/bustglass1.wav",		// PrecachedStringID_debris_slash_bustglass1_dot_wav
		"items/medshot4.wav",			// PrecachedStringID_items_slash_medshot4_dot_wav

		// Models:
//		"models/mechgibs.mdl",			// PrecachedStringID_models_slash_mechgibs_dot_mdl

		"sprites/laserbeam.spr",		// PrecachedStringID_sprites_slash_laserbeam_dot_spr
		"sprites/arrow1.spr"			// PrecachedStringID_sprites_slash_arrow1_dot_spr
	};

	Engine::Engine (SDK::Structures::EngineFunction_t *const engineFunctionTable, SDK::Structures::GlobalVariables_t *const globalVariables) :
		m_engineModule (MemoryUtilities::GetModuleInformation (globalVariables)),
		m_gameDLL (NULL),

		// Get the engine functions from the engine....
		m_engineFunctionTable (*engineFunctionTable),	// have a copy of this...

		m_gameModID (Version_Unknown),

		m_renderingMode ((*engineFunctionTable->pfnIsDedicatedServer) () || GetModuleHandle ("sw.DLL") != NULL ? RenderMode_Software : (*engineFunctionTable->pfnCVarGetFloat) ("vid_d3d") == 0.0f ? RenderMode_OpenGL : RenderMode_Direct3D),

		m_gameDLLFunctionAPITablePointer (NULL),
		m_newGameDLLFunctionAPITablePointer (NULL),
//		m_engineFunctionTablePointer (engineFunctionTable),
		m_clientEngineFunctionTablePointer ((*engineFunctionTable->pfnIsDedicatedServer) () ? NULL : &MemoryUtilities::GetElementAddressFromOtherAddressAndOffset <SDK::Structures::ClientEngineFunction_t> (MemoryUtilities::/*DecodeAnd*/FindPattern (m_engineModule, signature_ClientEngineFunctionTable, sizeof (signature_ClientEngineFunctionTable)), Offset_ClientEngineFunctionTable_Pointer)),
		m_clientDLLExportTablePointer ((*engineFunctionTable->pfnIsDedicatedServer) () ? NULL : CHEAT::OffsetExportTable (m_engineModule)),
		m_clientStudioModelRendererPointer ((*engineFunctionTable->pfnIsDedicatedServer) () ? NULL : CHEAT::OffsetCStudioModelRenderer ()),
		m_clientEngineStudioAPIPointer ((*engineFunctionTable->pfnIsDedicatedServer) () ? NULL : CHEAT::OffsetEngineStudio (m_engineModule)),
		m_playerMovePointer (NULL),
//		m_rotationMatrix (NULL),
		m_boneTransformMatrix (NULL),
		m_globalVariables (globalVariables),	// Keep track of the engine global variables structure.

		m_fileSystem (MemoryUtilities::GetElementAddressFromOtherAddressAndOffset <SDK::Classes::Interface::FileSystem *> (MemoryUtilities::/*DecodeAnd*/FindPattern (m_engineModule, signature_FileSystem, sizeof (signature_FileSystem)), Offset_FileSystem_Pointer)),

		m_engineFactory (m_engineModule.GetFunctionAddress <SDK::Classes::Interface::CreateFunction_t> (CREATEINTERFACE_PROCNAME)),
		m_engine (GetInterface_Engine (m_engineModule)),
		m_cdaudio ((*engineFunctionTable->pfnIsDedicatedServer) () ? NULL : GetInterface_CDAudio (m_engine)),
		m_surface (static_cast <SDK::VGUI::ISurface *> (m_engineFactory (VGUI_SURFACE_INTERFACE_VERSION, NULL))),

		m_realTime (MemoryUtilities::GetElementAddressFromOtherAddressAndOffset <const double> (MemoryUtilities::/*DecodeAnd*/FindPattern (m_engineModule, signature_RealTime, sizeof (signature_RealTime)), Offset_RealTime_Pointer)),
		m_SV_InactivateClientsFunction (static_cast <const SDK::TypeDefinitions::SV_InactivateClientsFunction_t> (MemoryUtilities::/*DecodeAnd*/FindPattern (m_engineModule, signature_SV_InactivateClients, sizeof (signature_SV_InactivateClients)))),
		m_GL_DisableMultitextureFunction ((*engineFunctionTable->pfnIsDedicatedServer) () ? NULL : static_cast <const SDK::TypeDefinitions::GL_DisableMultitextureFunction_t> (MemoryUtilities::/*DecodeAnd*/FindPattern (m_engineModule, signature_GL_DisableMultitexture, sizeof (signature_GL_DisableMultitexture)))),
		m_SV_CalcPing (::GetSV_CalcPingFunction (m_engineModule))
	{
		// This is a Half-Life engine class constructor.

		{
			assert (IsDedicatedServer () || (GetModuleHandle ("sw.DLL") == NULL) != (GetModuleHandle ("hw.DLL") == NULL));
			assert (m_fileSystem != NULL);
			assert (m_engine != NULL);
			assert (m_SV_InactivateClientsFunction != NULL);
			assert (m_SV_CalcPing != NULL);
		}

		Globals::g_halfLifeEngine = this;	/// @note Assign it here due to 'Globals::g_halfLifeEngine' pointer is used in AddLogEntry() function!

		// Reliability check.
		InternalAssert (m_surface != NULL);

		RedirectCallFromSV_InactivateClientsFunction (m_engineModule);
//		PatchSV_InactivateClientsFunction (m_SV_InactivateClientsFunction);

		const struct ModSupport_t
		{
			GameModType_t id;					// mod ID
			DynamicString name;					// mod name (the game directory)
			DynamicString windowsDLLFilename;	// filename of win32 DLL
			DynamicString description;			// mod description
		} supportedMods[Version_Total] =
		{
			{Version_Steam,         "cstrike", "mp.DLL", "Counter-Strike v1.6+"},
			{Version_ConditionZero, "czero",   "mp.DLL", "Counter-Strike: Condition Zero"}
		};

		if (!IsDedicatedServer ())
		{
			{
				assert (m_clientEngineFunctionTablePointer != NULL);
				assert (m_clientDLLExportTablePointer != NULL);
				assert (m_clientStudioModelRendererPointer != NULL);
				assert (m_clientEngineStudioAPIPointer != NULL);
				assert (m_GL_DisableMultitextureFunction != NULL);
				assert (m_cdaudio != NULL);

				assert (m_engineModule.GetPointer () == GetModuleHandle ("hw.DLL"));

//				assert (GetCDAudio () == GetEngineModule ().GetMemory <SDK::Classes::Interface::CDAudio *> (0x16656Cu));
			}

			// have a copy of this...
			m_clientEngineFunctionTable = *m_clientEngineFunctionTablePointer;
			m_clientDLLExportTable = *m_clientDLLExportTablePointer;
			m_clientStudioModelRenderer = *m_clientStudioModelRendererPointer;
			m_clientEngineStudioAPI = *m_clientEngineStudioAPIPointer;
/*
			{
			CHEAT::S_StartDynamicSound = (CHEAT::S_StartDynamicSound_t)((unsigned int)m_engineModule.GetPointer () + 0xA2BC0u);
			
			// Reliability checks.
			InternalAssert (CHEAT::S_StartDynamicSound != NULL);
			InternalAssert (((unsigned char *)CHEAT::S_StartDynamicSound)[0u] == 0x83 && ((unsigned char *)CHEAT::S_StartDynamicSound)[1u] == 0xEC && ((unsigned char *)CHEAT::S_StartDynamicSound)[2u] == 0x48);	// sub     esp, 48h        ; Integer Subtraction

			// Save the original signature....
			memcpy (CHEAT::S_StartDynamicSound_originalBytes, CHEAT::S_StartDynamicSound, sizeof (CHEAT::S_StartDynamicSound_originalBytes));

			if (!Trampolines::WriteFunctionJump32 (CHEAT::S_StartDynamicSound, &CHEAT::MY_S_StartDynamicSound))
				AddLogEntry (false, LogLevel_Critical, true, "S_StartDynamicSound(): Trampolines::WriteFunctionJump32() filed!");
			else
				memcpy (CHEAT::S_StartDynamicSound_patchedBytes, CHEAT::S_StartDynamicSound, sizeof (CHEAT::S_StartDynamicSound_patchedBytes));
			}
*/
			m_clientDLLExportTablePointer->HUD_DrawNormalTriangles = &CHEAT::HUD_DrawNormalTriangles;
//			m_clientDLLExportTablePointer->HUD_PlayerMove = &CHEAT::HUD_PlayerMove;
			m_clientDLLExportTablePointer->HUD_Redraw = &CHEAT::HUD_Redraw;
//			m_clientDLLExportTablePointer->IN_MouseEvent = &CHEAT::IN_MouseEvent;
//			m_clientDLLExportTablePointer->HUD_Key_Event = &CHEAT::HUD_Key_Event;
//			m_clientDLLExportTablePointer->CL_CreateMove = &CHEAT::CL_CreateMove;
//			m_clientDLLExportTablePointer->CL_CreateMove = &CHEAT::CL_CreateMove_DUMMY;
//			m_clientDLLExportTablePointer->V_CalcRefdef = &CHEAT::V_CalcRefdef;
//			m_clientDLLExportTablePointer->V_CalcRefdef = &CHEAT::V_CalcRefdef_DUMMY;

			{
			VoidBaseHookedClassWithVirtualFunctionTable *const eng (reinterpret_cast <VoidBaseHookedClassWithVirtualFunctionTable *> (m_engine));
			{
			const unsigned char index (10u);

			if (!Trampolines::VirtualProtect (&eng->GetPseudoVirtualFunctionTable ()[index], sizeof (eng->GetPseudoVirtualFunctionTable ()[index]), PAGE_READWRITE))
				AddLogEntry (false, LogLevel_Critical, true, "VirtualProtect(Engine::TrapKey_Event) failed!\nError code: %i.", GetLastError ());

			CHEAT::g_originalEngine__TrapKey_Event = reinterpret_cast <CHEAT::Engine__TrapKey_Event_t &> (eng->GetPseudoVirtualFunctionTable ()[index]);
			eng->GetPseudoVirtualFunctionTable ()[index] = &CHEAT::Engine__TrapKey_Event;
			}
			{
			const unsigned char index (11u);

			if (!Trampolines::VirtualProtect (&eng->GetPseudoVirtualFunctionTable ()[index], sizeof (eng->GetPseudoVirtualFunctionTable ()[index]), PAGE_READWRITE))
				AddLogEntry (false, LogLevel_Critical, true, "VirtualProtect(Engine::TrapMouse_Event) failed!\nError code: %i.", GetLastError ());

			CHEAT::g_originalEngine__TrapMouse_Event = reinterpret_cast <CHEAT::Engine__TrapMouse_Event_t &> (eng->GetPseudoVirtualFunctionTable ()[index]);
			eng->GetPseudoVirtualFunctionTable ()[index] = &CHEAT::Engine__TrapMouse_Event;
			}
			}
/*
			const unsigned int HUD_GetStudioModelInterfaceAddress ((unsigned int) GetProcAddress (GetModuleHandle ("client.DLL"), "HUD_GetStudioModelInterface"));
			const unsigned char HUD_GetStudioModelInterfaceOffset (GET_STATIC_STRING_LENGTH ("\x83\x7C\x24\x04\x01\x74\x03\x33\xC0\xC3\x8B\x44\x24\x08\x56\x8B\x74\x24\x10\x57\xB9\x2E\x00\x00\x00\xBF\x50\x8D\xA3\x01\xC7\x00\x40\x79\x9E\x01\xF3\xA5\xB9"));
			const unsigned int g_StudioRendererAddress (HUD_GetStudioModelInterfaceAddress + HUD_GetStudioModelInterfaceOffset);
			class StudioModelRenderer
			{
				public:
					SDK::Structures::StudioModelRenderer_t *m_virtualFunctionTable;
			};
			StudioModelRenderer &g_StudioRenderer (**(StudioModelRenderer **) g_StudioRendererAddress);	// dword_1A26960

			if (!Trampolines::VirtualProtect (&g_StudioRenderer.m_virtualFunctionTable->StudioRenderModel, sizeof (g_StudioRenderer.m_virtualFunctionTable->StudioRenderModel), PAGE_READWRITE))
				AddLogEntry (false, LogLevel_Critical, true, "VirtualProtect(StudioModelRenderer::StudioRenderModel) failed!\nError code: %i.", GetLastError ());

			g_StudioRenderer.m_virtualFunctionTable->StudioRenderModel = &CHEAT::StudioRenderModel;
*/
//			assert (m_engineModule.InterceptDllCall ("Kernel32.DLL", "LoadLibraryA", (PVOID)&LoadLibraryA_MY, (PVOID*)&g_originalLoadLibraryA));
//			assert (m_engineModule.InterceptDllCall ("Kernel32.DLL", "FreeLibrary", (PVOID)&FreeLibrary_MY, (PVOID*)&g_originalFreeLibrary));
//			assert (m_engineModule.InterceptDllCall ("Kernel32.DLL", "OutputDebugStringA", (PVOID)&OutputDebugStringA_MY, (PVOID*)&g_originalOutputDebugStringA));
		}
		else
			assert (m_engineModule.GetPointer () == GetModuleHandle ("swds.DLL"));

		// Assign the game mod name after we got the engine functions....
		{
			char gameDirectory[_MAX_DIR];

			// Ask the engine for the MOD directory path....
			// Prior to HL 1.1.1.1 (CS 1.6), this gave the full pathname of the game directory,
			// i.e. "/usr/local/half-life/cstrike" or "C:\Games\Half-Life\cstrike".
			// Now, it is only the string passed to HLDS as the argument to "-game", i.e. "cstrike".
			(*m_engineFunctionTable.pfnGetGameDir) (gameDirectory);

			m_gameModName = gameDirectory;
		}

		// Reliability check.
		InternalAssert (m_engineModule.IsValid ());

		// loop through all supported mods (find the known for YaPB mod, corresponding to the given game name)
		for (unsigned char knownGameModIndex (Version_Steam); knownGameModIndex < Version_Total; ++knownGameModIndex)
		{
			// is game mod found by name?
			if (supportedMods[knownGameModIndex].name.CompareWithoutCaseCheck (GetGameModName ()) != 0)
				continue;

			// keep track of this mod id
			m_gameModID = supportedMods[knownGameModIndex].id;	// Remember it

			// Get path to game DLL, check if DLL exists, othervise get local copy, and load it....
			{
				// get known mod game.DLL name....
				const DynamicString gameDLLPathName ("dlls/" + supportedMods[knownGameModIndex].windowsDLLFilename);	// (as the engine filesystem does not need a mod name in a path name)
				const DynamicString fullGameDLLPathName (GetGameModName () + '/' + gameDLLPathName);
/*! @note OLD CODE INSTEAD OF NEW: GetFileSystem ()->GetLocalCopy (fullGameDLLPathName);
				// test if the game.DLL file is NOT available outside of the steam cache
				if (!STDIOFile::IsExists (fullGameDLLPathName))
				{
					// try to extract the game.DLL out of the steam cache
/// @note TEMP FIX: VARIABLE "yb_log_level" IS NOT INITIALIZED YET!!!					AddLogEntry (false, LogLevel_Warning, true, "Trying to extract '%s' out of the steam cache....", gameDLLPathName.GetData ());	// log it

					const DynamicString gameDLLDirectory (GetGameModName () + "/dlls");

					// is directory for new game.DLL not exists?
					if (!IsDirectoryExists (gameDLLDirectory))
					{
/// @note TEMP FIX: VARIABLE "yb_log_level" IS NOT INITIALIZED YET!!!						AddLogEntry (false, LogLevel_Default, true, "Directory for new game.DLL '%s' not created, trying to create it....", gameDLLDirectory.GetData ());	// log it

						// is directory for new game.DLL not created?
						if (!CreateDirectory (gameDLLDirectory))
						{
							AddLogEntry (false, LogLevel_Critical, false, "Couldn't create a new game.DLL '%s' directory.", gameDLLDirectory.GetData ());

							return;
						}
					}

					Utilities::FileBuffer gameDLL (gameDLLPathName);	// open game.DLL from steam cache

					// is game.DLL found?
					InternalAssert (gameDLL.IsValid ());

					// create and open new game.DLL in binary write mode
					STDIOFile newGameDLL (fullGameDLLPathName, "wb");

					// is file created?
					if (!newGameDLL.IsValid ())
					{
						AddLogEntry (false, LogLevel_Critical, false, "Couldn't create a new '%s' file.", fullGameDLLPathName.GetData ());

						return;
					}

					// if in cache, then extract it
					if (!newGameDLL.WriteFile (gameDLL))
					{
						AddLogEntry (false, LogLevel_Critical, false, "Couldn't write buffer into a new '%s' file.", fullGameDLLPathName.GetData ());

						return;
					}
				}
*/
				MakeLocalFileCopy (fullGameDLLPathName);

				m_gameDLL = new LibraryWithSize (fullGameDLLPathName);

				// Reliability check.
				if (m_gameDLL == NULL)
					TerminateOnMalloc ();
			}
/*
			{
			unsigned char *address (reinterpret_cast <unsigned char *> (GetEngineModule ().GetPointer ()) + 0xDD480u);

			if (address != NULL)
			{
				// Reliability check.
				InternalAssert ((address - sizeof (Trampolines::Bytecode::CallImmediately32))[0u] == Trampolines::Bytecode::CallImmediately32);

				Trampolines::RedirectFunctionCall32 (address, &CHEAT::InsertLinkBefore, reinterpret_cast <const void *&> (CHEAT::g_originalInsertLinkBefore));
			}
			}
/*
			{
			CHEAT::BaseEntity__FireBullets3 = (CHEAT::BaseEntity__FireBullets3_t)((unsigned int)m_gameDLL->GetPointer () + 0x78270u);
			
			// Reliability checks.
			InternalAssert (CHEAT::BaseEntity__FireBullets3 != NULL);
			InternalAssert (((unsigned char *)CHEAT::BaseEntity__FireBullets3)[0u] == 0x81 && ((unsigned char *)CHEAT::BaseEntity__FireBullets3)[1u] == 0xEC);

			// Save the original signature....
			memcpy (CHEAT::BaseEntity__FireBullets3_originalBytes, CHEAT::BaseEntity__FireBullets3, sizeof (CHEAT::BaseEntity__FireBullets3_originalBytes));

			if (!Trampolines::WriteFunctionJump32 (CHEAT::BaseEntity__FireBullets3, &CHEAT::MY_BaseEntity__FireBullets3))
				AddLogEntry (false, LogLevel_Critical, true, "BaseEntity__FireBullets3(): Trampolines::WriteFunctionJump32() filed!");
			else
				memcpy (CHEAT::BaseEntity__FireBullets3_patchedBytes, CHEAT::BaseEntity__FireBullets3, sizeof (CHEAT::BaseEntity__FireBullets3_patchedBytes));
			}
/*
			{
			const unsigned char index (90u);
			const void **const CCSBot__vtable = (const void **)((unsigned int)m_gameDLL->GetPointer () + 0xFF158u);

			if (!Trampolines::VirtualProtect (&CCSBot__vtable[index], sizeof (CCSBot__vtable[index]), PAGE_READWRITE))
				AddLogEntry (false, LogLevel_Critical, true, "VirtualProtect(CCSBot::Upkeep) failed!\nError code: %i.", GetLastError ());

			CHEAT::g_originalCSBot__Upkeep = reinterpret_cast <CHEAT::CSBot__Upkeep_t &> (CCSBot__vtable[index]);
			CCSBot__vtable[index] = &CHEAT::CSBot__Upkeep;
			}
/*
			{
			CHEAT::CBaseMonster__RadiusDamage = (CHEAT::CBaseMonster__RadiusDamage_t)((unsigned int)m_gameDLL->GetPointer () + 0x77050u);
			
			// Reliability checks.
			InternalAssert (CHEAT::CBaseMonster__RadiusDamage != NULL);
			InternalAssert (((unsigned char *)CHEAT::CBaseMonster__RadiusDamage)[0u] == 0xD9 && ((unsigned char *)CHEAT::CBaseMonster__RadiusDamage)[1u] == 0x44);

			// Save the original signature....
			memcpy (CHEAT::CBaseMonster__RadiusDamage_originalBytes, CHEAT::CBaseMonster__RadiusDamage, sizeof (CHEAT::CBaseMonster__RadiusDamage_originalBytes));

			if (!Trampolines::WriteFunctionJump32 (CHEAT::CBaseMonster__RadiusDamage, &CHEAT::MY_CBaseMonster__RadiusDamage))
				AddLogEntry (false, LogLevel_Critical, true, "CBaseMonster__RadiusDamage(): Trampolines::WriteFunctionJump32() filed!");
			else
				memcpy (CHEAT::CBaseMonster__RadiusDamage_patchedBytes, CHEAT::CBaseMonster__RadiusDamage, sizeof (CHEAT::CBaseMonster__RadiusDamage_patchedBytes));
			}
			{
			CHEAT::RadiusDamage2 = (CHEAT::RadiusDamage2_t)((unsigned int)m_gameDLL->GetPointer () + 0x76D70u);
			
			// Reliability checks.
			InternalAssert (CHEAT::RadiusDamage2 != NULL);
			InternalAssert (((unsigned char *)CHEAT::RadiusDamage2)[0u] == 0xD9 && ((unsigned char *)CHEAT::RadiusDamage2)[1u] == 0x44);

			// Save the original signature....
			memcpy (CHEAT::RadiusDamage2_originalBytes, CHEAT::RadiusDamage2, sizeof (CHEAT::RadiusDamage2_originalBytes));

			if (!Trampolines::WriteFunctionJump32 (CHEAT::RadiusDamage2, &CHEAT::MY_RadiusDamage2))
				AddLogEntry (false, LogLevel_Critical, true, "RadiusDamage2(): Trampolines::WriteFunctionJump32() filed!");
			else
				memcpy (CHEAT::RadiusDamage2_patchedBytes, CHEAT::RadiusDamage2, sizeof (CHEAT::RadiusDamage2_patchedBytes));
			}
			{
			CHEAT::RadiusDamage = (CHEAT::RadiusDamage_t)((unsigned int)m_gameDLL->GetPointer () + 0x768D0u);
			
			// Reliability checks.
			InternalAssert (CHEAT::RadiusDamage != NULL);
			InternalAssert (((unsigned char *)CHEAT::RadiusDamage)[0u] == 0xD9 && ((unsigned char *)CHEAT::RadiusDamage)[1u] == 0x44);

			// Save the original signature....
			memcpy (CHEAT::RadiusDamage_originalBytes, CHEAT::RadiusDamage, sizeof (CHEAT::RadiusDamage_originalBytes));

			if (!Trampolines::WriteFunctionJump32 (CHEAT::RadiusDamage, &CHEAT::MY_RadiusDamage))
				AddLogEntry (false, LogLevel_Critical, true, "RadiusDamage(): Trampolines::WriteFunctionJump32() filed!");
			else
				memcpy (CHEAT::RadiusDamage_patchedBytes, CHEAT::RadiusDamage, sizeof (CHEAT::RadiusDamage_patchedBytes));
			}
*/
			const char *const renderingModeString[] =
			{
				"software",

				"hardware (OpenGL)",
				"hardware (Direct3D)"
			};

			// Print a message to notify about plugin attaching.
			ServerPrintFormat
			(
				supportedMods[knownGameModIndex].description + " game mod registered (0x%u).\n"
				"\tEngine DLL base address = 0x%X.\n"
				"\tGame mod DLL base address = 0x%X.\n"
				"\tYaPB DLL base address = 0x%X;\n"
				"Game is running in %s rendering mode.\n\n",

				sizeof (YaPB),
				m_engineModule.GetPointer (),
				m_gameDLL->GetPointer (),
				g_YaPBModuleHandle.GetPointer (),
				renderingModeString[GetRenderingMode ()]
			);	// print to console
/*! @note TEMP FIX: VARIABLE "yb_log_level" IS NOT INITIALIZED YET!!!			AddLogEntry
			(
				true, LogLevel_Default, true,
				supportedMods[knownGameModIndex].description + " game mod registered (0x%u).\n"
				"\tEngine DLL base address = 0x%X.\n"
				"\tGame mod DLL base address = 0x%X.\n"
				"\tYaPB DLL base address = 0x%X;\n"
				"Game is running in %s rendering mode.\n",

				sizeof (YaPB),
				m_engineModule.GetPointer (),
				m_gameDLL->GetPointer (),
				g_YaPBModuleHandle.GetPointer (),
				renderingModeString[GetRenderingMode ()]
			);	// print to console and log it
*/
			GetVersion ();
			Host_InitProcessor ();

			return;
		}

		// No match found....
		AddLogEntry (false, LogLevel_Critical, false, "Mod that you has started, not supported by YaPB (game directory: '%s').", GetGameModName ().GetData ());
	}

	void Engine::RestoreAllRedirectionsAndHooks (void)
	{
		// Restore all back....

		RestoreSV_InactivateClientsFunction ();

		if (!IsDedicatedServer ())
		{
			m_clientDLLExportTablePointer->HUD_DrawNormalTriangles = m_clientDLLExportTable.HUD_DrawNormalTriangles;
			m_clientDLLExportTablePointer->HUD_Redraw = m_clientDLLExportTable.HUD_Redraw;

			{
				VoidBaseHookedClassWithVirtualFunctionTable *const eng (reinterpret_cast <VoidBaseHookedClassWithVirtualFunctionTable *> (m_engine));
				{
				const unsigned char index (10u);

				if (!Trampolines::VirtualProtect (&eng->GetPseudoVirtualFunctionTable ()[index], sizeof (eng->GetPseudoVirtualFunctionTable ()[index]), PAGE_READWRITE))
					AddLogEntry (false, LogLevel_Critical, true, "VirtualProtect(Engine::TrapKey_Event) failed!\nError code: %i.", GetLastError ());

				eng->GetPseudoVirtualFunctionTable ()[index] = reinterpret_cast <void *&> (CHEAT::g_originalEngine__TrapKey_Event);
				}
				{
				const unsigned char index (11u);

				if (!Trampolines::VirtualProtect (&eng->GetPseudoVirtualFunctionTable ()[index], sizeof (eng->GetPseudoVirtualFunctionTable ()[index]), PAGE_READWRITE))
					AddLogEntry (false, LogLevel_Critical, true, "VirtualProtect(Engine::TrapMouse_Event) failed!\nError code: %i.", GetLastError ());

				eng->GetPseudoVirtualFunctionTable ()[index] = reinterpret_cast <void *&> (CHEAT::g_originalEngine__TrapMouse_Event);
				}
			}
		}
/*
		if (GetGameModID () != Engine::Version_ConditionZero)
			Utilities::g_ZBotInGameEnabler.DisableBotInGame ();
*/	}

	void Engine::ComputeBuild (void)
	{
		unsigned int DaysCounter = 0;
		const unsigned char monthsCount (12u);
		const struct Month_t
		{
			char          name[sizeof ("Jan")];	// month names
			unsigned char days;		// month days
		} months[monthsCount] =
		{
			{"Jan", 31u},
			{"Feb", 28u},
			{"Mar", 31u},
			{"Apr", 30u},
			{"May", 31u},
			{"Jun", 30u},
			{"Jul", 31u},
			{"Aug", 31u},
			{"Sep", 30u},
			{"Oct", 31u},
			{"Nov", 30u},
			{"Dec", 31u}
		};	// array of the months
		int year;
		int Res;
		unsigned char i = 0;

		do
		{
			if (strnicmp (months[i].name, m_buildInformation.date, GET_STATIC_STRING_LENGTH (months[i].name)) == 0)
				break;

			DaysCounter += months[i].days;
		} while (++i < monthsCount);

		DaysCounter += m_buildInformation.date.GetValue <unsigned char> (sizeof ("Mar")) - 1u;	// finally calculate day

		year = m_buildInformation.date.GetValue <unsigned short> (sizeof ("Mar  8")) - 1900;
		Res = DaysCounter + ((unsigned int)((year - 1) * 365.25f));

		if (year % 4u == 0 && i > 1)
		   ++Res;

		Res -= 34995;	// Oct 24 1996

		m_buildInformation.number = Res;	// DIFF IS +63!
	}

	void Engine::GetVersion (void)
	{
		const unsigned char versionPattern[] = {'E', 'x', 'e', ' ', 'b', 'u', 'i', 'l', 'd', ':', ' ', 0x2A, 0x2A, ':', 0x2A, 0x2A, ':', 0x2A, 0x2A, ' ', 0x2A, 0x2A, 0x2A, ' ', 0x2A, 0x2A, ' ', 0x2A, 0x2A, 0x2A, 0x2A, ' ', '(', '%', 'i', ')'};
		const char *string (static_cast <const char *> (MemoryUtilities::FindPattern (m_engineModule, versionPattern, sizeof (versionPattern))));

		// Reliability check.
		InternalAssert (string != NULL);

		string += GET_STATIC_STRING_LENGTH ("Exe build: ");

		m_buildInformation.time.Assign (string, GET_STATIC_STRING_LENGTH ("14:21:06"));

		string += m_buildInformation.time.GetElementNumber () + sizeof (' ');	// Example: "14:21:06 "

		m_buildInformation.date.Assign (string, GET_STATIC_STRING_LENGTH ("Mar  8 2010"));

		ComputeBuild ();

		m_buildInformation.protocol = 48u;	//! TEMP!!!!!!!!! @todo

		const DynamicString version ("EXTERNAL(Look in ModName\\steam.inf)");

		ServerPrintFormat ("Protocol version %u\nExe version %s (ModName)\nExe build: %s %s (%u).\n", m_buildInformation.protocol, version.GetData (), m_buildInformation.time.GetData (), m_buildInformation.date.GetData (), m_buildInformation.number);
	}

	const bool Engine::MakeLocalFileCopy (const DynamicString &filename) const
	{
		GetFileSystem ()->GetLocalCopy (filename);
/*! OLD CODE:
		// test if the file is NOT available outside of the steam cache
		if (STDIOFile::IsExists (filename))
			return true;	// Already....

		// try to extract the file out of the steam cache
/// @note TEMP FIX: VARIABLE "yb_log_level" IS NOT INITIALIZED YET!!!		AddLogEntry (false, LogLevel_Warning, true, "Trying to extract '%s' out of the steam cache....", filename.GetData ());	// log it

		// is directory for new file not exists?
		if (!IsDirectoryExists (filename))
		{
/// @note TEMP FIX: VARIABLE "yb_log_level" IS NOT INITIALIZED YET!!!			AddLogEntry (false, LogLevel_Default, true, "Directory for new file '%s' not created, trying to create it....", filename.GetData ());	// log it

			// is directory for new file not created?
			if (!CreateDirectory (filename))
			{
				AddLogEntry (true, LogLevel_Error, false, "Couldn't create a new file '%s' directory.", filename.GetData ());

				return false;
			}
		}

		Utilities::FileBuffer steamFile (filename);	// open file from steam cache

		// is file found?
		InternalAssert (steamFile.IsValid ());

		// create and open new file in binary write mode
		STDIOFile localFile (filename, "wb");

		// is file created?
		if (!localFile.IsValid ())
		{
			AddLogEntry (true, LogLevel_Error, false, "Couldn't create a new '%s' file.", filename.GetData ());

			return false;
		}

		// if in cache, then extract it
		if (!localFile.WriteFile (steamFile))
		{
			AddLogEntry (true, LogLevel_Error, false, "Couldn't write buffer into a new '%s' file.", filename.GetData ());

			return false;
		}
*/
		return true;	// Done.
	}
#if 0
const bool Engine::meta_init_gamedll(void)
{
// Set initial GameDLL fields (name, gamedir).
// meta_errno values:
//  - ME_NULLRESULT	getcwd failed

	const DynamicString &gamedir (GetGameModName ());

	GameDLL.Clear ();

	//
	// As of 1.1.1.1, the engine routine GET_GAME_DIR no longer returns a
	// full-pathname, but rather just the path specified as the argument to
	// "-game".
	//
	// However, since we have to work properly under both the new version
	// as well as previous versions, we have to support both possibilities.
	//
	// Note: the code has always assumed the server op wouldn't do:
	//       hlds.exe -game "other/firearms"
	//
	// Old style: GET_GAME_DIR returned full pathname. Copy this into
	// our gamedir, and truncate to get the game name.
	// New style: GET_GAME_DIR returned game name. Copy this into our
	// game name, and prepend the current working directory.
	char buf[MAX_PATH];

	if (getcwd (buf, sizeof (buf)) == NULL)
	{
		AddLogEntry (true, LogLevel_Warning, false, "DLL: Couldn't get cwd; %s", strerror (errno));

		return false;
	}

	GameDLL.name = gamedir;
	GameDLL.gamedir = buf;
	GameDLL.gamedir += '/' + gamedir;

	AddLogEntry (true, LogLevel_Default, false, "Game: %s", GameDLL.name);

	return true;
}

// Set all the fields in the gamedll struct, - based either on an entry in
// known_games matching the current gamedir, or on one specified manually 
// by the server admin.
//
// meta_errno values:
//  - ME_NOTFOUND	couldn't recognize game
const bool Engine::setup_gamedll(void)
{
	static char override_desc_buf[256]; // pointer is given outside function
	static char autodetect_desc_buf[256]; // pointer is given outside function
	char install_path[256];
	const game_modinfo_t *known;
	char *cp;
	const char *autofn = 0, *knownfn=0;
	int override=0;

	// Check for old-style "metagame.ini" file and complain.
	if(valid_gamedir_file(OLD_GAMEDLL_TXT))
		META_WARNING("File '%s' is no longer supported; instead, specify override gamedll in %s or with '+localinfo mm_gamedll <dllfile>'", OLD_GAMEDLL_TXT, CONFIG_INI);
	// First, look for a known game, based on gamedir.
	if((known=lookup_game(GameDLL.name)))
	{
		knownfn=known->win_dll;

		// Do this before autodetecting gamedll from "dlls/*.DLL"
		if(!Config->gamedll) {
			_snprintf(GameDLL.pathname, sizeof(GameDLL.pathname), "dlls/%s", 
				knownfn);
			// Check if the gamedll file exists. If not, try to install it from
			// the cache.
			if(!valid_gamedir_file(GameDLL.pathname)) {
				_snprintf(install_path, sizeof(install_path), "%s/dlls/%s", 
						GameDLL.gamedir, knownfn);
				install_gamedll(GameDLL.pathname, install_path);
			}
		}
	}

	// Then, autodetect gamedlls in "gamedir/dlls/"
	// autodetect_gamedll returns 0 if knownfn exists and is valid gamedll.
	if(Config->autodetect && (autofn=autodetect_gamedll(&GameDLL, knownfn))) {
		// If knownfn is set and autodetect_gamedll returns non-null
		// then knownfn doesn't exists and we should use autodetected
		// DLL instead.
		if(knownfn) {
			// Whine loud about fact that known-list DLL doesn't exists!
			//META_LOG(plapla);
			knownfn = autofn;
		}
	}

	// Neither override nor known-list nor auto-detect found a gamedll.
	if(!known && !Config->gamedll && !autofn)
			RETURN_ERRNO(mFALSE, ME_NOTFOUND);

	// Use override-DLL if specified.
	if(Config->gamedll) {
		strncpy(GameDLL.pathname, Config->gamedll, 
				sizeof(GameDLL.pathname));
		override=1;

		// If the path is relative, the gamedll file will be missing and
		// it might be found in the cache file.
		if(!is_absolute_path(GameDLL.pathname)) {
			_snprintf(install_path, sizeof(install_path),
					"%s/%s", GameDLL.gamedir, GameDLL.pathname);
			// If we could successfully install the gamedll from the cache we
			// rectify the pathname to be a full pathname.
			if(install_gamedll(GameDLL.pathname, install_path))
				strncpy(GameDLL.pathname, install_path, sizeof(GameDLL.pathname));
		}
	}
	// Else use Known-list DLL.
	else if(known) {
		_snprintf(GameDLL.pathname, sizeof(GameDLL.pathname), "%s/dlls/%s", 
				GameDLL.gamedir, knownfn);
	}
	// Else use Autodetect DLL.
	else {
		_snprintf(GameDLL.pathname, sizeof(GameDLL.pathname), "%s/dlls/%s", 
				GameDLL.gamedir, autofn);
	}

	// get filename from pathname
	cp=strrchr(GameDLL.pathname, '/');
	if(cp)
		cp++;
	else
		cp=GameDLL.pathname;
	GameDLL.file=cp;

	// If found, store also the supposed "real" DLL path based on the
	// gamedir, in case it differs from the "override" DLL path.
	if(known && override)
		_snprintf(GameDLL.real_pathname, sizeof(GameDLL.real_pathname),
				"%s/dlls/%s", GameDLL.gamedir, knownfn);
	else if(known && autofn)
		_snprintf(GameDLL.real_pathname, sizeof(GameDLL.real_pathname),
				"%s/dlls/%s", GameDLL.gamedir, knownfn);
	else // !known or (!override and !autofn)
		strncpy(GameDLL.real_pathname, GameDLL.pathname, 
				sizeof(GameDLL.real_pathname));

	if(override) {
		// generate a desc
		_snprintf(override_desc_buf, sizeof(override_desc_buf), "%s (override)", GameDLL.file);
		GameDLL.desc=override_desc_buf;
		// log result
		META_LOG("Overriding game '%s' with dllfile '%s'", GameDLL.name, GameDLL.file);
	}
	else if(known && autofn) {
		// DLL in known-list doesn't exists but we found new one with autodetect.

		// generate a desc
		_snprintf(autodetect_desc_buf, sizeof(autodetect_desc_buf), "%s (autodetect-override)", GameDLL.file);
		GameDLL.desc=autodetect_desc_buf;
		META_LOG("Recognized game '%s'; Autodetection override; using dllfile '%s'", GameDLL.name, GameDLL.file);
	}
	else if(autofn) {
		// generate a desc
		_snprintf(autodetect_desc_buf, sizeof(autodetect_desc_buf), "%s (autodetect)", GameDLL.file);
		GameDLL.desc=autodetect_desc_buf;
		META_LOG("Autodetected game '%s'; using dllfile '%s'", GameDLL.name, GameDLL.file);
	}
	else if(known) {
		GameDLL.desc=known->desc;
		META_LOG("Recognized game '%s'; using dllfile '%s'", GameDLL.name, GameDLL.file);
	}

	return true;
}

const char * DLLINTERNAL autodetect_gamedll(const gamedll_t *gamedll, const char *knownfn)
{
// Search gamedir/dlls/*.DLL for gamedlls
//! @todo add META_DEBUG

	static char buf[256];
	char dllpath[256];
	char fnpath[256];
	DIR *dir;
	struct dirent *ent;
	unsigned int fn_len;

	// Generate dllpath
	_snprintf(buf, sizeof(buf), "%s/dlls", gamedll->gamedir);
	if(!full_gamedir_path(buf, dllpath)) {
		//whine & return
		META_WARNING("GameDLL-Autodetection: Directory '%s' doesn't exist.", buf);
		return 0;
	}

	// Generate knownfn path
	_snprintf(fnpath, sizeof(fnpath), "%s/%s", dllpath, knownfn);

	// Check if knownfn exists and is valid gamedll
	if(is_gamedll(fnpath)) {
		// knownfn exists and is loadable gamedll, return 0.
		return 0;
	}

	// Open directory
	if(!(dir = opendir(dllpath))) {
		//whine & return
		META_WARNING("GameDLL-Autodetection: Couldn't open directory '%s'.", dllpath);
		return 0;
	}

	while((ent = readdir(dir)) != 0) {
		fn_len = strlen(ent->d_name);

		if(fn_len <= strlen(PLATFORM_DLEXT)) {
			// Filename is too short
			continue;
		}

		// Compare end of filename with PLATFORM_DLEXT
		if(!strcasematch(&ent->d_name[fn_len - strlen(PLATFORM_DLEXT)], PLATFORM_DLEXT)) {
			// File isn't DLL
			continue;
		}

		// Exclude all metamods
		if(strncasematch(ent->d_name, "metamod", strlen("metamod"))) {
			continue;
		}

		// Exclude all bots
		strncpy(buf, ent->d_name, sizeof(buf));
		strlwr(buf);
		if(strstr(buf, "bot.")) {
			continue;
		}

		// Generate full path
		_snprintf(fnpath, sizeof(fnpath), "%s/%s", dllpath, ent->d_name);

		// Check if DLL is gamedll
		if(is_gamedll(fnpath)) {
			META_DEBUG(8, ("is_gamedll(%s): ok.", fnpath));
			//gamedll detected
			strncpy(buf, ent->d_name, sizeof(buf));
			closedir(dir);
			return buf;
		}
		META_DEBUG(8, ("is_gamedll(%s): failed.", fnpath));
	}

	//not found
	META_WARNING("GameDLL-Autodetection: Couldn't find gamedll in '%s'.", dllpath);
	closedir(dir);

	return 0;
}
#endif	// if 0
	Engine_WithoutMetaMod::Engine_WithoutMetaMod (SDK::Structures::EngineFunction_t *const engineFunctionTable, SDK::Structures::GlobalVariables_t *const globalVariables) :
		Engine (engineFunctionTable, globalVariables)
	{
		// Get engine function pointer from mp.DLL....
		m_engineFunctionTablePointer = &MemoryUtilities::GetElementAddressFromOtherAddressAndOffset <SDK::Structures::EngineFunction_t> (GetGameLibrary ()->GetFunctionAddress <void *> ("GiveFnptrsToDll"), GET_STATIC_STRING_LENGTH ("\x8B\x44\x24\x08\x56\x8B\x74\x24\x08\x57\xB9\x9E\x00\x00\x00\xBF"));
	}
}