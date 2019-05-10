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
// Definitions.hpp
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef CORE_TOOL_LIBRARY_DEFINITIONS_INCLUDED
#define CORE_TOOL_LIBRARY_DEFINITIONS_INCLUDED

// Makes these more explicit, and easier to find.
#define FILE_GLOBAL static
#define CLASS_GLOBAL
#define NAMESPACE_GLOBAL
#define DLL_GLOBAL

#define NULL_POINTER nullptr

#define BIT(x) (1u << (x))
#define BIT64(x) (static_cast <unsigned long long> (1u) << (x))

#define EXPAND_CALL(function, arguments) function arguments
#define CHARIZE(character) #@character	// "a = CHARIZE(b);" Equal to: "a = 'b';"
#define CONCATENATE(argument1, argument2) argument1##argument2	// Split two arguments.
#define STRINGIZE(text) #text	// "ptr = STRINGIZE(some text);" Equal to: "ptr = "some text";"
#define STRINGIZE_EXPAND(text) EXPAND_CALL (STRINGIZE, (text))

#define DECLARE_TYPED_ENUMERATION(elementType, name)	\
	typedef elementType name;							\
														\
	enum												\

#define IS_IDENT_STRINGS(s1, s2) (*((void **) &(s1)) == *((void **) &(s2)))
#define GET_STATIC_STRING_LENGTH(staticString) (sizeof (staticString) - sizeof ('\0'))

//check for invalid handle values
#define IsValidHANDLE(handle) ((handle) != NULL && (handle) != INVALID_HANDLE_VALUE)

//relative virtual address to virtual address
#define RelativeVirtualAddressToVirtualAddress(base, rva) ((const unsigned long) (base) + (const unsigned long) (rva))
// virtual address to relative virtual address
#define VirtualAddressToRelativeVirtualAddress(base, va) ((const unsigned long) (va) - (const unsigned long) (base))

// Gets the offset of a variable in a structure.
#define GET_OFFSET_OF_MEMBER_VARIABLE(structureName, memberName) reinterpret_cast <const size_t> (&static_cast <const structureName *> (NULL)->memberName)

// Gets the size of a variable in a structure.
#define GET_SIZE_OF_MEMBER_VARIABLE(structureName, memberName) sizeof (static_cast <const structureName *> (NULL)->memberName)

// Used to step into the debugger
#define DEBUGGER_BREAK()  __asm { int 3 }

// Force a function call site -not- to inlined. (useful for profiling)
#define DONT_INLINE(a) (((int)(a) + 1) ? (a) : (a))

// Pass hints to the compiler to prevent it from generating unnessecary / stupid code
// in certain situations. Several compilers other than MSVC also have an equivilent construct.
//
// Essentially the 'Hint' is that the condition specified is assumed to be true at
// that point in the compilation. If '0' is passed, then the compiler assumes that
// any subsequent code in the same 'basic block' is unreachable, and thus usually removed.
#define HINT(THE_HINT) __assume ((THE_HINT))

// Marks the codepath from here until the next branch entry point as unreachable,
// and asserts if any attempt is made to execute it.
#define UNREACHABLE() { InternalAssert (false); HINT (0); }

// In cases where no default is present or appropriate, this causes MSVC to generate as little code as possible, and throw an assertion in debug.
#define NO_DEFAULT default: UNREACHABLE ();

#define DECLARE_CLASS_WITHOUT_BASE(thisClassName) typedef thisClassName ThisClass
#define DECLARE_CLASS_WITH_BASE(baseClassName, thisClassName)	\
	typedef baseClassName BaseClass;							\
	DECLARE_CLASS_WITHOUT_BASE (thisClassName)

#define COLLECT_AND_STORE_ARGUMENTS_INTO_ONE_STRING(format, string, maximumLength)	\
{																					\
	va_list argumentPointer;														\
																					\
	/* Concatenate all the arguments in one string.... */							\
	va_start (argumentPointer, format);												\
		vsnprintf (string, maximumLength, format, argumentPointer);					\
	va_end (argumentPointer);														\
}

static NAMESPACE_GLOBAL const unsigned int BadPosition = static_cast <unsigned int> (-1);	// Generic bad/missing length/position.

enum GlobalConstant_t
{
	MximumCharactersNumber = static_cast <unsigned char> (-1) + 1u,

