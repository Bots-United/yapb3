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
// Library.hpp
//
// Class: Library
//
// Description: Simple library wrapper class, that holds the data required to get symbols from libraries loaded at run-time (i.e. from DLL's for so's).
//
/// @todo NEED TO DELETE ALL MESSAGES FROM MEMBER FUNCTIONS!!!
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef LIBRARY_INCLUDED
#define LIBRARY_INCLUDED

class LibraryHandle
{
	//
	// Group: Private members.
	//
	protected:
		union Module_t
		{
			HMODULE           handle;
			void             *pointer;
			char             *characterBuffer;
			unsigned char    *byteBuffer;
			IMAGE_DOS_HEADER *DOSHeader;

			inline Module_t (HMODULE inputHandle = NULL) : handle (inputHandle) { /* VOID */ }

			// casting operators
			inline operator       HMODULE (void)       { return handle; }	// Library will now automatically convert to 'HMODULE' when needed
			inline operator const HMODULE (void) const { return handle; }	// Library will now automatically convert to 'const HMODULE' when needed
		} m_module;	// Handle to the loaded library. (Specific handle that points to dynamic library (This comes from LoadLibrary() on Windows))

	//
	// Group: (Con/De)structors.
	//
	public:
		inline LibraryHandle (HMODULE pointer = NULL)     : m_module (pointer)                    { /* VOID */ }
		inline LibraryHandle (const LibraryHandle &other) : m_module (other.m_module)             { /* VOID */ }
//		explicit inline LibraryHandle (const char *const pathName) : m_module (GetModuleHandle (pathName)) { /* VOID */ }
		inline LibraryHandle (const MemoryUtilities::ModuleInformation_t &dynamicLibraryInformation);	// Note: This function declared in Trampolines.hpp.

		virtual inline ~LibraryHandle (void) { /* VOID */ }

	//
	// Group: Operators.
	//
	public:
		// casting operators
		inline operator       HMODULE (void)       { return m_module; }	// Library will now automatically convert to 'HMODULE' when needed
		inline operator const HMODULE (void) const { return m_module; }	// Library will now automatically convert to 'const HMODULE' when needed
		inline operator const bool    (void) const { return IsValid (); }

		inline LibraryHandle &operator = (HMODULE pointer)
		{
			m_module = pointer;

			// Reliability check.
			InternalAssert (IsValid ());

			return *this;
		}
		inline LibraryHandle &operator = (const LibraryHandle &right)
		{
			// Reliability check.
			InternalAssert (right.IsValid ());

			m_module = right.m_module;

			return *this;
		}
		inline LibraryHandle &operator = (const MemoryUtilities::ModuleInformation_t &dynamicLibraryInformation);	// Note: This function declared in Trampolines.hpp.

	//
	// Group: Functions.
	//
	public:
		inline const bool IsValid (void) const { return m_module.handle != NULL; }

		inline const bool IsDataFile     (void) const { return (reinterpret_cast <unsigned int> (m_module.handle) & 1u) > 0u; }	// If this function returns TRUE, the module was loaded as a data file (LOAD_LIBRARY_AS_DATAFILE or LOAD_LIBRARY_AS_DATAFILE_EXCLUSIVE).
		inline const bool IsImageMapping (void) const { return (reinterpret_cast <unsigned int> (m_module.handle) & 2u) > 0u; }	// If this function returns TRUE, the module was loaded as an image file (LOAD_LIBRARY_AS_IMAGE_RESOURCE).
		inline const bool IsResource     (void) const { return IsImageMapping () || IsDataFile (); }						// If this function returns TRUE, the module was loaded as either a data file or an image file.

		//
		// Function: GetFunctionAddress
		//
		// Description: Gets the address of the given symbol from the loaded dynamic library.
		//
		// Parameters:
		//	functionName - Name of the symbol to search for.
		//
		// Returns: If the function succeeds, the returned value is a pointer to the symbol, NULL otherwise.
		//
		template <typename elementType> inline const elementType GetFunctionAddress (const char *const functionName) const
		{
			const elementType address (reinterpret_cast <elementType> (GetProcAddress (m_module, functionName)));

			#if defined _DEBUG
				// Reliability check.
				if (address == NULL)
				{
					AddLogEntry (false, LogLevel_Critical, false, "Couldn't get address of function '%s()' from DLL handle.\nError message: '%s'.", functionName, GetLastErrorMessage ());

					return NULL;
				}
			#endif	// if defined _DEBUG

			return address;
		}

