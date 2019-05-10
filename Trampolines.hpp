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
// Trampolines.hpp
//
// Namespace: Trampolines
//
// Version: $ID:$
//

#if defined _MSC_VER && _MSC_VER > 1000
#	pragma once
#endif	// if defined _MSC_VER && _MSC_VER > 1000

#ifndef TRAMPOLINES_INCLUDED
#define TRAMPOLINES_INCLUDED

namespace Trampolines
{
	// Reliability checks.
	CompileTimeAssert (sizeof (int) == sizeof (long));
	CompileTimeAssert (sizeof (bool) == sizeof (char));
	CompileTimeAssert (sizeof (bool) == 1u);
	CompileTimeAssert (sizeof (BOOL) == 4u);
	CompileTimeAssert (sizeof (char) == 1u);
	CompileTimeAssert (sizeof (short) == 2u);
	CompileTimeAssert (sizeof (int) == 4u);
	CompileTimeAssert (sizeof (long) == 4u);
	CompileTimeAssert (sizeof (enum Test_t) == 4u);
	CompileTimeAssert (sizeof (long long) == 8u);
	CompileTimeAssert (sizeof (float) == 4u);
	CompileTimeAssert (sizeof (double) == 8u);

	/**
	* List of x86 bytecodes for creating basic trampolines at runtime.
	* -
	* These are defined here so that, should
	* the need ever arise, this can be ported
	* to other architectures fairly painlessly.
	*/
	namespace Bytecode
	{
/**             32 bit                32 bit                32 bit                32 bit
		•———————————————————• •———————————————————• •———————————————————• •———————————————————•
		|        EAX        | |        ECX        | |        EDX        | |        EBX        |
		|———————————————————| |———————————————————| |———————————————————| |———————————————————|
		|         |   AX    | |         |   CX    | |         |   DX    | |         |   BX    |
		|———————————————————| |———————————————————| |———————————————————| |———————————————————|
		|         | AH | AL | |         | CH | CL | |         | DH | DL | |         | BH | BL |
		•———————————————————• •———————————————————• •———————————————————• •———————————————————•

		•———————————————————• •———————————————————• •———————————————————• •———————————————————•
		|        ESP        | |        EBP        | |        ESI        | |        EDI        |
		|———————————————————| |———————————————————| |———————————————————| |———————————————————|
		|         |   SP    | |         |   BP    | |         |   SI    | |         |   DI    |
		|———————————————————| |———————————————————| |———————————————————| |———————————————————|
		|         |   | SPL | |         |    | BP | |         |   | SIL | |         |   | DIL |
		•———————————————————• •———————————————————• •———————————————————• •———————————————————•
*/
		/**
		* EAX - result of call.
		* ESP - stack pointer to function arguments.
		* ECX - pointer to 'this' pointer in member functions.
		*/

		const unsigned char Pointer8Size (sizeof (unsigned char));
		const unsigned char Pointer16Size (sizeof (unsigned short));
		const unsigned char Pointer32Size (sizeof (void *));
		const unsigned char Pointer64Size (sizeof (unsigned long long));

		// Values to replace....
		const unsigned char RawNumber[4u] = {0xDE, 0xFA, 0xAD, 0xDE};	// Raw number (DEADFADEh)
		const unsigned char SomeByteValue/*[sizeof (char)]*/ (0xFF);	// Some byte value (0xFF)
		const unsigned char SomeShortValue[sizeof (short)] = {0xCD, 0xAB};	// Some short value (0xABCD)
		const unsigned char SomeValue[sizeof (int)] = {SomeByteValue, SomeByteValue, SomeByteValue, SomeByteValue};	// Some value (0xFF, 0xFF, 0xFF, 0xFF)

		// Opcodes with encoding information....
		const unsigned char WildCard (0x2A);	// Unknown opcode - used for signature scanning. ('*')
		const unsigned char Breakpoint (0xCC);	// int 3 (no extra encoding)
		const unsigned char Nop (0x90);	// nop (no extra encoding)
		const unsigned char PushEAX (0x50);	// push eax (encoding is +r)
		const unsigned char PushECX (0x51);	// push ecx
		const unsigned char PushEDX (0x52);	// push edx
		const unsigned char PushEBX (0x53);	// push ebx
		const unsigned char PushEBP (0x55);	// push ebp
		const unsigned char PushESI (0x56);	// push esi
		const unsigned char PushEDI (0x57);	// push edi
		const unsigned char PushESPWithOffset8[2u] = {0xFF, 0x74};	// push [esp+'.']
		const unsigned char PushEBPWithOffset8[2u] = {0xFF, 0x75};	// push [ebp+'.']
		const unsigned char Push8 (0x6A);	// push '.' (encoding is <imm8>)
		const unsigned char Push32 (0x68);	// push '....' (encoding is <imm32>)
		const unsigned char Push32Size (sizeof (Push32) + Pointer32Size);	// size of the push instruction
		const unsigned char MoveRegisterImmediately (0xB8);	// mov eax, '....' (encoding is +r <imm32>)
		const unsigned char MoveESPToEBP[2u] = {0x89, 0xE5};	// mov ebp, esp
		const unsigned char MoveValue32ToECX[2u] = {0x8B, 0x0D};	// mov ecx, '....'
		const unsigned char MoveValue32ToECXSize (sizeof (MoveValue32ToECX) + Pointer32Size);
		const unsigned char MoveESIToValue32[2u] = {0x89, 0x35};	// mov '....', esi
		const unsigned char MoveESIToValue32Size (sizeof (MoveESIToValue32) + Pointer32Size);
		const unsigned char MoveValue32ToEAX (0xA1);	// mov eax, '....'
		const unsigned char MoveValue32ToEAXSize (sizeof (MoveValue32ToEAX) + Pointer32Size);
		const unsigned char MovePointer32ToPointer32[] = {0xC7, 0x05};	// mov '....', '....'
		const unsigned char MovePointer32ToPointer32Size (sizeof (MovePointer32ToPointer32) + Pointer32Size + Pointer32Size);
		const unsigned char CallEAX[2u] = {0xFF, 0xD0};	// call eax
		const unsigned char AddESP8[2u] = {0x83, 0xC4};	// add esp, '.'
		const unsigned char AddESP8Size (sizeof (AddESP8) + Pointer8Size);
		const unsigned char AddESP32[2u] = {0x81, 0xC4};	// add esp, '....'
		const unsigned char AddESP32Size (sizeof (AddESP32) + Pointer32Size);
		const unsigned char PopECX (0x59);	// pop ecx
		const unsigned char PopEBP (0x5D);	// pop ebp
		const unsigned char PopESI (0x5E);	// pop esi
		const unsigned char Return (0xC3);	// ret (no extra encoding)
		const unsigned char ReturnNearValue (0xC2);	// retn '..' (encoding is <imm16>)
		const unsigned char PopRegister (0x58);	// pop eax (encoding is +r)
		const unsigned char CallImmediately32 (0xE8);	// call '....' (relative call, <imm32>)
		const unsigned char CallImmediately32Size (sizeof (CallImmediately32) + Pointer32Size);
		const unsigned char JumpImmediately8 (0xEB);	// jmp '.' (encoding is imm8)
		const unsigned char JumpImmediately8Size (sizeof (JumpImmediately8) + Pointer8Size);
		const unsigned char JumpImmediately32 (0xE9);	// jmp '....' (encoding is imm32)
		const unsigned char JumpImmediately32Size (sizeof (JumpImmediately32) + Pointer32Size);	// size of the jump-to instruction
		const unsigned char JumpIfZeroImmediately32[] = {0x0F, 0x84};	// jz '....' (encoding is imm32)
		const unsigned char JumpIfZeroImmediately32Size (sizeof (JumpIfZeroImmediately32) + Pointer32Size);	// size of the jump-to instruction
		const unsigned char JumpIfNotZeroImmediately32[] = {0x0F, 0x85};	// jnz '....' (encoding is imm32)
		const unsigned char JumpIfNotZeroImmediately32Size (sizeof (JumpIfNotZeroImmediately32) + Pointer32Size);	// size of the jump-to instruction

		/**
		* Prologue for a void function Clobbers EBX and EAX
		*/
		const unsigned char codeVoidPrologue[sizeof (PushEBP) + sizeof (MoveESPToEBP) + sizeof (PushEAX)] =
		{
			PushEBP,							// push ebp
			MoveESPToEBP[0u], MoveESPToEBP[1u],	// mov  ebp, esp
			PushEAX								// push eax
		};

		/**
		* Prologue for a function that returns Clobbers EBX, EAX too but not after call.
		*/
		const unsigned char codeReturnPrologue[sizeof (PushEBP) + sizeof (MoveESPToEBP)] =
		{
			PushEBP,							// push ebp
			MoveESPToEBP[0u], MoveESPToEBP[1u]	// mov  ebp, esp
		};
		const unsigned char codeThisReturnPrologue[sizeof (PushEBP) + sizeof (MoveESPToEBP)] =
		{
			PushEBP,							// push ebp
			MoveESPToEBP[0u], MoveESPToEBP[1u]	// mov  ebp, esp
		};

		/**
		* Takes a paramter from the trampoline's stack and pushes it onto the target's stack.
		*/
		const unsigned char codePushParam[sizeof (PushEBPWithOffset8) + sizeof (SomeByteValue)] =
		{
			PushEBPWithOffset8[0u], PushEBPWithOffset8[1u], SomeByteValue	// push [ebp+'SomeByteValue']
		};

		/**
		* Offset of 'codePushParam' to modify at runtime that contains the stack offset.
		*/
		const unsigned char codePushParamReplace (sizeof (PushEBPWithOffset8));

		/**
		* Takes the "this" pointer from the trampoline and pushes it onto the target's stack.
		*/
		const unsigned char codePushThis (0x51);	// push ecx

		/**
		* Pushes a raw number onto the target's stack.
		*/
		const unsigned char codePushID[Push32Size] =
		{
			Push32, RawNumber[0u], RawNumber[1u], RawNumber[2u], RawNumber[3u]	// push 'RawNumber'
		};

		/**
		* Offset of 'codePushID' to modify at runtime to contain the number to push.
		*/
		const unsigned char codePushIDReplace (sizeof (Push32));

		/**
		* Call our procedure.
		*/
		const unsigned char codeCall[sizeof (MoveRegisterImmediately) + sizeof (RawNumber) + sizeof (CallEAX)] =
		{
			MoveRegisterImmediately, RawNumber[0u], RawNumber[1u], RawNumber[2u], RawNumber[3u],	// mov  eax, 'RawNumber'
			CallEAX[0u], CallEAX[1u]																// call eax
		};

		/**
		* Offset of 'codeCall' to modify at runtime to contain the pointer to the function.
		*/
		const unsigned char codeCallReplace (sizeof (MoveRegisterImmediately));

		/**
		* Adds to ESP, freeing up stack space
		*/
		const unsigned char codeFreeStack8[sizeof (AddESP8) + sizeof (SomeByteValue)] =
		{
			AddESP8[0u], AddESP8[1u], SomeByteValue	// add esp, 'SomeByteValue'
		};
		const unsigned char codeFreeStack32[sizeof (AddESP32) + sizeof (SomeValue)] =
		{
			AddESP32[0u], AddESP32[1u], SomeValue[0u], SomeValue[1u], SomeValue[2u], SomeValue[3u]	// add esp, 'SomeValue'
		};

		/**
		* Offset of 'codeFreeStack*' to modify at runtime to contain how much data to free.
		*/
		const unsigned char codeFreeStackReplace (sizeof (AddESP32));

		/**
		* Epilogue of a simple return function.
		*/
		const unsigned char codeReturnEpilogue[sizeof (PopEBP) + sizeof (Return)] =
		{
			PopEBP,	// pop ebp
			Return	// ret
		};
		const unsigned char codeReturnEpilogueN[sizeof (PopEBP) + sizeof (ReturnNearValue) + sizeof (SomeShortValue)] =
		{
			PopEBP,													// pop  ebp
			ReturnNearValue, SomeShortValue[0u], SomeShortValue[1u]	// retn 'SomeShortValue'
		};
		const unsigned char codeReturnEpilogueNReplace (sizeof (PopEBP) + sizeof (ReturnNearValue));

		/**
		* Epilogue of a void return function.
		*/
		const unsigned char codeVoidEpilogue[sizeof (PopRegister) + sizeof (PopEBP) + sizeof (Return)] =
		{
			PopRegister,	// pop eax
			PopEBP,			// pop ebp
			Return			// ret
		};

		const unsigned char codeVoidEpilogueN[sizeof (PopRegister) + sizeof (PopEBP) + sizeof (ReturnNearValue) + sizeof (SomeShortValue)] =
		{
			PopRegister,											// pop  eax
			PopEBP,													// pop  ebp
			ReturnNearValue, SomeShortValue[0u], SomeShortValue[1u]	// retn 'SomeShortValue'
		};
		const unsigned char codeVoidEpilogueNReplace (sizeof (PopRegister) + sizeof (PopEBP) + sizeof (ReturnNearValue));

		#pragma pack (push, 1)	// Structures must be packed (byte-aligned) (Very important!)
			struct MoveESIToValue32_t
			{
				unsigned char       opcodes[sizeof (MoveESIToValue32)];	// 0x8935 = "mov '....', esi" (from 'MoveESIToValue32')
				union /* Unnamed */
				{
					unsigned char   bytes[Pointer32Size];				// Raw bytes.

					void           *pointer;							// Pointer to assign at.
					void          **pointerToPointer;					// Pointer to pointer to assign at.
					unsigned int    value;								// Value to assign at.
				};
			};	// sizeof (MoveESIToValue32_t) == 6 (1.5)

			template <const unsigned char instruction, typename functionType> struct BaseCallOrJumpImmediately32_t
			{
				private:
					const unsigned char  opcode;				// "instruction '....'" (relative call, <imm32>) (from 'instruction')

				public:
					union /* Unnamed */
					{
						unsigned char    bytes[Pointer32Size];	// Raw bytes.
						void            *pointer;				// Pointer to call.
						unsigned int     value;					// Value to call.
						functionType     function;				// Function to call.
					};

					inline BaseCallOrJumpImmediately32_t (void *const address = *reinterpret_cast <void **> (const_cast <unsigned char *> (RawNumber))) :
						opcode (instruction),
						pointer (address)
					{ /* VOID */ }
					inline BaseCallOrJumpImmediately32_t (const BaseCallOrJumpImmediately32_t &otherCallImmediately32) :
						opcode (instruction),
						pointer (otherCallImmediately32.pointer)
					{ /* VOID */ }

					inline BaseCallOrJumpImmediately32_t &operator = (const BaseCallOrJumpImmediately32_t &otherCallImmediately32)
					{
						pointer = otherCallImmediately32.pointer;

						return *this;
					}
			};	// sizeof (BaseCallOrJumpImmediately32_t) == 5 (1.25)
			template <typename functionType = void *> struct CallImmediately32_t : BaseCallOrJumpImmediately32_t <CallImmediately32, functionType> { /* VOID */ };
			template <typename functionType = void *> struct JumpImmediately32_t : BaseCallOrJumpImmediately32_t <JumpImmediately32, functionType> { /* VOID */ };
		#pragma pack (pop)	// Reset default packing.

		// Reliability checks.
		CompileTimeAssert (sizeof (MoveESIToValue32_t) == MoveValue32ToECXSize);
		CompileTimeAssert (sizeof (CallImmediately32_t <>) == CallImmediately32Size);
		CompileTimeAssert (sizeof (JumpImmediately32_t <>) == JumpImmediately32Size);
	}