	GlobalConstant_MaximumBufferLength = 1024u	// For various character string buffers.
};

// WAVE file related structure definitions
struct WaveFileHeader_t
{
	struct WaveFormat_t
	{
/*! Off=0(0)/20(5) */	unsigned short tag;					// format type of the sound data (1 - simple, 0x101 - IBM mu-law, 0x102 - IBM a-law, 0x103 - ADPCM)
/*! Off=2(0.5)/22(5.5) */	unsigned short channels;			// number of channels (0x01 - mono, 0x02 - stereo)
/*! Off=4(1)/24(6) */	unsigned int   sampleRate;			// sample rate in hertz (num samples per second (e.g. 11025 - 11 khz))
/*! Off=8(2)/28(7) */	unsigned int   bytesPerSecond;		// bytes of data read per second (bps) when playing the file
/*! Off=12(3)/32(8) */	unsigned short bytesPerSample;		// bytes per sample (including all channels)
/*! Off=14(3.5)/34(8.5) */	unsigned short bitsPerSample;	// bits per sample, bytesPerSample * 8
	};	// sizeof (WaveFormat_t) == 16 (4)

	// RIFF chunk
/*! Off=0(0) */	char           riffChunkID[GET_STATIC_STRING_LENGTH ("RIFF")];		// RIFF chunk ID ("RIFF")
/*! Off=4(1) */	unsigned int   packageSize;											// total length of package (all following chunks) in bytes

	// WAVE chunk
/*! Off=8(2) */	char           waveChunkID[GET_STATIC_STRING_LENGTH ("WAVE")];		// WAVE chunk ID ("WAVE")
/*! Off=12(3) */	char           formatChunkID[GET_STATIC_STRING_LENGTH ("fmt ")];	// FORMAT chunk ID ("fmt ")
/*! Off=16(4) */	unsigned int   formatChunkLength;									// length of FORMAT chunk in bytes

	// WAVE ID
/*! Off=20(5) */	WaveFormat_t   format;

	// DATA chunk
/*! Off=36(9) */	char           dataChunkID[GET_STATIC_STRING_LENGTH ("data")];		// DATA chunk ID ("data")
/*! Off=40(10) */	unsigned int   dataChunkLength;										// length of DATA chunk in bytes
};	// sizeof (WaveFileHeader_t) == 44 (11)

//---------------------------------------------------------------------------
// Use 'NoCopy' as a base class to easily prevent copy init & assign for any class.
class NoCopy
{
	public:
		inline NoCopy (void) { /* VOID */ }

	private:
		inline NoCopy (const NoCopy &/*other*/);

	private:
		inline NoCopy &operator = (const NoCopy &/*right*/);	// Avoid "warning C4512: 'NoCopy' : assignment operator could not be generated".
};

// This can be used to declare an abstract (interface only) class.
// Classes marked abstract should not be instantiated. If they are, and access violation will occur.
//
// Example of use:
//
// abstract_class CFoo
// {
//  	...
// }
//
// MSDN __declspec(novtable) documentation: http://msdn.microsoft.com/library/default.asp?url=/library/en-us/vclang/html/_langref_novtable.asp
//
#define abstract_class class

#define GET_STATIC_ARRAY_SIZE(array) (sizeof (array) / sizeof (array[0u]))

#define STORE_FLAGS_TO_STRING(flags, string, startFlagType, maximumFlagsNumber, flagStrings, separatingSymbol)	\
	bool isFlagWasAdded (false);																				\
	unsigned char flagType (startFlagType);																		\
																												\
	if (flags & BIT (flagType))																					\
		goto Label_AssignFlagAndCheckWhenStringIsntEmpty;														\
	else for (++flagType; flagType < maximumFlagsNumber; ++flagType)											\
	{																											\
		if (!(flags & BIT (flagType)))																			\
			continue;																							\
																												\
		if (isFlagWasAdded)																						\
		{																										\
			string += " " separatingSymbol " ";																	\
																												\
			{																									\
				Label_AssignFlagAndCheckWhenStringIsntEmpty:													\
				{																								\
					string += flagStrings[flagType];															\
																												\
					for (++flagType; flagType < maximumFlagsNumber; ++flagType)									\
						if (flags & BIT (flagType))																\
						{																						\
							string += " " separatingSymbol " ";													\
							string += flagStrings[flagType];													\
						}																						\
				}																								\
			}																									\
																												\
			break;																								\
		}																										\
																												\
		string += flagStrings[flagType];																		\
		isFlagWasAdded = true;																					\
	}																											\

