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
// Functions.cpp
//
// Description: Links Functions, handles Client Commands, initializes DLL and misc Stuff.
//
// Version: $ID:$
//

#include <Core.hpp>

namespace HalfLifeEngine
{
	namespace Export
	{
		EXPORT BOOL GetEngineFunctions (SDK::Structures::EngineFunction_t *engineFunctionTable, int * /*interfaceVersion*/)
		{
			// This is one of the initialization functions hooked by Meta mod in the engine API.

			// Pass engine functions table to the game.DLL or Meta mod.
			if (MetaModEngine::Globals::g_metaModEngine == NULL)
			{
/*				engineFunctionTable->pfnSetModel      = ReplacedFunctions::EngineTable::pfnSetModel;
				engineFunctionTable->pfnEmitSound     = ReplacedFunctions::EngineTable::pfnEmitSound;
				engineFunctionTable->pfnClientCommand = ReplacedFunctions::EngineTable::pfnClientCommand;
/*				engineFunctionTable->pfnMessageBegin  = ReplacedFunctions::EngineTable::pfnMessageBegin;
				engineFunctionTable->pfnMessageEnd    = ReplacedFunctions::EngineTable::pfnMessageEnd;
				engineFunctionTable->pfnWriteByte     = ReplacedFunctions::EngineTable::pfnWriteByte;
				engineFunctionTable->pfnWriteChar     = ReplacedFunctions::EngineTable::pfnWriteChar;
				engineFunctionTable->pfnWriteShort    = ReplacedFunctions::EngineTable::pfnWriteShort;
				engineFunctionTable->pfnWriteLong     = ReplacedFunctions::EngineTable::pfnWriteLong;
				engineFunctionTable->pfnWriteAngle    = ReplacedFunctions::EngineTable::pfnWriteAngle;
				engineFunctionTable->pfnWriteCoord    = ReplacedFunctions::EngineTable::pfnWriteCoord;
				engineFunctionTable->pfnWriteString   = ReplacedFunctions::EngineTable::pfnWriteString;
				engineFunctionTable->pfnWriteEntity   = ReplacedFunctions::EngineTable::pfnWriteEntity;*/
				engineFunctionTable->pfnRegUserMsg    = ReplacedFunctions::EngineTable::pfnRegUserMsg;
/*				engineFunctionTable->pfnClientPrintf  = ReplacedFunctions::EngineTable::pfnClientPrintf;
				engineFunctionTable->pfnCmd_Args      = ReplacedFunctions::EngineTable::pfnCmd_Args;
				engineFunctionTable->pfnCmd_Argv      = ReplacedFunctions::EngineTable::pfnCmd_Argv;
				engineFunctionTable->pfnCmd_Argc      = ReplacedFunctions::EngineTable::pfnCmd_Argc;
*/			}
			else
			{
				// Zero out the function table if we are under Meta mod.
				memset (engineFunctionTable, NULL, sizeof (*engineFunctionTable));

				Globals::g_halfLifeEngine->GetEngineWithMetaModPointer ()->SetEnginePREFunctionTablePointer (engineFunctionTable);

/*				engineFunctionTable->pfnEmitSound     = MetaModEngine::Export::ReplacedFunctions::EngineTable::pfnEmitSound;
				engineFunctionTable->pfnClientCommand = MetaModEngine::Export::ReplacedFunctions::EngineTable::pfnClientCommand;
/*				engineFunctionTable->pfnMessageBegin  = MetaModEngine::Export::ReplacedFunctions::EngineTable::pfnMessageBegin;
				engineFunctionTable->pfnMessageEnd    = MetaModEngine::Export::ReplacedFunctions::EngineTable::pfnMessageEnd;
				engineFunctionTable->pfnWriteByte     = MetaModEngine::Export::ReplacedFunctions::EngineTable::pfnWriteByte;
				engineFunctionTable->pfnWriteChar     = MetaModEngine::Export::ReplacedFunctions::EngineTable::pfnWriteChar;
				engineFunctionTable->pfnWriteShort    = MetaModEngine::Export::ReplacedFunctions::EngineTable::pfnWriteShort;
				engineFunctionTable->pfnWriteLong     = MetaModEngine::Export::ReplacedFunctions::EngineTable::pfnWriteLong;
				engineFunctionTable->pfnWriteAngle    = MetaModEngine::Export::ReplacedFunctions::EngineTable::pfnWriteAngle;
				engineFunctionTable->pfnWriteCoord    = MetaModEngine::Export::ReplacedFunctions::EngineTable::pfnWriteCoord;
				engineFunctionTable->pfnWriteString   = MetaModEngine::Export::ReplacedFunctions::EngineTable::pfnWriteString;
				engineFunctionTable->pfnWriteEntity   = MetaModEngine::Export::ReplacedFunctions::EngineTable::pfnWriteEntity;*/
				engineFunctionTable->pfnRegUserMsg    = MetaModEngine::Export::ReplacedFunctions::EngineTable::pfnRegUserMsg;
/*				engineFunctionTable->pfnClientPrintf  = MetaModEngine::Export::ReplacedFunctions::EngineTable::pfnClientPrintf;
				engineFunctionTable->pfnCmd_Args      = MetaModEngine::Export::ReplacedFunctions::EngineTable::pfnCmd_Args;
				engineFunctionTable->pfnCmd_Argv      = MetaModEngine::Export::ReplacedFunctions::EngineTable::pfnCmd_Argv;
				engineFunctionTable->pfnCmd_Argc      = MetaModEngine::Export::ReplacedFunctions::EngineTable::pfnCmd_Argc;
*/			}

			// finished, interfacing from game.DLL to engine complete
			return TRUE;
		}

		EXPORT BOOL GetEngineFunctions_Post (SDK::Structures::EngineFunction_t *engineFunctionTable, int * /*interfaceVersion*/)
		{
			// This is one of the initialization functions hooked by Meta mod in the engine API.

			// Zero out the function table....
			memset (engineFunctionTable, NULL, sizeof (*engineFunctionTable));

			// Pass engine functions table to the Meta mod.
			Globals::g_halfLifeEngine->GetEngineWithMetaModPointer ()->SetEnginePOSTFunctionTablePointer (engineFunctionTable);
//			engineFunctionTable->pfnSetModel = MetaModEngine::Export::ReplacedFunctions::EngineTable::Post::pfnSetModel;

			// This is a last called by meta mod function,
			// so once all shit is loaded and if server already running,
			// we must emulate basic calls, like HalfLifeEngine::Export::Callbacks::DLLTable::pfnServerActivate() for example....
			if (HalfLifeEngine::Globals::g_halfLifeEngine->GetWorldSpawnEdict () != NULL)
				InternalAttach ();

			// finished, interfacing from engine to game.DLL complete
			return TRUE;
		}

						static inline const bool IsLowerPriority (const unsigned char &node1, const unsigned char &node2)
						{
							// Nodes with greater time to say are lower priority.
							return node1 < node2;
						}
/*		static int Func (bool b)
		{
			int result (!b);

			if (b)
			{
				MessageBox (NULL, "Func(): HELLO! 1", "YaPB Critical Error", MB_ICONASTERISK);

				LABEL_1:
					result = b;
			}
			else
			{
				goto LABEL_1;

				/// @warning THE BELOW FUNCTION IS NOT CALLED!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
				MessageBox (NULL, "Func(): HELLO! 2", "YaPB Critical Error", MB_ICONASTERISK);
			}

			MessageBox (NULL, "Func(): HELLO! 3", "YaPB Critical Error", MB_ICONASTERISK);

			return result;
		}
*//*
		static bool FUNC1 (void *p, char *&s)
		{
			s = "FUNC1";

			return true;
		}
		static void FUNC2 (void *p, char *s)
		{
			if (FUNC1 (p, s))
				return;

			s = (char*)*s;
//			char c (*s);

			s = "FUNC2";
		}
*/
		// if we're using Microsoft Visual C++ compiler, we need to specify the export parameter (be sure DLL has data section and RW access)...
		#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
		#	pragma comment (linker, "/EXPORT:GiveFnptrsToDll=_GiveFnptrsToDll@8,@1")
		#	pragma comment (linker, "/SECTION:.data,RW")
		#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