	// Ideas by Don Clugston.
	// Check out his excellent paper: http://www.codeproject.com/cpp/FastDelegate.asp

	namespace
	{
		// GenericClass is a fake class, ONLY used to provide a type.
		// It is vitally important that it is never defined, so that the compiler doesn't
		// think it can optimize the invocation. For example, Borland generates simpler
		// code if it knows the class only uses single inheritance.

		// Compilers using Microsoft's structure need to be treated as a special case.

		// For Microsoft and Intel, we want to ensure that it's the most efficient type of MFP
		// (4 bytes), even when the /vmg option is used. Declaring an empty class
		// would give 16 byte pointers in this case....
		class __single_inheritance GenericClass;

		// ...but for Codeplay, an empty class *always* gives 4 byte pointers.
		// If compiled with the /clr option ("managed C++"), the JIT compiler thinks
		// it needs to load GenericClass before it can call any of its functions,
		// (compiles OK but crashes at runtime!), so we need to declare an
		// empty class to make it happy.
		// Codeplay and VC4 can't cope with the unknown_inheritance case either.
		class GenericClass { /* Empty */ };	// Sigscanned member functions are casted to member function pointers of this class and called with member function pointer syntax.

		class EmptyClass { /* Empty */ };	// An empty class. No inheritance used. Used for original-function-call hacks.

		// virtual inheritance is a real nuisance. It's inefficient and complicated.
		// On MSVC and Intel, there isn't enough information in the pointer itself to
		// enable conversion to a closure pointer. Earlier versions of this code didn't
		// work for all cases, and generated a compile-time error instead.
		// But a very clever hack invented by John M. Dlugosz solves this problem.
		// My code is somewhat different to his: I have no asm code, and I make no
		// assumptions about the calling convention that is used.

		// In VC++ and ICL, a virtual_inheritance member pointer
		// is internally defined as:
		struct MicrosoftVirtualMFP_t
		{
			void (GenericClass::*codeptr) (void);	// points to the actual member function
			int  delta;								// #bytes to be added to the 'this' pointer
			int  virtualFunctionTableIndex;			// or 0 if no virtual inheritance
		};
		// The CRUCIAL feature of Microsoft/Intel MFPs which we exploit is that the
		// m_codeptr member is *always* called, regardless of the values of the other
		// members. (This is *not* true for other compilers, eg GCC, which obtain the
		// function address from the vtable if a virtual function is being called).
		// Dlugosz's trick is to make the codeptr point to a probe function which
		// returns the 'this' pointer that was used.

		// Define a generic class that uses virtual inheritance.
		// It has a trival member function that returns the value of the 'this' pointer.
		struct GenericVirtualClass : virtual public GenericClass
		{
			typedef GenericVirtualClass * (GenericVirtualClass::*ProbePtrType) (void);

			inline GenericVirtualClass *const GetThis (void) { return this; }
		};

		// The size of a single inheritance member function pointer.
		const unsigned char MemoryFunctionPointerSize = sizeof (void (GenericClass::*) (void));
	}

	inline const int GetVirtualFunctionTableOffset (const void *const memberFunctionPointer)
	{
		const unsigned char *address (static_cast <const unsigned char *> (memberFunctionPointer));

		if (*address == Bytecode::JumpImmediately32)	// Jmp
		{
			// May or may not be!
			// Check where it'd jump
			address += Bytecode::JumpImmediately32Size + *reinterpret_cast <const unsigned long *> (address + sizeof (Bytecode::JumpImmediately32));
		}

		// Check whether it's a virtual function call
		// They look like this:
		// 004125A0 8B 01             mov         eax, dword ptr [ecx]
		// 004125A2 FF 60 04          jmp         dword ptr [eax+4]
		//		==OR==
		// 00411B80 8B 01             mov         eax, dword ptr [ecx]
		// 00411B82 FF A0 18 03 00 00 jmp         dword ptr [eax+318h]

		// However, for vararg functions, they look like this:
		// 0048F0B0 8B 44 24 04       mov         eax, dword ptr [esp+4]
		// 0048F0B4 8B 00             mov         eax, dword ptr [eax]
		// 0048F0B6 FF 60 08          jmp         dword ptr [eax+8]
		//		==OR==
		// 0048F0B0 8B 44 24 04       mov         eax, dword ptr [esp+4]
		// 0048F0B4 8B 00             mov         eax, dword ptr [eax]
		// 00411B82 FF A0 18 03 00 00 jmp         dword ptr [eax+318h]

		// With varargs, the this pointer is passed as if it was the first argument

		if (address[0u] != 0x8B)
			return -1;

		if (address[1u] == 0x44 && address[2u] == 0x24 && address[3u] == 0x04 && address[4u] == 0x8B && address[5u] == 0x00)
			address += 6u;
		else if (address[1u] == 0x01)
			address += 2u;
		else
			return -1;

		if (*address != 0xFF)
			return -1;

		if (*++address == 0x60)
			return *++address / 4u;

		if (*address == 0xA0)
			return *reinterpret_cast <const unsigned int *> (++address) / 4u;

		if (*address == 0x20)
			return 0;

		return -1;
	}

	namespace
	{
		//		implicit_cast < type >
		// I believe this was originally going to be in the C++ standard but
		// was left out by accident. It's even milder than static_cast.
		// I use it instead of static_cast<> to emphasize that I'm not doing
		// anything nasty.
		// Usage is identical to static_cast<>.
		template <typename OutputClass, typename InputClass> inline OutputClass implicit_cast (InputClass input) { return input; }

		//		horrible_cast < type >
		// This is truly evil. It completely subverts C++'s type system, allowing you
		// to cast from any class to any other class. Technically, using a union
		// to perform the cast is undefined behaviour (even in C). But we can see if
		// it is OK by checking that the union is the same size as each of its members.
		// horrible_cast<> should only be used for compiler-specific workarounds.
		// Usage is identical to reinterpret_cast<>.
		template <typename OutputClass, typename InputClass> inline OutputClass horrible_cast (const InputClass input)
		{
			// This union is declared outside the horrible_cast because BCC 5.5.1
			// can't inline a function with a nested class, and gives a warning.
			union /* Unnamed */
			{
				OutputClass out;
				InputClass  in;
			} horribleUnion;

			// Cause a compile-time error if in, out and horribleUnion are not the same size.
			// If the compile fails here, it means the compiler has peculiar
			// unions which would prevent the cast from working.
			CompileTimeAssert (sizeof (InputClass) == sizeof (horribleUnion) && sizeof (InputClass) == sizeof (OutputClass));	// Cant use horrible case?

			horribleUnion.in = input;

			return horribleUnion.out;
		}
	}

	struct MemoryFunctionInformation_t
	{
		int  thisPointerOffset;				// The this pointer the function expects to be called with
											// If -1, you need to call the GetFuncInfo_GetThisPtr function
		int  virtualFunctionTableIndex;		// The function's index in the vtable (-1-function not virtual, 0-based, 1=second entry, 2=third entry, ...)
		int  virtualFunctionTableOffset;	// The vtable pointer

		inline MemoryFunctionInformation_t (void) :
			thisPointerOffset (-1),
			virtualFunctionTableIndex (-1),
			virtualFunctionTableOffset (0)
		{ /* VOID */ }
	};

	// general case -- don't know how to get info. Force a compile failure.
	template <const unsigned short size> struct MFI_Impl
	{
		template <typename MFP> static inline void GetFunctionInformation (MFP *mfp, MemoryFunctionInformation_t &out)
		{
			// Unsupported member function type -- force a compile failure. (it's illegal to have a array with negative size).
			static char weird_memfunc_pointer_exclamation_mark_arrow_error[static_cast <int> (size) - 1000];
		}
	};

	// For compilers where all member func ptrs are the same size, everything goes here.
	// For non-standard compilers, only single_inheritance classes go here.
	template <> struct MFI_Impl <MemoryFunctionPointerSize>	// simple ones
	{
		template <typename MFP> static inline void GetFunctionInformation (MFP mfp, MemoryFunctionInformation_t &out)
		{
			out.virtualFunctionTableIndex = GetVirtualFunctionTableOffset (*reinterpret_cast <void **> (&mfp));
			out.thisPointerOffset = 0;
			out.virtualFunctionTableOffset = 0;
		}
	};

	// __multiple_inheritance classes go here
	template <> struct MFI_Impl <2u * MemoryFunctionPointerSize>	// more complicated ones!
	{
		template <typename MFP> static inline void GetFunctionInformation (MFP mfp, MemoryFunctionInformation_t &out)
		{
			struct MSVC_MemFunPtr2
			{
				void *funcadr;	// points to the actual member function
				int   delta;	// #BYTES to be added to the 'this' pointer
			};

			out.virtualFunctionTableIndex = GetVirtualFunctionTableOffset (*reinterpret_cast <void **> (&mfp));
			out.thisPointerOffset = reinterpret_cast <MSVC_MemFunPtr2 *> (&mfp)->delta;
			out.virtualFunctionTableOffset = 0;
		}
	};

	// __virtual_inheritance classes go here
	template <> struct MFI_Impl <3u * MemoryFunctionPointerSize>	// WOW IT'S GETTING BIGGER OMGOMOGMG
	{
		template <typename MFP> static inline void GetFunctionInformation (MFP mfp, MemoryFunctionInformation_t &out)
		{
			out.virtualFunctionTableIndex = GetVirtualFunctionTableOffset (*reinterpret_cast <void **> (&mfp));

			// This pointer
/*
			union /* Unnamed *//*
			{
				MFP func;
				GenericClass * (T::*ProbeFunc) (void);
				MicrosoftVirtualMFP_t s;
			} u;
			u.func = mfp;
			union /* Unnamed *//*
			{
				GenericVirtualClass::ProbePtrType virtfunc;
				MicrosoftVirtualMFP_t s;
			} u2;

			// Check that the horrible_cast<>s will work
			typedef int ERROR_CantUsehorrible_cast[sizeof(mfp)==sizeof(u.s)
				&& sizeof(mfp)==sizeof(u.ProbeFunc)
				&& sizeof(u2.virtfunc)==sizeof(u2.s) ? 1 : -1];
			// Unfortunately, taking the address of a MF prevents it from being inlined, so
			// this next line can't be completely optimised away by the compiler.
			u2.virtfunc = &GenericVirtualClass::GetThis;
			u.s.codeptr = u2.s.codeptr;
			out.thisPointerOffset = (reinterpret_cast<T*>(NULL)->*u.ProbeFunc) ();
*/
			out.thisPointerOffset = -1;
			out.virtualFunctionTableOffset = 0;
		}
	};

	// Nasty hack for Microsoft and Intel (IA32 and Itanium)
	// unknown_inheritance classes go here
	// This is probably the ugliest bit of code I've ever written. Look at the casts!
	// There is a compiler bug in MSVC6 which prevents it from using this code.
	template <> struct MFI_Impl <4u * MemoryFunctionPointerSize>	// THE BIGGEST ONE!!!1GABEN
	{
		template <typename MFP> static inline void GetFunctionInformation (MFP mfp, MemoryFunctionInformation_t &out)
		{
			out.virtualFunctionTableIndex = GetVirtualFunctionTableOffset (*reinterpret_cast <void **> (&mfp));

			// The member function pointer is 16 bytes long. We can't use a normal cast, but
			// we can use a union to do the conversion.
			union /* Unnamed */
			{
				MFP func;
				// In VC++ and ICL, an unknown_inheritance member pointer is internally defined as:
				struct
				{
					void *m_funcaddress;	// points to the actual member function
					int delta;				// #bytes to be added to the 'this' pointer
					int vtordisp;			// #bytes to add to 'this' to find the vtable
					int virtualFunctionTableIndex;		// or 0 if no virtual inheritance
				} s;
			} u;
			// Check that the horrible_cast will work
			typedef int ERROR_CantUsehorrible_cast[sizeof (u.func) == sizeof (u.s) ? 1 : -1];
			u.func = mfp;

			if (u.s.virtualFunctionTableIndex > 0)
			{
				// Virtual inheritance is used
				/*
				// First, get to the vtable.
				// It is 'vtordisp' bytes from the start of the class.
				int *vtable (*reinterpret_cast<int **> (reinterpret_cast<char *>(thisptr) + u.s.vtordisp));

				// 'virtualFunctionTableIndex' tells us where in the table we should be looking.
				const int virtual_delta = u.s.vtordisp + *reinterpret_cast <const int *> (reinterpret_cast <const char *> (vtable) + u.s.virtualFunctionTableIndex);
			// The int at 'virtual_delta' gives us the amount to add to 'this'.
			// Finally we can add the three components together. Phew!
			out.thisptr = reinterpret_cast <void *> (reinterpret_cast <char *> (thisptr) + u.s.delta + virtual_delta);
				*/
				out.virtualFunctionTableOffset = u.s.vtordisp;
				out.thisPointerOffset = -1;
			}
			else
			{
				out.virtualFunctionTableOffset = out.virtualFunctionTableIndex < 0 ? 0 : u.s.delta;
				out.thisPointerOffset = u.s.delta;
			}
		};
	};

	// This version does not take a this pointer
	// Useful for hookdecls, as they ensure that mfp is correct through a static_cast
	template <typename X> inline void GetFunctionInformation (X mfp, MemoryFunctionInformation_t &out)
	{
		MFI_Impl <sizeof (mfp)>::GetFunctionInformation (mfp, out);
	}

	// Versions which do take a this

	template <typename X, typename Y, typename RetType>
	inline void GetFunctionInformation (Y *ptr, RetType (X::*mfp) (...), MemoryFunctionInformation_t &out)
	{
		RetType (Y::*mfp2) (...) = mfp;

		MFI_Impl <sizeof (mfp2)>::GetFunctionInformation (mfp2, out);
	}

	template <typename X, typename Y, typename RetType>
		inline void GetFunctionInformation (Y *ptr, RetType (X::*mfp) (...) const, MemoryFunctionInformation_t &out)
	{
		RetType (Y::*mfp2) (...) const = mfp;

		MFI_Impl <sizeof (mfp2)>::GetFunctionInformation (mfp2, out);
	}


	template <typename X, typename Y, typename RetType, typename Param1>
		inline void GetFunctionInformation (Y *ptr, RetType (X::*mfp) (Param1, ...), MemoryFunctionInformation_t &out)
	{
		RetType (Y::*mfp2) (Param1, ...) = mfp;

		MFI_Impl <sizeof (mfp2)>::GetFunctionInformation (mfp2, out);
	}

	template <typename X, typename Y, typename RetType, typename Param1>
		inline void GetFunctionInformation (Y *ptr, RetType (X::*mfp) (Param1, ...) const, MemoryFunctionInformation_t &out)
	{
		RetType (Y::*mfp2) (Param1, ...) const = mfp;

		MFI_Impl <sizeof (mfp2)>::GetFunctionInformation (mfp2, out);
	}


	template <typename X, typename Y, typename RetType, typename Param1, typename Param2>
		inline void GetFunctionInformation (Y *ptr, RetType (X::*mfp) (Param1, Param2, ...), MemoryFunctionInformation_t &out)
	{
		RetType (Y::*mfp2) (Param1, Param2, ...) = mfp;

		MFI_Impl <sizeof (mfp2)>::GetFunctionInformation (mfp2, out);
	}