//
// Title: Utility Classes Header
//
//
// Function: Hash
//
// Description: Hash template for <Map>, <HashTable>.
//
template <typename elementType> inline unsigned int Hash (const elementType &);

//
// Function: Hash
//
// Description: Hash for unsigned integer.
//
// Parameters:
//	tag - Value that should be hashed.
//
// Returns: Hashed value.
//
template </* unsigned int */> inline unsigned int Hash (const unsigned int &tag)
{
	unsigned int key (tag);

	key += ~(key << 16u);
	key ^= (key >> 5u);

	key += (key << 3u);
	key ^= (key >> 13u);

	key += ~(key << 9u);
	key ^= (key >> 17u);

	return key;
}

//! creates four CC codes used in YaPB for simple ids
/// some compilers can create those by directly writing the code like 'code', but some generate warnings so we use this macro here
// Makes a 4-byte "packed ID" int out of 4 characters
inline const unsigned int MakeID (const unsigned char c0, const unsigned char c1, const unsigned char c2, const unsigned char c3)
{
	return static_cast <unsigned int> (c0)       |
		(static_cast <unsigned int> (c1) << 8u)  |
		(static_cast <unsigned int> (c2) << 16u) |
		(static_cast <unsigned int> (c3) << 24u);
}
inline const unsigned int MakeID (const unsigned char c0, const unsigned char c1, const unsigned char c2)
{
	return MakeID (c0, c1, c2, 0u);
}
inline const unsigned short MakeID (const unsigned char c0, const unsigned char c1)
{
	return static_cast <const unsigned short> (MakeID (c0, c1, 0u));
}

inline const char GetLowerCharacter (const char character) { return character >= 'A' && character <= 'Z' ? character - 'A' + 'a' : character; }
inline const char GetUpperCharacter (const char character) { return character >= 'a' && character <= 'z' ? character - 'a' + 'A' : character; }
inline const bool IsAlphaCharacter  (const char character) { return /*isalpha (character) == TRUE - @warning NOT WORKS PROPERLY!!!/*/('A' <= character && character <= 'Z') || ('a' <= character && character <= 'z'); }
inline const bool IsDigitCharacter  (const char character) { return /*isdigit (character) == TRUE/*/'0' <= character && character <= '9'; }

inline const bool IsWhiteSpaceCharacter (const char character)
{
	switch (character)
	{
		case ' ':
		case '\t':
		case '\n':
		case '\r':
			return true;

		default:
			return false;
	}
}

//
// Function: IsTrimmingCharacter
//
// Description: Checks whether input is trimming character.
//
// Parameters:
//	character - Input character to check for.
//
// Returns: True if it's a trim character, false otherwise.
//
inline const bool IsTrimmingCharacter (const char character)
{
	if (IsWhiteSpaceCharacter (character))
		return true;

	switch (character)
	{
		case '\v':
		case '\f':
			return true;

		default:
			return false;
	}
}

inline const char *const FormatInputBuffer (char *const buffer, const unsigned int maximumLength, const char *const format, ...)
{
	COLLECT_AND_STORE_ARGUMENTS_INTO_ONE_STRING (format, buffer, maximumLength);

	return buffer;
}
inline const char *const FormatBuffer (const char *const format, ...)
{
	static char string[4096u];

	COLLECT_AND_STORE_ARGUMENTS_INTO_ONE_STRING (format, string, sizeof (string));

	return string;
}

//
// Type: StringVector
//
// Description: Array of strings.
//
//typedef DynamicArray <const DynamicString &> StringVector;

//
// Macro: ThrowException
//
// Description: Throws debug exception.
//
// Parameters:
//	text - Text of error.
//
#define ThrowException(text) throw Exception (text, __FILE__, __LINE__)

#if defined _DEBUG
// Macro to assist in asserting constant invariants during compilation
#define CompileTimeAssert(expression) static_assert (expression, #expression)