		typedef const bool (*FunctionNameComparator_t) (const char *const functionName1, const char *const functionName2);
		inline void *const GetFunctionAddressByNameComparator (const char *const functionName, const FunctionNameComparator_t functionNameComparator) const
		{
			IMAGE_EXPORT_DIRECTORY *const exportTable (GetExportTable ());	// Get export table.
			char *const baseAddress (m_module.characterBuffer);

			for (DWORD i (0u), *const names (reinterpret_cast <DWORD *> (baseAddress + exportTable->AddressOfNames)); i < exportTable->NumberOfNames; ++i)
			{
				if (!functionNameComparator (baseAddress + names[i], functionName))
					continue;

				const WORD &ordinal (reinterpret_cast <WORD *> (baseAddress + exportTable->AddressOfNameOrdinals)[i]);

				// Reliability check.
				InternalAssert (ordinal < exportTable->NumberOfFunctions);

				const DWORD &originalAddress (reinterpret_cast <DWORD *> (baseAddress + exportTable->AddressOfFunctions)[ordinal]);

				// Reliability check.
				InternalAssert (originalAddress > 0u);

				return reinterpret_cast <void *> (baseAddress + originalAddress);
			}

			return NULL;
		}

		inline       HMODULE &GetPointer  (void)       { return m_module.handle; }	// Get the pointer of the library.
		inline const HMODULE &GetPointer  (void) const { return m_module.handle; }	// Get the pointer of the library.

		template <typename elementType> inline elementType &GetMemory (const unsigned int address)
		{
			elementType *const memoryPointer (reinterpret_cast <elementType *const> (reinterpret_cast <unsigned int> (m_module.handle) + address));

			#if defined _DEBUG
				// Reliability check.
				if (memoryPointer == NULL)
					AddLogEntry (false, LogLevel_Critical, false, "Couldn't get memory at address 0x%X from DLL handle.", address);
			#endif	// if defined _DEBUG

			return memoryPointer[0u];
		}
		template <typename elementType> inline const elementType &GetMemory (const unsigned int address) const
		{
			const elementType *const memoryPointer (reinterpret_cast <const elementType *const> (reinterpret_cast <const unsigned int> (m_module.handle) + address));

			#if defined _DEBUG
				// Reliability check.
				if (memoryPointer == NULL)
					AddLogEntry (false, LogLevel_Critical, false, "Couldn't get memory at address 0x%X from DLL handle.", address);
			#endif	// if defined _DEBUG

			return memoryPointer[0u];
		}
		template <typename functionType> inline const functionType GetFunction (const unsigned int address) const
		{
			return reinterpret_cast <const functionType> (reinterpret_cast <const unsigned int> (m_module.handle) + address);
		}

		// From Source Mod Crazy Debugger....
		const bool HookImportAddrTable (const char *const func, DWORD hookfunc);
		// From cheats....
		const bool ReRoute (PIMAGE_NT_HEADERS pNTHeader, DWORD *const new_addr, const char *const modulename, const char *const funcname, DWORD &out_offset);
		const bool IATHook (const DWORD OriginalFunc, const DWORD HookFunc, void **const pOriginalFunc);
		const bool InterceptDllCall (const char *const c_szDllName, const char *const c_szApiName, PVOID pApiNew, PVOID *const p_pApiOrg, PVOID pApiToChange = NULL);
		const bool exportchange (const char *const procname, const DWORD newaddr, DWORD *const oldaddr = NULL);
		const bool importchange (const char *const thedllname, const char *const functionname, const DWORD newaddr, DWORD *const oldaddr = NULL);
		const bool importoffset (const char *const thedllname, const char *const functionname, DWORD &offset);
		char *const lookup_export_name (const DWORD search_ordinal);