	template <typename X, typename Y, typename RetType, typename Param1, typename Param2>
		inline void GetFunctionInformation (Y *ptr, RetType (X::*mfp) (Param1, Param2, ...) const, MemoryFunctionInformation_t &out)
	{
		RetType (Y::*mfp2) (Param1, Param2, ...) const = mfp;

		MFI_Impl <sizeof (mfp2)>::GetFunctionInformation (mfp2, out);
	}


	template <typename X, typename Y, typename RetType, typename Param1, typename Param2, typename Param3>
		inline void GetFunctionInformation (Y *ptr, RetType (X::*mfp) (Param1, Param2, Param3, ...), MemoryFunctionInformation_t &out)
	{
		RetType (Y::*mfp2) (Param1, Param2, Param3, ...) = mfp;

		MFI_Impl <sizeof (mfp2)>::GetFunctionInformation (mfp2, out);
	}

	template <typename X, typename Y, typename RetType, typename Param1, typename Param2, typename Param3>
		inline void GetFunctionInformation (Y *ptr, RetType (X::*mfp) (Param1, Param2, Param3, ...) const, MemoryFunctionInformation_t &out)
	{
		RetType (Y::*mfp2) (Param1, Param2, Param3, ...) const = mfp;

		MFI_Impl <sizeof (mfp2)>::GetFunctionInformation (mfp2, out);
	}


	template <typename X, typename Y, typename RetType, typename Param1, typename Param2, typename Param3, typename Param4>
		inline void GetFunctionInformation (Y *ptr, RetType (X::*mfp) (Param1, Param2, Param3, Param4, ...), MemoryFunctionInformation_t &out)
	{
		RetType (Y::*mfp2) (Param1, Param2, Param3, Param4, ...) = mfp;

		MFI_Impl <sizeof (mfp2)>::GetFunctionInformation (mfp2, out);
	}

	template <typename X, typename Y, typename RetType, typename Param1, typename Param2, typename Param3, typename Param4>
		inline void GetFunctionInformation (Y *ptr, RetType (X::*mfp) (Param1, Param2, Param3, Param4, ...) const, MemoryFunctionInformation_t &out)
	{
		RetType (Y::*mfp2) (Param1, Param2, Param3, Param4, ...) const = mfp;

		MFI_Impl <sizeof (mfp2)>::GetFunctionInformation (mfp2, out);
	}


	template <typename X, typename Y, typename RetType, typename Param1, typename Param2, typename Param3, typename Param4, typename Param5>
		inline void GetFunctionInformation (Y *ptr, RetType (X::*mfp) (Param1, Param2, Param3, Param4, Param5, ...), MemoryFunctionInformation_t &out)
	{
		RetType (Y::*mfp2) (Param1, Param2, Param3, Param4, Param5, ...) = mfp;

		MFI_Impl <sizeof (mfp2)>::GetFunctionInformation (mfp2, out);
	}

	template <typename X, typename Y, typename RetType, typename Param1, typename Param2, typename Param3, typename Param4, typename Param5>
		inline void GetFunctionInformation (Y *ptr, RetType (X::*mfp) (Param1, Param2, Param3, Param4, Param5, ...) const, MemoryFunctionInformation_t &out)
	{
		RetType (Y::*mfp2) (Param1, Param2, Param3, Param4, Param5, ...) const = mfp;

		MFI_Impl <sizeof (mfp2)>::GetFunctionInformation (mfp2, out);
	}


	template <typename X, typename Y, typename RetType, typename Param1, typename Param2, typename Param3, typename Param4, typename Param5, typename Param6>
		inline void GetFunctionInformation (Y *ptr, RetType (X::*mfp) (Param1, Param2, Param3, Param4, Param5, Param6, ...), MemoryFunctionInformation_t &out)
	{
		RetType (Y::*mfp2) (Param1, Param2, Param3, Param4, Param5, Param6, ...) = mfp;

		MFI_Impl <sizeof (mfp2)>::GetFunctionInformation (mfp2, out);
	}

	template <typename X, typename Y, typename RetType, typename Param1, typename Param2, typename Param3, typename Param4, typename Param5, typename Param6>
		inline void GetFunctionInformation (Y *ptr, RetType (X::*mfp) (Param1, Param2, Param3, Param4, Param5, Param6, ...) const, MemoryFunctionInformation_t &out)
	{
		RetType (Y::*mfp2) (Param1, Param2, Param3, Param4, Param5, Param6, ...) const = mfp;

		MFI_Impl <sizeof (mfp2)>::GetFunctionInformation (mfp2, out);
	}


	template <typename X, typename Y, typename RetType, typename Param1, typename Param2, typename Param3, typename Param4, typename Param5, typename Param6, typename Param7>
		inline void GetFunctionInformation (Y *ptr, RetType (X::*mfp) (Param1, Param2, Param3, Param4, Param5, Param6, Param7, ...), MemoryFunctionInformation_t &out)
	{
		RetType (Y::*mfp2) (Param1, Param2, Param3, Param4, Param5, Param6, Param7, ...) = mfp;

		MFI_Impl <sizeof (mfp2)>::GetFunctionInformation (mfp2, out);
	}

	template <typename X, typename Y, typename RetType, typename Param1, typename Param2, typename Param3, typename Param4, typename Param5, typename Param6, typename Param7>
		inline void GetFunctionInformation (Y *ptr, RetType (X::*mfp) (Param1, Param2, Param3, Param4, Param5, Param6, Param7, ...) const, MemoryFunctionInformation_t &out)
	{
		RetType (Y::*mfp2) (Param1, Param2, Param3, Param4, Param5, Param6, Param7, ...) const = mfp;

		MFI_Impl <sizeof (mfp2)>::GetFunctionInformation (mfp2, out);
	}


	template <typename X, typename Y, typename RetType, typename Param1, typename Param2, typename Param3, typename Param4, typename Param5, typename Param6, typename Param7, typename Param8>
		inline void GetFunctionInformation (Y *ptr, RetType (X::*mfp) (Param1, Param2, Param3, Param4, Param5, Param6, Param7, Param8, ...), MemoryFunctionInformation_t &out)
	{
		RetType (Y::*mfp2) (Param1, Param2, Param3, Param4, Param5, Param6, Param7, Param8, ...) = mfp;
		MFI_Impl <sizeof (mfp2)>::GetFunctionInformation (mfp2, out);
	}

	template <typename X, typename Y, typename RetType, typename Param1, typename Param2, typename Param3, typename Param4, typename Param5, typename Param6, typename Param7, typename Param8>
		inline void GetFunctionInformation (Y *ptr, RetType (X::*mfp) (Param1, Param2, Param3, Param4, Param5, Param6, Param7, Param8, ...) const, MemoryFunctionInformation_t &out)
	{
		RetType (Y::*mfp2) (Param1, Param2, Param3, Param4, Param5, Param6, Param7, Param8, ...) const = mfp;

		MFI_Impl <sizeof (mfp2)>::GetFunctionInformation (mfp2, out);
	}


	template <typename X, typename Y, typename RetType, typename Param1, typename Param2, typename Param3, typename Param4, typename Param5, typename Param6, typename Param7, typename Param8, typename Param9>
		inline void GetFunctionInformation (Y *ptr, RetType (X::*mfp) (Param1, Param2, Param3, Param4, Param5, Param6, Param7, Param8, Param9, ...), MemoryFunctionInformation_t &out)
	{
		RetType (Y::*mfp2) (Param1, Param2, Param3, Param4, Param5, Param6, Param7, Param8, Param9, ...) = mfp;

		MFI_Impl <sizeof (mfp2)>::GetFunctionInformation (mfp2, out);
	}

	template <typename X, typename Y, typename RetType, typename Param1, typename Param2, typename Param3, typename Param4, typename Param5, typename Param6, typename Param7, typename Param8, typename Param9>
		inline void GetFunctionInformation (Y *ptr, RetType (X::*mfp) (Param1, Param2, Param3, Param4, Param5, Param6, Param7, Param8, Param9, ...) const, MemoryFunctionInformation_t &out)
	{
		RetType (Y::*mfp2) (Param1, Param2, Param3, Param4, Param5, Param6, Param7, Param8, Param9, ...) const = mfp;

		MFI_Impl <sizeof (mfp2)>::GetFunctionInformation (mfp2, out);
	}


	template <typename X, typename Y, typename RetType, typename Param1, typename Param2, typename Param3, typename Param4, typename Param5, typename Param6, typename Param7, typename Param8, typename Param9, typename Param10>
		inline void GetFunctionInformation (Y *ptr, RetType (X::*mfp) (Param1, Param2, Param3, Param4, Param5, Param6, Param7, Param8, Param9, Param10, ...), MemoryFunctionInformation_t &out)
	{
		RetType (Y::*mfp2) (Param1, Param2, Param3, Param4, Param5, Param6, Param7, Param8, Param9, Param10, ...) = mfp;

		MFI_Impl <sizeof (mfp2)>::GetFunctionInformation (mfp2, out);
	}

	template <typename X, typename Y, typename RetType, typename Param1, typename Param2, typename Param3, typename Param4, typename Param5, typename Param6, typename Param7, typename Param8, typename Param9, typename Param10>
		inline void GetFunctionInformation (Y *ptr, RetType (X::*mfp) (Param1, Param2, Param3, Param4, Param5, Param6, Param7, Param8, Param9, Param10, ...) const, MemoryFunctionInformation_t &out)
	{
		RetType (Y::*mfp2) (Param1, Param2, Param3, Param4, Param5, Param6, Param7, Param8, Param9, Param10, ...) const = mfp;

		MFI_Impl <sizeof (mfp2)>::GetFunctionInformation (mfp2, out);
	}


	template <typename X, typename Y, typename RetType, typename Param1, typename Param2, typename Param3, typename Param4, typename Param5, typename Param6, typename Param7, typename Param8, typename Param9, typename Param10, typename Param11>
		inline void GetFunctionInformation (Y *ptr, RetType (X::*mfp) (Param1, Param2, Param3, Param4, Param5, Param6, Param7, Param8, Param9, Param10, Param11, ...), MemoryFunctionInformation_t &out)
	{
		RetType (Y::*mfp2) (Param1, Param2, Param3, Param4, Param5, Param6, Param7, Param8, Param9, Param10, Param11, ...) = mfp;

		MFI_Impl <sizeof (mfp2)>::GetFunctionInformation (mfp2, out);
	}

	template <typename X, typename Y, typename RetType, typename Param1, typename Param2, typename Param3, typename Param4, typename Param5, typename Param6, typename Param7, typename Param8, typename Param9, typename Param10, typename Param11>
		inline void GetFunctionInformation (Y *ptr, RetType (X::*mfp) (Param1, Param2, Param3, Param4, Param5, Param6, Param7, Param8, Param9, Param10, Param11, ...) const, MemoryFunctionInformation_t &out)
	{
		RetType (Y::*mfp2) (Param1, Param2, Param3, Param4, Param5, Param6, Param7, Param8, Param9, Param10, Param11, ...) const = mfp;

		MFI_Impl <sizeof (mfp2)>::GetFunctionInformation (mfp2, out);
	}


	template <typename X, typename Y, typename RetType, typename Param1, typename Param2, typename Param3, typename Param4, typename Param5, typename Param6, typename Param7, typename Param8, typename Param9, typename Param10, typename Param11, typename Param12>
		inline void GetFunctionInformation (Y *ptr, RetType (X::*mfp) (Param1, Param2, Param3, Param4, Param5, Param6, Param7, Param8, Param9, Param10, Param11, Param12, ...), MemoryFunctionInformation_t &out)
	{
		RetType (Y::*mfp2) (Param1, Param2, Param3, Param4, Param5, Param6, Param7, Param8, Param9, Param10, Param11, Param12, ...) = mfp;

		MFI_Impl <sizeof (mfp2)>::GetFunctionInformation (mfp2, out);
	}

	template <typename X, typename Y, typename RetType, typename Param1, typename Param2, typename Param3, typename Param4, typename Param5, typename Param6, typename Param7, typename Param8, typename Param9, typename Param10, typename Param11, typename Param12>
		inline void GetFunctionInformation (Y *ptr, RetType (X::*mfp) (Param1, Param2, Param3, Param4, Param5, Param6, Param7, Param8, Param9, Param10, Param11, Param12, ...) const, MemoryFunctionInformation_t &out)
	{
		RetType (Y::*mfp2) (Param1, Param2, Param3, Param4, Param5, Param6, Param7, Param8, Param9, Param10, Param11, Param12, ...) const = mfp;

		MFI_Impl <sizeof (mfp2)>::GetFunctionInformation (mfp2, out);
	}


	template <typename X, typename Y, typename RetType, typename Param1, typename Param2, typename Param3, typename Param4, typename Param5, typename Param6, typename Param7, typename Param8, typename Param9, typename Param10, typename Param11, typename Param12, typename Param13>
		inline void GetFunctionInformation (Y *ptr, RetType (X::*mfp) (Param1, Param2, Param3, Param4, Param5, Param6, Param7, Param8, Param9, Param10, Param11, Param12, Param13, ...), MemoryFunctionInformation_t &out)
	{
		RetType (Y::*mfp2) (Param1, Param2, Param3, Param4, Param5, Param6, Param7, Param8, Param9, Param10, Param11, Param12, Param13, ...) = mfp;

		MFI_Impl <sizeof (mfp2)>::GetFunctionInformation (mfp2, out);
	}

	template <typename X, typename Y, typename RetType, typename Param1, typename Param2, typename Param3, typename Param4, typename Param5, typename Param6, typename Param7, typename Param8, typename Param9, typename Param10, typename Param11, typename Param12, typename Param13>
		inline void GetFunctionInformation (Y *ptr, RetType (X::*mfp) (Param1, Param2, Param3, Param4, Param5, Param6, Param7, Param8, Param9, Param10, Param11, Param12, Param13, ...) const, MemoryFunctionInformation_t &out)
	{
		RetType (Y::*mfp2) (Param1, Param2, Param3, Param4, Param5, Param6, Param7, Param8, Param9, Param10, Param11, Param12, Param13, ...) const = mfp;

		MFI_Impl <sizeof (mfp2)>::GetFunctionInformation (mfp2, out);
	}


	template <typename X, typename Y, typename RetType, typename Param1, typename Param2, typename Param3, typename Param4, typename Param5, typename Param6, typename Param7, typename Param8, typename Param9, typename Param10, typename Param11, typename Param12, typename Param13, typename Param14>
		inline void GetFunctionInformation (Y *ptr, RetType (X::*mfp) (Param1, Param2, Param3, Param4, Param5, Param6, Param7, Param8, Param9, Param10, Param11, Param12, Param13, Param14, ...), MemoryFunctionInformation_t &out)
	{
		RetType (Y::*mfp2) (Param1, Param2, Param3, Param4, Param5, Param6, Param7, Param8, Param9, Param10, Param11, Param12, Param13, Param14, ...) = mfp;

		MFI_Impl <sizeof (mfp2)>::GetFunctionInformation (mfp2, out);
	}

	template <typename X, typename Y, typename RetType, typename Param1, typename Param2, typename Param3, typename Param4, typename Param5, typename Param6, typename Param7, typename Param8, typename Param9, typename Param10, typename Param11, typename Param12, typename Param13, typename Param14>
		inline void GetFunctionInformation (Y *ptr, RetType (X::*mfp) (Param1, Param2, Param3, Param4, Param5, Param6, Param7, Param8, Param9, Param10, Param11, Param12, Param13, Param14, ...) const, MemoryFunctionInformation_t &out)
	{
		RetType (Y::*mfp2) (Param1, Param2, Param3, Param4, Param5, Param6, Param7, Param8, Param9, Param10, Param11, Param12, Param13, Param14, ...) const = mfp;

		MFI_Impl <sizeof (mfp2)>::GetFunctionInformation (mfp2, out);
	}


