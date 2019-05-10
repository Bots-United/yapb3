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
// ZBotInGameEnabler.hpp
//
// Class: ZBotInGameEnabler
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef HALF_LIFE_ENGINE_UTILITIES_ZBOT_IN_GAME_ENABLER_INCLUDED
#define HALF_LIFE_ENGINE_UTILITIES_ZBOT_IN_GAME_ENABLER_INCLUDED

class ZBotInGameEnabler
{/*
	private:
		/// @todo OPTIMIZE THIS SHIT.....
		struct RestoreItem_t
		{
			unsigned char *address;
			unsigned char  originalOpcodes[7u];
			unsigned char  originalOpcodesLength;
		};
		typedef StaticArray <RestoreItem_t, unsigned char, 10u> RestoreItemArray_t;
		RestoreItemArray_t m_itemsToRestore;
*/
	private:
/*		template <const unsigned char originalOpcodesLength> inline void AddNewItemToRestore (unsigned char *const address)
		{
			// Increase items to restore number.
			m_itemsToRestore.IncreaseElementNumber ();

			RestoreItem_t &newItem (m_itemsToRestore.GetLastElement ());

			// Reliability check.
			CompileTimeAssert (sizeof (newItem.originalOpcodes) >= originalOpcodesLength);

			newItem.address = address;	// Store address.
			newItem.originalOpcodesLength = originalOpcodesLength;	// Store length.

			MemoryCopy (address, newItem.originalOpcodes, originalOpcodesLength);	// Copy original opcodes from this address.
		}
*/
		static inline const bool RemoveUTIL_IsGameFunctionCall (unsigned char *const address, const unsigned char length)
		{
//			AddNewItemToRestore <Trampolines::Bytecode::JumpImmediately32Size> (address);

			// Construct jump trough UTIL_IsGame() function call....
			if (!Trampolines::WriteFunctionJump32 (address, address + length))
			{
				// Failed to write jump instruction at 'address', so decrease items to restore number back to previous value....
//				m_itemsToRestore.DecreaseElementNumber ();

				AddLogEntry (false, LogLevel_Critical, true, "RemoveUTIL_IsGameFunctionCall(): Trampolines::WriteFunctionJump32() filed!");

				return false;
			}

			return true;
		}
		static inline const bool FindAndStoreCzeroStringAddress (const LibraryHandleWithSize &base, const void *&signature)
		{
			const unsigned char string[] = {"czero"};
			void *const address (MemoryUtilities::FindPatternWithoutWildCard (base, string, GET_STATIC_STRING_LENGTH (string)));

			if (address == NULL)
			{
				AddLogEntry (true, LogLevel_Warning, false, "EnableBotInGame(): Can't find string \"%s\" address!!!", string);

				return false;
			}

			signature = address;

			return true;
		}
		template <const unsigned char signatureBaseLength> static inline const bool EnableBotPhraseManagerInitializeFailMessage (const LibraryHandleWithSize &base)
		{
			const unsigned char string[] = {"WARNING: Cannot access bot phrase database '%s'\n"};
			unsigned char *address (static_cast <unsigned char *> (MemoryUtilities::FindPatternWithoutWildCard (base, string, GET_STATIC_STRING_LENGTH (string))));

			if (address == NULL)
			{
				AddLogEntry (true, LogLevel_Warning, false, "EnableBotInGame(): Can't find string \"%s\" address!!!", string);

				return false;
			}

			// (Address: 0x1AD54)
			address = static_cast <unsigned char *> (MemoryUtilities::FindMemoryChunkReference32 (base, address, Trampolines::Bytecode::Push32));

			if (address == NULL)
			{
				AddLogEntry (true, LogLevel_Warning, false, "EnableBotInGame(): Can't find string \"%s\" push address!!!", string);

				return false;
			}

			const unsigned char offset (signatureBaseLength + sizeof ('\x56'));	// 22

			address -= offset;

			if (!RemoveUTIL_IsGameFunctionCall (address/* (Address: 0x1AD3E) */, signatureBaseLength))
			{
				AddLogEntry (true, LogLevel_Warning, false, "EnableBotInGame(): Can't disable UTIL_IsGame() function call on address 0x%X!!!", address - (unsigned char *)base.GetPointer ());

				return false;
			}

			return true;
		}
		template <const unsigned char signatureBaseLength> static inline const bool EnableBotConsoleVariableRegister (const LibraryHandleWithSize &base, const unsigned char *const &signatureBase)
		{
				const unsigned char signatureLocal[] =
				{
					Trampolines::Bytecode::Push32, Trampolines::Bytecode::WildCard, Trampolines::Bytecode::WildCard, Trampolines::Bytecode::WildCard, Trampolines::Bytecode::WildCard,		// push    offset off_10112BB8 ; _DWORD
					0xFF, 0x15, Trampolines::Bytecode::WildCard, Trampolines::Bytecode::WildCard, Trampolines::Bytecode::WildCard, Trampolines::Bytecode::WildCard,	// call    dword_10162420  ; Indirect Call Near Procedure
					Trampolines::Bytecode::Push32, Trampolines::Bytecode::WildCard, Trampolines::Bytecode::WildCard, Trampolines::Bytecode::WildCard, Trampolines::Bytecode::WildCard,		// push    offset off_10112BD0 ; _DWORD
					0xFF, 0x15, Trampolines::Bytecode::WildCard, Trampolines::Bytecode::WildCard, Trampolines::Bytecode::WildCard, Trampolines::Bytecode::WildCard	// call    dword_10162420  ; Indirect Call Near Procedure
				};
				unsigned char signatureFull[signatureBaseLength + sizeof (signatureLocal)];

				MemoryCopy (signatureBase, signatureFull, signatureBaseLength);
				MemoryCopy (signatureLocal, signatureFull + signatureBaseLength, sizeof (signatureLocal));

				unsigned char *const address (static_cast <unsigned char *> (MemoryUtilities::FindPattern (base, signatureFull, sizeof (signatureFull))));

				if (address == NULL)
				{
					AddLogEntry (true, LogLevel_Warning, false, "EnableBotInGame(): Can't find bot console variable register signature address!!!");

					return false;
				}

				if (!RemoveUTIL_IsGameFunctionCall (address, signatureBaseLength))
				{
					AddLogEntry (true, LogLevel_Warning, false, "EnableBotInGame(): Can't disable UTIL_IsGame() function call on address 0x%X!!!", address - (unsigned char *)base.GetPointer ());

					return false;
				}

			return true;
		}
		template <const unsigned char signatureBaseLength> static inline const bool EnableBotConsoleCommandRegister (const LibraryHandleWithSize &base, const unsigned char *const &signatureBase)
		{
			const unsigned char signatureLocal[] =
			{
				0xC6, 0x05, Trampolines::Bytecode::WildCard, Trampolines::Bytecode::WildCard, Trampolines::Bytecode::WildCard, Trampolines::Bytecode::WildCard, 0x00	// mov byte_1011322C, 0
			};
			unsigned char signatureFull[signatureBaseLength + sizeof (signatureLocal)];	// Just a bit changed 'signatureBase'....

			MemoryCopy (signatureBase, signatureFull, Trampolines::Bytecode::Push32Size);
			MemoryCopy (signatureLocal, signatureFull + Trampolines::Bytecode::Push32Size, sizeof (signatureLocal));
			MemoryCopy (signatureBase + Trampolines::Bytecode::Push32Size, signatureFull + Trampolines::Bytecode::Push32Size + sizeof (signatureLocal), signatureBaseLength - Trampolines::Bytecode::Push32Size);

			unsigned char *address (static_cast <unsigned char *> (MemoryUtilities::FindPattern (base, signatureFull, sizeof (signatureFull))));

			if (address == NULL)
			{
				AddLogEntry (true, LogLevel_Warning, false, "EnableBotInGame(): Can't find bot console command register signature address!!!");

				return false;
			}

			if (!Trampolines::VirtualProtect (address, sizeof (signatureLocal), PAGE_EXECUTE_READWRITE))
			{
				AddLogEntry (true, LogLevel_Warning, false, "EnableBotInGame(): Can't VirtualProtect() for address 0x%X!!!", address - (unsigned char *)base.GetPointer ());

				return false;
			}

//			AddNewItemToRestore <sizeof (signatureLocal)> (address);

			MemoryCopy (address + Trampolines::Bytecode::Push32Size, address, sizeof (signatureLocal));

			address += sizeof (signatureLocal);

			if (!RemoveUTIL_IsGameFunctionCall (address, signatureBaseLength))
			{
				AddLogEntry (true, LogLevel_Warning, false, "EnableBotInGame(): Can't disable UTIL_IsGame() function call on address 0x%X!!!", address - (unsigned char *)base.GetPointer ());

				return false;
			}

			return true;
		}
		template <const unsigned char signatureBaseLength> static inline const bool EnableBotConsoleCommandExecute (const LibraryHandleWithSize &base, const unsigned char *const &signatureBase)
		{
			const unsigned char signatureLocal[] =
			{
				0x89, 0x6C, 0x24, 0x1C	// mov [esp+1ECh+var_1D0], ebp
			};
			unsigned char signatureFull[signatureBaseLength + sizeof (signatureLocal)];	// Just a bit changed 'signatureBase'....

			MemoryCopy (signatureBase, signatureFull, Trampolines::Bytecode::Push32Size);
			MemoryCopy (signatureLocal, signatureFull + Trampolines::Bytecode::Push32Size, sizeof (signatureLocal));
			MemoryCopy (signatureBase + Trampolines::Bytecode::Push32Size, signatureFull + Trampolines::Bytecode::Push32Size + sizeof (signatureLocal), signatureBaseLength - Trampolines::Bytecode::Push32Size);

			// (Address: 0x24840)
			unsigned char *address (static_cast <unsigned char *> (MemoryUtilities::FindPattern (base, signatureFull, sizeof (signatureFull))));

			if (address == NULL)
			{
				AddLogEntry (true, LogLevel_Warning, false, "EnableBotInGame(): Can't find bot console command execute signature address!!!");

				return false;
			}

			if (!Trampolines::VirtualProtect (address, sizeof (signatureLocal), PAGE_EXECUTE_READWRITE))
			{
				AddLogEntry (true, LogLevel_Warning, false, "EnableBotInGame(): Can't VirtualProtect() for address 0x%X!!!", address - (unsigned char *)base.GetPointer ());

				return false;
			}

//			AddNewItemToRestore <sizeof (signatureLocal)> (address);

			// Change "mov [esp+1ECh+var_1D0], ebp" to "mov [esp+1E8h+var_1D0], ebp"
			address[0u] = signatureLocal[0u];
			(++address)[0u] = signatureLocal[1u];
			(++address)[0u] = signatureLocal[2u];
			(++address)[0u] = signatureLocal[3u] - 0x04;	// Only changed.

			if (!RemoveUTIL_IsGameFunctionCall (++address/* (Address: 0x24844) */, signatureBaseLength))
			{
				AddLogEntry (true, LogLevel_Warning, false, "EnableBotInGame(): Can't disable UTIL_IsGame() function call on address 0x%X!!!", address - (unsigned char *)base.GetPointer ());

				return false;
			}

			return true;
		}
		template <const unsigned char signatureBaseLength> static inline const bool EnableValidateMapData (const LibraryHandleWithSize &base)
		{
			const unsigned char string[] = {"Failed to load navigation map.\n"};
			unsigned char *address (static_cast <unsigned char *> (MemoryUtilities::FindPatternWithoutWildCard (base, string, GET_STATIC_STRING_LENGTH (string))));

			if (address == NULL)
			{
				AddLogEntry (true, LogLevel_Warning, false, "EnableBotInGame(): Can't find string \"%s\" address!!!", string);

				return false;
			}

			address = static_cast <unsigned char *> (MemoryUtilities::FindMemoryChunkReference32 (base, address, Trampolines::Bytecode::Push32));

			if (address == NULL)
			{
				AddLogEntry (true, LogLevel_Warning, false, "EnableBotInGame(): Can't find string \"%s\" push address!!!", string);

				return false;
			}

			const unsigned char offset (signatureBaseLength + GET_STATIC_STRING_LENGTH ("\xC6\x05\x82\x8F\x12\x10\x01\xE8\x29\x2E\x02\x00\x85\xC0\x74\x15"));	// 37

			address -= offset;

			if (!RemoveUTIL_IsGameFunctionCall (address, signatureBaseLength))
			{
				AddLogEntry (true, LogLevel_Warning, false, "EnableBotInGame(): Can't disable UTIL_IsGame() function call on address 0x%X!!!", address - (unsigned char *)base.GetPointer ());

				return false;
			}

			return true;
		}
		template <const unsigned char signatureBaseLength> static inline const bool EnableAddBot (const LibraryHandleWithSize &base, const unsigned char *const &signatureBase)
		{
			const unsigned char signatureLocal[] =
			{
				0x8B, 0x74, 0x24, 0x58,		// mov     esi, [esp+50h+arg_4]
				0x8B, 0x3D, Trampolines::Bytecode::WildCard, Trampolines::Bytecode::WildCard, Trampolines::Bytecode::WildCard, Trampolines::Bytecode::WildCard,		// mov     edi, dword_10162304
				0x83, 0xFE, 0x02		// cmp     esi, 2          ; Compare Two Operands
			};
			unsigned char signatureFull[signatureBaseLength + sizeof (signatureLocal)];

			MemoryCopy (signatureBase, signatureFull, signatureBaseLength);
			MemoryCopy (signatureLocal, signatureFull + signatureBaseLength, sizeof (signatureLocal));

			unsigned char *const address (static_cast <unsigned char *> (MemoryUtilities::FindPattern (base, signatureFull, sizeof (signatureFull))));

			if (address == NULL)
			{
				AddLogEntry (true, LogLevel_Warning, false, "EnableBotInGame(): Can't find bot console variable register signature address!!!");

				return false;
			}

			if (!RemoveUTIL_IsGameFunctionCall (address, signatureBaseLength))
			{
				AddLogEntry (true, LogLevel_Warning, false, "EnableBotInGame(): Can't disable UTIL_IsGame() function call on address 0x%X!!!", address - (unsigned char *)base.GetPointer ());

				return false;
			}

			return true;
		}
		template <const unsigned char signatureBaseLength> static inline const bool EnableBotProfileManagerInitialize (const LibraryHandleWithSize &base)
		{
			const unsigned char string[] = {"WARNING: Cannot access bot profile database '%s'\n"};
			unsigned char *address (static_cast <unsigned char *> (MemoryUtilities::FindPatternWithoutWildCard (base, string, GET_STATIC_STRING_LENGTH (string))));

			if (address == NULL)
			{
				AddLogEntry (true, LogLevel_Warning, false, "EnableBotInGame(): Can't find string \"%s\" address!!!", string);

				return false;
			}

			address = static_cast <unsigned char *> (MemoryUtilities::FindMemoryChunkReference32 (base, address, Trampolines::Bytecode::Push32));

			if (address == NULL)
			{
				AddLogEntry (true, LogLevel_Warning, false, "EnableBotInGame(): Can't find string \"%s\" push address!!!", string);

				return false;
			}

			const unsigned char offset (signatureBaseLength + sizeof ('\x57'));	// 22

			address -= offset;

			if (!RemoveUTIL_IsGameFunctionCall (address, signatureBaseLength))
			{
				AddLogEntry (true, LogLevel_Warning, false, "EnableBotInGame(): Can't disable UTIL_IsGame() function call on address 0x%X!!!", address - (unsigned char *)base.GetPointer ());

				return false;
			}

			return true;
		}
		static inline const bool RemoveNobotsLaunchParameterCheck (const LibraryHandleWithSize &base)
		{
			const unsigned char string[] = {"-nobots"};
			unsigned char *address (static_cast <unsigned char *> (MemoryUtilities::FindPatternWithoutWildCard (base, string, GET_STATIC_STRING_LENGTH (string))));

			if (address == NULL)
				return true;	// Just older version of mp.DLL, dont care about it....

			address = static_cast <unsigned char *> (MemoryUtilities::FindMemoryChunkReference32 (base, address, Trampolines::Bytecode::Push32));

			if (address == NULL)
			{
				AddLogEntry (true, LogLevel_Warning, false, "EnableBotInGame(): Can't find string \"%s\" push address!!!", string);

				return false;
			}

			address -= GET_STATIC_STRING_LENGTH ("\x6A\x00");
/*
			const unsigned char length (GET_STATIC_STRING_LENGTH ("\x6A\x00\x68\x90\x38\x11\x10\xFF\x15\xB4\x25\x16\x10\x83\xC4\x08\x85\xC0"));	// 18

			if (!Trampolines::VirtualProtect (address, length + sizeof (JumpImmediately8), PAGE_EXECUTE_READWRITE))
			{
				AddLogEntry (true, LogLevel_Warning, false, "EnableBotInGame(): Can't VirtualProtect() for address 0x%X!!!\n", address - (unsigned char *)base.GetPointer ());

				return false;
			}

//			AddNewItemToRestore <length + Trampolines::Bytecode::JumpImmediately8Size> (address);

			for (unsigned char index (0u); index < length; ++index, ++address)
				address[0u] = Trampolines::Bytecode::Nop;

			address[0u] = Trampolines::Bytecode::JumpImmediately8;	// Replace 'jz' to 'jmp'.
*/
			// Construct jump trough g_enginefuncs.pfnCheckParametr() function call....
			if (!Trampolines::VirtualProtect (address, Trampolines::Bytecode::JumpImmediately8Size, PAGE_EXECUTE_READWRITE))
			{
				AddLogEntry (true, LogLevel_Warning, false, "RemoveNobotsLaunchParameterCheck(): VirtualProtect() filed!");

				return false;
			}

//			AddNewItemToRestore <Trampolines::Bytecode::JumpImmediately8Size> (address);

			address[0u] = Trampolines::Bytecode::JumpImmediately8;

			const unsigned char length (GET_STATIC_STRING_LENGTH ("\x6A\x00\x68\x90\x38\x11\x10\xFF\x15\xB4\x25\x16\x10\x83\xC4\x08\x85\xC0\x74\x08\x5F\x5E\x5D\x32\xC0\x5B\x59\xC3"));	// 28

			reinterpret_cast <unsigned int *> (address + sizeof (Trampolines::Bytecode::JumpImmediately8))[0u] = length - Trampolines::Bytecode::JumpImmediately8Size;

			return true;
		}