		inline IMAGE_NT_HEADERS *const GetNTHeaders (void) const
		{
			// Checks module signatures and return ntheaders pointer for valid module.

			// Check if valid dos header
			if (IsBadReadPtr (m_module.DOSHeader, sizeof (*m_module.DOSHeader)) || m_module.DOSHeader->e_magic != IMAGE_DOS_SIGNATURE)
				return NULL;

			// Get and check pe header
			IMAGE_NT_HEADERS *const pe (reinterpret_cast <IMAGE_NT_HEADERS *> (RelativeVirtualAddressToVirtualAddress (m_module.pointer, m_module.DOSHeader->e_lfanew)));

			if (IsBadReadPtr (pe, sizeof (*pe)) || pe->Signature != IMAGE_NT_SIGNATURE)
				return NULL;

			return pe;
		}

		inline IMAGE_EXPORT_DIRECTORY *const GetExportTable (void) const
		{
			// Returns export table for valid module.

			IMAGE_NT_HEADERS *const pe (GetNTHeaders ());

			// Check module
			if (pe == NULL)
				return NULL;

			// Check for exports
			if (pe->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress == 0u)
				return NULL;

			IMAGE_EXPORT_DIRECTORY *const exportDirectory (reinterpret_cast <IMAGE_EXPORT_DIRECTORY *> (RelativeVirtualAddressToVirtualAddress (m_module.pointer, pe->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress)));

			if (IsBadReadPtr (exportDirectory, sizeof (*exportDirectory)))
				return NULL;

			return exportDirectory;
		}

		//
		// Win32 code for dynamic link entities
		//  -- by Jussi Kivilinna
		//

		// Reads source.DLL and destination.DLL function export tables and combines theim to single table that replaces source.DLL's original table.
		const bool CombineModuleExportTables (LibraryHandle &moduleDestination);
/*		inline void FreeNewExportFunctions (void)
		{
			// Frees memory allocated by CombineModuleExportTables() function.

			// Get export table.
			IMAGE_EXPORT_DIRECTORY *const exportSource (GetExportTable ());

			delete [] reinterpret_cast <unsigned long *> (RelativeVirtualAddressToVirtualAddress (m_module.pointer, exportSource->AddressOfFunctions));
		}*/
};

class LibraryHandleWithSize : public LibraryHandle
{
	//
	// Group: Private members.
	//
	protected:
		unsigned int m_size;

	//
	// Group: (Con/De)structors.
	//
	public:
		inline LibraryHandleWithSize (void) : LibraryHandle (), m_size (0u) { /* VOID */ }
		inline LibraryHandleWithSize (HMODULE pointer);							// Note: This function declared in Trampolines.hpp.
		inline LibraryHandleWithSize (const LibraryHandle &other);				// Note: This function declared in Trampolines.hpp.
		inline LibraryHandleWithSize (const LibraryHandleWithSize &other) : LibraryHandle (other), m_size (other.GetSize ()) { /* VOID */ }
		inline LibraryHandleWithSize (const MemoryUtilities::ModuleInformation_t &dynamicLibraryInformation);	// Note: This function declared in Trampolines.hpp.

		virtual inline ~LibraryHandleWithSize (void) { /* VOID */ }

	//
	// Group: Operators.
	//
	public:
		inline LibraryHandleWithSize &operator = (HMODULE pointer);						// Note: This function declared in Trampolines.hpp.
		inline LibraryHandleWithSize &operator = (const LibraryHandle &right) { return operator = (right.GetPointer ()); }
		inline LibraryHandleWithSize &operator = (const LibraryHandleWithSize &right)
		{
			// Reliability check.
			InternalAssert (right.IsValid ());

			m_module = right.m_module;
			m_size = right.GetSize ();

			return *this;
		}
		inline LibraryHandleWithSize &operator = (const MemoryUtilities::ModuleInformation_t &dynamicLibraryInformation);	// Note: This function declared in Trampolines.hpp.

	//
	// Group: Functions.
	//
	public:
		inline const unsigned int GetSize (void) const { return m_size; }

		inline const bool IsContainingAddress (const void *const address) const
		{
			// Reliability check.
			InternalAssert (address != NULL);

			return static_cast <const unsigned char *const> (address) >= m_module.byteBuffer && static_cast <const unsigned char *const> (address) < m_module.byteBuffer + GetSize ();
		}
};

class Library : public LibraryHandle
{
	//
	// Group: Private members.
	//
	protected:
		DynamicString m_pathName;	// path to library