	template <typename X, typename Y, typename RetType, typename Param1, typename Param2, typename Param3, typename Param4, typename Param5, typename Param6, typename Param7, typename Param8, typename Param9, typename Param10, typename Param11, typename Param12, typename Param13, typename Param14, typename Param15>
		inline void GetFunctionInformation (Y *ptr, RetType (X::*mfp) (Param1, Param2, Param3, Param4, Param5, Param6, Param7, Param8, Param9, Param10, Param11, Param12, Param13, Param14, Param15, ...), MemoryFunctionInformation_t &out)
	{
		RetType (Y::*mfp2) (Param1, Param2, Param3, Param4, Param5, Param6, Param7, Param8, Param9, Param10, Param11, Param12, Param13, Param14, Param15, ...) = mfp;

		MFI_Impl <sizeof (mfp2)>::GetFunctionInformation (mfp2, out);
	}

	template <typename X, typename Y, typename RetType, typename Param1, typename Param2, typename Param3, typename Param4, typename Param5, typename Param6, typename Param7, typename Param8, typename Param9, typename Param10, typename Param11, typename Param12, typename Param13, typename Param14, typename Param15>
		inline void GetFunctionInformation (Y *ptr, RetType (X::*mfp) (Param1, Param2, Param3, Param4, Param5, Param6, Param7, Param8, Param9, Param10, Param11, Param12, Param13, Param14, Param15, ...) const, MemoryFunctionInformation_t &out)
	{
		RetType (Y::*mfp2) (Param1, Param2, Param3, Param4, Param5, Param6, Param7, Param8, Param9, Param10, Param11, Param12, Param13, Param14, Param15, ...) const = mfp;

		MFI_Impl <sizeof (mfp2)>::GetFunctionInformation (mfp2, out);
	}


	template <typename X, typename Y, typename RetType, typename Param1, typename Param2, typename Param3, typename Param4, typename Param5, typename Param6, typename Param7, typename Param8, typename Param9, typename Param10, typename Param11, typename Param12, typename Param13, typename Param14, typename Param15, typename Param16>
		inline void GetFunctionInformation (Y *ptr, RetType (X::*mfp) (Param1, Param2, Param3, Param4, Param5, Param6, Param7, Param8, Param9, Param10, Param11, Param12, Param13, Param14, Param15, Param16, ...), MemoryFunctionInformation_t &out)
	{
		RetType (Y::*mfp2) (Param1, Param2, Param3, Param4, Param5, Param6, Param7, Param8, Param9, Param10, Param11, Param12, Param13, Param14, Param15, Param16, ...) = mfp;

		MFI_Impl <sizeof (mfp2)>::GetFunctionInformation (mfp2, out);
	}

	template <typename X, typename Y, typename RetType, typename Param1, typename Param2, typename Param3, typename Param4, typename Param5, typename Param6, typename Param7, typename Param8, typename Param9, typename Param10, typename Param11, typename Param12, typename Param13, typename Param14, typename Param15, typename Param16>
		inline void GetFunctionInformation (Y *ptr, RetType (X::*mfp) (Param1, Param2, Param3, Param4, Param5, Param6, Param7, Param8, Param9, Param10, Param11, Param12, Param13, Param14, Param15, Param16, ...) const, MemoryFunctionInformation_t &out)
	{
		RetType (Y::*mfp2) (Param1, Param2, Param3, Param4, Param5, Param6, Param7, Param8, Param9, Param10, Param11, Param12, Param13, Param14, Param15, Param16, ...) const = mfp;

		MFI_Impl <sizeof (mfp2)>::GetFunctionInformation (mfp2, out);
	}

	inline void *const GetRealAddressOfRelativeAddress8 (const void *const relativeAddress)
	{
		// Reliability check.
		InternalAssert (relativeAddress != NULL);

		return reinterpret_cast <void *> (static_cast <const unsigned char *const> (relativeAddress)[0u] + reinterpret_cast <const unsigned int> (relativeAddress) + Bytecode::Pointer8Size);
	}
	inline void *const GetRelativeAddressOfRealAddress8 (const void *const callAddress, const void *const realAddress)
	{
		// Reliability checks.
		InternalAssert (callAddress != NULL);
		InternalAssert (realAddress != NULL);

		return reinterpret_cast <void *> (reinterpret_cast <const unsigned int> (realAddress) - reinterpret_cast <const unsigned int> (callAddress) - Bytecode::Pointer8Size);
	}
	inline void *const GetRealAddressOfRelativeAddress16 (const void *const relativeAddress)
	{
		// Reliability check.
		InternalAssert (relativeAddress != NULL);

		return reinterpret_cast <void *> (static_cast <const unsigned short *const> (relativeAddress)[0u] + reinterpret_cast <const unsigned int> (relativeAddress) + Bytecode::Pointer16Size);
	}
	inline void *const GetRelativeAddressOfRealAddress16 (const void *const callAddress, const void *const realAddress)
	{
		// Reliability checks.
		InternalAssert (callAddress != NULL);
		InternalAssert (realAddress != NULL);

		return reinterpret_cast <void *> (reinterpret_cast <const unsigned int> (realAddress) - reinterpret_cast <const unsigned int> (callAddress) - Bytecode::Pointer16Size);
	}
	inline void *const GetRealAddressOfRelativeAddress32 (const void *const relativeAddress)
	{
		// Reliability check.
		InternalAssert (relativeAddress != NULL);

		return reinterpret_cast <void *> (static_cast <const unsigned int *const> (relativeAddress)[0u] + reinterpret_cast <const unsigned int> (relativeAddress) + Bytecode::Pointer32Size);
	}
	inline void *const GetRelativeAddressOfRealAddress32 (const void *const callAddress, const void *const realAddress)
	{
		// Reliability checks.
		InternalAssert (callAddress != NULL);
		InternalAssert (realAddress != NULL);

		return reinterpret_cast <void *> (reinterpret_cast <const unsigned int> (realAddress) - reinterpret_cast <const unsigned int> (callAddress) - Bytecode::Pointer32Size);
	}
	template <typename elementType> inline const bool PatchMemoryData (void *const realAddress, const elementType &newValue)
	{
/*		unsigned long oldProtectFlags (0ul);

		if (!::VirtualProtect (realAddress, sizeof (elementType), PAGE_EXECUTE_READWRITE, &oldProtectFlags))*/
		if (!VirtualProtect (realAddress, sizeof (elementType), PAGE_EXECUTE_READWRITE))
			return false;

		static_cast <elementType *const> (realAddress)[0u] = newValue;

//		return oldProtectFlags == PAGE_EXECUTE_READWRITE || VirtualProtect (realAddress, sizeof (elementType), oldProtectFlags);
		return true;
	}
	inline const bool RedirectFunctionJump32 (void *const callAddress, const void *const newFunctionAddress)
	{
		// Reliability checks.
		InternalAssert (callAddress != NULL);
		InternalAssert ((static_cast <unsigned char *const> (callAddress) - sizeof (Bytecode::JumpImmediately32))[0u] == Bytecode::JumpImmediately32);

		return PatchMemoryData <unsigned int> (callAddress, reinterpret_cast <unsigned int> (GetRelativeAddressOfRealAddress32 (callAddress, newFunctionAddress))/* calculate relative offset */);
	}
	inline const bool WriteFunctionJump32 (void *const address, const void *const newFunctionAddress)
	{
		// Reliability checks.
		InternalAssert (address != NULL);

		unsigned char *const tempByteAddress (static_cast <unsigned char *> (address));

		if (!PatchMemoryData <unsigned char> (tempByteAddress, Bytecode::JumpImmediately32))
			return false;

		return RedirectFunctionJump32 (tempByteAddress + sizeof (Bytecode::JumpImmediately32), newFunctionAddress);
	}
	inline const bool RedirectFunctionCall32 (void *const callAddress, const void *const newFunctionAddress)
	{
		// Reliability checks.
		InternalAssert (callAddress != NULL);
		InternalAssert ((static_cast <unsigned char *const> (callAddress) - sizeof (Bytecode::CallImmediately32))[0u] == Bytecode::CallImmediately32);

		return PatchMemoryData <unsigned int> (callAddress, reinterpret_cast <unsigned int> (GetRelativeAddressOfRealAddress32 (callAddress, newFunctionAddress))/* calculate relative offset */);
	}
	inline const bool RedirectFunctionCall32 (void *const callAddress, const void *const newFunctionAddress, const void *&originalFunctionAddress)
	{
		// Save the original function address at first....
		originalFunctionAddress = GetRealAddressOfRelativeAddress32 (callAddress);

		return RedirectFunctionCall32 (callAddress, newFunctionAddress);
	}
	inline const bool RedirectPush32 (void *const pushAddress, const void *const newArgumentAddress)
	{
		// Reliability checks.
		InternalAssert (pushAddress != NULL);
		InternalAssert ((static_cast <unsigned char *const> (pushAddress) - sizeof (Bytecode::Push32))[0u] == Bytecode::Push32);

		return PatchMemoryData <unsigned int> (pushAddress, reinterpret_cast <unsigned int> (newArgumentAddress));
	}
	inline const bool RedirectPush32 (void *const pushAddress, const void *const newArgumentAddress, const void *&originalArgumentAddress)
	{
		// Save the original argument address at first....
		originalArgumentAddress = pushAddress;

		return RedirectPush32 (pushAddress, newArgumentAddress);
	}

	/**
	* Our actual maker of the trampolines!!@$
	* I've no idea why I made this a class and not a namespace.
	* Oh well!
	*/
	class TrampolineMaker
	{
		//
		// Group: Private members.
		//
		private:
			typedef DynamicArray <unsigned char, unsigned char> BufferArray_t;

			BufferArray_t m_buffer;			// the actual buffer containing the assembler code.
			unsigned char m_myStack;		// stack for the trampoline itself.
			unsigned char m_calledStack;	// stack for the target function.

		//
		// Group: (Con/De)structors.
		//
		public:
			inline TrampolineMaker (void) : m_myStack (0u), m_calledStack (0u) { /*VOID*/ }

		//
		// Group: Operators.
		//
		public:
			/**
			* Finalizes the trampoline. Do not try to modify it after this.
			*
			* @return				The trampoline pointer, cast to void *const.
			*/
			inline operator void *const (void) const
			{
				// Reallocate with proper flags...,
				void *const trampoline (Detours::g_codeAllocator.Allocate <void> (m_buffer.GetElementNumber ()));

				#if defined _DEBUG
					// Reliability check.
					if (trampoline == NULL)
					{
						AddLogEntry (true, LogLevel_Error, false, "Couldn't allocate %u bytes of virtual memory for creating new trampoline!", m_buffer.GetElementNumber ());

						return NULL;
					}
				#endif	// if defined _DEBUG

				// copy the main trampoline function...,
				memcpy (trampoline, m_buffer.GetData (), m_buffer.GetElementNumber ());

				// and return it....
				return trampoline;
			}

		//
		// Group: Functions.
		//
		public:
			/**
			* Inserts a breakpoint (int 3) into the trampoline.
			*/
			inline void Breakpoint (void)
			{
				m_buffer.PushBack (Bytecode::Breakpoint);
			}

			/**
			* Adds the "return prologue", pushes registers and prepares stack
			*/
			inline void ReturnPrologue (void)
			{
				m_buffer.PushBack (Bytecode::codeReturnPrologue, sizeof (Bytecode::codeReturnPrologue));
			}

			/**
			* Adds the void prologue pushes registers, prepares the stack and void prologue.
			*/
			inline void VoidPrologue (void)
			{
				m_buffer.PushBack (Bytecode::codeVoidPrologue, sizeof (Bytecode::codeVoidPrologue));
			}

			/**
			* Epilogue for a returning function pops registers but does not free any more of the stack!
			*/
			inline void ReturnEpilogue (void)
			{
				m_buffer.PushBack (Bytecode::codeReturnEpilogue, sizeof (Bytecode::codeReturnEpilogue));
			}

			/**
			* Epilogue that also frees it's estimated stack usage. Useful for stdcall/thiscall/fastcall.
			*/
			inline void ReturnEpilogueAndFree (void)
			{
				ReturnEpilogue (m_myStack);
			}

			/**
			* Return epilogue. Pops registers, and frees given amount of data from the stack.
			*
			* @param howMuch - How many bytes to free from the stack.
			*/
			inline void ReturnEpilogue (const unsigned short howMuch)
			{
				const union /* Unnamed */
				{
					unsigned short integer;
					unsigned char  byte[sizeof (unsigned short)];
				} integerToByte = {howMuch/*, Do not initialize me! */};
				const unsigned char code[sizeof (Bytecode::codeReturnEpilogueN)] =
				{
					Bytecode::PopEBP,															// pop  ebp
					Bytecode::ReturnNearValue, integerToByte.byte[0u], integerToByte.byte[1u]	// retn 'howMuch'
				};

				m_buffer.PushBack (code, sizeof (code));
			}

			/**
			* Void epilogue, pops registers and frees the estimated stack usage of the trampoline.
			*/
			inline void VoidEpilogueAndFree (void)
			{
				VoidEpilogue (m_myStack);
			}
			/**
			* Void epilogue, pops registers, nothing else done with stack.
			*/
			inline void VoidEpilogue (void)
			{
				m_buffer.PushBack (Bytecode::codeVoidEpilogue, sizeof (Bytecode::codeVoidEpilogue));
			}
			/**
			* Void epilogue, pops registers, frees given amount of data off of the stack.
			*
			* @param howMuch - How many bytes to free from the stack.
			*/
			inline void VoidEpilogue (const unsigned short howMuch)
			{
				const union /* Unnamed */
				{
					unsigned short integer;
					unsigned char  byte[sizeof (unsigned short)];
				} integerToByte = {howMuch/*, Do not initialize me! */};
				const unsigned char code[sizeof (Bytecode::codeVoidEpilogueN)] =
				{
					Bytecode::PopRegister,														// pop  eax
					Bytecode::PopEBP,															// pop  ebp
					Bytecode::ReturnNearValue, integerToByte.byte[0u], integerToByte.byte[1u]	// retn 'howMuch'
				};

				m_buffer.PushBack (code, sizeof (code));
				m_buffer.PushBack (Bytecode::codeVoidEpilogueN, sizeof (Bytecode::codeVoidEpilogueN));
			}

			/**
			* Pushes the "this" pointer onto the callee stack. Pushes ECX for MSVC, and param0 on GCC.
			*/
			inline void PushThis (void)
			{
				m_buffer.PushBack (Bytecode::codePushThis);

				// Reliability check.
				InternalAssert (static_cast <unsigned short> (m_calledStack) + Bytecode::Pointer32Size <= static_cast <unsigned char> (-1));

				m_calledStack += Bytecode::Pointer32Size;
			}

			/**
			* Frees what is estimated as the stack usage of the trampoline.
			*/
			inline void FreeMyStack (void)
			{
				FreeStack8 (m_myStack);
			}

			/**
			* Frees the estimated stack usage of the callee.
			*/
			inline void FreeTargetStack (void)
			{
				FreeStack8 (m_calledStack);
			}