#	define InternalAssert(expression)																																						\
	{																																														\
		if ((expression) == false)																																							\
		{																																													\
			/*DEBUGGER_BREAK ();*/																																								\
																																															\
			AddLogEntry (false, LogLevel_Critical, true, "Assertion Fail!\n\tExpression: %s,\n\tFile: '%s',\n\tline: %u,\n\tfunction: %s.", #expression, __FILE__, __LINE__, __FUNCTION__);	\
		}																																													\
	}

struct FunctionInformation_t
{
	char               filename[32u];
	char               name[32u];
	const unsigned int line;

	inline FunctionInformation_t (const char *const inputFilename, const char *const inputName, const unsigned int inputLine) : line (inputLine)
	{
		strncpy (filename, inputFilename, sizeof (filename));
		strncpy (name, inputName, sizeof (name));
	}

	//
	// Group: Private operators.
	//
	private:
		inline FunctionInformation_t &operator = (const FunctionInformation_t &/*right*/);	// Avoid "warning C4512: 'FunctionInformation_t' : assignment operator could not be generated".
};
struct FunctionCall_t
{
	inline          FunctionCall_t (void) { /* Empty */ }
	virtual inline ~FunctionCall_t (void) { /* Empty */ }

	//
	// Group: Private operators.
	//
	private:
		inline FunctionCall_t &operator = (const FunctionCall_t &/*right*/);	// Avoid "warning C4512: 'FunctionCall_t' : assignment operator could not be generated".
};
struct FunctionCallTrace_t : public FunctionCall_t
{
	private:
		const FunctionInformation_t &m_functionInformation;

	public:
		inline          FunctionCallTrace_t (const FunctionInformation_t &functionInformation) : FunctionCall_t (), m_functionInformation (functionInformation) { AddLogEntry (true, LogLevel_Default, true, "Tracing STARTED!\n\tFile: '%s',\n\tfunction: %s,\n\tline: %i.", functionInformation.filename, functionInformation.name, functionInformation.line); }
		virtual inline ~FunctionCallTrace_t (void) { AddLogEntry (true, LogLevel_Default, true, "Tracing ENDED!\n\tFile: '%s',\n\tfunction: %s,\n\tline: %i.", m_functionInformation.filename, m_functionInformation.name, m_functionInformation.line); }
};
struct FunctionProfileTrace_t : public FunctionCall_t
{
	private:
		const FunctionInformation_t &m_functionInformation;

	public:
		inline          FunctionProfileTrace_t (const FunctionInformation_t &functionInformation) : FunctionCall_t (), m_functionInformation (functionInformation) { AddLogEntry (true, LogLevel_Default, true, "Tracing STARTED!\n\tFile: '%s',\n\tfunction: %s,\n\tline: %i,\n\n\tTime: %.2f.", functionInformation.filename, functionInformation.name, functionInformation.line, 0.0f/*! todo !*/); }
		virtual inline ~FunctionProfileTrace_t (void) { AddLogEntry (true, LogLevel_Default, true, "Tracing ENDED!\n\tFile: '%s',\n\tfunction: %s,\n\tline: %i,\n\n\tTime: %.2f.", m_functionInformation.filename, m_functionInformation.name, m_functionInformation.line, 0.0f/*! todo !*/); }
};

#	define DEBUG_FUNCTION_TRACING()   FunctionCallTrace_t functionCallTrace (FunctionInformation_t (__FILE__, __FUNCTION__, __LINE__));
#	define DEBUG_FUNCTION_PROFILING() FunctionProfileTrace_t functionProfileTrace (FunctionInformation_t (__FILE__, __FUNCTION__, __LINE__));

#	define DEBUG_FUNCTION_TRACING_START() AddLogEntry (true, LogLevel_Default, true, "Tracing STARTED!\n\tFile: '%s',\n\tfunction: %s,\n\tline: %i.", __FILE__, __FUNCTION__, __LINE__);
#	define DEBUG_FUNCTION_TRACING_END()   AddLogEntry (true, LogLevel_Default, true, "Tracing ENDED!\n\tFile: '%s',\n\tfunction: %s,\n\tline: %i.", __FILE__, __FUNCTION__, __LINE__);
#else	// Wothout debug mode....
#	define CompileTimeAssert(expression) /* Empty */
#	define InternalAssert(expression)    { /* Empty */ }

#	define DEBUG_FUNCTION_TRACING()   /* Empty */
#	define DEBUG_FUNCTION_PROFILING() /* Empty */

#	define DEBUG_FUNCTION_TRACING_START() /* Empty */
#	define DEBUG_FUNCTION_TRACING_END()   /* Empty */
#endif	// if defined _DEBUG

template <typename virtualTable = const void *> class BaseHookedClassWithVirtualFunctionTable
{
	/// @warning This and derived classes can't have virtual functions!

	//
	// Group: Protected members.
	//
	protected:
		virtualTable *const m_pseudoVirtualFunctionTable;	// Array of virtual function pointers....

	//
	// Group: Protected (con/de)structors.
	//
	protected:
		inline BaseHookedClassWithVirtualFunctionTable (void);

	//
	// Group: Private operators.
	//
	private:
		inline BaseHookedClassWithVirtualFunctionTable &operator = (const BaseHookedClassWithVirtualFunctionTable &/*right*/);

	//
	// Group: Functions.
	//
	public:
		inline       virtualTable *const &GetPseudoVirtualFunctionTable (void)       { return m_pseudoVirtualFunctionTable; }
		inline const virtualTable *const  GetPseudoVirtualFunctionTable (void) const { return m_pseudoVirtualFunctionTable; }

		#define DECLARE_VIRTUAL_FUNCTION_CALL_1(functionName, returnType, argumentsDefinitions, argumentsCalls)	\
			inline returnType Call##functionName argumentsDefinitions { return (this->*reinterpret_cast <const Function##functionName##_t &> (GetPseudoVirtualFunctionTable ()[VirtualTableFunctionID_##functionName])) argumentsCalls; }
		#define DECLARE_VIRTUAL_FUNCTION_CALL_2(functionName, returnType, argumentsDefinitions, argumentsCalls)	\
			inline returnType Call##functionName argumentsDefinitions { return (this->*GetPseudoVirtualFunctionTable ()->functionName) argumentsCalls; }

	/**
	* Example using:
	* Declaration:
	* Method #1:
	*@{
		class Interface_SomeHookedInterface : public VoidBaseHookedClassWithVirtualFunctionTable
		{
			private:
				enum VirtualTableFunctionID_t
				{
					// ....

					VirtualTableFunctionID_SomeFunction = 1u	// 1 is a SomeFunction() virtual table index.

					// ....
				};
				typedef void (Interface_SomeHookedInterface::*FunctionSomeFunction_t) (bool someBool, int someInt) const;
				// Or:
				typedef void (__stdcall *FunctionSomeFunction_t) (Interface_SomeHookedInterface *const thisPointer, bool someBool, int someInt) const;
				// Or:
				typedef void (__fastcall *FunctionSomeFunction_t) (Interface_SomeHookedInterface *const thisPointer, int /* dummy - DO NOT USE THIS ARGUMENT! * /, bool someBool, int someInt) const;

			public:
				DECLARE_VIRTUAL_FUNCTION_CALL_1 (SomeFunction, void, (bool someBool, int someInt) const, (someBool, someInt));
		};
	*@}
	* OR method #2:
	*@{
		class Interface_SomeHookedInterface : public BaseHookedClassWithVirtualFunctionTable <Interface_SomeHookedInterface>
		{
			private:
				void (Interface_SomeHookedInterface::*UNKNOWN_FUNCTION_0) (void);	// Declared as first one function (virtual table index = 0) to keep SomeFunction() places #1 index in a virtual table.
				void (Interface_SomeHookedInterface::*SomeFunction)       (bool someBool, int someInt) const;

			public:
				DECLARE_VIRTUAL_FUNCTION_CALL_2 (SomeFunction, void, (bool someBool, int someInt) const, (someBool, someInt));
		};
	*@}
	* Call example:
	*@{
		Interface_SomeHookedInterface *const g_someHookedInterface (*reinterpret_cast <Interface_SomeHookedInterface **> ((reinterpret_cast <unsigned int> (GetModuleHandle (hw.DLL)) + 0xSOMEADDRESS));

		g_someHookedInterface->CallSomeFunction (true, 11);
	*@}
	**/
};
typedef BaseHookedClassWithVirtualFunctionTable <> VoidBaseHookedClassWithVirtualFunctionTable;

inline const bool IsNullString (const char *const bufferPointer)
{
	// This function testings input buffer pointer string for nullity.

	return bufferPointer == NULL;
}
inline const bool IsEmptyString (const char *const bufferPointer)
{
	// This function testings input buffer pointer string for emptiness.

	// Reliability check.
	InternalAssert (!IsNullString (bufferPointer));

	return bufferPointer[0u] == '\0';
}
inline const bool IsNullOrEmptyString (const char *const bufferPointer)
{
	// This function testings input buffer pointer string for nullity or emptiness.

	return IsNullString (bufferPointer) || IsEmptyString (bufferPointer);
}

// Macro to handle memory allocation fails.
#define TerminateOnMalloc() AddLogEntry (false, LogLevel_Critical, true, "Memory allocation fail!\n\tFile: '%s',\n\tfunction: %s,\n\tline: %i.", __FILE__, __FUNCTION__, __LINE__)
/*
inline const unsigned short FormatBuffer (char *const bufferPointer, unsigned short maximumLength, const char *const format, ...)
{
	// Reliability check.
	InternalAssert (maximumLength > 0u);

	va_list argumentPointer;

	// Concatenate all the arguments in one string....
	va_start (argumentPointer, format);
	const unsigned short length (static_cast <unsigned short> (vsnprintf (bufferPointer, maximumLength, format, argumentPointer)));
	va_end (argumentPointer);

	if (length < maximumLength)
		return length;

	bufferPointer[--maximumLength] = '\0';

	return maximumLength;
}
*/
//
// Function: CharactersCompareWithCaseCheck
//
// Description: Compares two characters, returning less than, equal to, or greater than.
//
// Parameters:
//	source - Character for left-hand side of comparison.
//	destination - Character for right-hand side of comparison.
//
// Returns: -1 if source < destination, 0 if source == destination, +1 if source > destination.
//
inline const char CharactersCompareWithCaseCheck (const char source, const char destination)
{
	const short returnValue (static_cast <unsigned char> (source) - static_cast <unsigned char> (destination));

	return returnValue < 0 ? -1 : returnValue > 0 ? 1 : 0;
}

//
// Function: CharactersCompareWithoutCaseCheck
//
// Description: Compare characters, ignore case.
//
// Parameters:
//	source - Character for left-hand side of comparison.
//	destination - Character for right-hand side of comparison.
//
// Returns: < 0 if source > destination, 0 if source == destination, > 0 if source < destination.
//
inline const short CharactersCompareWithoutCaseCheck (const char source, const char destination)
{
	short f (static_cast <unsigned char> (destination)), l (static_cast <unsigned char> (source));

	if (f >= 'A' && f <= 'Z')
		f -= 'A' - 'a';

	if (l >= 'A' && l <= 'Z')
		l -= 'A' - 'a';

	return f - l;
}

template <typename elementType, typename indexType> inline const bool MemoryIsEqual (const elementType *left, const elementType *right, const indexType count)
{
	// This template function exchanges values stored at 'left' and 'right'.

	// Reliability checks.
	InternalAssert (count > 0u);
	InternalAssert (left != NULL);
	InternalAssert (right != NULL);
	InternalAssert (left != right);	// different, worth copying

	if (*left != *right)
		return false;

	const elementType *const leftEnd (left + count);

	// Reliability check.
	InternalAssert (leftEnd != NULL);

	for (/* Empty */; /* Empty */; /* Empty */)
	{
		if (++left == leftEnd)
			return true;

		if (*left != *++right)
			return false;
	}
}
template <typename elementType, typename indexType> inline const bool MemoryIsLess (const elementType *left, const elementType *right, const indexType count)
{
	// This template function exchanges values stored at 'left' and 'right'.

	// Reliability checks.
	InternalAssert (count > 0u);
	InternalAssert (left != NULL);
	InternalAssert (right != NULL);
	InternalAssert (left != right);	// different, worth copying

	if (*left >= *right)
		return false;

	const elementType *const leftEnd (left + count);

	// Reliability check.
	InternalAssert (leftEnd != NULL);

	for (/* Empty */; /* Empty */; /* Empty */)
	{
		if (++left == leftEnd)
			return true;

		if (*left >= *++right)
			return false;
	}
}

template <typename elementType, typename indexType> inline void MemoryCopy (const elementType *source, elementType *destination, const indexType count)
{
	// This template function exchanges values stored at 'source' and 'destination'.

	// Reliability checks.
	InternalAssert (count > 0u);
	InternalAssert (source != NULL);
	InternalAssert (destination != NULL);
	InternalAssert (source != destination);	// different, worth copying

	const elementType *const sourceEnd (source + count);

	// Reliability check.
	InternalAssert (sourceEnd != NULL);

	for (/* Empty */; /* Empty */; /* Empty */)
	{
		*destination = *source;

		if (++source == sourceEnd)
			break;

		++destination;
	}
}
template <typename elementType, typename indexType> inline void MemoryReverseCopy (const elementType *source, elementType *destination, const indexType count)
{
	// This template function exchanges values stored at 'source' and 'destination'.

	// Reliability checks.
	InternalAssert (count > 0u);
	InternalAssert (source != NULL);
	InternalAssert (destination != NULL);
	InternalAssert (source != destination);	// different, worth copying

	const elementType *const sourceEnd (source - count);

	// Reliability check.
	InternalAssert (sourceEnd != NULL);

	for (/* Empty */; /* Empty */; /* Empty */)
	{
		*destination = *source;

		if (--source == sourceEnd)
			break;

		--destination;
	}
}

template <typename elementType, typename indexType> inline void MemoryFill (elementType *source, const elementType &value, const indexType count)
{
	// This template function exchanges values stored at 'source' and 'destination'.

	// Reliability checks.
	InternalAssert (count > 0u);
	InternalAssert (source != NULL);

	const elementType *const sourceEnd (source + count);

	// Reliability check.
	InternalAssert (sourceEnd != NULL);

	do
	{
		*source = value;
	} while (++source != sourceEnd);
}

template <typename elementType> inline void SwapElements (elementType &left, elementType &right)
{
	// This template function exchanges values stored at 'left' and 'right'.

	// different, worth swapping
	InternalAssert (&left != &right);

	elementType tempLeft (left);

	// swap....
	left = right;
	right = tempLeft;
}
template <typename elementType, typename indexType> inline void SwapBuffers (elementType *left, elementType *right, const indexType count)
{
	// This template function exchanges values stored at 'left' and 'right'.

	// Reliability checks.
	InternalAssert (count > 0u);
	InternalAssert (left != NULL);
	InternalAssert (right != NULL);
	InternalAssert (left != right);	// different, worth swapping

	const elementType *const leftEnd (left + count);

	// Reliability check.
	InternalAssert (leftEnd != NULL);

	for (/* Empty */; /* Empty */; /* Empty */)
	{
		SwapElements (*left, *right);

		if (++left == leftEnd)
			break;

		++right;
	}
}

/***
*GetStringLength - return the length of a null-terminated string
*
*Purpose:
*       Finds the length in bytes of the given string, not including the final null character.
*
*Entry:
*       const char *const string - string whose length is to be computed
*
*Exit:
*       length of the string "string", exclusive of the final null byte
*
*Exceptions:
*
*******************************************************************************/
template <typename indexType> inline const indexType GetStringLength (const char *const string)
{
	// Reliability check.
	InternalAssert (!IsNullString (string));

	indexType length (0u);

	while (string[length] != '\0')
		++length;

	return length;
}

template <typename elementType, typename indexType> inline void MemoryReverse (elementType *bufferPointer, const indexType count)
{	// reverse elements in [bufferPointer, bufferPointer + count), bidirectional iterators

	// Reliability checks.
	InternalAssert (count > 0u);
	InternalAssert (bufferPointer != NULL);

	elementType *bufferPointerEnd (bufferPointer + count);

	// Reliability check.
	InternalAssert (bufferPointerEnd != NULL);

	for (/* Empty */; /* Empty */; /* Empty */)
	{
		SwapElements (*bufferPointer, *--bufferPointerEnd);

		if (++bufferPointer == bufferPointerEnd)
			break;
	}
}

/***
*char *StringDuplicate(string) - duplicate string into malloc'd memory
*
*Purpose:
*       Allocates enough storage via malloc() for a copy of the
*       string, copies the string into the new memory, and returns
*       a pointer to it.
*
*Entry:
*       char *string - string to copy into new memory
*
*Exit:
*       returns a pointer to the newly allocated storage with the
*       string in it.
*
*       returns NULL if enough memory could not be allocated, or
*       string was NULL.
*
*Uses:
*
*Exceptions:
*
*******************************************************************************/
inline char *const StringDuplicate (const char *const string)
{
	if (IsNullString (string))
		return NULL;

	const size_t size (GetStringLength <size_t> (string) + 1u);
	char *const memory (new char[size]);

	if (memory == NULL)
		return NULL;

	MemoryCopy (string, memory, size);

	return memory;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
// Methods to invoke the constructor, copy constructor, and destructor
//-----------------------------------------------------------------------------

template <typename elementType> inline void Construct (elementType *const memory)
{
	/*::*/new (memory) elementType;
}

template <typename elementType> inline void CopyConstruct (elementType *const memory, const elementType &source)
{
	/*::*/new (memory) elementType (source);	// construct object at 'memory' with value 'source'.
}

template <typename elementType> inline void Destruct (elementType *const memory)
{
	memory->~elementType ();	// destroy object at 'memory'.

	#if defined _DEBUG
		memset (memory, 0xDD, sizeof (memory));
	#endif	// if defined _DEBUG
}
/*
// TEMPLATE FUNCTION _Allocate
template<class _Ty> inline _Ty *_Allocate(size_t _Count, _Ty *)
{	// check for integer overflow
	if (_Count <= 0)
		_Count = 0;
	else if (((size_t)(-1) / _Count) < sizeof (_Ty))
		_THROW_NCEE(std::bad_alloc, NULL);
		throw std::bad_alloc(NULL);

		// allocate storage for _Count elements of type _Ty
	return ((_Ty *)::operator new(_Count * sizeof (_Ty)));
}

		// TEMPLATE FUNCTION _Construct
template<class _T1, class _T2> inline void _Construct(_T1 *_Ptr, const _T2& _Val)
{	// construct object at _Ptr with value _Val
	void *_Vptr = _Ptr;

	::new (_Vptr) _T1(_Val);
}

		// TEMPLATE FUNCTION _Destroy
template<class _Ty> inline void _Destroy(_Ty  *_Ptr)
{	// destroy object at _Ptr
	_Ptr->~_Ty ();
}

template<> inline void _Destroy(char *)
{	// destroy a char (do nothing)
}

template<> inline void _Destroy(unsigned short *)
{	// destroy a char (do nothing)
}
*/
/***
*UninitializedMemoryNonScalarMove - Copy source buffer to destination buffer
*
*Purpose:
*       UninitializedMemoryNonScalarMove() copies a source memory buffer to a destination memory buffer.
*       This routine recognize overlapping buffers to avoid propogation.
*       For cases where propogation is not a problem, memcpy() can be used.
*
*Entry:
*       elementType *destination = pointer to destination buffer
*       const elementType *source = pointer to source buffer
*       indexType count = number of bytes to copy
*
*Exit:
*       Returns a pointer to the destination buffer
*
*Exceptions: None
*******************************************************************************/
template <typename elementType, typename indexType> inline void UninitializedMemoryNonScalarMove (elementType *destination, const elementType *source, const indexType count)
{	// move [_First, _Last) to raw _Dest, arbitrary type

	// Reliability checks.
	InternalAssert (destination != NULL);
	InternalAssert (source != NULL);
//	InternalAssert (count > 0u);

	if (count == 0u)
		return;

	if (destination <= source || destination >= source + count)
	{
		/*
		* Non-Overlapping Buffers
		* copy from lower addresses to higher addresses
		*/
/*		do
		{
			*destination = *source;

			++destination;
			++source;
		} while (--count > 0u);*/
		MemoryCopy (source, destination, count);
	}
	else
	{
		/*
		* Overlapping Buffers
		* copy from higher addresses to lower addresses
		*/
		destination += count - 1u;
		source += count - 1u;
/*
		do
		{
			*destination = *source;

			--destination;
			--source;
		} while (--count > 0u);*/
		MemoryReverseCopy (source, destination, count);
	}
}

#endif	// ifndef CORE_TOOL_LIBRARY_DEFINITIONS_INCLUDED