	//
	// Group: (Con/De)structors.
	//
	public:
		inline Library (void) : LibraryHandle () { /* VOID */ }
		inline Library (const DynamicString &pathName) : LibraryHandle (LoadLibrary (pathName)), m_pathName (pathName)
		{
			#if defined _DEBUG
				// Has the library.DLL not been loaded successfully?
				if (!IsValid ())
					AddLogEntry (false, LogLevel_Critical, false, "Couldn't load DLL '%s'.\nError message: '%s'.", pathName.GetData (), GetLastErrorMessage ());
			#endif	// if defined _DEBUG
		}
/*		inline Library (HMODULE pointer) : LibraryHandle (pointer), m_pathName (MemoryUtilities::GetModuleName (pointer))
		{
			#if defined _DEBUG
				// Has the library.DLL not been loaded successfully?
				if (!IsValid ())
					AddLogEntry (false, LogLevel_Critical, false, "Couldn't load DLL '%s'.\nError message: '%s'.", m_pathName.GetData (), GetLastErrorMessage ());
			#endif	// if defined _DEBUG
		}
*/
		virtual inline ~Library (void)
		{
			// Reliability check.
			InternalAssert (IsValid ());

			if (!FreeLibrary (m_module))
				AddLogEntry (false, LogLevel_Critical, false, "Couldn't close DLL '%s'.\nError message: '%s'.", m_pathName.GetData (), GetLastErrorMessage ());
		}

	//
	// Group: Private operators.
	//
	private:
		inline Library &operator = (const Library &/*right*/);	// Avoid "warning C4512: 'Library' : assignment operator could not be generated".

	//
	// Group: Functions.
	//
	public:
		inline const DynamicString &GetPathName (void) const { return m_pathName; }	// Get the path name of the library.

		inline const bool IsFreedByOtherDLL (void) const
		{
			// Check if library loaded by this DLL freed in other DLL.

			// Reliability check.
			InternalAssert (IsValid ());

			return m_module != GetModuleHandle (GetPathName ());
		}

		inline const bool Load (const DynamicString &pathName)
		{
			// Reliability check.
			InternalAssert (!IsValid ());

			m_module = LoadLibrary (pathName);
			m_pathName = pathName;

			return IsValid ();
		}
		inline const bool Free (void)
		{
			// Reliability check.
			InternalAssert (IsValid ());

			m_pathName.MakeEmpty ();

			const bool result (FreeLibrary (m_module) == TRUE);

			m_module.handle = NULL;

			return result;
		}
#if 0
		//
		// Function: GetFunctionAddress
		//
		// Description: Gets the address of the given symbol from the loaded dynamic library.
		//
		// Parameters:
		//	functionName - Name of the symbol to search for.
		//
		// Returns: If the function succeeds, the returned value is a pointer to the symbol, NULL otherwise.
		//
		template <typename elementType> inline const elementType GetFunctionAddress (const char *const functionName) const
		{
			const elementType address (reinterpret_cast <elementType> (GetProcAddress (m_module, functionName)));

			#if defined _DEBUG
				// Reliability check.
				if (address == NULL)
				{
					AddLogEntry (false, LogLevel_Critical, false, "Couldn't get address of function '%s()' from DLL '%s'.\nError message: '%s'.", functionName, m_pathName.GetData (), GetLastErrorMessage ());

					return NULL;
				}
			#endif	// if defined _DEBUG

			return address;
		}

		template <typename elementType> inline elementType &GetMemory (const unsigned int address)
		{
			elementType *const memoryPointer (reinterpret_cast <elementType *const> (reinterpret_cast <unsigned int> (m_module.handle) + address));

			#if defined _DEBUG
				// Reliability check.
				if (memoryPointer == NULL)
					AddLogEntry (false, LogLevel_Critical, false, "Couldn't get memory at address 0x%X from DLL '%s'.", address, m_pathName.GetData ());
			#endif	// if defined _DEBUG

			return memoryPointer[0u];
		}
		template <typename elementType> inline const elementType &GetMemory (const unsigned int address) const
		{
			const elementType *const memoryPointer (reinterpret_cast <const elementType *const> (reinterpret_cast <const unsigned int> (m_module.handle) + address));

			#if defined _DEBUG
				// Reliability check.
				if (memoryPointer == NULL)
					AddLogEntry (false, LogLevel_Critical, false, "Couldn't get memory at address 0x%X from DLL '%s'.", address, m_pathName.GetData ());
			#endif	// if defined _DEBUG

			return memoryPointer[0u];
		}
#endif	// if 0
};