			/**
			* Frees the estimated stack usage of the callee and the trampoline.
			*/
			inline void FreeBothStacks (void)
			{
				// Reliability check.
				InternalAssert (static_cast <unsigned short> (m_calledStack) + static_cast <unsigned short> (m_myStack) <= static_cast <unsigned char> (-1));

				FreeStack8 (m_calledStack + m_myStack);
			}

			/**
			* Frees a given amount of bytes from the stack.
			*
			* @param howMuch - How many bytes to free.
			*/
			inline void FreeStack8 (const unsigned char howMuch)
			{
				const unsigned char code[sizeof (Bytecode::codeFreeStack8)] =
				{
					Bytecode::AddESP8[0u], Bytecode::AddESP8[1u], howMuch	// add esp, 'howMuch'
				};

				m_buffer.PushBack (code, sizeof (code));
			}
			inline void FreeStack32 (const unsigned int howMuch)
			{
				const union /* Unnamed */
				{
					unsigned int  integer;
					unsigned char byte[sizeof (unsigned int)];
				} integerToByte = {howMuch/*, Do not initialize me! */};
				const unsigned char code[sizeof (Bytecode::codeFreeStack32)] =
				{
					Bytecode::AddESP32[0u], Bytecode::AddESP32[1u], integerToByte.byte[0u], integerToByte.byte[1u], integerToByte.byte[2u], integerToByte.byte[3u]	// add esp, 'howMuch'
				};

				m_buffer.PushBack (code, sizeof (code));
			}

			/**
			* Pushes a raw number onto the callee stack.
			*
			* @param number - The number to push onto the callee stack.
			*/
			inline void PushNumber (const unsigned int number)
			{
				const union /* Unnamed */
				{
					unsigned int  integer;
					unsigned char byte[sizeof (unsigned int)];
				} integerToByte = {number/*, Do not initialize me! */};
				const unsigned char code[sizeof (Bytecode::codePushID)] =
				{
					Bytecode::Push32, integerToByte.byte[0u], integerToByte.byte[1u], integerToByte.byte[2u], integerToByte.byte[3u]	// push 'number'
				};

				m_buffer.PushBack (code, sizeof (code));

				// Reliability check.
				InternalAssert (static_cast <unsigned short> (m_calledStack) + sizeof (unsigned int) <= static_cast <unsigned char> (-1));

				m_calledStack += sizeof (unsigned int);	// increase auto detected stack size
			}
			inline void PushPointer (const void *const pointer)
			{
				PushNumber (reinterpret_cast <const unsigned int> (pointer));
			}

			/**
			* Takes a parameter passed on the trampoline's stack and inserts it into the callee's stack.
			*
			* @param parameterNumber - The parameter number to push. 1-based. "thiscall" trampolines automatically compensate for the off-number on GCC.
			*/
			inline void PushParameter (const unsigned char parameterNumber, const unsigned char parameterSize = Bytecode::Pointer32Size)
			{
				const unsigned char code[sizeof (Bytecode::codePushParam)] =
				{
					Bytecode::PushEBPWithOffset8[0u], Bytecode::PushEBPWithOffset8[1u], parameterNumber * parameterSize + 4u	// push [ebp+'parameterNumber * parameterSize + 4u']
				};

				m_buffer.PushBack (code, sizeof (code));

				// Reliability checks.
				InternalAssert (static_cast <unsigned short> (m_calledStack) + parameterSize <= static_cast <unsigned char> (-1));
				InternalAssert (static_cast <unsigned short> (m_myStack) + parameterSize <= static_cast <unsigned char> (-1));

				m_calledStack += parameterSize;	// increase auto detected stack size
				m_myStack += parameterSize;
			}
			inline void PushParameters (unsigned char parametersNumber, const unsigned char parameterSize = Bytecode::Pointer32Size)
			{
				do
				{
					PushParameter (parametersNumber, parameterSize);
				} while (--parametersNumber > 0u);
			}

			/**
			* Insert a function to call into the trampoline.
			*
			* @param pointer - The function to call, cast to const void *const.
			*/
			inline void Call (const void *const function)
			{
				const union /* Unnamed */
				{
					const void    *pointer;
					unsigned char  byte[Bytecode::Pointer32Size];
				} pointerToByte = {function/*, Do not initialize me! */};
				const unsigned char code[sizeof (Bytecode::codeCall)] =
				{
					Bytecode::MoveRegisterImmediately, pointerToByte.byte[0u], pointerToByte.byte[1u], pointerToByte.byte[2u], pointerToByte.byte[3u],	// mov  eax, 'pointer'
					Bytecode::CallEAX[0u], Bytecode::CallEAX[1u]																						// call eax
				};

				m_buffer.PushBack (code, sizeof (code));
			}
	};

	inline void *const CreateThisGenericTrampoline (const bool isVoidCall, unsigned char parametersNumber, const void *const extraPointer, const void *const callee)
	{
		// Utility to make a generic trampoline.

		TrampolineMaker trampolineMaker;

		if (isVoidCall)
			trampolineMaker.VoidPrologue ();
		else
			trampolineMaker.ReturnPrologue ();

		if (parametersNumber > 0u)
			trampolineMaker.PushParameters (parametersNumber);

		trampolineMaker.PushThis ();

		trampolineMaker.PushPointer (extraPointer);
		trampolineMaker.Call (callee);
		trampolineMaker.FreeTargetStack ();

		if (isVoidCall)
			trampolineMaker.VoidEpilogueAndFree ();
		else
			trampolineMaker.ReturnEpilogueAndFree ();

		return trampolineMaker;
	}

	inline void *const CreateGenericTrampoline (const bool isVoidCall, unsigned char parametersNumber, const void *const extraPointer, const void *const callee)
	{
		// Utility to make a generic trampoline.

		TrampolineMaker trampolineMaker;

		if (isVoidCall)
			trampolineMaker.VoidPrologue ();
		else
			trampolineMaker.ReturnPrologue ();

		if (parametersNumber > 0u)
			trampolineMaker.PushParameters (parametersNumber);

		trampolineMaker.PushPointer (extraPointer);
		trampolineMaker.Call (callee);
		trampolineMaker.FreeTargetStack ();

		if (isVoidCall)
			trampolineMaker.VoidEpilogueAndFree ();
		else
			trampolineMaker.ReturnEpilogueAndFree ();

		return trampolineMaker;
	}

	inline void *const CreateGenericTrampoline (const bool isThisCall, const bool isVoidCall, const unsigned char parametersNumber, const void *const extraPointer, const void *const callee)
	{
		return isThisCall ? CreateThisGenericTrampoline (isVoidCall, parametersNumber, extraPointer, callee) : CreateGenericTrampoline (isVoidCall, parametersNumber, extraPointer, callee);
	}
}

/**
* @file MemoryUtilities.hpp
* @brief Interface for finding patterns in memory.
*/
namespace MemoryUtilities
{
	struct ModuleInformation_t
	{
		union /* Unnamed */
		{
			void             *baseAddress;
			unsigned char    *buffer;
			HMODULE           module;
			IMAGE_DOS_HEADER *DOSHeader;
		};
		unsigned int          memorySize;

		inline ModuleInformation_t (void) :
			baseAddress (NULL),
			memorySize (0u)
		{ /* VOID */ }
	};

	inline const bool GetModuleInformation (const void *const containedAddress, ModuleInformation_t &dynamicLibraryInformation)
	{
		// Get base address of the module (dynamicLibraryInformation.baseAddress) and get its ending offset (dynamicLibraryInformation.memorySize)
/*
		// First - reset dynamic library information structure....
		dynamicLibraryInformation.baseAddress = NULL;
		dynamicLibraryInformation.memorySize = 0u;
*/
		// Reliability check.
		InternalAssert (containedAddress != NULL);	// GetModuleInformation() failed!

		MEMORY_BASIC_INFORMATION info;

		if (VirtualQuery (containedAddress, &info, sizeof (info)) == 0)
			return false;

		/// @note THIS CHECK IS ADDED FROM CSigMngr.cpp
		InternalAssert (info.AllocationBase != NULL);

		// All this is for our insane sanity checks :O
		const IMAGE_DOS_HEADER *const dos (static_cast <IMAGE_DOS_HEADER *> (info.AllocationBase));
		const IMAGE_NT_HEADERS *const pe (reinterpret_cast <IMAGE_NT_HEADERS *> (reinterpret_cast <unsigned int> (info.AllocationBase) + dos->e_lfanew));

		// Check PE magic and signature
		/// @note IsBadReadPtr() CHECKS ARE ADDED FROM CSigMngr.cpp
		InternalAssert (!IsBadReadPtr (dos, sizeof (IMAGE_DOS_HEADER)));
		InternalAssert (dos->e_magic == IMAGE_DOS_SIGNATURE);
		InternalAssert (!IsBadReadPtr (pe, sizeof (IMAGE_NT_HEADERS)));	// pe points to a bad location?
		InternalAssert (pe->Signature == IMAGE_NT_SIGNATURE);
		InternalAssert (pe->OptionalHeader.Magic == IMAGE_NT_OPTIONAL_HDR32_MAGIC);

		/* Check architecture, which is 32-bit/x86 right now
		* Should change this for 64-bit if Valve gets their act together
		*/
		InternalAssert (pe->FileHeader.Machine == IMAGE_FILE_MACHINE_I386);

		// For our purposes, this must be a dynamic library
//		InternalAssert (pe->FileHeader.Characteristics & IMAGE_FILE_DLL);

		// Finally, we can do this
		dynamicLibraryInformation.baseAddress = info.AllocationBase;
		dynamicLibraryInformation.memorySize = pe->OptionalHeader.SizeOfImage;
#if 0
/// @note FROM AMXX module "Rage":
		HMODULE module;

		if (!GetModuleHandleEx (GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, static_cast <LPCSTR> (containedAddress), &module))
			return false;

		HANDLE process (GetCurrentProcess ());
		_MODULEINFO moduleInfo;

		if (!GetModuleInformation (process, module, &moduleInfo, sizeof (moduleInfo)))
			return false;
		
		CloseHandle (process);

		// Finally, we can do this
		dynamicLibraryInformation.baseAddress = info.lpBaseOfDll;
		dynamicLibraryInformation.memorySize = info.SizeOfImage;
#endif	// if 0
		return true;
	}
	inline const ModuleInformation_t GetModuleInformation (const void *const containedAddress)
	{
		ModuleInformation_t dynamicLibraryInformation;

		#if defined _DEBUG
			// Reliability check.
			InternalAssert (GetModuleInformation (containedAddress, dynamicLibraryInformation));
		#else	// if defined _DEBUG
			GetModuleInformation (containedAddress, dynamicLibraryInformation);
		#endif	// if !defined _DEBUG

		return dynamicLibraryInformation;
	}

	template <typename elementType> inline const bool CompareMemoryWithoutWildCard (const elementType *const startPointer, const elementType *const pattern, const unsigned char length)
	{
		// Reliability checks.
		InternalAssert (length > 0u);
		InternalAssert (!IsBadReadPtr (startPointer, length));

		// Compare each byte, except wildcards....
		for (unsigned char index (0u); startPointer[index] == pattern[index]; /* Empty */)
			if (++index == length)	// If 'index' reached the end, we know we have a match!
				return true;	// Found a match.

		return false;
	}
	template <typename elementType> inline const bool CompareMemory (const elementType *const startPointer, const elementType *const pattern, const unsigned char length, const unsigned char wildCard = Trampolines::Bytecode::WildCard)
	{
		// Reliability checks.
		InternalAssert (length > 0u);
		InternalAssert (!IsBadReadPtr (startPointer, length));

		// Compare each byte, except wildcards....
		for (unsigned char index (0u); pattern[index] == wildCard || startPointer[index] == pattern[index]; /* Empty */)
			if (++index == length)	// If 'index' reached the end, we know we have a match!
				return true;	// Found a match.

		return false;
	}
	template <typename elementType, typename valueElementType> inline const bool CompareMemoryValue (const elementType *const startPointer, const valueElementType &value, const unsigned char wildCard = Trampolines::Bytecode::WildCard)
	{
		// Reliability check.
		CompileTimeAssert (sizeof (value) >= sizeof (elementType));

		return CompareMemory (startPointer, reinterpret_cast <const elementType *> (&value), sizeof (value) / sizeof (elementType), wildCard);
	}

	/**
	* @brief Searches for a pattern of bytes within the memory of a dynamic library.
	*
	* @param startPointer:	Pointer to memory to search for.
	* @param searchLength:	Length of sequence of memory to search for.
	* @param pattern:		Pattern of bytes to search for. 0x2A can be used as a wildcard.
	* @param length:		Size of the pattern in bytes.
	* @return				Pointer to pattern found in memory, NULL if not found.
	*/
	inline void *const FindPattern (void *startPointer, const unsigned int searchLength, const unsigned char *const pattern, const unsigned char length, const unsigned char stepSize = sizeof (unsigned char), const unsigned char wildCard = Trampolines::Bytecode::WildCard)
	{
		// Scan for the signature in memory then return the starting position's address.

		// Reliability checks.
		InternalAssert (startPointer != NULL);
		InternalAssert (searchLength >= length);
		InternalAssert (length >= 2u);

		const unsigned char *const endPointer (static_cast <unsigned char *> (startPointer) + searchLength);	// prevent a crash maybe?

		// Reliability check.
		InternalAssert (endPointer != NULL);

		do
		{
			// Compare each byte, except wildcards....
			if (CompareMemory (static_cast <unsigned char *> (startPointer), pattern, length, wildCard))
				return startPointer;	// Found a match.
		} while ((reinterpret_cast <unsigned char *&> (startPointer) += stepSize/* search memory in an aligned manner, or not */) + length <= endPointer);	// prevent a crash maybe?

		return NULL;
	}
	inline void *const FindPattern (const LibraryHandleWithSize &library, const unsigned char *const pattern, const unsigned char length, const unsigned char stepSize = sizeof (unsigned char), const unsigned char wildCard = Trampolines::Bytecode::WildCard)
	{
		return FindPattern (library.GetPointer (), library.GetSize (), pattern, length, stepSize, wildCard);
	}
	inline void *const FindPattern (void *const startPointer, const unsigned int searchLength, const unsigned char *const pattern)
	{
		return FindPattern (startPointer, searchLength, pattern, GetStringLength <unsigned char> (reinterpret_cast <const char *const> (pattern)));
	}
	inline void *const FindPattern (const LibraryHandleWithSize &library, const unsigned char *const pattern)
	{
		return FindPattern (library.GetPointer (), library.GetSize (), pattern, GetStringLength <unsigned char> (reinterpret_cast <const char *const> (pattern)));
	}
	inline void *const FindPatternWithoutWildCard (void *startPointer, const unsigned int searchLength, const unsigned char *const pattern, const unsigned char length, const unsigned char stepSize = sizeof (unsigned char))
	{
		// Scan for the signature in memory then return the starting position's address.

		// Reliability checks.
		InternalAssert (startPointer != NULL);
		InternalAssert (searchLength >= length);
		InternalAssert (length >= 2u);

		const unsigned char *const endPointer (static_cast <unsigned char *> (startPointer) + searchLength);	// prevent a crash maybe?

		// Reliability check.
		InternalAssert (endPointer != NULL);

		do
		{
			// Compare each byte, except wildcards....
			if (CompareMemoryWithoutWildCard (static_cast <unsigned char *> (startPointer), pattern, length))
				return startPointer;	// Found a match.
		} while ((reinterpret_cast <unsigned char *&> (startPointer) += stepSize/* search memory in an aligned manner, or not */) + length <= endPointer);	// prevent a crash maybe?

		return NULL;
	}
	inline void *const FindPatternWithoutWildCard (const LibraryHandleWithSize &library, const unsigned char *const pattern, const unsigned char length, const unsigned char stepSize = sizeof (unsigned char))
	{
		return FindPatternWithoutWildCard (library.GetPointer (), library.GetSize (), pattern, length, stepSize);
	}
	inline void *const DecodeAndFindPattern (void *const startPointer, const unsigned int searchLength, const unsigned char *const pattern)
	{
		// First, preprocess the signature
		unsigned char signature[static_cast <unsigned char> (-1) + 1u];
		const unsigned char signatureLength (static_cast <const unsigned char> (DecodeHexString (signature, sizeof (signature), reinterpret_cast <const char *const> (pattern))));

		if (signatureLength < 1u)
			return NULL;

		return FindPattern (startPointer, searchLength, signature, signatureLength);
	}
	inline void *const DecodeAndFindPattern (void *const startPointer, const unsigned int searchLength, const unsigned char *const pattern, const unsigned char length)
	{
		// First, preprocess the signature
		unsigned char signature[static_cast <unsigned char> (-1) + 1u];
		const unsigned char signatureLength (static_cast <const unsigned char> (DecodeHexString (signature, sizeof (signature), reinterpret_cast <const char *const> (pattern), length)));

		if (signatureLength < 1u)
			return NULL;

		return FindPattern (startPointer, searchLength, signature, signatureLength);
	}