	public:
		static inline const bool EnableBotInGame (void)
		{/*
			// Reliability check.
			InternalAssert (m_itemsToRestore.IsEmpty ());
*/
			const LibraryHandleWithSize &base (*Globals::g_halfLifeEngine->GetGameLibrary ());
			unsigned char signatureBase[] =
			{
				Trampolines::Bytecode::Push32, Trampolines::Bytecode::WildCard, Trampolines::Bytecode::WildCard, Trampolines::Bytecode::WildCard, Trampolines::Bytecode::WildCard,		// push    offset aCzero   ; "czero"
				Trampolines::Bytecode::CallImmediately32, Trampolines::Bytecode::WildCard, Trampolines::Bytecode::WildCard, Trampolines::Bytecode::WildCard, Trampolines::Bytecode::WildCard,		// call    UTIL_IsGame    ; Call Procedure
				Trampolines::Bytecode::AddESP8[0u], Trampolines::Bytecode::AddESP8[1u], 0x04,																																// add     esp, 4          ; Add
				0x84, 0xC0,																																		// test    al, al          ; Logical Compare
				0x0F, 0x84, Trampolines::Bytecode::WildCard, Trampolines::Bytecode::WildCard, Trampolines::Bytecode::WildCard, Trampolines::Bytecode::WildCard	// jz      locret_100219AA ; Jump if Zero (ZF=1)
			};	// 21
			const unsigned char signatureBaseLength (sizeof (signatureBase));

			// Find and store "czero" string address to 'signatureBase':
			{
				if (!FindAndStoreCzeroStringAddress (base, reinterpret_cast <const void **> (signatureBase + sizeof (Trampolines::Bytecode::Push32))[0u]))
					return false;
			}

			// Bot phrase manager initialize fail message:
			{
				if (!EnableBotPhraseManagerInitializeFailMessage <signatureBaseLength> (base))
					return false;
			}

			// Bot console variable register:
			{
				if (!EnableBotConsoleVariableRegister <signatureBaseLength> (base, signatureBase))
					return false;
			}

			// Bot console command register:
			{
				if (!EnableBotConsoleCommandRegister <signatureBaseLength> (base, signatureBase))
					return false;
			}

			// Bot console command execute:
			{
				if (!EnableBotConsoleCommandExecute <signatureBaseLength> (base, signatureBase))
					return false;
			}

			// Validate map data:
			{
				if (!EnableValidateMapData <signatureBaseLength> (base))
					return false;
			}

			// Add bot:
			{
				if (!EnableAddBot <signatureBaseLength> (base, signatureBase))
					return false;
			}

			// Bot profile manager initialize:
			{
				if (!EnableBotProfileManagerInitialize <signatureBaseLength> (base))
					return false;
			}

			// Remove "-nobots" launch parameter check: (ONLY IF UNDER META MOD)
			if (Globals::g_halfLifeEngine->IsUnderMetaMod () && !RemoveNobotsLaunchParameterCheck (base))
				return false;

			return true;
		}
/*		inline const bool DisableBotInGame (void)
		{
			if (m_itemsToRestore.IsEmpty ())
				return true;

			for (RestoreItemArray_t::IndexType_t index (0u); index < m_itemsToRestore.GetElementNumber (); ++index)
			{
				// Reliability check.
				InternalAssert (!IsBadWritePtr (m_itemsToRestore[index].address, m_itemsToRestore[index].originalOpcodesLength));

				MemoryCopy (m_itemsToRestore[index].originalOpcodes, m_itemsToRestore[index].address, m_itemsToRestore[index].originalOpcodesLength);
			}

			m_itemsToRestore.RemoveAll ();

			return true;
		}
*/
		static inline void ToggleAdvancedHostageAI (void)
		{
			static unsigned char originalBytes[] = {0xA1, 0x50, 0xA8, 0x12, 0x10};	// mov eax, dword_1012A850
			const unsigned char replacedBytes[] = {0xB8, 0x00, 0x00, 0x00, 0x00};	// mov eax, 0
			unsigned char *const address (Globals::g_halfLifeEngine->GetGameLibrary ()->GetFunctionAddress <unsigned char *> ("?IdleThink@CHostage@@QAEXXZ"));

			if (address == NULL)
			{
				AddLogEntry (true, LogLevel_Warning, false, "ToggleAdvancedHostageAI(): FILED!!! Can't find CHostage::IdleThink() function!");

				return;
			}

			if (address[0u] == originalBytes[0u])
			{
				// Not patched yet.

				if (address[1u] != originalBytes[1u] || address[sizeof (originalBytes)] != 0x83 || address[sizeof (originalBytes) + 1u] != 0xEC)
				{
					AddLogEntry (true, LogLevel_Warning, false, "ToggleAdvancedHostageAI(): FILED!!! Can't find pattern!");

					return;
				}

				MemoryCopy (address, originalBytes, sizeof (originalBytes));

				if (!Trampolines::VirtualProtect (address, sizeof (replacedBytes), PAGE_EXECUTE_READWRITE))
				{
					AddLogEntry (true, LogLevel_Warning, false, "ToggleAdvancedHostageAI(): VirtualProtect() filed!");

					return;
				}

				MemoryCopy (replacedBytes, address, sizeof (replacedBytes));
			}
			else
			{
				// Patched already.

				MemoryCopy (originalBytes, address, sizeof (originalBytes));
			}
		}
};

//extern ZBotInGameEnabler g_ZBotInGameEnabler;

#endif	// ifndef HALF_LIFE_ENGINE_UTILITIES_ZBOT_IN_GAME_ENABLER_INCLUDED