class LibraryWithSize : public LibraryHandleWithSize
{
	//
	// Group: Private members.
	//
	protected:
		DynamicString m_pathName;	// path to library

	//
	// Group: (Con/De)structors.
	//
	public:
		inline LibraryWithSize (const DynamicString &pathName) : LibraryHandleWithSize (LoadLibrary (pathName)), m_pathName (pathName)
		{
			#if defined _DEBUG
				// Has the library.DLL not been loaded successfully?
				if (!IsValid ())
					AddLogEntry (false, LogLevel_Critical, false, "Couldn't load DLL '%s'.\nError message: '%s'.", pathName.GetData (), GetLastErrorMessage ());
			#endif	// if defined _DEBUG
		}
/*		inline LibraryWithSize (HMODULE pointer) : LibraryHandleWithSize (pointer), m_pathName (MemoryUtilities::GetModuleName (pointer))
		{
			#if defined _DEBUG
				// Has the library.DLL not been loaded successfully?
				if (!IsValid ())
					AddLogEntry (false, LogLevel_Critical, false, "Couldn't load DLL '%s'.\nError message: '%s'.", m_pathName.GetData (), GetLastErrorMessage ());
			#endif	// if defined _DEBUG
		}
*/
		virtual inline ~LibraryWithSize (void)
		{
			// Reliability check.
			InternalAssert (IsValid ());

			if (!FreeLibrary (m_module))
				AddLogEntry (false, LogLevel_Critical, false, "Couldn't close DLL '%s'.\nError message: '%s'.", m_pathName.GetData (), GetLastErrorMessage ());
		}

	//
	// Group: Private operators.
	//
	private:
		inline LibraryWithSize &operator = (const LibraryWithSize &/*right*/);	// Avoid "warning C4512: 'LibraryWithSize' : assignment operator could not be generated".

	//
	// Group: Functions.
	//
	public:
		inline const DynamicString &GetPathName (void) const { return m_pathName; }	// Get the path name of the library.
#if 0
		//
		// Function: GetFunctionAddress
		//
		// Description: Gets the address of the given symbol from the loaded dynamic library.
		//
		// Parameters:
		//	functionName - Name of the symbol to search for.
		//
		// Returns: If the function succeeds, the returned value is a pointer to the symbol, NULL otherwise.
		//
		template <typename elementType> inline const elementType GetFunctionAddress (const char *const functionName) const
		{
			const elementType address (reinterpret_cast <elementType> (GetProcAddress (m_module, functionName)));

			#if defined _DEBUG
				// Reliability check.
				if (address == NULL)
				{
					AddLogEntry (false, LogLevel_Critical, false, "Couldn't get address of function '%s()' from DLL '%s'.\nError message: '%s'.", functionName, m_pathName.GetData (), GetLastErrorMessage ());

					return NULL;
				}
			#endif	// if defined _DEBUG

			return address;
		}

		template <typename elementType> inline elementType &GetMemory (const unsigned int address)
		{
			elementType *const memoryPointer (reinterpret_cast <elementType *const> (reinterpret_cast <unsigned int> (m_module.handle) + address));

			#if defined _DEBUG
				// Reliability check.
				if (memoryPointer == NULL)
					AddLogEntry (false, LogLevel_Critical, false, "Couldn't get memory at address 0x%X from DLL '%s'.", address, m_pathName.GetData ());
			#endif	// if defined _DEBUG

			return memoryPointer[0u];
		}
		template <typename elementType> inline const elementType &GetMemory (const unsigned int address) const
		{
			const elementType *const memoryPointer (reinterpret_cast <const elementType *const> (reinterpret_cast <const unsigned int> (m_module.handle) + address));

			#if defined _DEBUG
				// Reliability check.
				if (memoryPointer == NULL)
					AddLogEntry (false, LogLevel_Critical, false, "Couldn't get memory at address 0x%X from DLL '%s'.", address, m_pathName.GetData ());
			#endif	// if defined _DEBUG

			return memoryPointer[0u];
		}
#endif	// if 0
};

#endif	// ifndef LIBRARY_INCLUDED