	inline void *const FindMemoryChunkReference32 (void *const startPointer, const unsigned int searchLength, const void *const address, const unsigned char instruction)
	{
		const union /* Unnamed */
		{
			const void    *pointer;
			unsigned char  byte[Trampolines::Bytecode::Pointer32Size];
		} pointerToByte = {address/*, Do not initialize me! */};
		const unsigned char pattern[sizeof (instruction) + Trampolines::Bytecode::Pointer32Size] =
		{
			instruction, pointerToByte.byte[0u], pointerToByte.byte[1u], pointerToByte.byte[2u], pointerToByte.byte[3u]	// instruction 'address'
		};

		return FindPatternWithoutWildCard (startPointer, searchLength, pattern, sizeof (pattern));
	}
	inline void *const FindMemoryChunkReference32 (const LibraryHandleWithSize &library, const void *const address, const unsigned char instruction)
	{
		return FindMemoryChunkReference32 (library.GetPointer (), library.GetSize (), address, instruction);
	}
	inline void *const FindMemoryChunkReference32 (void *const startPointer, const unsigned int searchLength, const void *const address, const unsigned char *const instruction, const unsigned char instructionLength)
	{
		const union /* Unnamed */
		{
			const void    *pointer;
			unsigned char  byte[Trampolines::Bytecode::Pointer32Size];
		} pointerToByte = {address/*, Do not initialize me! */};
		unsigned char pattern[static_cast <unsigned char> (-1)];

		MemoryCopy (instruction, pattern, instructionLength);
		MemoryCopy (pointerToByte.byte, pattern + instructionLength, Trampolines::Bytecode::Pointer32Size);

		return FindPatternWithoutWildCard (startPointer, searchLength, pattern, instructionLength + Trampolines::Bytecode::Pointer32Size);
	}
	inline void *const FindMemoryChunkReference32 (const LibraryHandleWithSize &library, const void *const address, const unsigned char *const instruction, const unsigned char instructionLength)
	{
		return FindMemoryChunkReference32 (library.GetPointer (), library.GetSize (), address, instruction, instructionLength);
	}

	/**
	* @brief Searches for a pattern of bytes within the memory of a dynamic library.
	*
	* @param libraryPointer:	Pointer to any chunk of memory that resides in the dynamic library.
	* @param pattern:			Pattern of bytes to search for. 0x2A can be used as a wildcard.
	* @param length:			Size of the pattern in bytes.
	* @return					Pointer to pattern found in memory, NULL if not found.
	*/
	inline void *const FindPatternByMemoryChunkOfModule (const void *const libraryPointer, const unsigned char *const pattern, const unsigned char length)
	{
		// Scan for the signature in memory then return the starting position's address.

		ModuleInformation_t dynamicLibraryInformation;

		#if defined _DEBUG
			// Reliability check.
			InternalAssert (GetModuleInformation (libraryPointer, dynamicLibraryInformation));
		#else	// if define _DEBUG
			if (!GetModuleInformation (libraryPointer, dynamicLibraryInformation))
				return NULL;
		#endif	// if !defined _DEBUG

		return FindPattern (dynamicLibraryInformation.baseAddress, dynamicLibraryInformation.memorySize, pattern, length);
	}
	inline void *const FindPatternByMemoryChunkOfModule (const void *const libraryPointer, const unsigned char *const pattern)
	{
		return FindPatternByMemoryChunkOfModule (libraryPointer, pattern, GetStringLength <unsigned char> (reinterpret_cast <const char *const> (pattern)));
	}
	inline void *const DecodeAndFindPatternByMemoryChunkOfModule (const void *const libraryPointer, const unsigned char *const pattern)
	{
		// First, preprocess the signature
		unsigned char signature[static_cast <unsigned char> (-1) + 1u];
		const unsigned char signatureLength (static_cast <const unsigned char> (DecodeHexString (signature, sizeof (signature), reinterpret_cast <const char *const> (pattern))));

		if (signatureLength < 1u)
			return NULL;

		return FindPatternByMemoryChunkOfModule (libraryPointer, signature, signatureLength);
	}
	inline void *const DecodeAndFindPatternByMemoryChunkOfModule (const void *const libraryPointer, const unsigned char *const pattern, const unsigned char length)
	{
		// First, preprocess the signature
		unsigned char signature[static_cast <unsigned char> (-1) + 1u];
		const unsigned char signatureLength (static_cast <const unsigned char> (DecodeHexString (signature, sizeof (signature), reinterpret_cast <const char *const> (pattern), length)));

		if (signatureLength < 1u)
			return NULL;

		return FindPatternByMemoryChunkOfModule (libraryPointer, signature, signatureLength);
	}

	static HMODULE GetThisModuleHandle (void)
	{
		MEMORY_BASIC_INFORMATION mbi;

		if (VirtualQuery (&GetThisModuleHandle, &mbi, sizeof (mbi)) == 0)
			return NULL;

		return static_cast <HMODULE> (mbi.AllocationBase);
//		return g_YaPBModuleHandle;
	}
	inline const DynamicString GetModuleName (const HMODULE module)
	{
		char name[MAX_PATH];

		GetModuleFileName (module, name, sizeof (name));

		return name;
	}
	inline const DynamicString GetThisModuleName (void) { return GetModuleName (GetThisModuleHandle ()); }
	inline const DynamicString GetThisModuleDirectory (void)
	{
		DynamicString name (GetThisModuleName ());
		const DynamicString::IndexType_t index (name.ReverseFindWithCaseCheck ('\\'));

		if (index != DynamicString::BadPosition)
			name.ReleaseBuffer (index);	// Remove filename....

		return name;
	}
	inline const DynamicString GetThisProcessName (void) { return GetModuleName (GetModuleHandle (NULL)); }

	template <typename elementType> inline elementType *const GetElementsTable (const void *const address)
	{
		// Reliability check.
		InternalAssert (address != NULL);

		elementType *const *const tableOfPointersTables = static_cast <elementType *const *const> (address);

		// Reliability check.
		InternalAssert (tableOfPointersTables != NULL);

		return tableOfPointersTables[0u];
	}

	template <typename elementType> inline void SetOffsetAddressReference (void *const address, const int offset, const elementType &newValue)
	{
		// Reliability check.
		InternalAssert (address != NULL);

		(static_cast <elementType *const> (address) + offset)[0u] = newValue;
	}
	template <typename elementType> inline const elementType &GetOffsetAddressReference (const void *const address, const int offset)
	{
		// Reliability check.
		InternalAssert (address != NULL);

		return (static_cast <const elementType *const> (address) + offset)[0u];
	}

	inline const void *const GetAddressFromSignature (const unsigned char *const signature, const unsigned char length)
	{
		return FindPattern (*HalfLifeEngine::Globals::g_halfLifeEngine->GetGameLibrary (), signature, length);
	}
	inline const void *const GetAddressFromSignature (const unsigned char *const signature)
	{
		return GetAddressFromSignature (signature, GetStringLength <unsigned char> (reinterpret_cast <const char *const> (signature)));
	}
	template <typename elementType> inline elementType *const GetPointerAddressFromOtherAddress (const void *const address)
	{
		// Reliability check.
		InternalAssert (address != NULL);

		// We've passed all tests...
		return GetElementsTable <elementType> (address);
	}
	template <typename elementType> inline elementType *const GetPointerAddressFromOtherAddressAndOffset (const void *const address, const short offset)
	{
		// Reliability check.
		InternalAssert (address != NULL);

		// We've passed all tests...
		return GetElementsTable <elementType> (static_cast <const unsigned char *const> (address) + offset);
	}
	template <typename elementType> inline elementType &GetElementAddressFromOtherAddress (const void *const address)
	{
		// Reliability check.
		InternalAssert (address != NULL);

		elementType *const elementsTable = GetPointerAddressFromOtherAddress <elementType> (address);

		// Reliability check.
		InternalAssert (elementsTable != NULL);

		// We've passed all tests...
		return elementsTable[0u];
	}
	template <typename elementType> inline elementType &GetElementAddressFromOtherAddressAndOffset (const void *const address, const short offset)
	{
		// Reliability check.
		InternalAssert (address != NULL);

		elementType *const elementsTable = GetPointerAddressFromOtherAddressAndOffset <elementType> (address, offset);

		// Reliability check.
		InternalAssert (elementsTable != NULL);

		// We've passed all tests...
		return elementsTable[0u];
	}
	template <typename elementType> inline elementType &GetElementAddressFromSignatureAndOffset (const unsigned char *const signature, const unsigned char length, const short offset)
	{
		return GetElementAddressFromOtherAddressAndOffset <elementType> (GetAddressFromSignature (signature, length), offset);
	}
	template <typename elementType> inline elementType &GetElementAddressFromSignatureAndOffset (const unsigned char *const signature, const short offset)
	{
		return GetElementAddressFromSignatureAndOffset <elementType> (signature, GetStringLength <unsigned char> (reinterpret_cast <const char *const> (signature)), offset);
	}
}

namespace CoreToolLibrary
{
	inline LibraryHandle::LibraryHandle (const MemoryUtilities::ModuleInformation_t &dynamicLibraryInformation) :
		m_module (dynamicLibraryInformation.module)
	{ /* VOID */ }

	inline LibraryHandle &LibraryHandle::operator = (const MemoryUtilities::ModuleInformation_t &dynamicLibraryInformation)
	{
		m_module = dynamicLibraryInformation.module;

		return *this;
	}

	/// @todo OPTIMIZE ME!!!
	inline LibraryHandleWithSize::LibraryHandleWithSize (HMODULE pointer) :
		LibraryHandle (pointer),

		m_size (MemoryUtilities::GetModuleInformation (pointer).memorySize)
	{ /* VOID */ }
	inline LibraryHandleWithSize::LibraryHandleWithSize (const LibraryHandle &other) :
		LibraryHandle (other.GetPointer ()),

		m_size (MemoryUtilities::GetModuleInformation (other.GetPointer ()).memorySize)
	{ /* VOID */ }
	inline LibraryHandleWithSize::LibraryHandleWithSize (const MemoryUtilities::ModuleInformation_t &dynamicLibraryInformation) :
		LibraryHandle (dynamicLibraryInformation),

		m_size (dynamicLibraryInformation.memorySize)
	{ /* VOID */ }

	inline LibraryHandleWithSize &LibraryHandleWithSize::operator = (HMODULE pointer)
	{
		m_module = pointer;

		// Reliability check.
		InternalAssert (IsValid ());

		m_size = MemoryUtilities::GetModuleInformation (pointer).memorySize;

		return *this;
	}
	inline LibraryHandleWithSize &LibraryHandleWithSize::operator = (const MemoryUtilities::ModuleInformation_t &dynamicLibraryInformation)
	{
		m_module = dynamicLibraryInformation.module;
		m_size = dynamicLibraryInformation.memorySize;

		return *this;
	}
}

namespace Trampolines
{
	inline const unsigned int RedirectFunctionCalls32 (void *startPointer, const unsigned int searchLength, const void *const originalFunctionRealAddress, const void *const newFunctionAddress, const unsigned char stepSize = sizeof (unsigned char))
	{
		// Reliability checks.
		InternalAssert (startPointer != NULL);
		InternalAssert (searchLength >= Bytecode::CallImmediately32Size);

		const unsigned char *const endPointer (static_cast <unsigned char *> (startPointer) + searchLength);
		unsigned int count (0u);

		do
		{
			// Try to find call instruction....
			if (static_cast <unsigned char *> (startPointer)[0u] != Bytecode::CallImmediately32)
				continue;

			// Skip call instruction....
			reinterpret_cast <unsigned char *&> (startPointer) += sizeof (Bytecode::CallImmediately32);

			// Compare addresses of functions....
			if (GetRealAddressOfRelativeAddress32 (startPointer) != originalFunctionRealAddress)
				continue;

			// Replace this function with our's new function....
			RedirectFunctionCall32 (startPointer, newFunctionAddress);

			// Skip replaced function address....
			reinterpret_cast <unsigned char *&> (startPointer) += Bytecode::Pointer32Size;

			// Increment debug counting....
			++count;
		} while ((reinterpret_cast <unsigned char *&> (startPointer) += stepSize/* search memory in an aligned manner, or not */) + Bytecode::CallImmediately32Size <= endPointer);	// prevent a crash maybe?

		return count;
	}
}

inline void Log::SetDefaultDirectory (void)
{
	m_directory = MemoryUtilities::GetThisModuleName ();

	StripFilenameAndPathSeparator (m_directory);
}