		EXPORT inline void WINAPI GiveFnptrsToDll (SDK::Structures::EngineFunction_t *engineFunctionTable, SDK::Structures::GlobalVariables_t *globalVariables)
		{
/*		DEBUGGER_BREAK();
		char *p ("HELLO");
		__asm
		{
			mov ecx, p
			lea eax, [ecx]
		}*/
//			FUNC2 (NULL, "GiveFnptrsToDll");
/*			__asm
			{
				int 3
				mov ebp, 0
				mov eax, ebp
				mov eax, 12345	// ebp doesn't changed.... ok
			}
/*			__asm
			{
				int 3	// Trap to debugger
				mov ebp, 0	// now ebp is 0
				mov eax, 12345	// now eax is 12345
				push eax	// push eax into stack (esp)
				pop ebp	// get value from stack, now ebp is 12345
			}
*/
			// this is the very first function that is called in the game.DLL by the engine. Its purpose
			// is to set the functions interfacing up, by exchanging the 'engineFunctionTable' function list
			// along with a pointer to the engine's global variables structure 'globalVariables', with the game.DLL.
			// We can there decide if we want to load the normal game.DLL just behind our bot DLL,
			// or any other game.DLL that is present, such as Will Day's Meta mod. Also, since there is
			// a known bug on Win32 platforms that prevent hook DLLs (such as our bot DLL) to be used in
			// single player games (because they don't export all the stuff they should), we may need to
			// build our own array of exported symbols from the actual game.DLL in order to use it as
			// such if necessary. Nothing really bot-related is done in this function. The actual bot
			// initialization stuff will be done later, when we'll be certain to have a multilayer game.
/*
			const DynamicString string = "\"1\" \"2\" \"3\" \"4\" \"5\" \"6\" \"7\" \"8\" \"9\" \"10\"", field = string.GetField (6u);
//			const DynamicString string = "\"\" \"\" \"\" \"\" \"\" \"\" \"\" \"\" \"\" \"\"", field = string.GetField (6u);

			AddLogEntry (false, LogLevel_Critical, false, "string: \"%s\",\nGetElementNumber() == %u,\nGetAllocatedSize() == %u,\n\nfield: \"%s\",\nGetElementNumber() == %u,\nGetAllocatedSize() == %u.", string.GetData (), string.GetElementNumber (), string.GetAllocatedSize (), field.GetData (), field.GetElementNumber (), field.GetAllocatedSize ());
/*
			const DynamicString string = "0 1 2 3 4 5 6 7 8 9 10", reversedString = string.ToReverse ();

			AddLogEntry (false, LogLevel_Critical, false, "string: \"%s\",\nGetElementNumber() == %u,\nGetAllocatedSize() == %u,\n\nToReverse(): \"%s\",\nGetElementNumber() == %u,\nGetAllocatedSize() == %u.", string.GetData (), string.GetElementNumber (), string.GetAllocatedSize (), reversedString.GetData (), reversedString.GetElementNumber (), reversedString.GetAllocatedSize ());
/*
			DynamicString string = "Immortal*BLG";
			std::string string2 = "Immortal*BLG";

			string.Assign ('1', 6u);
			string2.assign (6u, '1');

			AddLogEntry (false, LogLevel_Critical, false, "string.Assign('1', 6u): \"%s\",\nstring.GetElementNumber() == %u,\nstring.GetAllocatedSize() == %u,\n\nstring2.assign(6u, '1'): \"%s\",\nstring2.length() == %u,\nstring2.capacity() == %u.", string.GetData (), string.GetElementNumber (), string.GetAllocatedSize (), string2.c_str (), string2.length (), string2.capacity ());
/*
			DynamicString string = "Immortal*BLG*BlG";
			std::string string2 = "Immortal*BLG*BlG";

//			string.Insert (12u, string, 12u, 6u);
			string.Insert (16u, "1234", 6u);
//			string2.insert (12u, string2, 12u, 6u);
			string2.insert (16u, "1234", 6u);

//			AddLogEntry (false, LogLevel_Critical, false, "string: \"%s\",\nstring.GetElementNumber() == %u,\nstring.GetAllocatedSize() == %u.", string.GetData (), string.GetElementNumber (), string.GetAllocatedSize ());
//			AddLogEntry (false, LogLevel_Critical, false, "string: \"%s\",\nstring.length() == %u,\nstring.capacity() == %u.", string.c_str (), string.length (), string.capacity ());
			AddLogEntry (false, LogLevel_Critical, false, "string: \"%s\",\nstring.GetElementNumber() == %u,\nstring.GetAllocatedSize() == %u,\n\nstring2: \"%s\",\nstring2.length() == %u,\nstring2.capacity() == %u.", string.GetData (), string.GetElementNumber (), string.GetAllocatedSize (), string2.c_str (), string2.length (), string2.capacity ());
/*
			DynamicString string = "Immortal*BLG";
			unsigned index = 8u;//string.GetElementNumber () - 1u;

			string.InsertSpace (index, 2u);

			AddLogEntry (false, LogLevel_Critical, false, "string.InsertSpace(string.GetElementNumber () - 1u, 8u): \"%s\",\nstring.GetElementNumber() == %u,\nstring.GetAllocatedSize() == %u.", string (), string.GetElementNumber (), string.GetAllocatedSize ());
/*
//			const DynamicString string = "Immortal*BLG";
//			const std::string string2 = "Immortal*BLG";
//			const unsigned findIndex = string.Find ("mortal", 0u, 6u/*CoreToolLibrary::BadPosition/*2u*//*), findIndex2 = string2.find ("mortal", 0u, 6u/*CoreToolLibrary::BadPosition/*2u*//*);
//			const unsigned findIndex = string./*rfind*//*ReverseFind ("mortal", 12u, 6u/*CoreToolLibrary::BadPosition/*2u*//*), findIndex2 = string2.rfind ("mortal", 12u, 6u/*CoreToolLibrary::BadPosition/*2u*//*);
			unsigned int startTime (GetTickCount ());
			const DynamicString string = "a0 1 2 3 4 5 6 7 8 9 10 0 1 2 3 4 5 6 7 8 9 10 0 1 2 3 4 5 6 7 8 9 10 0 1 2 3 4 5 6 7 8 9 10 0 1 2 3 4 5 6 7 8 9 10 0 1 2 3 4 5 6 7 8 9 10 0 1 2 3 4 5 6 7 8 9 10 0 1 2 3 4 5 6 7 8 9 10";
//			const unsigned findIndex = string./*rfind*//*ReverseFind ("0", 22u, 1u/*CoreToolLibrary::BadPosition/*2u*//*), findIndex2 = string2.rfind ("0", 22u, 1u/*CoreToolLibrary::BadPosition/*2u*//*);
			const unsigned findIndex = string./*rfind*//*ReverseFind (string, 184u, 1u/*CoreToolLibrary::BadPosition/*2u*//*);
			startTime = GetTickCount () - startTime;
			unsigned int startTime2 (GetTickCount ());
			const std::string string2 = "a0 1 2 3 4 5 6 7 8 9 10 0 1 2 3 4 5 6 7 8 9 10 0 1 2 3 4 5 6 7 8 9 10 0 1 2 3 4 5 6 7 8 9 10 0 1 2 3 4 5 6 7 8 9 10 0 1 2 3 4 5 6 7 8 9 10 0 1 2 3 4 5 6 7 8 9 10 0 1 2 3 4 5 6 7 8 9 10";
			const unsigned findIndex2 = string2.rfind (string2.c_str (), 184u, 1u/*CoreToolLibrary::BadPosition/*2u*//*);
			startTime2 = GetTickCount () - startTime2;

//			AddLogEntry (false, LogLevel_Critical, false, "string: \"%s\", Find(\"*BLG\", 7u) == %u,\nstring2: \"%s\", string2.find(\"*BLG\", 7u) == %u.", string (), findIndex, string2.c_str (), findIndex2);
			AddLogEntry (false, LogLevel_Critical, false, "string: \"%s\",\nReverseFind(\"*BLG\", 7u) == %u,\ndelta == %u,\n\nstring2: \"%s\",\nstring2.rfind(\"*BLG\", 7u) == %u,\ndelta2 == %u.", string (), findIndex, startTime, string2.c_str (), findIndex2, startTime2);
/*
			const DynamicString string ("Immortal*BLG"), substring (string.GetSubstring ());

			AddLogEntry (false, LogLevel_Critical, false, "string: \"%s\",\nstring.GetAllocatedSize() == %u,\nstring.GetElementNumber() == %u,\n\nsubstring: \"%s\",\nsubstring.GetAllocatedSize() == %u,\nsubstring.GetElementNumber() == %u,\n\nsubstring.ToUpper(): \"%s\",\nsubstring.ToLower(): \"%s\".", string (), string.GetAllocatedSize (), string.GetElementNumber (), substring (), substring.GetAllocatedSize (), substring.GetElementNumber (), substring.ToUpper ().GetData (), substring.ToLower ().GetData ());
/*
			DynamicString string = "1, 2, 3, 4, 5, 6, 7, 8, 9, 10.";
			const DynamicString serorator = "";//" ,.";

			AddLogEntry (false, LogLevel_Critical, false, "string: \"%s\".FindOneOf(\" ,.\") == %u,\nstrcspn(string, \" ,.\") == %u.", string.GetData (), string.FindOneOf (serorator, 1u), strcspn (&string[1u], serorator));
//			AddLogEntry (false, LogLevel_Critical, false, "string: \"%s\".FindOneOf(\" ,.\") == %u,\nstrcspn(string, \" ,.\") == %u.", string.GetData (), string.FindOneOf (serorator, 100009u), strcspn (&string1[500], serorator));
/*
			DynamicString string = "1, 2, 3, 4, 5, 6, 7, 8, 9, 10";
			const DynamicString serorator = " ,.";
			const unsigned index = 27u;

			AddLogEntry (false, LogLevel_Critical, false, "string: \"%s\",\nindex == %u,\n&string[index]: \"%s\",\nstring.FindOneDiscrepancyOf(\" ,.\", index) == %u,\nstrspn(&string[index], \" ,.\") == %u,\n\nstring.FindOneOf(\" ,.\", index) == %u,\nstrcspn(&string[index], \" ,.\") == %u.", string.GetData (), index, string.GetData () + index, string.FindOneDiscrepancyOf (serorator, index), strspn (&string[index], serorator), string.FindOneOf (serorator, index), strcspn (&string[index], serorator));
/*
			const DynamicString string = "1, 2, 3, 4, 5, 6, 7, 8, 9, 10.";
//			const DynamicString string = ", a, b.	c.d=a.s,h 1 == 2 = 10						";
			unsigned int startTime (GetTickCount ());
			const DynamicArray <DynamicString> temp = string.Split (" \t,.=");
			startTime = GetTickCount () - startTime;

			InternalAssert (temp.GetElementNumber () == 10u);

			AddLogEntry (false, LogLevel_Critical, false, "temp[0u]: \"%s\",\ntemp[1u]: \"%s\",\ntemp[2u]: \"%s\",\ntemp[3u]: \"%s\",\ntemp[4u]: \"%s\",\ntemp[5u]: \"%s\",\ntemp[6u]: \"%s\",\ntemp[7u]: \"%s\",\ntemp[8u]: \"%s\",\ntemp[9u]: \"%s\",\n\ntemp.GetAllocatedSize() == %u,\ntemp.GetElementNumber() == %u,\n\ndelta == %u.", temp[0u].GetData (), temp[1u].GetData (), temp[2u].GetData (), temp[3u].GetData (), temp[4u].GetData (), temp[5u].GetData (), temp[6u].GetData (), temp[7u].GetData (), temp[8u].GetData (), temp[9u].GetData (), temp.GetAllocatedSize (), temp.GetElementNumber (), startTime);
/*
			char *string = "Immortal*BLG";
			const unsigned int stringLength (GetStringLength <unsigned short> (string));
			char *stringBuffer = new char[stringLength];

			strcpy (stringBuffer, string);

			stringBuffer[stringLength] = 0;

			AddLogEntry (false, LogLevel_Critical, false, "string: \"%s\",\nstringLength == %u,\nstringBuffer: \"%s\".", string, stringLength, stringBuffer);
/*
			const DynamicString string = "a";
			DynamicString string1;

			string1 = static_cast <char> (0);//'b';
			string1 = 'b';

			AddLogEntry (false, LogLevel_Critical, false, "string: \"%s\",\nstring1: \"%s\"", string.GetData (), string1.GetData ());
/*
//			DynamicString string = "Immortal*BLG*BlG";
			DynamicString string = "Immortal*BLG*BlG";
			std::string string3 = "Immortal*BLG*BlG";
//			const DynamicString oldStringToReplace = "l", newStringToReplace = "12345";
//			const unsigned int replacedOccurrencesCount = string.Replace ("*BLG", "");
//			const unsigned int replacedOccurrencesCount = string.Replace (oldStringToReplace, newStringToReplace, 1u, false);
//			string.replace (7u, 5u, newStringToReplace, 5u);
//			string.ReplaceAll (" 1", "Immortal*BLG*BlG");
			string.Replace (0u, CoreToolLibrary::BadPosition, "1234"/*, 0u*//*, 0u);
//			string.Replace (16u, CoreToolLibrary::BadPosition, NULL/*, 0u*//*, CoreToolLibrary::BadPosition);
			string3.replace (0u, CoreToolLibrary::BadPosition, "1234"/*, 0u*//*, 0u);
//			string3.replace (16u, CoreToolLibrary::BadPosition, NULL/*, 0u*//*, CoreToolLibrary::BadPosition);

//			AddLogEntry (false, LogLevel_Critical, false, "string: \"%s\",\nstring.GetElementNumber() == %u,\nstring.GetAllocatedSize() == %u,\nreplacedOccurrencesCount == %u.", string.GetData (), string.GetElementNumber (), string.GetAllocatedSize (), replacedOccurrencesCount);
//			AddLogEntry (false, LogLevel_Critical, false, "string: \"%s\",\noldStringToReplace: \"%s\",\nnewStringToReplace: \"%s\",\nstring.GetElementNumber() == %u,\nstring.GetAllocatedSize() == %u.", string.GetData (), oldStringToReplace.GetData (), newStringToReplace.GetData (), string.GetElementNumber (), string.GetAllocatedSize ());
			AddLogEntry (false, LogLevel_Critical, false, "string.Replace(): \"%s\",\nstring.GetElementNumber() == %u,\nstring.GetAllocatedSize() == %u,\n\nstring3.replace(): \"%s\",\nstring3.length() == %u,\nstring3.capacity() == %u.", string.GetData (), string.GetElementNumber (), string.GetAllocatedSize (), string3.c_str (), string3.length (), string3.capacity ());
/*
			DynamicString string;

			string += 'a';

			AddLogEntry (false, LogLevel_Critical, false, "NewString: \"%s\",\nstring.GetElementNumber () == %u,\nstring.GetAllocatedSize () == %u", string.GetData (), string.GetElementNumber (), string.GetAllocatedSize ());
/*
			DynamicString a = "Immortal*BLG", b = "*BLG";
			const char *const c = "*BLG";

			a += b += c;// += 's';
			a += 's';

//			AddLogEntry (false, LogLevel_Critical, false, "GetStringLength(a) == 12,\na.GetElementNumber() == 12,\na.GetAllocatedSize() == 12,\na[a.GetElementNumber() - 1u] = \"G\",\nstatic_cast <int> (a[a.GetElementNumber()]) == 0.", GetStringLength <unsigned short> (a), a.GetElementNumber (), a.GetAllocatedSize (), a[a.GetElementNumber () - 1u], static_cast <int> (a[a.GetElementNumber ()]));
//			AddLogEntry (false, LogLevel_Critical, false, "(a += b).GetData(): \"%s\",\nstrlen (a) == %u,\na.GetElementNumber() == %u,\na.GetAllocatedSize() == %u,\na[a.GetElementNumber() - 1u] = \"%c\",\nstatic_cast <int> (a[a.GetElementNumber()]) == %i.", (a += b).GetData (), GetStringLength <unsigned short> (a), a.GetElementNumber (), a.GetAllocatedSize (), a[a.GetElementNumber () - 1u], static_cast <int> (a[a.GetElementNumber ()]));
			AddLogEntry (false, LogLevel_Critical, false, "(a += b).GetData(): \"%s\",\nstrlen (a) == %u,\na.GetElementNumber() == %u,\na.GetAllocatedSize() == %u,\nstatic_cast <int> (a[a.GetElementNumber ()]) == %i.", a.GetData (), GetStringLength <unsigned short> (a), a.GetElementNumber (), a.GetAllocatedSize (), static_cast <int> (a[a.GetElementNumber () - 1u]));
*/
			if (g_YaPBNotLoaded)
				return;

			// Initialize engine.
			if (MetaModEngine::Globals::g_metaModEngine != NULL)
				Globals::g_halfLifeEngine = new Engine_WithMetaMod (engineFunctionTable, globalVariables);
			else
				Globals::g_halfLifeEngine = new Engine_WithoutMetaMod (engineFunctionTable, globalVariables);

			// Reliability check.
			if (Globals::g_halfLifeEngine == NULL)
				TerminateOnMalloc ();

			g_log.SetDirectory (Globals::g_halfLifeEngine->GetGameModName () + "/Addons/YaPB/Logs");

//			SDK::Structures::PhysEntity_t mdl;
//			AddLogEntry (false, LogLevel_Critical, true, "off=%u, %u", reinterpret_cast <unsigned char *> (&mdl.info) - reinterpret_cast <unsigned char *> (&mdl), sizeof (mdl));
/*			{
//			int val = SDK::Constants::FL_FROZEN;
//			int val = SDK::Constants::FL_ONGROUND;
			int val = 0x10u;
//			AddLogEntry (false, LogLevel_Critical, true, "%u", *((unsigned char *)&val + 1));
			AddLogEntry (false, LogLevel_Critical, true, "%u", *(unsigned int *)((unsigned char *)&val - 1));
			}
/*
			typedef float (*UTIL_SharedRandomFloat_t) (unsigned int seed, float low, float high);
			UTIL_SharedRandomFloat_t pfn ((UTIL_SharedRandomFloat_t)((unsigned int)Globals::g_halfLifeEngine->GetGameLibrary ()->GetPointer () + 0xCD030u));
			InternalAssert (pfn != NULL);
			const float rnd0 (pfn (0u, -0.5f, 0.5f)), rnd1 (pfn (0u + 1u, -0.5f, 0.5f));
			const float rnd2 (pfn (0u + 2u, -0.5f, 0.5f)), rnd3 (pfn (0u + 3u, -0.5f, 0.5f));
			AddLogEntry (false, LogLevel_Critical, true, "0=%f, 1=%f, 2=%f, 3=%f (summ: [%f, %f])\n0=%f, 1=%f, 2=%f, 3=%f.", rnd0, rnd1, rnd2, rnd3, rnd0 + rnd1, rnd2 + rnd3, pfn (0u, -0.5f, 0.5f), pfn (0u + 1u, -0.5f, 0.5f), pfn (0u + 2u, -0.5f, 0.5f), pfn (0u + 3u, -0.5f, 0.5f));
*/
			#define PRINT_STRUCTURE_MEMBER_OFFSET(structureName, memberName)	\
				AddLogEntry (false, LogLevel_Critical, true, "sizeof(%s) == %u, offset to \"%s\" == %u(%u).", #structureName, sizeof (structureName), #memberName, GET_OFFSET_OF_MEMBER_VARIABLE (structureName, memberName), GET_OFFSET_OF_MEMBER_VARIABLE (structureName, memberName) / Trampolines::Bytecode::Pointer32Size);

//			AddLogEntry (false, LogLevel_Critical, true, "%u", sizeof (SDK::Structures::ClientEngineFunction_t));
//			PRINT_STRUCTURE_MEMBER_OFFSET (SDK::Classes::Edict, privateData);	// (sizeof == 804, offset == 124)
			PRINT_STRUCTURE_MEMBER_OFFSET (SDK::Structures::EntityVariables_t, euser1);	// (sizeof == 804, offset == 28)
//			PRINT_STRUCTURE_MEMBER_OFFSET (SDK::Structures::UserCommand_t, lightLevel);	// (offset == 28)
//			PRINT_STRUCTURE_MEMBER_OFFSET (SDK::Structures::PlayerMove_t, physEntities);	// (offset == 592)
//			PRINT_STRUCTURE_MEMBER_OFFSET (SDK::Structures::EntityState_t, gaitsequence);	// (offset == 184)
//			PRINT_STRUCTURE_MEMBER_OFFSET (SDK::Structures::Model_t, entities);	// (sizeof == 392, offset == 384)
//			PRINT_STRUCTURE_MEMBER_OFFSET (SDK::Structures::Model_t, surfaces);	// (sizeof == 392, offset == 180)
//			PRINT_STRUCTURE_MEMBER_OFFSET (SDK::Structures::msurface_t, pdecals);	// (sizeof == 92, offset == 88)
//			PRINT_STRUCTURE_MEMBER_OFFSET (SDK::Structures::ClientEngineFunction_t, pfnGetMousePos);	// (sizeof == 532, offset == 392)
//			PRINT_STRUCTURE_MEMBER_OFFSET (SDK::Classes::Interface::IGameUI, m_szPlatformDir);	// (sizeof == 316, offset == 5)
//			PRINT_STRUCTURE_MEMBER_OFFSET (SDK::Classes::Interface::IGameUI, m_iNumFactories);	// (sizeof == 316, offset == 268)
//			PRINT_STRUCTURE_MEMBER_OFFSET (SDK::Classes::Interface::IGameUI, m_pszCurrentProgressType);	// (sizeof == 316, offset == 304)
//			PRINT_STRUCTURE_MEMBER_OFFSET (SDK::Classes::Interface::IGameUI, m_pMaster);	// (sizeof == 316, offset == 312)
//			PRINT_STRUCTURE_MEMBER_OFFSET (SDK::Structures::msurface_t, flags);	// (sizeof == 92, offset == 8)
//			PRINT_STRUCTURE_MEMBER_OFFSET (SDK::Structures::Model_t, texinfo);	// (sizeof == 392, offset == 172)
//			PRINT_STRUCTURE_MEMBER_OFFSET (SDK::Classes::BasePlayer, m_displayAutoHelpHUDTextHistoryBits);	// (sizeof == 2116, offset == 204)
//			PRINT_STRUCTURE_MEMBER_OFFSET (SDK::Classes::BasePlayer, m_openedMenuIndex);	// (sizeof == 2116, offset == 205)
//			PRINT_STRUCTURE_MEMBER_OFFSET (SDK::Classes::BasePlayer, m_lastPosition);	// (sizeof == 2116, offset == 211)
//			PRINT_STRUCTURE_MEMBER_OFFSET (SDK::Classes::BasePlayer, m_isWasKilledWithHeadShot);	// (sizeof == 2116, offset == 888)
//			PRINT_STRUCTURE_MEMBER_OFFSET (SDK::Classes::BasePlayer, m_isDefusingBomb);	// (sizeof == 2116, offset == 929)
//			PRINT_STRUCTURE_MEMBER_OFFSET (SDK::Classes::BasePlayer, m_flFallVelocity);	// (sizeof == 2116, offset == 251)
//			PRINT_STRUCTURE_MEMBER_OFFSET (SDK::Classes::BasePlayer, m_fDeadTime);	// (sizeof == 2116, offset == 354)
//			PRINT_STRUCTURE_MEMBER_OFFSET (SDK::Classes::BasePlayer, m_rgpPlayerItems);	// (sizeof == 2116, offset == 367)
//			PRINT_STRUCTURE_MEMBER_OFFSET (SDK::Classes::BasePlayer, m_iDeaths);	// (sizeof == 2116, offset == 444)
//			PRINT_STRUCTURE_MEMBER_OFFSET (SDK::Classes::BasePlayer, m_flNextDecalTime);	// (sizeof == 2116, offset == 486)
//			PRINT_STRUCTURE_MEMBER_OFFSET (SDK::Classes::BasePlayer, m_szAnimExtention);	// (sizeof == 2116, offset == 492)
//			PRINT_STRUCTURE_MEMBER_OFFSET (SDK::Classes::BasePlayer, m_nextAmmoBurn);	// (sizeof == 2116, offset == 611)
//			PRINT_STRUCTURE_MEMBER_OFFSET (SDK::Classes::NonCZCSBot, m_profile);	// (sizeof == 11404, offset == 620)
//			PRINT_STRUCTURE_MEMBER_OFFSET (SDK::Classes::NonCZCSBot, m_idleState);	// (sizeof == 11404, offset == 671)
//			PRINT_STRUCTURE_MEMBER_OFFSET (SDK::Classes::NonCZCSBot, m_discontinuityJumpTimestamp);	// (sizeof == 11404, offset == 765)
//			PRINT_STRUCTURE_MEMBER_OFFSET (SDK::Classes::NonCZCSBot, m_currentPathInformationNumber);	// (sizeof == 11404, offset == 2409)
//			PRINT_STRUCTURE_MEMBER_OFFSET (SDK::Classes::NonCZCSBot, m_gameState);	// (sizeof == 11404, offset == 2322)
//			PRINT_STRUCTURE_MEMBER_OFFSET (SDK::Classes::NonCZCSBot, m_gameState.m_mapZones);	// (sizeof == 11404, offset == 2333)
//			PRINT_STRUCTURE_MEMBER_OFFSET (SDK::Classes::NonCZCSBot, m_gameState.m_mapZonesNumber);	// (sizeof == 11404, offset == 2338)
//			PRINT_STRUCTURE_MEMBER_OFFSET (SDK::Classes::NonCZCSBot, m_hostageEscortCount);	// (sizeof == 11404, offset == 2302)
//			PRINT_STRUCTURE_MEMBER_OFFSET (SDK::Classes::NonCZCSBot, m_attacker);	// (sizeof == 11404, offset == 2730)
//			PRINT_STRUCTURE_MEMBER_OFFSET (SDK::Classes::NonCZCSBot, m_UNKNOWN_CountdownTimer2);	// (sizeof == 11404, offset == 2849)
//			PRINT_STRUCTURE_MEMBER_OFFSET (SDK::Classes::Hostage, m_improvPointer);	// (sizeof == 1968, offset == 1960)
//			PRINT_STRUCTURE_MEMBER_OFFSET (SDK::Classes::HostageImprov, m_followState);	// (sizeof == ????, offset == 252)
//			PRINT_STRUCTURE_MEMBER_OFFSET (SDK::Structures::netchan_t, connect_time);	// (sizeof == 9504, offset == 32)
//			PRINT_STRUCTURE_MEMBER_OFFSET (SDK::Structures::netchan_t, rate);	// (sizeof == 9504, offset == 40)
//			PRINT_STRUCTURE_MEMBER_OFFSET (SDK::Structures::Client_t, last_cmdtime);	// (sizeof == 20488, offset == 9608)
//			PRINT_STRUCTURE_MEMBER_OFFSET (SDK::Structures::Client_t, connection_started);	// (sizeof == 20488, offset == 13688)
//			PRINT_STRUCTURE_MEMBER_OFFSET (SDK::Structures::Client_t, isCustomResourcePropagationCompleted);	// (sizeof == 20488, offset == 20040)
//			PRINT_STRUCTURE_MEMBER_OFFSET (SDK::Structures::mnode_t, parent);	// (sizeof == 52, offset == 32)
//			PRINT_STRUCTURE_MEMBER_OFFSET (SDK::Structures::medge_t, cachededgeoffset);	// (sizeof == 8, offset == 4)

			#undef PRINT_STRUCTURE_MEMBER_OFFSET
/*
			#define PRINT_CLASS_VIRTUAL_FUNCTION_OFFSET(className, functionName)	\
			{	\
				Trampolines::MemoryFunctionInformation_t mfi;	\
				Trampolines::GetFunctionInformation (&className::functionName, mfi);	\
				AddLogEntry (false, LogLevel_Critical, true, "sizeof(%s) == %u, mfi for \"%s\": [thisPointerOffset=%i, virtualFunctionTableIndex=%i, virtualFunctionTableOffset=%i].", #className, sizeof (className), #functionName, mfi.thisPointerOffset, mfi.virtualFunctionTableIndex, mfi.virtualFunctionTableOffset);	\
			}

//			SDK::VGUI::ISurface g_srf;
//			PRINT_CLASS_VIRTUAL_FUNCTION_OFFSET (SDK::VGUI::ISurface, Shutdown);
//			PRINT_CLASS_VIRTUAL_FUNCTION_OFFSET (YaPB, Think);

			#undef PRINT_CLASS_VIRTUAL_FUNCTION_OFFSET
//*/
			if (MetaModEngine::Globals::g_metaModEngine == NULL)
			{
				// activate link entities-replacement after give engine functions so that if game DLL is
				// plugin too and uses same method we get combined export table of plugin and game DLL
				if (!g_YaPBModuleHandle.CombineModuleExportTables (*Globals::g_halfLifeEngine->GetGameLibrary ()))
					AddLogEntry (false, LogLevel_Critical, true, "Couldn't load link entity replacement for game DLL!");
			}
			else
			{
				// Initialize the meta mod DLL pointer
				MetaModEngine::Globals::g_metaModEngine->m_library = MemoryUtilities::GetModuleInformation (engineFunctionTable);

				// Reliability checks.
				InternalAssert (MetaModEngine::Globals::g_metaModEngine->GetLibrary ().IsValid ());
				InternalAssert (MetaModEngine::Globals::g_metaModEngine->GetLibrary ().GetPointer () == GetModuleHandle ("metamod.DLL"));

				MetaModEngine::Globals::g_metaModEngine->RegisterExportRedirections ();
			}

//			AddLogEntry (false, LogLevel_Critical, true, "%u, 136", sizeof (SDK::Structures::Resource_t));
//			AddLogEntry (false, LogLevel_Critical, true, "%u, 20488", sizeof (SDK::Structures::Client_t));
//			AddLogEntry (false, LogLevel_Critical, true, "%u, 9504", sizeof (SDK::Structures::netchan_t));
/*			SDK::Structures::Client_t r;

			AddLogEntry (false, LogLevel_Critical, true, "%u, 20488, off=%u", sizeof (SDK::Structures::Client_t), reinterpret_cast <unsigned char *> (&r.name) - reinterpret_cast <unsigned char *> (&r));
/*			SDK::Structures::Server_t::Resource_t r;
			AddLogEntry (false, LogLevel_Critical, true, "%u", reinterpret_cast <unsigned char *> (&r.UNKNOWN6) - reinterpret_cast <unsigned char *> (&r));

			InternalAssert (sizeof (SDK::Structures::Server_t) == 287768);
			InternalAssert (sizeof (SDK::Structures::Server_t::Resource_t) == 136);
			InternalAssert (0);
*/
/*			SDK::Structures::Server_t sv;

			AddLogEntry (false, LogLevel_Critical, true, "size=%u, size=%u, Offset1 == %u!", sizeof (SDK::Structures::EntityState_t), sizeof (sv), reinterpret_cast <unsigned int *> (&sv.signon_buf) - reinterpret_cast <unsigned int *> (&sv));
*/
			Console::Bot::variableManager.RegisterVariables ();	// Register bot console variables....
			Console::Server::variableManager.RegisterVariables ();	// Register server console variables....

			Console::g_consoleVariableValueChangeHooker.CreateAndDoPatch ();

			// Initialize random number generator.
			g_randomNumberGenerator.Initialize (static_cast <unsigned int> (time (NULL)));

			if (Globals::g_halfLifeEngine->GetGameModID () != Engine::Version_ConditionZero)
			{
				if (Utilities::ZBotInGameEnabler::EnableBotInGame ())
					AddLogEntry (true, LogLevel_Default, false, "EnableBotInGame(): SUCCEED!!!");
				else
					AddLogEntry (true, LogLevel_Warning, false, "EnableBotInGame(): FILED!!!");
			}
/*
			const char *const names[] =
			{
				"DA",
				"NET",
			};

			AddLogEntry (false, LogLevel_Critical, true, "MI BUDEM SMOTRET \"%s\"", names[g_randomNumberGenerator.GetValueBetween <unsigned char> (0u, GET_STATIC_ARRAY_SIZE (names) - 1u)]);
/*
			DynamicString string;

			string.Assign ('=');

			AddLogEntry (false, LogLevel_Critical, false, "string: \"%s\".", string.GetData ());
*
//			LinkedList <int, unsigned char> integerList;*/
/*			DynamicArray <int, unsigned char> integerList;

//			integerList.PushFront (1);
//			integerList.PushBack (10);
//			integerList.PushFront (0);
			for (unsigned char index (0u); index < 10u; ++index)
			{
				integerList.PushBack (index);
//				integerList.PushFront (index, 2u);
//				integerList.Insert (0u, index, 2u);

				AddLogEntry (false, LogLevel_Default, true, "integerList[%u] == %i.", index, integerList[index]);
			}
//			const int integer[3u] = {-1, -2, -3};
//			const DynamicArray <int, unsigned char> integerList2 (integer, 3u);
//			integerList.PushFront (integer, 3u);
//			integerList.Insert (0u, integerList, 1u, 2u);

			integerList.Reverse ();

			for (unsigned char index (0u); index < integerList.GetElementNumber (); ++index)
				AddLogEntry (false, LogLevel_Default, true, "integerList[%u] == %i.", index, integerList[index]);

//			for (unsigned char iterator = integerList.GetFirst (); iterator < integerList.BadPosition; iterator = integerList.GetNext (iterator))
//				AddLogEntry (false, LogLevel_Default, true, "integerList[%u] == %i.", iterator, integerList[iterator]);

//			AddLogEntry (false, LogLevel_Critical, false, "integerList: integerList[0u] == %i,\nintegerList[1u] == %i,\nintegerList[2u] == %i\n\nintegerList.BadPosition == %u,\nintegerList.MaximumPossibleSize == %u.", integerList[0u], integerList[1u], integerList[2u], integerList.BadPosition, integerList.MaximumPossibleSize);
			AddLogEntry (false, LogLevel_Critical, false, "Done! integerList.GetElementNumber() == %u.", integerList.GetElementNumber ());

/*			DynamicString Lstring ("IMMORTAL*BLG"), Ustring ("immortal*blg");

//			Lstring.MakeLower ();
//			Ustring.MakeUpper ();

			AddLogEntry (false, LogLevel_Critical, false, "Lstring(IMMORTAL*BLG): \"%s\",\nUstring(immortal*blg): \"%s\".", Lstring.ToLower (9u).GetData (), Ustring.ToUpper (9u).GetData ());
*
			/*const *//*DynamicString string ("0 1 2 3 4 5 6 7 8 9 10"), reversedString (string.ToReverse (10u, 3u));

			string.Reverse (10u, 3u);

			AddLogEntry (false, LogLevel_Critical, false, "string: \"%s\",\nGetElementNumber() == %u,\nGetAllocatedSize() == %u,\n\nToReverse(): \"%s\",\nGetElementNumber() == %u,\nGetAllocatedSize() == %u.", string.GetData (), string.GetElementNumber (), string.GetAllocatedSize (), reversedString.GetData (), reversedString.GetElementNumber (), reversedString.GetAllocatedSize ());
*/
/*			Stack <int> integerStack;

			integerStack.Push (300);
			integerStack.Push (200);
			integerStack.Push (100);

			for (unsigned char index = 0u; index < integerStack.GetElementNumber (); ++index)
				AddLogEntry (false, LogLevel_Default, true, "integerStack[%u] == %i.", index, integerStack[index]);

			AddLogEntry (false, LogLevel_Critical, false, "Done! integerStack: GetElementNumber() == %u, GetAllocatedSize() == %u.", integerStack.GetElementNumber (), integerStack.GetAllocatedSize ());
*
			Deque <int, unsigned char> integerDeque;

			integerDeque.PushFront (300);
			integerDeque.PushFront (200);
			integerDeque.PushFront (100);

			for (unsigned char index = 0u; index < integerDeque.GetElementNumber (); ++index)
				AddLogEntry (false, LogLevel_Default, true, "integerDeque[%u] == %i.", index, integerDeque[index]);

			AddLogEntry (false, LogLevel_Critical, false, "Done! integerDeque: GetElementNumber() == %u, GetAllocatedSize() == %u.", integerDeque.GetElementNumber (), integerDeque.GetAllocatedSize ());

/*			deque <int, unsigned char> integerDeque;

			integerDeque.PushFront (300);
			integerDeque.PushFront (200);
			integerDeque.PushFront (100);

			for (unsigned char index = 0u; index < integerDeque.GetElementNumber (); ++index)
//				AddLogEntry (false, LogLevel_Default, true, "integerDeque[%u] == %i.", index, integerDeque[index]);
				AddLogEntry (false, LogLevel_Default, true, "index == %u, integerDeque.PopFront() == %i.", index, integerDeque.PopFront ());

			AddLogEntry (false, LogLevel_Critical, false, "Done! integerDeque: GetElementNumber() == %u.", integerDeque.GetElementNumber ());
*/
/*			double startTime = UTIL_GetSecs ();

			for (float angle = 200000.0f; angle >= 0.0f; angle -= 0.5759f)
//			for (float angle = -200000.0f; angle < 170000.0f; angle += 0.5759f)
				Math::PMBAngleNormalize (angle);

			AddLogEntry (false, LogLevel_Default, false, "PMB Done! delta == %f", UTIL_GetSecs () - startTime);

			startTime = UTIL_GetSecs ();

			for (float angle = 200000.0f; angle >= 0.0f; angle -= 0.5759f)
//			for (float angle = -200000.0f; angle < 170000.0f; angle += 0.5759f)
				Math::KORAXAngleNormalize (angle);

			AddLogEntry (false, LogLevel_Default, false, "KORAX Done! delta == %f", UTIL_GetSecs () - startTime);

			startTime = UTIL_GetSecs ();

			for (float angle = 200000.0f; angle >= 0.0f; angle -= 0.5759f)
//			for (float angle = -200000.0f; angle < 170000.0f; angle += 0.5759f)
				Math::JKAngleNormalize (angle);

			AddLogEntry (false, LogLevel_Default, false, "JK Done! delta == %f", UTIL_GetSecs () - startTime);

			startTime = UTIL_GetSecs ();

			for (float angle = 200000.0f; angle >= 0.0f; angle -= 0.5759f)
//			for (float angle = -200000.0f; angle < 170000.0f; angle += 0.5759f)
				Math::ETQWAngleNormalize (angle);

			AddLogEntry (false, LogLevel_Default, false, "ETQW Done! delta == %f", UTIL_GetSecs () - startTime);

			startTime = UTIL_GetSecs ();

			for (float angle = 200000.0f; angle >= 0.0f; angle -= 0.5759f)
//			for (float angle = -200000.0f; angle < 170000.0f; angle += 0.5759f)
				Math::HL2AngleNormalize (angle);

			AddLogEntry (false, LogLevel_Critical, false, "HL2 Done! delta == %f", UTIL_GetSecs () - startTime);
*/
//			for (unsigned int money = 0u; money <= 16000u; ++money)
//				if (static_cast <float> ((money - 3000) / (16000 - 3000) * 3) != static_cast <float> ((money - 3000.0) / (16000 - 3000) * 3))
//					AddLogEntry (false, LogLevel_Critical, false, "static_cast <float> ((money - 3000) / (16000 - 3000) * 3)(%lf) != static_cast <float> ((money - 3000.0) / (16000 - 3000) * 3)(%lf).", static_cast <float> ((money - 3000) / (16000 - 3000) * 3), static_cast <float> ((money - 3000.0) / (16000 - 3000) * 3));
//			AddLogEntry (false, LogLevel_Critical, false, "static_cast <float> (100u - 3000) == %lf,\nstatic_cast <float> (100u - 3000.0)(%lf) == static_cast <float> (100u - 3000.0f)(%lf)?== %s.", static_cast <float> (100u - 3000), static_cast <float> (100u - 3000.0), static_cast <float> (100u - 3000.0f), static_cast <float> (100u - 3000.0) == static_cast <float> (100u - 3000.0f) ? "true" : "false");
/*
			Math::Vector3D vector (0.0f);
//			AddLogEntry (false, LogLevel_Critical, false, "vector.GetLength2D() == %f.", vector.GetLength2D ());
//			AddLogEntry (false, LogLevel_Critical, false, "vector.GetLength2D() == %f.", vector.GetLength2D ());

			Math::Angles3D angles (vector.ToAngles3D ());
//			Angles3D angles (RadianToDegree (atan2f (SDK::Constants::MapSize, GetLength2D ())), RadianToDegree (atan2f (SDK::Constants::MapSize, SDK::Constants::MapSize)), 0.0f);

			AddLogEntry (false, LogLevel_Critical, false, "angles == [%.2f, %.2f], AngleNormalize (270.0f) == %.2f.", angles.pitch, angles.yaw, Math::AngleNormalize (270.0f));
*
			const DynamicString string ("Immortal*BLG");
			const DynamicArray <DynamicString, unsigned char> stringArray (string.Split <unsigned char> ('/', 2u, 6u));

			AddLogEntry (false, LogLevel_Critical, false, "string: \"%s\", stringArray.GetElementNumber()=%u, stringArray[0u]: \"%s\".", string.GetData (), stringArray.GetElementNumber (), stringArray[0u].GetData ());
*
			const float flt (150.4f);
			const unsigned short us_flt (Utilities::GetFixedUnsigned16 (flt, 8u));
			const float flt2 (us_flt << 12u);
			const float flt3 (us_flt / BIT (8u));
			const float flt4 (us_flt << 8u);
			const float flt5 (us_flt >> 8u);

			AddLogEntry (false, LogLevel_Critical, false, "flt = %.2f, us_flt = %u, flt2 = %.2f, flt3 = %.2f, flt4 = %.2f, flt5 = %.2f.", flt, us_flt, flt2, flt3, flt4, flt5);
*
			const DynamicString &NULLPathname ("de_inferno.BSP"), &bspPathname ("maps/" + NULLPathname);
			unsigned int bspSize, NULLSize;
			const int bspSizeAnother (HalfLifeEngine::Globals::g_halfLifeEngine->GetFileSize (bspPathname));
			const int NULLSizeAnother (HalfLifeEngine::Globals::g_halfLifeEngine->GetFileSize (NULLPathname));
			unsigned char *const BSPMapFile_NULL (HalfLifeEngine::Globals::g_halfLifeEngine->LoadFile (bspPathname));
			unsigned char *const BSPMapFile (HalfLifeEngine::Globals::g_halfLifeEngine->LoadFile (bspPathname, &bspSize));
			unsigned char *const NULLFile (HalfLifeEngine::Globals::g_halfLifeEngine->LoadFile (NULLPathname, &NULLSize));

			HalfLifeEngine::Globals::g_halfLifeEngine->FreeFile (BSPMapFile_NULL);
			HalfLifeEngine::Globals::g_halfLifeEngine->FreeFile (BSPMapFile);

			AddLogEntry (false, LogLevel_Critical, false, "BSPMapFile != NULL: %s,\nbspPathname: \"%s\",\nbspSize=%i,\nbspSizeAnother=%i,\n\nNULLFile != NULL: %s,\nNULLSize=%i, NULLSizeAnother=%i.", BSPMapFile != NULL ? "true" : "false", bspPathname.GetData (), bspSize, bspSizeAnother, NULLFile != NULL ? "true" : "false", NULLSize, NULLSizeAnother);
*
			const DynamicString string ("Immortal*BLG");
			const unsigned short index (string.FindOneDiscrepancyOf ("iL", 7u, string.GetElementNumber () - 7u, false));

			AddLogEntry (false, LogLevel_Critical, false, "string: \"%s\", index=%u(%c).", string.GetData (), index, index < string.GetElementNumber () ? string[index] : 'W');
*/
/*			struct Item_t
			{
//				DynamicString str;
				char          str[2u];
				int           num;

				inline Item_t (void) : num (0) { str[0u] = '\0'; }
//				inline Item_t (const DynamicString &inputStr, const int inputNum) : str (inputStr), num (inputNum) {}
				inline Item_t (const char inputStr[2u], const int inputNum) : num (inputNum) { str[0u] = inputStr[0u]; str[1u] = inputStr[1u]; }
			};
			class TEST_PriorityQueue : public PriorityQueue <Item_t, unsigned char>
			{
				private:
					static inline const bool IsLowerPriority (const Item_t &node1, const Item_t &node2) { return node1.num <= node2.num; }

				public:
					inline TEST_PriorityQueue (void) : PriorityQueue (IsLowerPriority) {}
			} testPriorityQueue;

			testPriorityQueue += Item_t ("1", 100);
			testPriorityQueue += Item_t ("2", 100);
			testPriorityQueue += Item_t ("3", 100);
			testPriorityQueue += Item_t ("4", 10);
			testPriorityQueue += Item_t ("5", 9);
			testPriorityQueue += Item_t ("6", 11);
			testPriorityQueue += Item_t ("7", 1);
			testPriorityQueue += Item_t ("8", -100);

			for (unsigned char index (0u); index < testPriorityQueue.GetElementNumber (); ++index)
				AddLogEntry (false, LogLevel_Default, true, "testPriorityQueue[%u] == [%s, %i].", index, testPriorityQueue[index].str/*.GetData ()*//*, testPriorityQueue[index].num);

			AddLogEntry (false, LogLevel_Critical, true, "DONE!!!");
*/
//			AddLogEntry (false, LogLevel_Critical, true, "sizeof (SDK::Structures::EngineFunction_t)=628.");
/*
			SDK::Classes::BaseDelay baseDelay;

			InternalAssert (sizeof (SDK::Classes::BaseEntity) == 136u);
			InternalAssert (reinterpret_cast <unsigned char *> (&baseDelay->m_pfnThink) == reinterpret_cast <unsigned char *> (&baseDelay) + 16u);
			InternalAssert (reinterpret_cast <unsigned char *> (&baseDelay->m_pfnTouch) == reinterpret_cast <unsigned char *> (&baseDelay) + 20u);
			InternalAssert (reinterpret_cast <unsigned char *> (&baseDelay->m_pfnUse) == reinterpret_cast <unsigned char *> (&baseDelay) + 24u);
			InternalAssert (reinterpret_cast <unsigned char *> (&baseDelay->m_pfnBlocked) == reinterpret_cast <unsigned char *> (&baseDelay) + 28u);
			InternalAssert (reinterpret_cast <unsigned char *> (&baseDelay->ammo_338Magnum) == reinterpret_cast <unsigned char *> (&baseDelay) + 100u);
			InternalAssert (reinterpret_cast <unsigned char *> (&baseDelay->m_isPlayerHasDisconnected) == reinterpret_cast <unsigned char *> (&baseDelay) + 132u);

//			AddLogEntry (false, LogLevel_Default, true, "sizeof (SDK::Classes::BaseEntity)=%u, sizeof (SDK::Classes::BaseDelay)=%u, baseDelay.m_flDelay address=%u.", sizeof (SDK::Classes::BaseEntity), sizeof (SDK::Classes::BaseDelay), reinterpret_cast <unsigned char *> (&baseDelay.m_flDelay) - reinterpret_cast <unsigned char *> (&baseDelay));

			AddLogEntry (false, LogLevel_Critical, true, "DONE!!!");
/*
			SDK::Classes::BaseDelay baseDelay;
			const int magic (-987654321);

			baseDelay.m_iszKillTarget = magic;

			for (unsigned char index (0u); index < 40; ++index)
				if (reinterpret_cast <int *> (&baseDelay)[index] == magic)
					AddLogEntry (false, LogLevel_Critical, true, "FOUND, index=%u.", index);

			AddLogEntry (false, LogLevel_Critical, true, "NOT FOUND!!!");
/*
			SDK::Classes::HalfLifeTraining halfLifeTraining;

			AddLogEntry (false, LogLevel_Default, true, "reinterpret_cast <unsigned char *> (&halfLifeTraining) + SDK::Constants::GameRules::Offset_IsFreezePeriod == %u\nreinterpret_cast <unsigned char *> (&halfLifeTraining.m_isFreezePeriod) == %u\n.", static_cast <unsigned int> (reinterpret_cast <unsigned char *> (&halfLifeTraining) + SDK::Constants::GameRules::Offset_IsFreezePeriod - reinterpret_cast <unsigned char *> (&halfLifeTraining)), static_cast <unsigned int> (reinterpret_cast <unsigned char *> (&halfLifeTraining.m_isFreezePeriod) - reinterpret_cast <unsigned char *> (&halfLifeTraining)));
			AddLogEntry (false, LogLevel_Default, true, "reinterpret_cast <unsigned char *> (&halfLifeTraining) + SDK::Constants::GameRules::Offset_VIP == %u\nreinterpret_cast <unsigned char *> (&halfLifeTraining.m_VIP) == %u\n.", static_cast <unsigned int> (reinterpret_cast <unsigned char *> (&halfLifeTraining) + SDK::Constants::GameRules::Offset_VIP * 4 - reinterpret_cast <unsigned char *> (&halfLifeTraining)), static_cast <unsigned int> (reinterpret_cast <unsigned char *> (&halfLifeTraining.m_VIP) - reinterpret_cast <unsigned char *> (&halfLifeTraining)));
			AddLogEntry (false, LogLevel_Default, true, "reinterpret_cast <unsigned char *> (&halfLifeTraining) + SDK::Constants::GameRules::Offset_IsMapHasEscapeZone == %u\nreinterpret_cast <unsigned char *> (&halfLifeTraining.m_isMapHasEscapeZone) == %u\n.", static_cast <unsigned int> (reinterpret_cast <unsigned char *> (&halfLifeTraining) + SDK::Constants::GameRules::Offset_IsMapHasEscapeZone - reinterpret_cast <unsigned char *> (&halfLifeTraining)), static_cast <unsigned int> (reinterpret_cast <unsigned char *> (&halfLifeTraining.m_isMapHasEscapeZone) - reinterpret_cast <unsigned char *> (&halfLifeTraining)));
			AddLogEntry (false, LogLevel_Default, true, "reinterpret_cast <unsigned char *> (&halfLifeTraining) + SDK::Constants::GameRules::Offset_IsShouldSkipSpawn == %u\nreinterpret_cast <unsigned char *> (&halfLifeTraining.m_isShouldSkipSpawn) == %u\n.", static_cast <unsigned int> (reinterpret_cast <unsigned char *> (&halfLifeTraining) + SDK::Constants::GameRules::Offset_IsShouldSkipSpawn - reinterpret_cast <unsigned char *> (&halfLifeTraining)), static_cast <unsigned int> (reinterpret_cast <unsigned char *> (&halfLifeTraining.m_isShouldSkipSpawn) - reinterpret_cast <unsigned char *> (&halfLifeTraining)));
			AddLogEntry (false, LogLevel_Default, true, "reinterpret_cast <unsigned char *> (&halfLifeTraining) + SDK::Constants::GameRules::Offset_UNKNOWN7Time == %u\nreinterpret_cast <unsigned char *> (&halfLifeTraining.m_UNKNOWN7Time) == %u\n.", static_cast <unsigned int> (reinterpret_cast <unsigned char *> (&halfLifeTraining) + 180 * 4 - reinterpret_cast <unsigned char *> (&halfLifeTraining)), static_cast <unsigned int> (reinterpret_cast <unsigned char *> (&halfLifeTraining.m_UNKNOWN7Time) - reinterpret_cast <unsigned char *> (&halfLifeTraining)));
			AddLogEntry (false, LogLevel_Default, true, "reinterpret_cast <unsigned char *> (&halfLifeTraining) + SDK::Constants::GameRules::Offset_UNKNOWN10 == %u\nreinterpret_cast <unsigned char *> (&halfLifeTraining.m_UNKNOWN10) == %u\n.", static_cast <unsigned int> (reinterpret_cast <unsigned char *> (&halfLifeTraining) + 183 * 4 - reinterpret_cast <unsigned char *> (&halfLifeTraining)), static_cast <unsigned int> (reinterpret_cast <unsigned char *> (&halfLifeTraining.m_UNKNOWN10) - reinterpret_cast <unsigned char *> (&halfLifeTraining)));
			AddLogEntry (false, LogLevel_Default, true, "reinterpret_cast <unsigned char *> (&halfLifeTraining) + SDK::Constants::GameRules::Offset_IsShowVGUIMenus == %u\nreinterpret_cast <unsigned char *> (&halfLifeTraining.m_isShowVGUIMenus) == %u\n.", static_cast <unsigned int> (reinterpret_cast <unsigned char *> (&halfLifeTraining) + SDK::Constants::GameRules::Offset_IsShowVGUIMenus - reinterpret_cast <unsigned char *> (&halfLifeTraining)), static_cast <unsigned int> (reinterpret_cast <unsigned char *> (&halfLifeTraining.m_isShowVGUIMenus) - reinterpret_cast <unsigned char *> (&halfLifeTraining)));

			InternalAssert (MemoryUtilities::GetElementsTable <void *const> (&halfLifeTraining) == halfLifeTraining.m_pseudoVirtualFunctionTable);
			InternalAssert (reinterpret_cast <unsigned char *> (&halfLifeTraining) + SDK::Constants::GameRules::Offset_IsFreezePeriod == reinterpret_cast <unsigned char *> (&halfLifeTraining.m_isFreezePeriod));
			InternalAssert (reinterpret_cast <unsigned char *> (&halfLifeTraining) + SDK::Constants::GameRules::Offset_VIP * 4 == reinterpret_cast <unsigned char *> (&halfLifeTraining.m_VIP));
			InternalAssert (reinterpret_cast <unsigned char *> (&halfLifeTraining) + SDK::Constants::GameRules::Offset_IsMapHasEscapeZone == reinterpret_cast <unsigned char *> (&halfLifeTraining.m_isMapHasEscapeZone));
			InternalAssert (reinterpret_cast <unsigned char *> (&halfLifeTraining) + SDK::Constants::GameRules::Offset_IsShouldSkipSpawn == reinterpret_cast <unsigned char *> (&halfLifeTraining.m_isShouldSkipSpawn));
			InternalAssert (reinterpret_cast <unsigned char *> (&halfLifeTraining) + 180 * 4 == reinterpret_cast <unsigned char *> (&halfLifeTraining.m_UNKNOWN7Time));
			InternalAssert (reinterpret_cast <unsigned char *> (&halfLifeTraining) + 183 * 4 == reinterpret_cast <unsigned char *> (&halfLifeTraining.m_UNKNOWN10));
			InternalAssert (reinterpret_cast <unsigned char *> (&halfLifeTraining) + SDK::Constants::GameRules::Offset_IsShowVGUIMenus == reinterpret_cast <unsigned char *> (&halfLifeTraining.m_isShowVGUIMenus));

			AddLogEntry (false, LogLevel_Critical, true, "sizeof (SDK::Classes::HalfLifeMultiplay) == %u,\nsizeof (SDK::Classes::HalfLifeTraining) == %u.", sizeof (SDK::Classes::HalfLifeMultiplay), sizeof (SDK::Classes::HalfLifeTraining));
*
//			SDK::Classes::Edict edict;
			SDK::Structures::EntityVariables_t variables;
//			SDK::Structures::EntityVariables_t &variables (edict.variables);

//			AddLogEntry (false, LogLevel_Critical, true, "edict.variables.solid == %u,\nvariables.solid == %u.", reinterpret_cast <unsigned char *> (&edict.variables.solid) - reinterpret_cast <unsigned char *> (&edict), reinterpret_cast <unsigned char *> (&variables.solid) - reinterpret_cast <unsigned char *> (&variables));
			AddLogEntry (false, LogLevel_Critical, true, "variables.solid == %u.", reinterpret_cast <unsigned char *> (&variables.solid) - reinterpret_cast <unsigned char *> (&variables));
*/
/*			SDK::Classes::Edict ed;

			// offset freeTime                           == 120 (0)
			// offset privateData                        == 124 (0)
			// offset variables                          == 128 (0)
			// offset variables.className                == 128 (0)
			// offset variables.globalName               == 132 (4)
			// offset variables.origin                   == 136 (8)
			// offset variables.oldOrigin                == 148 (29)
			// offset variables.velocity                 == 160 (32)
			// offset variables.baseVelocity             == 172 (44)
			// offset variables.clBaseVelocity           == 184 (56)
			// offset variables.moveDirection            == 196 (68)
			// offset variables.modelAngles              == 208 (80)
			// offset variables.angleVelocity            == 220 (92)
			// offset variables.punchAngles              == 232 (104)
			// offset variables.viewAngles               == 244 (116)

			// offset variables.fixAngle                 == 288 (160)
			// offset variables.idealPitch               == 292 (164)
			// offset variables.pitchSpeed               == 296 (168)
			// offset variables.idealYaw                 == 300 (172)
			// offset variables.yawSpeed                 == 304 (176)
			// offset variables.modelIndex               == 308 (180)
			// offset variables.model                    == 312 (184)
			// offset variables.viewModelName            == 316 (188)
			// offset variables.weaponModelName          == 320 (192)
			// offset variables.absoluteBoundingBox.mins == 324 (196)
			// offset variables.absoluteBoundingBox.maxs == 336 (208)
			// offset variables.boundingBox.mins         == 348 (220)
			// offset variables.boundingBox.maxs         == 360 (232)
			// offset variables.size                     == 372 (244)
			// offset variables.localTime                == 384 (256)
			// offset variables.nextThinkTime            == 388 (260)
			// offset variables.moveType                 == 392 (264)
			// offset variables.solid                    == 396 (268)
			// offset variables.skin                     == 400 (272)
			// offset variables.body                     == 404 (276)
			// offset variables.effects                  == 408 (280)
			// offset variables.gravity                  == 412 (284)
			// offset variables.friction                 == 416 (288)
			// offset variables.lightLevel               == 420 (292)
			// offset variables.sequence                 == 424 (296)
			// offset variables.gaitSequence             == 428 (300)
			// offset variables.frame                    == 432 (304)
			// offset variables.animationTime            == 436 (308)
			// offset variables.frameRate                == 440 (312)
			// offset variables.controller               == 444 (316)
			// offset variables.blending                 == 448 (320)
			// offset variables.scale                    == 452 (324)
			// offset variables.renderMode               == 456 (328)
			// offset variables.renderAmount             == 460 (332)
			// offset variables.renderColor              == 464 (336)
			// offset variables.renderFx                 == 476 (348)
			// offset variables.health                   == 480 (352)
			// offset variables.frags                    == 484 (356)
			// offset variables.weapons                  == 488 (360)
			// offset variables.takeDamageType           == 492 (364)

			// offset variables.viewOffset               == 500 (372)
			// offset variables.buttons                  == 512 (384)
			// offset variables.impulse                  == 516 (388)
			// offset variables.chain                    == 520 (392)
			// offset variables.damageInflictor          == 524 (396)

			// offset variables.owner                    == 536 (408)
			// offset variables.groundEntity             == 540 (412)
			// offset variables.spawnFlags               == 544 (416)
			// offset variables.flags                    == 548 (420)
			// offset variables.colorMap                 == 552 (424)

			// offset variables.waterLevel               == 576 (448)
			// offset variables.waterType                == 580 (452)
			// offset variables.target                   == 584 (456)
			// offset variables.targetName               == 588 (460)
			// offset variables.netName                  == 592 (464)
			// offset variables.message                  == 596 (468)

			// offset variables.containingEntity         == 648 (520)
			// offset variables.playerClass              == 652 (524)
			// offset variables.maximumSpeed             == 656 (528)

			// offset variables.fov                      == 660 (532)
			// offset variables.weaponAnimation          == 664 (536)
			// offset variables.pushMsec                 == 668 (540)
			// offset variables.isInDuck                 == 672 (544)
			// offset variables.timeStepSound            == 676 (548)
			// offset variables.swimTime                 == 680 (552)
			// offset variables.duckTime                 == 684 (556)
			// offset variables.stepLeft                 == 688 (560)
			// offset variables.fallVelocity             == 692 (564)
			// offset variables.gameState                == 696 (568)
			// offset variables.oldButtons               == 700 (572)

			// offset variables.groupInfo                == 704 (576)
			// offset variables.iuser1                   == 708 (580)
			// offset variables.iuser2                   == 712 (584)

			AddLogEntry (false, LogLevel_Critical, true, "off=%u", reinterpret_cast <unsigned char *> (&ed.variables.modelIndex) - reinterpret_cast <unsigned char *> (&ed));

/*			int flags (SDK::Constants::FL_CONVEYOR);// (0);// (SDK::Constants::FL_ONGROUND | SDK::Constants::FL_PARTIALGROUND);

//			reinterpret_cast <unsigned char*> (&flags)[1u] |= 0xFDu;
//			reinterpret_cast <unsigned char*> (&flags)[1u] |= 0x28;

			AddLogEntry (false, LogLevel_Critical, true, "flags=%u, static_cast <unsigned char> (SDK::Constants::FL_ONGROUND)=%u", flags, *(reinterpret_cast <unsigned char*> (&flags) + 1u));
*/
/*//		Stack <unsigned char, unsigned char> s;
			Queue <unsigned char, unsigned char> s;

			for (unsigned char i (1u); i <= 10u; ++i)
				s.PushBack (i);

			while (!s.IsEmpty ())
			{
				const unsigned char element (s.PopFront ());

				AddLogEntry (false, LogLevel_Default, true, "s.Pop()=%u, elements left = %u", element, s.GetElementNumber ());
			}

			AddLogEntry (false, LogLevel_Critical, true, "DONE!");
*
			float f;
//			double f;

			*reinterpret_cast <int *> (&f) = 1137999872;
//			*reinterpret_cast <long long *> (&f) = 4670021707350671360i64;

			AddLogEntry (false, LogLevel_Critical, true, "%f", f);

/*			PriorityStack <unsigned char, unsigned char> s (IsLowerPriority);
			PriorityStack <unsigned char, unsigned char> s1 (IsLowerPriority);
//			Stack <unsigned char, unsigned char> s;

//			for (unsigned char i (1u); i <= 10u; ++i)
//				s += i;

			s += 100u;
			s += 78u;
			s += 99u;
			s += 120u;
			s += 101u;

			s1 = s;

			while (!s.IsEmpty ())
			{
				const unsigned char element (s.Pop ());

				AddLogEntry (false, LogLevel_Default, true, "s.Pop()=%u, elements left = %u", element, s.GetElementNumber ());
			}
/*			while (!s1.IsEmpty ())
			{
				const unsigned char element (s1.Pop ());

				AddLogEntry (false, LogLevel_Default, true, "s1.Pop()=%u, elements left = %u", element, s1.GetElementNumber ());
			}
*/
//			AddLogEntry (false, LogLevel_Critical, true, "DONE!");
/*			SDK::Classes::CSBotManager csbm;

			InternalAssert (reinterpret_cast <unsigned char *> (&csbm) + SDK::Classes::CSBotManager::Offset_MapType * sizeof (void *) == reinterpret_cast <unsigned char *> (&csbm.m_mapType));
			InternalAssert (reinterpret_cast <unsigned char *> (&csbm) + SDK::Classes::CSBotManager::Offset_MapZonesNumber * sizeof (void *) == reinterpret_cast <unsigned char *> (&csbm.m_mapZonesNumber));
			InternalAssert (reinterpret_cast <unsigned char *> (&csbm) + SDK::Classes::CSBotManager::Offset_LastEnemySpottedRadioCommandTime * sizeof (void *) == reinterpret_cast <unsigned char *> (&csbm.m_lastEnemySpottedRadioCommandTime));
//			AddLogEntry (false, LogLevel_Critical, true, "%u, %u diff == -8uc", reinterpret_cast <unsigned char *> (&csbm) + SDK::Classes::CSBotManager::Offset_LastEnemySpottedRadioCommandTime * sizeof (void *), reinterpret_cast <unsigned char *> (&csbm.m_lastEnemySpottedRadioCommandTime));

			InternalAssert (0);
*/
/*			SDK::Classes::NavigationArea area;

			AddLogEntry (false, LogLevel_Critical, true, "off = %u", reinterpret_cast <unsigned int *> (area.m_danger) - reinterpret_cast <unsigned int *> (&area));
			AddLogEntry (false, LogLevel_Critical, true, "off = %u", reinterpret_cast <unsigned int *> (area.m_dangerTimestamp) - reinterpret_cast <unsigned int *> (&area));

			InternalAssert (area.m_dangerTimestamp == reinterpret_cast <float *> (&area) + 18);
			InternalAssert (0);
*/
/*			DynamicString filename ("../../somedir/./blah.bsp");

			ReplaceFileExtension (filename, "NAV");

			AddLogEntry (false, LogLevel_Critical, true, "IsFilenameHasExtension()=%u, IsFilenameHasExtension(\"BSP\")=%u, IsFilenameHasExtension(\"NAV\")=%u, filename: \"%s\".", IsFilenameHasExtension (filename), IsFilenameHasExtension (filename, "BSP"), IsFilenameHasExtension (filename, "NAV"), filename.GetData ());
*/
/*			struct A
			{
				int i;
				float f[3];
				char *ps;
				char s[10];
			}a;

			InternalAssert (a.ps == reinterpret_cast <char **> (&a)[4u]);
			InternalAssert (a.s == reinterpret_cast <char *> (&a) + 20u);
			InternalAssert (a.f == reinterpret_cast <float *> (&a) + 1u);
			InternalAssert (a.f != &reinterpret_cast <Math::Vector3D *> (&a)[1u].x);
			InternalAssert (0);
*/
			// We should stop the attempt for loading the real game.DLL, since Meta mod handle this for us.
			if (MetaModEngine::Globals::g_metaModEngine != NULL)
				return;

			// ...and pass this 'modified' function table to the engine
			// and now we need to pass engine functions table to the game.DLL (in fact it's our own functions we are passing here, but the game.DLL won't notice)...
			GetEngineFunctions (engineFunctionTable, NULL);

			// give the engine functions to the other DLL...
			(*Globals::g_halfLifeEngine->GetGameLibrary ()->GetFunctionAddress <SDK::TypeDefinitions::FunctionPointer_t> ("GiveFnptrsToDll")) (engineFunctionTable, globalVariables);
		}

		EXPORT BOOL GetEntityAPI2 (SDK::Structures::DLLFunction_t *DLLFunctionTable, int *interfaceVersion)
		{
			// this function is called right after FunctionPointer_t() by the engine in the game.DLL (or
			// what it BELIEVES to be the game.DLL), in order to copy the list of MOD functions that can
			// be called by the engine, into a memory block pointed to by the 'DLLFunctionTable' pointer
			// that is passed into this function (explanation comes straight from botman). This allows
			// the Half-Life engine to call these MOD DLL functions when it needs to spawn an entity,
			// connect or disconnect a player, call Think() functions, Touch() functions, or Use()
			// functions, etc. The bot.DLL passes its OWN list of these functions back to the Half-Life
			// engine, and then calls the MOD DLL's version of GetEntityAPI() to get the REAL game.DLL
			// functions this time (to use in the bot code).

			if (g_YaPBNotLoaded)
				return FALSE;

			// pass game.DLL functions table to engine (in fact it's our own functions we are passing here, but the engine won't notice)...
			if (MetaModEngine::Globals::g_metaModEngine == NULL)
			{
				// pass other DLLs engine callbacks to function table...
				if ((*Globals::g_halfLifeEngine->GetGameLibrary ()->GetFunctionAddress <SDK::TypeDefinitions::EntityAPI_t> ("GetEntityAPI")) (DLLFunctionTable, *interfaceVersion) == FALSE)
				{
					AddLogEntry (false, LogLevel_Critical, false, "GetEntityAPI(): ERROR - Not Initialized.");

					// Error initializing function table!!!
					return FALSE;
				}

				// Copy the original function pointers....
				Globals::g_halfLifeEngine->m_gameDLLFunctionAPITablePointer = DLLFunctionTable;
				Globals::g_halfLifeEngine->m_gameDLLFunctionAPITable = *DLLFunctionTable;

				DLLFunctionTable->pfnGameInit              = ReplacedFunctions::DLLTable::GameDLLInit;
				DLLFunctionTable->pfnSpawn                 = ReplacedFunctions::DLLTable::Spawn;
/*				DLLFunctionTable->pfnTouch                 = ReplacedFunctions::DLLTable::Touch;
				DLLFunctionTable->pfnClientConnect         = ReplacedFunctions::DLLTable::ClientConnect;
				DLLFunctionTable->pfnClientDisconnect      = ReplacedFunctions::DLLTable::ClientDisconnect;
				DLLFunctionTable->pfnClientCommand         = ReplacedFunctions::DLLTable::ClientCommand;
				DLLFunctionTable->pfnClientUserInfoChanged = ReplacedFunctions::DLLTable::ClientUserInfoChanged;
*/				DLLFunctionTable->pfnServerActivate        = ReplacedFunctions::DLLTable::ServerActivate;
/*				DLLFunctionTable->pfnServerDeactivate      = ReplacedFunctions::DLLTable::ServerDeactivate;
				DLLFunctionTable->pfnStartFrame            = ReplacedFunctions::DLLTable::StartFrame;
*/				DLLFunctionTable->pfnPM_Move               = ReplacedFunctions::DLLTable::PM_Move;
				DLLFunctionTable->pfnPM_Init               = ReplacedFunctions::DLLTable::PM_Init;
//				DLLFunctionTable->pfnUpdateClientData      = ReplacedFunctions::DLLTable::UpdateClientData;
				DLLFunctionTable->pfnCmdStart              = ReplacedFunctions::DLLTable::CmdStart;
			}
			else
			{
				// Zero out the function table....
				memset (DLLFunctionTable, NULL, sizeof (*DLLFunctionTable));

				Globals::g_halfLifeEngine->GetEngineWithMetaModPointer ()->SetGameDLLPREFunctionAPITablePointer (DLLFunctionTable);

				DLLFunctionTable->pfnGameInit              = MetaModEngine::Export::ReplacedFunctions::DLLTable::GameDLLInit;
				DLLFunctionTable->pfnSpawn                 = MetaModEngine::Export::ReplacedFunctions::DLLTable::Spawn;
/*				DLLFunctionTable->pfnTouch                 = MetaModEngine::Export::ReplacedFunctions::DLLTable::Touch;
				DLLFunctionTable->pfnClientConnect         = MetaModEngine::Export::ReplacedFunctions::DLLTable::ClientConnect;
				DLLFunctionTable->pfnClientDisconnect      = MetaModEngine::Export::ReplacedFunctions::DLLTable::ClientDisconnect;
				DLLFunctionTable->pfnClientCommand         = MetaModEngine::Export::ReplacedFunctions::DLLTable::ClientCommand;
				DLLFunctionTable->pfnClientUserInfoChanged = MetaModEngine::Export::ReplacedFunctions::DLLTable::ClientUserInfoChanged;
*/				DLLFunctionTable->pfnServerActivate        = MetaModEngine::Export::ReplacedFunctions::DLLTable::ServerActivate;
/*				DLLFunctionTable->pfnServerDeactivate      = MetaModEngine::Export::ReplacedFunctions::DLLTable::ServerDeactivate;
				DLLFunctionTable->pfnStartFrame            = MetaModEngine::Export::ReplacedFunctions::DLLTable::StartFrame;
*/				DLLFunctionTable->pfnPM_Move               = MetaModEngine::Export::ReplacedFunctions::DLLTable::PM_Move;
				DLLFunctionTable->pfnPM_Init               = MetaModEngine::Export::ReplacedFunctions::DLLTable::PM_Init;
//				DLLFunctionTable->pfnUpdateClientData      = MetaModEngine::Export::ReplacedFunctions::DLLTable::UpdateClientData;
			}

			// finished, interfacing from engine to game.DLL complete
			return TRUE;
		}

		EXPORT BOOL GetEntityAPI2_Post (SDK::Structures::DLLFunction_t *DLLFunctionTable, int * /*interfaceVersion*/)
		{
			// this function is called right after FunctionPointer_t() by the engine in the game DLL (or
			// what it BELIEVES to be the game.DLL), in order to copy the list of MOD functions that can
			// be called by the engine, into a memory block pointed to by the 'DLLFunctionTable' pointer
			// that is passed into this function (explanation comes straight from botman). This allows
			// the Half-Life engine to call these MOD DLL functions when it needs to spawn an entity,
			// connect or disconnect a player, call Think() functions, Touch() functions, or Use()
			// functions, etc. The bot DLL passes its OWN list of these functions back to the Half-Life
			// engine, and then calls the MOD DLL's version of GetEntityAPI to get the REAL game.DLL
			// functions this time (to use in the bot code). Post version, called only by Meta mod.

			if (g_YaPBNotLoaded)
				return FALSE;

			// Zero out the function table....
			memset (DLLFunctionTable, NULL, sizeof (*DLLFunctionTable));

			// pass game.DLL functions table to engine (in fact it's our own functions we are passing here, but the engine won't notice)...
			Globals::g_halfLifeEngine->GetEngineWithMetaModPointer ()->SetGameDLLPOSTFunctionAPITablePointer (DLLFunctionTable);

			DLLFunctionTable->pfnSpawn          = MetaModEngine::Export::ReplacedFunctions::DLLTable::Post::Spawn;
			DLLFunctionTable->pfnServerActivate = MetaModEngine::Export::ReplacedFunctions::DLLTable::Post::ServerActivate;
			DLLFunctionTable->pfnCmdStart       = MetaModEngine::Export::ReplacedFunctions::DLLTable::Post::CmdStart;

			// finished, interfacing from engine to game.DLL complete
			return TRUE;
		}
/*
		// New DLL API:
		static void GameShutdown (void)
		{
//			AddLogEntry (true, LogLevel_Default, false, "GameShutdown(): Called!");
			MessageBox (NULL, "GameShutdown(): Called!", "YaPB", MB_ICONASTERISK | MB_TOPMOST);

			if (MetaModEngine::Globals::g_metaModEngine != NULL)
				MetaModEngine::Globals::g_metaModEngine->m_globalVariables->returnFlag = MetaModEngine::SDK::Constants::MRES_IGNORED;
//				(*Globals::g_halfLifeEngine->m_newGameDLLFunctionAPITable.pfnGameShutdown) ();
		}
*/
		EXPORT BOOL GetNewDLLFunctions (SDK::Structures::NewDLLFunction_t *newDLLFunctionTable, int *interfaceVersion)
		{
			// it appears that an extra function table has been added in the engine to game.DLL interface
			// since the date where the first enginefuncs table standard was frozen. These ones are
			// facultative and we don't hook them, but since some MODs might be featuring it, we have to
			// pass them too, else the DLL interfacing wouldn't be complete and the game possibly wouldn't
			// run properly.

			if (g_YaPBNotLoaded)
				return FALSE;

			// pass game.DLL functions table to engine (in fact it's our own functions we are passing here, but the engine won't notice)...
			if (MetaModEngine::Globals::g_metaModEngine == NULL)
			{
				// else call the function that provides the new DLL functions interface on request
				if ((*Globals::g_halfLifeEngine->GetGameLibrary ()->GetFunctionAddress <SDK::TypeDefinitions::NewEntityAPI_t> ("GetNewDLLFunctions")) (newDLLFunctionTable, interfaceVersion) == FALSE)
				{
					AddLogEntry (false, LogLevel_Critical, false, "GetNewDLLFunctions(): ERROR - Not Initialized.");

					return FALSE;
				}

				// Copy the original function pointers....
				Globals::g_halfLifeEngine->m_newGameDLLFunctionAPITablePointer = newDLLFunctionTable;
				Globals::g_halfLifeEngine->m_newGameDLLFunctionAPITable = *newDLLFunctionTable;

//				newDLLFunctionTable->pfnGameShutdown = &GameShutdown;
			}
			else
			{
				// Zero out the function table....
				memset (newDLLFunctionTable, NULL, sizeof (*newDLLFunctionTable));

				Globals::g_halfLifeEngine->GetEngineWithMetaModPointer ()->SetGameDLLPREFunctionNewAPITablePointer (newDLLFunctionTable);

//				newDLLFunctionTable->pfnGameShutdown = &GameShutdown;
			}

			return TRUE;
		}
/*
		static void SV_StudioSetupBones (SDK::Structures::Model_t *const model, const float frame, const int sequence, const Math::Angles3D &angles, const Math::Vector3D &origin, const unsigned char *const controller, const unsigned char *const blending, const int boneID, const SDK::Classes::Edict *const edict)
		{
			// Call the original function....
			Globals::g_halfLifeEngine->GetBlendingInterface ().SV_StudioSetupBones
			(
				model,
				frame,
				sequence,
				angles,
				origin,
				controller,
				blending,
				boneID,
				edict
			);

			// POST:
			Globals::g_halfLifeEngine->ServerPrintFormat ("SV_StudioSetupBones(): Called for \"%s\", boneID=%i, origin: [%.2f, %.2f, %.2f].\n", edict->IsValid () ? edict->IsNotWorldspawnPlayer () ? edict->GetNetName ().GetData () : edict->GetClassName ().GetData () : "NULL", boneID, origin.x, origin.y, origin.z);

			// Reliability check.
			InternalAssert (g_server->IsRunning ());

//			g_server->GetClientManager ()->ClientStudioSetupBones (edict, model, frame, sequence, angles, origin, controller, blending, boneID);
		}
*/
		EXPORT BOOL Server_GetBlendingInterface (int version, SDK::Structures::ServerBlendingInterface_t **blendInterface, SDK::Structures::ServerStudioAPI_t *studio, SDK::TypeDefinitions::RotationMatrix_t *const rotationMatrix, SDK::TypeDefinitions::BoneTransformMatrix_t *const boneTransform)
		{
			// this function synchronizes the studio model animation blending interface (i.e, what parts
			// of the body move, which bones, which hitboxes and how) between the server and the game.DLL.
			// Some MODs can be using a different hitbox scheme than the standard one.

			// Another GET-API routine, another interface version.
			//
			// Added 5/2003 based on information from Leon Hartwig and Alfred Reynolds,
			// in order to address observed problems with hitboxes not matching between
			// client and server, in CS and DOD.
			//
			// Based around code apparently added to the Engine around 1/2002,
			// seemingly to address "the old CS hitbox problem", where "the client was
			// doing custom player model blending and the server had no idea what was going on".
			//
			// http://www.mail-archive.com/hlcoders@list.valvesoftware.com/msg01224.html
			// http://www.mail-archive.com/hlcoders@list.valvesoftware.com/msg02724.html

			// Note that we're not checking "if (version == SV_BLENDING_INTERFACE_VERSION)"
			// because at this point, we don't really care, as we're not looking at
			// or using the contents of the function tables; we're only passing them
			// through to the gamedll, which presumably will check for version match,
			// since it's the one that cares and actually uses the function tables.
			//
			// Return(FALSE) if the gamedll does not provide this routine, and the
			// Engine will use its own builtin blending. The Engine will report
			// "Couldn't get server .dll studio model blending interface. Version mismatch?\n",
			// but this will only show in "developer" (-dev) mode.

			/// @todo MAKE THIS FUNCTION CALLED UNDER METAMOD!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

			if (g_YaPBNotLoaded)
				return FALSE;

//			MessageBox (NULL, "Server_GetBlendingInterface(): Called!", "YaPB", MB_ICONASTERISK | MB_TOPMOST);

			// call the function that provides the blending interface on request
			const BOOL result ((*Globals::g_halfLifeEngine->GetGameLibrary ()->GetFunctionAddress <SDK::TypeDefinitions::BlendAPI_t> ("Server_GetBlendingInterface")) (version, blendInterface, studio, rotationMatrix, boneTransform));

			Globals::g_halfLifeEngine->m_blendInterface = **blendInterface;

//			(*blendInterface)->SV_StudioSetupBones = &SV_StudioSetupBones;	// Replace the original function pointer with our after we got copy of original game DLL blending interface....

			// Copy in engine helper functions....
			Globals::g_halfLifeEngine->m_engineStudioFunctionTable = *studio;

//			Globals::g_halfLifeEngine->m_rotationMatrix = rotationMatrix;
			Globals::g_halfLifeEngine->m_boneTransformMatrix = boneTransform;

			return result;
		}

		// Required DLL entry point.
		EXPORT inline BOOL WINAPI DllMain (HINSTANCE module, unsigned int reason, void * /* reserved */)
		{
			// Dynamic library entry point, can be used for uninitialization stuff. NOT for initializing
			// anything because if you ever attempt to wander outside the scope of this function on a
			// DLL attach, LoadLibrary() will simply fail. And you can't do I/Os here either. Nice eh?

			switch (reason)
			{
				// Dynamic library ataching?
				case DLL_PROCESS_ATTACH:
					g_YaPBModuleHandle = module;	// Hang on to this DLL's instance handle.

					// Disable thread notification. (Disable this function calls with 'reason == (DLL_THREAD_ATTACH || DLL_THREAD_DETACH)')
					/// @note MSDN: Do not call this function from a DLL that is linked to the static C run-time library (CRT). The static CRT requires DLL_THREAD_ATTACH and DLL_THREAD_DETATCH notifications to function properly.
					DisableThreadLibraryCalls (module);

					break;

				// Dynamic library detaching?
				case DLL_PROCESS_DETACH:
					// Not good solution, MSDN library says we should not call free library here
					// Maybe in 'pfnGameShutdown' in NewDLLAPI function, but I dunno....

					// Free everything that's freeable upon DLL detaching.
					FreeAllMemory ();

//					MessageBox (NULL, "DllMain() called with reason == DLL_PROCESS_DETACH. (POST FreeAllMemory() call)", "YaPB Critical Error", MB_ICONASTERISK | MB_TOPMOST);

					break;
			}

			// The return data type is OS specific too.
			return TRUE;
		}
	}
}