class HooksManager
{
	public:
#if 0
	struct AMX;
	typedef int cell;
	enum ForwardParam
	{
		FP_DONE = -1,					// specify this as the last argument
										// only tells the function that there are no more arguments
		FP_CELL,						// normal cell
		FP_FLOAT,						// float; used as normal cell though
		FP_STRING,						// string
		FP_STRINGEX,					// string; will be updated to the last function's value
		FP_ARRAY,						// array; use the return value of prepareArray.
	};

// FROM Orpheu
//{
long Hook       (long argument, ...);
long HookMethod (long object, long argument, ...);

long _stdcall HookMethod_0()
{
	long object;
	__asm { mov object, ecx; }
	return HookMethod(object,NULL);
}
long _stdcall HookMethod_1(long argument)
{
	long object;
	__asm { mov object, ecx; }
	return HookMethod(object,argument);
}
long _stdcall HookMethod_2(long argument,long argument2)
{
	long object;
	__asm { mov object, ecx; }
	return HookMethod(object,argument,argument2);
}
long _stdcall HookMethod_3(long argument,long argument2,long argument3)
{
	long object;
	__asm { mov object, ecx; }
	return HookMethod(object,argument,argument2,argument3);
}
long _stdcall HookMethod_4(long argument,long argument2,long argument3,long argument4)
{
	long object;
	__asm { mov object, ecx; }
	return HookMethod(object,argument,argument2,argument3,argument4);
}
long _stdcall HookMethod_5(long argument,long argument2,long argument3,long argument4,long argument5)
{
	long object;
	__asm { mov object, ecx; }
	return HookMethod(object,argument,argument2,argument3,argument4,argument5);
}
long _stdcall HookMethod_6(long argument,long argument2,long argument3,long argument4,long argument5,long argument6)
{
	long object;
	__asm { mov object, ecx; }
	return HookMethod(object,argument,argument2,argument3,argument4,argument5,argument6);
}
long _stdcall HookMethod_7(long argument,long argument2,long argument3,long argument4,long argument5,long argument6,long argument7)
{
	long object;
	__asm { mov object, ecx; }
	return HookMethod(object,argument,argument2,argument3,argument4,argument5,argument6,argument7);
}
long _stdcall HookMethod_8(long argument,long argument2,long argument3,long argument4,long argument5,long argument6,long argument7,long argument8)
{
	long object;
	__asm { mov object, ecx; }
	return HookMethod(object,argument,argument2,argument3,argument4,argument5,argument6,argument7,argument8);
}
long _stdcall HookMethod_9(long argument,long argument2,long argument3,long argument4,long argument5,long argument6,long argument7,long argument8,long argument9)
{
	long object;
	__asm { mov object, ecx; }
	return HookMethod(object,argument,argument2,argument3,argument4,argument5,argument6,argument7,argument8,argument9);
}
long _stdcall HookMethod_10(long argument,long argument2,long argument3,long argument4,long argument5,long argument6,long argument7,long argument8,long argument9,long argument10)
{
	long object;
	__asm { mov object, ecx; }
	return HookMethod(object,argument,argument2,argument3,argument4,argument5,argument6,argument7,argument8,argument9,argument10);
}
long _stdcall HookMethod_11(long argument,long argument2,long argument3,long argument4,long argument5,long argument6,long argument7,long argument8,long argument9,long argument10,long argument11)
{
	long object;
	__asm { mov object, ecx; }
	return HookMethod(object,argument,argument2,argument3,argument4,argument5,argument6,argument7,argument8,argument9,argument10,argument11);
}
long _stdcall HookMethod_12(long argument,long argument2,long argument3,long argument4,long argument5,long argument6,long argument7,long argument8,long argument9,long argument10,long argument11,long argument12)
{
	long object;
	__asm { mov object, ecx; }
	return HookMethod(object,argument,argument2,argument3,argument4,argument5,argument6,argument7,argument8,argument9,argument10,argument11,argument12);
}

void *const HooksMethod[] =
{
	&HookMethod_0,
	&HookMethod_1,
	&HookMethod_2,
	&HookMethod_3,
	&HookMethod_4,
	&HookMethod_5,
	&HookMethod_6,
	&HookMethod_7,
	&HookMethod_8,
	&HookMethod_9,
	&HookMethod_10,
	&HookMethod_11,
	&HookMethod_12
};

const unsigned char patch[] = {Bytecode::JumpImmediately32, 0x00, 0x00, 0x00, 0x00};
const unsigned char patchSize = sizeof (patch);
//unsigned short *patchFunctionIDAddress = (unsigned short*) (patch + 2);
//long *patchFunctionJumpAddress = (long*) (patch + 1);

/*const */unsigned short g_ID;

class Function
{
	public:

class TypeHandler
{
	public:
		enum ConvertMode
		{
			Call,
			CallHooked,
			Pass
		};

	private:
		typedef DynamicArray <void *> AllocatedMemoryPointersArray_t;
		AllocatedMemoryPointersArray_t m_allocatedMemoryPointers;

	protected:
		long m_virtualTableOffset;
		bool m_virtualTableOffsetSet;

	public:
		inline TypeHandler (void) : m_virtualTableOffsetSet (false) { /* VOID */ }
		virtual inline ~TypeHandler (void) { /* VOID */ }

	public:
		virtual inline void *const ConvertFromAmx    (AMX *amx, const cell param) = 0;
		virtual inline void        ConvertToAmx      (cell &value, const long standardReturn, const ConvertMode convertMode = Pass) = 0;
		virtual inline void        ConvertToAmxByRef (AMX *amx, cell *const params, const long standardReturn) { /* VOID */ }

		virtual inline       void ConvertFromAmxToStructure (AMX *amx, const cell param, void *const address) = 0;
		virtual inline const cell ConvertToAmxFromStructure (AMX *amx, cell *const params, void *const address) = 0;

		virtual inline const unsigned int GetIfByRefParamsCount (void) const { return 0u; }
		virtual inline const ForwardParam GetParamType          (void) const { return FP_CELL; }
		virtual inline const char *const  GetName               (void) const = 0;
		virtual inline const unsigned int GetSize               (AMX *amx, const cell param) const = 0;
		virtual inline const bool         IsPassedByRef         (void) const { return false; }
		virtual inline const bool         IsStructure           (void) const { return false; }
		virtual inline const bool         IsPointer             (void) const { return false; }

		virtual inline const bool CompareTo (AMX *amx, cell *const params, void *const address)
		{
			return *MF_GetAmxAddr (amx, params[0u]) == ConvertToAmxFromStructure (amx, params, address);
		}

		inline void SetVirtualTableOffset (const long offset)
		{
			m_virtualTableOffsetSet = true;
			m_virtualTableOffset = offset;
		}
		inline long &GetVirtualTableOffset (void)
		{
			return m_virtualTableOffset;
		}

		inline const bool HasVirtualTableOffset (void) const
		{
			return m_virtualTableOffsetSet;
		}

		inline void FreeAllocatedMemory (void)
		{
			for (AllocatedMemoryPointersArray_t::IndexType_t index (0u); index < m_allocatedMemoryPointers.GetElementNumber (); ++index)
				delete [] m_allocatedMemoryPointers[index];

			m_allocatedMemoryPointers.RemoveAll ();
		}

	protected:
		template <typename elementType> inline void ConvertFromAmxToStructureStandard (AMX *amx, const cell param, void *const address)
		{
			*reinterpret_cast <elementType *> (address) = reinterpret_cast <elementType> (reinterpret_cast <long> (ConvertFromAmx (amx, param)));
		}
		template <typename elementType> inline const cell ConvertToAmxFromStructureStandard (AMX *amx, cell *const params, void *const address)
		{
			cell value;

			ConvertToAmx (value, *reinterpret_cast <elementType *> (address));

			return value;
		}

		template <typename elementType> inline elementType *const AllocateMemory (const unsigned int numElements = 1u)
		{
			elementType *const pointer (new elementType[numElements]);

			m_allocatedMemoryPointers += static_cast <void *> (pointer);

			return pointer;
		}
		inline void *const AllocateMemoryBySize (const unsigned int size)
		{
			void *const pointer (new unsigned char[size]);

			m_allocatedMemoryPointers += pointer;

			return pointer;
		}
};
class Bool_TypeHandler : public TypeHandler
{
	public:
		inline void *const ConvertFromAmx (AMX *amx, const cell param)
		{
			cell *const value (AllocateMemory <cell> ());

			*value = *MF_GetAmxAddr (amx, param);

			return reinterpret_cast <void *> (*value);
		}
		inline void ConvertToAmx (cell &value, const long standardReturn, const ConvertMode convertMode)
		{
			value = static_cast <cell> (static_cast <bool> (standardReturn));
		}
		inline void ConvertFromAmxToStructure (AMX *amx, const cell param, void *const address)
		{
			ConvertFromAmxToStructureStandard <bool> (amx, param, address);
		}
		inline const cell ConvertToAmxFromStructure (AMX *amx, cell *const params, void *const address)
		{
			return ConvertToAmxFromStructureStandard <bool> (amx, params, address);
		}

		inline const unsigned int GetSize (AMX *amx, const cell param) const
		{
			return sizeof (bool);
		}

		inline const char *const GetName (void) const { return "bool"; }
};

	private:
		unsigned char m_originalBytes[patchSize];
		unsigned char m_patchedBytes[patchSize];
		unsigned char m_argumentsCount;
		void *m_address;
		bool m_isMethod;
		unsigned char m_ifReturnByRefParamsCount;
		long *m_normalArguments;

		bool m_isPatched;

		long m_espDislocation;
		long m_espDislocationAddHook;
		long m_espDislocationCallForward;
		long m_espDislocationCall;

		const unsigned char PreJumpBase[] = {0x66, 0xC7, 0x5, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, Bytecode::JumpImmediately32, 0x00, 0x00, 0x00, 0x00};
		const unsigned char PreJumpSize = sizeof (PreJumpBase);
		const unsigned char PreJumpIDAddressPosition = 3u;
		const unsigned char PreJumpIDValuePosition = 7u;
		const unsigned char PreJumpIDHookAddressPosition = 10u;

		unsigned char m_preJump[PreJumpSize];

	private:
		typedef void (__thiscall Function::*MemberFunction_t) (void);
		MemberFunction_t m_passObject;

		inline void PassObjectMethod (void)
		{
			long object (m_normalArguments[0u]);

			__asm { mov ecx, object; }
		}
		inline void PassObjectNonMethod (void) { /* VOID */ }

	public:
		inline Function (void *const address, const unsigned int argumentsCount, const bool isMethod, TypeHandler **argumentsHandlers, TypeHandler *returnHandler) :
			m_address (address),
			m_normalArguments (new long[argumentsCount]),
			m_argumentsCount (argumentsCount),
			m_isMethod (isMethod),
			m_ifReturnByRefParamsCount (returnHandler != NULL ? returnHandler->GetIfByRefParamsCount () : 0u),

			m_espDislocation (sizeof (long) * argumentsCount),
			m_espDislocationAddHook (m_espDislocation + sizeof (long) + sizeof (char *) + sizeof (AMX *)),
			m_espDislocationCallForward (m_espDislocation + sizeof (long)),
			m_espDislocationCall (isMethod ? 0 : m_espDislocation),

			m_passObject (isMethod ? &Function::PassObjectMethod : &Function::PassObjectNonMethod),

			m_isPatched (false)
		{ /* VOID */ }
		inline ~Function (void) { delete [] m_normalArguments; }

	public:
		inline void PreparePatch (void)
		{
			static unsigned short id (0u);

			memcpy (m_preJump, PreJumpBase, PreJumpSize);

			reinterpret_cast <long *> (m_preJump + PreJumpIDAddressPosition)[0u] = reinterpret_cast <long> (&g_ID);
			reinterpret_cast <unsigned short *> (m_preJump + PreJumpIDValuePosition)[0u] = id++;

			if (m_isMethod)
				reinterpret_cast <long *> (m_preJump + PreJumpIDHookAddressPosition)[0u] = reinterpret_cast <char *> (HooksMethod[m_argumentsCount - 1u]) - reinterpret_cast <char *> (m_preJump) - PreJumpSize;
			else
				reinterpret_cast <long *> (m_preJump + PreJumpIDHookAddressPosition)[0u] = reinterpret_cast <char *> (&Hook) - reinterpret_cast <char *> (m_preJump) - PreJumpSize;

			m_patchedBytes[0u] = Bytecode::JumpImmediately32;
			reinterpret_cast <long *> (m_patchedBytes + 1)[0u] = reinterpret_cast <char *> (m_preJump) - reinterpret_cast <char *> (m_address) - Bytecode::JumpImmediately32Size;

			memcpy (m_originalBytes, m_address, patchSize);
		}
		inline void DoPatch (void)
		{
			if (m_isPatched)
				return;

			if (!Trampolines::VirtualProtect (m_address, patchSize, PAGE_EXECUTE_READWRITE))
			{
				AddLogEntry (true, LogLevel_Error, true, "Memory patching failed!");

				return;
			}

			memcpy (m_address, m_patchedBytes, patchSize);

			m_isPatched = true;
		}
		inline void UndoPatch (void)
		{
			if (!m_isPatched)
				return;

			if (!Trampolines::VirtualProtect (m_address, patchSize, PAGE_EXECUTE_READWRITE))
			{
				AddLogEntry (true, LogLevel_Error, true, "Memory patching failed!");

				return;
			}

			memcpy (m_address, m_originalBytes, patchSize);

			m_isPatched = false;
		}
		inline void DoHook (void)
		{
			DoPatch ();
/*
	unsigned int start = m_argumentsCount;
	long addressStart = (long) parameters;
	long espDislocation = m_espDislocationAddHook;

	__asm
	{
		mov eax, addressStart
		mov ebx, start
		push FP_DONE
		loopStartAdd:
		cmp ebx, 0
		je loopEndAdd
		dec ebx
		mov ecx, [eax + ebx * 4u]
		push ecx
		jmp loopStartAdd
		loopEndAdd:
		push functionName
		push amx
		call MF_RegisterSPForwardByName
		mov forward, eax
		add esp, espDislocation
	}*/
		}
		inline long CallOriginal (void)
		{
			// Reverse push arguments into stack
			for (char i (m_argumentsCount - 1); i >= m_isMethod; --i)
			{
				long argument (m_normalArguments[i]);

				__asm { push argument; }
			}

			long returnValue;

			(this->*m_passObject) ();

			__asm
			{
				call m_address;
				mov returnValue, eax;
			}

			__asm { add esp, m_espDislocationCall; }

			return returnValue;
		}
		inline long BeginHook (void)
		{
			int hookReturnValue = 0;

			// Call PRE hooks here....
			{
				//
			}

			{
				UndoPatch ();
				{
					const long returnValue (CallOriginal ());	// Original function call....
				}
				DoPatch ();

				{
					hookReturnValue = returnValue;

//					if (!m_isVoid && m_ifReturnByRefParamsCount == 0u)
//						returnHandler->ConvertToAmx (hookReturnValueConverted, hookReturnValue, CallHooked);
				}
			}

			// Call POST hooks here....
			{
				//
			}

			return hookReturnValue;
		}
};

long Hook(long argument,...)
{
	Function *function = Global::FunctionManagerObj->getFunction(ID);
	long *argumentsHolder = function->m_normalArguments;

	va_list argumentPointer;
	va_start(argumentPointer,argument);

	for(unsigned char i=0;i<function->m_argumentsCount;++i)
	{
		argumentsHolder[i] = argument;
		argument = va_arg(argumentPointer,long);
	}

	va_end(argumentPointer);

	return function->BeginHook ();
}

long HookMethod(long object,long argument,...)
{
	Function *function = Global::FunctionManagerObj->getFunction(ID);
	long *argumentsHolder = function->m_normalArguments;

	argumentsHolder[0] = object;

	va_list argumentPointer;
	va_start(argumentPointer,argument);

	for(unsigned char i=1;i<function->m_argumentsCount;++i)
	{
		argumentsHolder[i] = argument;
		argument = va_arg(argumentPointer,long);
	}

	va_end(argumentPointer);

	return function->BeginHook ();
}
//}
#endif	// if 0

// FROM_ANDREY
//{
struct hook_data;

typedef unsigned int (*HOOK_PROC) (hook_data/*void*/ *, unsigned int *);

#pragma pack(push, 1)
struct hook_data
{
// -5 - address of E8 instruction
	unsigned char code_call;
// -1 - address which leads to the hook function
	unsigned int hook_offset;
// -0
	unsigned char code_ret;
// here - begin real hook data.
// 0x01 - number of arguments of the called function. CommonHookProc() should clean them from the stack
	unsigned short count_args4;
// 0x03 - original 5 bytes of function on which we set hook
	unsigned int code_store[2u];	// actually changing 5 bytes, the remaining 3 - to align
// 0x0B - pointer to the original function
	void *originalFunction;

// 0x0f - address the real hook function
	HOOK_PROC pfn_hook;
};
#pragma pack(pop)

static inline void CommonHookProc (void);

inline void set_hook (void *const pfn, const void *const pfn_new, const unsigned char count_args)
{
	InternalAssert (pfn != NULL);
	InternalAssert (pfn_new != NULL);

				// Reallocate with proper flags...,
				hook_data *const hook (Detours::g_codeAllocator.Allocate <hook_data> (sizeof (hook_data)));

				#if defined _DEBUG
					// Reliability check.
					if (hook == NULL)
					{
						AddLogEntry (true, LogLevel_Error, false, "Couldn't allocate %u bytes of virtual memory for creating new hook!", sizeof (hook_data));

						return;
					}
				#endif	// if defined _DEBUG

//	void *ptr_mem = (void*)((unsigned int)pfn & ~0xFFF);
	if (!Trampolines::VirtualProtect (/*ptr_mem/*/pfn, /*4096/*/sizeof (hook_data), PAGE_EXECUTE_READWRITE))
	{
		// Free memory used...
		Detours::g_codeAllocator.Free (hook);

		AddLogEntry (false, LogLevel_Critical, true, "VirtualProtect() failed!\nError code: %i.", GetLastError ());

		return;
	}

//	InternalAssert (pfn == ptr_mem);	// OCCURS!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	hook->pfn_hook = static_cast <const HOOK_PROC> (pfn_new);
	static_cast <void *&> (hook->originalFunction) = pfn;

	// forming code call
	hook->code_call = Trampolines::Bytecode::CallImmediately32;	// call
	hook->hook_offset = reinterpret_cast <unsigned int> (CommonHookProc) - reinterpret_cast <unsigned int> (&hook->code_call) - Trampolines::Bytecode::CallImmediately32Size;

	// forming code return
	hook->code_ret = Trampolines::Bytecode::ReturnNearValue;
	hook->count_args4 = count_args * Trampolines::Bytecode::Pointer32Size;

	unsigned char *porig_code (static_cast <unsigned char *> (pfn));

	// Save the original bytes....
	hook->code_store[0u] = static_cast <unsigned int *> (static_cast <void *> (porig_code))[0u];
	hook->code_store[1u] = static_cast <unsigned int *> (static_cast <void *> (porig_code + Trampolines::Bytecode::JumpImmediately32Size))[0u];

	// call hooked_proc
	porig_code[0u] = Trampolines::Bytecode::JumpImmediately32;	// jmp
	static_cast <unsigned int *> (static_cast <void *> (++porig_code))[0u] = reinterpret_cast <unsigned int> (&hook->code_call) - reinterpret_cast <unsigned int> (porig_code) - Trampolines::Bytecode::Pointer32Size;
}
//}

// This is just a simple container for data so I only have to add 1 extra parameter to calls that get trampolined.
class Hook
{
	friend HooksManager;

	//
	// Group: Private members.
	//
	private:
		const void        *&m_functionAddress;	// The original function location

		const void *const   m_originalFunction;	// original function
		void *const         m_trampoline;		// trampoline function for this hook

	//
	// Group: (Con/De)structors.
	//
	public:
		inline Hook (const void *&address, const bool isVoid, const unsigned char parametersNumber, const void *const callbackFunction) :
			m_functionAddress (address),	// Get the original function address....
			m_originalFunction (address),
			m_trampoline (Trampolines::CreateThisGenericTrampoline (isVoid, parametersNumber, address, callbackFunction))	// now install a trampoline
		{
			// Now overwrite the target function with our trampoline
			if (!Trampolines::VirtualProtect (&address, sizeof (address), PAGE_READWRITE))	// Insert into virtual table
				AddLogEntry (false, LogLevel_Critical, true, "VirtualProtect() failed!\nError code: %i.", GetLastError ());

			// Insert the ours function into the virtual table...
			address = m_trampoline;
		}

		inline ~Hook (void)
		{
			if (!Trampolines::VirtualProtect (&m_functionAddress, sizeof (m_functionAddress), PAGE_READWRITE))
				AddLogEntry (false, LogLevel_Critical, true, "VirtualProtect() failed!\nError code: %i.", GetLastError ());

			// Insert the original function back into the virtual table...
			m_functionAddress = m_originalFunction;

			// Free memory used...
			Detours::g_codeAllocator.Free (m_trampoline);
		}

	//
	// Group: Private operators.
	//
	private:
		inline Hook &operator = (const Hook &/*right*/);	// Avoid "warning C4512: 'Hook' : assignment operator could not be generated".
};
#if 0
class RedirectHook
{
	friend HooksManager;

	//
	// Group: Private members.
	//
	private:
		const void        *m_functionAddress;	// The original function location

		const void *const  m_originalFunction;	// original function

	//
	// Group: (Con/De)structors.
	//
	public:
		inline RedirectHook (const void *address, const bool isVoid, const unsigned char parametersNumber, const void *const callbackFunction) :
			m_functionAddress (address),	// Get the original function address....
			m_originalFunction (address),
			m_trampoline (Trampolines::CreateThisGenericTrampoline (isVoid, parametersNumber, address, callbackFunction))	// now install a trampoline
		{
			// Now overwrite the target function with our trampoline
			if (!Trampolines::VirtualProtect (&address, sizeof (address), PAGE_READWRITE))	// Insert into virtual table
				AddLogEntry (false, LogLevel_Critical, true, "VirtualProtect() failed!\nError code: %i.", GetLastError ());

			// Insert the ours function into the virtual table...
			address = m_trampoline;
		}

		inline ~RedirectHook (void)
		{
			if (!Trampolines::VirtualProtect (&m_functionAddress, sizeof (m_functionAddress), PAGE_READWRITE))
				AddLogEntry (false, LogLevel_Critical, true, "VirtualProtect() failed!\nError code: %i.", GetLastError ());

			// Insert the original function back into the virtual table...
			m_functionAddress = m_originalFunction;

			// Free memory used...
			Detours::g_codeAllocator.Free (m_trampoline);
		}

	//
	// Group: Private operators.
	//
	private:
		inline RedirectHook &operator = (const RedirectHook &/*right*/);	// Avoid "warning C4512: 'RedirectHook' : assignment operator could not be generated".
};
#endif	// if 0
	private:
		typedef DynamicArray <Hook *, unsigned char> HooksArray;

		HooksArray m_virtualTableHooks;
		HooksArray m_hooks;

	public:
		inline ~HooksManager (void)
		{
			// Delete all virtual table hooks....
			DeleteAllHooks ();
		}

	private:
		inline const unsigned char GetHookID (const void *const function) const
		{
			// Check the list of this function's hooks, see if the function we have is a hook.
			for (unsigned char hookIndex (0u); hookIndex < m_virtualTableHooks.GetElementNumber (); ++hookIndex)
				if (m_virtualTableHooks[hookIndex]->m_trampoline == function)
					return hookIndex;	// Yes, this function is hooked.

			return HooksArray::BadPosition;	// Function not found.
		}
		inline const bool IsMember (const void *const function) const
		{
			// Check the list of this function's hooks, see if the function we have is a hook.
			for (unsigned char hookIndex (0u); hookIndex < m_virtualTableHooks.GetElementNumber (); ++hookIndex)
				if (m_virtualTableHooks[hookIndex]->m_trampoline == function)
					return true;	// Yes, this function is hooked.

			return false;	// Function not found.
		}
		inline const bool IsMember (const HalfLifeEngine::SDK::Classes::Edict *const edict, const unsigned char virtualTableID) const
		{
			return IsMember (MemoryUtilities::GetElementsTable <void *const> (edict->privateData)[virtualTableID]);
		}

	public:
		inline void DeleteAllVirtualHooks (void)
		{
			// Delete all virtual table hooks....
			m_virtualTableHooks.DeleteAndPurgeAll ();
		}
		inline void DeleteAllEngineHooks (void)
		{
			// Delete all hooks....
			m_hooks.DeleteAndPurgeAll ();
		}
		inline void DeleteAllHooks (void)
		{
			// Delete all virtual table hooks....
			DeleteAllVirtualHooks ();

			// Delete all hooks....
			DeleteAllEngineHooks ();
		}

		void RegisterEngineHookFromAddress (const unsigned int address, const bool isVoid, const unsigned char parametersNumber, const void *const callbackFunction);

		void RegisterEngineHook (const unsigned char *const pattern, const unsigned char length, const bool isVoid, const unsigned char parametersNumber, const void *const callbackFunction);
		inline void RegisterEngineHook (const unsigned char *const pattern, const bool isVoid, const unsigned char parametersNumber, const void *const callbackFunction)
		{
			RegisterEngineHook (pattern, GetStringLength <unsigned char> (reinterpret_cast <const char *const> (pattern)), isVoid, parametersNumber, callbackFunction);
		}

		/**
		* Hooks the virtual table for the specified entity class.
		* An example would be: RegisterEngineVirtualTableHookByEntityClassName (EntityVirtualTableFunctionID_TakeDamage, "player");
		* Look at the EntityVirtualTableFunctionID_t enumeration for parameter lists.
		*
		* @param entityClassName:	The entity class name to hook from.
		* @param virtualTableID:	Virtual table index of this function. (virtual function table entry index of the function)
		* @param isVoid:			Whether or not the target trampoline uses voids.
		* @param parametersNumber:	How many parameters are in the function.
		* @param callbackFunction:	The forward to call.
		* @return 					Returns a handle to the forward. Use EnableForward/DisableForward to toggle the forward on or off.
		*/
		inline void RegisterEngineVirtualTableHookByEntityClassName (const DynamicString &entityClassName, const unsigned char virtualTableID, const bool isVoid, const unsigned char parametersNumber, const void *const callbackFunction)
		{
			// Check the entity

			// create an entity, assign it the game.DLL's class, hook it and destroy it
			HalfLifeEngine::SDK::Classes::Edict *const entity (HalfLifeEngine::Globals::g_halfLifeEngine->CreateEntity ());

			// Reliability check.
			InternalAssert (entity->IsValid ());

			// Call the entity function...
			HalfLifeEngine::Globals::g_halfLifeEngine->CallGameEntity (entityClassName, entity->variables);

			// Get virtual table....
			const void **const virtualFunctionTable = MemoryUtilities::GetElementsTable <const void *> (entity->privateData);

			// now entity no needed for us...
			HalfLifeEngine::Globals::g_halfLifeEngine->RemoveEntity (entity);

			// Reliability check.
			InternalAssert (virtualFunctionTable != NULL);

			// We've passed all tests...
			RegisterEngineVirtualTableHook (virtualFunctionTable[virtualTableID], isVoid, parametersNumber, callbackFunction);

			// CZ bots seem to use a different "classtype" for player entities (or something like that) which needs to be hooked separately....
			if (entityClassName == "player")
			{
				// Reliability check.
				InternalAssert (virtualFunctionTable - HalfLifeEngine::SDK::Constants::Offset_CSBot_VirtualFunctionTablePointer != NULL);

				// Register Forwards for CZ bots....
				RegisterEngineVirtualTableHook ((virtualFunctionTable - HalfLifeEngine::SDK::Constants::Offset_CSBot_VirtualFunctionTablePointer)[virtualTableID], isVoid, parametersNumber, callbackFunction);
			}
		}
		/**
		* Hooks the virtual table for the specified entity's class.
		* An example would be: RegisterEngineVirtualTableHookFromEntity (EntityVirtualTableFunctionID_TakeDamage, entity, "player_hurt");
		* Look at the EntityVirtualTableFunctionID_t enumeration for parameter lists.
		* @note This will cause hooks for the entire internal class that the entity is not exclusively for the provided entity.
		*
		* @param entity:			The entity to hook from.
		* @param virtualTableID:	Virtual table index of this function. (virtual function table entry index of the function)
		* @param isVoid:			Whether or not the target trampoline uses voids.
		* @param parametersNumber:	How many parameters are in the function.
		* @param callbackFunction:	The forward to call.
		* @return 			Returns a handle to the forward. Use EnableForward/DisableForward to toggle the forward on or off.
		*/
		inline void RegisterEngineVirtualTableHookFromEntity        (const HalfLifeEngine::SDK::Classes::Edict *const entity, const unsigned char virtualTableID, const bool isVoid, const unsigned char parametersNumber, const void *const callbackFunction)
		{
			// Check the entity

			// Reliability check.
			InternalAssert (entity->IsValid ());

			RegisterEngineVirtualTableHookFromClassPointer (entity->privateData, virtualTableID, isVoid, parametersNumber, callbackFunction);
		}

		inline void RegisterEngineVirtualTableHookFromClassPointer (const void *const classPointer, const unsigned char virtualTableID, const bool isVoid, const unsigned char parametersNumber, const void *const callbackFunction)
		{
			// Check the class pointer

			// Get virtual table....
			const void **const virtualFunctionTable = MemoryUtilities::GetElementsTable <const void *> (classPointer);

			// Reliability check.
			InternalAssert (virtualFunctionTable != NULL);

			// We've passed all tests...
			RegisterEngineVirtualTableHook (virtualFunctionTable[virtualTableID], isVoid, parametersNumber, callbackFunction);
		}
		inline void RegisterEngineVirtualTableHook                 (const void *&virtualFunction, const bool isVoid, const unsigned char parametersNumber, const void *const callbackFunction)
		{
			// Reliability check.
			InternalAssert (virtualFunction != NULL);

			// We've passed all tests...

			// Check the list of this function's hooks, see if the function we have is a hook.
			InternalAssert (!IsMember (virtualFunction));	// Yes, this function is hooked.

			// If we got here, the function is not hooked (yet)
			Hook *const hook (new Hook (virtualFunction, isVoid, parametersNumber, callbackFunction));

			#if defined _DEBUG
				// Reliability check.
				if (hook == NULL)
				{
					AddLogEntry (true, LogLevel_Error, false, "Couldn't allocate %u bytes for creating entity hook!", sizeof (Hook));

					return;
				}

				if (!m_virtualTableHooks.PushBack (hook))
					delete hook;	// Delete this hook....
			#else
				m_virtualTableHooks += hook;
			#endif	// if defined _DEBUG
		}
};

inline __declspec (naked) void HooksManager::CommonHookProc (void)
{
	__asm
	{
		// Standard prologue and the preservation of registers:
		push ebp
		mov  ebp, esp

		// Code
		push esi
		push edi
		push ebx

		// Load the address in EBX hook_data, in ESI-address code stored beginning of the original function, in EDI - address of the original function:
		mov  ebx, dword ptr[ebp+04h]	// (04h == 4)
		mov  esi, ebx
		add  esi, 03h	// (03h == 3)
		mov  edi, dword ptr[ebx+0Bh]	// (0Bh == 11)

		// Keeping the relative jump address in the stack and restore the first bytes of the original code
		push dword ptr[edi+1]

		movsd
		movsd

		// Then put the stack arguments and call the function-sniffer
		mov  esi, ebp
		add  esi, 0Ch	// (0Ch == 12)
		push esi

		mov  esi, ebx
		sub  esi, 5
		push esi

		call dword ptr[ebx+0fh]	// (0Fh == 15)

		add  esp, 8

		// Restore JMP at the beginning of the original function, we restore the values of registers and exit:
		// again erase start function
		sub  edi, 5
		mov  byte ptr[edi], 0E9h	// prescribe jmp instruction
		pop  dword ptr[edi+1]	// restore address

		pop  edx
		pop  edi
		pop  esi

		// Epilogue
		pop  ebp
		ret
	}
}

#endif	// ifndef TRAMPOLINES_INCLUDED