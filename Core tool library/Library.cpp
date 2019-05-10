//
// Copyright © 2003-2012, by YaPB Development Team. All rights reserved.
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
// Library.cpp
//
// Class: Library
//
// Description: Simple library wrapper class, that holds the data required to get symbols from libraries loaded at run-time (i.e. from DLL's for so's).
//
/// @todo NEED TO DELETE ALL MESSAGES FROM MEMBER FUNCTIONS!!!
//
// Version: $ID:$
//

#include <Core.hpp>

namespace CoreToolLibrary
{
	template <typename elementType> inline elementType MakePtr (void *const Base, const DWORD Offset)
	{
		return reinterpret_cast <elementType> (reinterpret_cast <DWORD> (Base) + Offset);
	}
	static const bool ScanPattern_FF15calladr(BYTE* start, DWORD calladr,int array_size,BYTE **pos )
	{
		array_size-=6;
		while(array_size)
		{
			if( start[0]==0xFF && 
				start[1]==0x15 &&
				*(DWORD*)(start+2)==calladr ) { *pos = start; return true; }

			array_size--;
			start++;
		}
		return false;
	}
	static const bool ScanPattern_fixstackandret(BYTE* start,BYTE stacklen,int array_size,BYTE **pos)
	{
		if (stacklen)
		{
		array_size-=6;
		while(array_size)
		{
			if( start[0]==0x83 && 
				start[1]==0xC4 &&
				start[2]==stacklen &&
				start[3]==0xC3  ) { *pos = start; return true; }
			array_size--;
			start++;
		}
		}
		else
		{
			array_size-=1;
			while(array_size)
			{
			if( start[0]==0xC3 ) { *pos = start; return true; }
			array_size--;
			start++;
			}
		}
		return false;
	}
	static const bool ScanPattern_8B2Dcalladr(BYTE* start, DWORD calladr,int array_size,BYTE **pos )
	{
		array_size-=6;
		while(array_size)
		{
			if( start[0]==0x8B && 
				start[1]==0x2D &&
				*(DWORD*)(start+2)==calladr ) { *pos = start; return true; }

			array_size--;
			start++;
		}
		return false;
	}
	static const bool ScanPattern_8B35calladr(BYTE* start, DWORD calladr,int array_size,BYTE **pos )
	{
		array_size-=6;
		while(array_size)
		{
			if( start[0]==0x8B && 
				start[1]==0x35 &&
				*(DWORD*)(start+2)==calladr ) { *pos = start; return true; }

			array_size--;
			start++;
		}
		return false;
	}

	const bool LibraryHandle::HookImportAddrTable (const char *const func, DWORD hookfunc)
	{
		unsigned char *const base (m_module.byteBuffer);
		IMAGE_DOS_HEADER *const dos (m_module.DOSHeader);
		if (dos->e_magic != IMAGE_DOS_SIGNATURE)
		{
			AddLogEntry (true, LogLevel_Error, false, "Could not detect valid DOS signature");
			return false;
		}

		IMAGE_NT_HEADERS *nt ((IMAGE_NT_HEADERS *)(base + dos->e_lfanew));
		if (nt->Signature != IMAGE_NT_SIGNATURE)
		{
			AddLogEntry (true, LogLevel_Error, false, "Could not detect valid NT signature");
			return false;
		}

		IMAGE_IMPORT_DESCRIPTOR *desc ((IMAGE_IMPORT_DESCRIPTOR *) (base + nt->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress));
		if (base == (BYTE *)desc)
		{
			AddLogEntry (true, LogLevel_Error, false, "Could not find IAT");
			return false;
		}

		bool entryFound (false);
		for (/* Empty */; desc->Name; ++desc)
		{
			if (desc->FirstThunk == 0u)
				continue;

			DWORD *iat ((DWORD *)(base + desc->FirstThunk));
			for (IMAGE_THUNK_DATA *data = (IMAGE_THUNK_DATA *)(base + desc->OriginalFirstThunk); data->u1.Function > 0u; ++data, ++iat)
			{
				if (IMAGE_SNAP_BY_ORDINAL32 (data->u1.Ordinal))
					continue;

				IMAGE_IMPORT_BY_NAME *import ((IMAGE_IMPORT_BY_NAME *)(base + data->u1.AddressOfData));

				if (strcmp ((char *)import->Name, func) == 0)
				{
					DWORD oldprot, oldprot2;
					::VirtualProtect(iat, sizeof (*iat), PAGE_READWRITE, &oldprot);
					*iat = hookfunc;
					::VirtualProtect(iat, sizeof (*iat), oldprot, &oldprot2);
					entryFound = true;
				}
			}
		}

		if (!entryFound)
		{
			AddLogEntry (true, LogLevel_Error, false, "Could not find IAT entry for %s", func);
			return false;
		}

		return true;
	}
	const bool LibraryHandle::ReRoute (PIMAGE_NT_HEADERS pNTHeader, DWORD *const new_addr, const char *const modulename, const char *const funcname, DWORD &out_offset)
	{
		HMODULE res (m_module);
		//generic call to old_add patcher
		bool success = importoffset(modulename,funcname,out_offset)!=0;
		if(!success)
			return false;

		BYTE *pos;
		BYTE* codestart=(BYTE*)res+pNTHeader->OptionalHeader.BaseOfCode;
		DWORD length=pNTHeader->OptionalHeader.SizeOfCode;
		while (ScanPattern_FF15calladr(codestart,out_offset,length,&pos))
		{
			length-=pos+6-codestart;
			codestart=pos+6;
			Trampolines::VirtualProtect (pos+2, sizeof (DWORD), PAGE_EXECUTE_READWRITE);
			DWORD* blub0r=(DWORD*)(pos+2);
			*blub0r = (DWORD)new_addr; 
			Trampolines::VirtualProtect (pos+2, sizeof (DWORD), PAGE_EXECUTE_READWRITE);
		}
		codestart=(BYTE*)res+pNTHeader->OptionalHeader.BaseOfCode;
		length=pNTHeader->OptionalHeader.SizeOfCode;
		while (ScanPattern_8B2Dcalladr(codestart,out_offset,length,&pos))
		{
			length-=pos+6-codestart;
			codestart=pos+6;
			Trampolines::VirtualProtect (pos+2, sizeof (DWORD), PAGE_EXECUTE_READWRITE);
			DWORD* blub0r=(DWORD*)(pos+2);
			*blub0r = (DWORD)new_addr; 
			Trampolines::VirtualProtect (pos+2, sizeof (DWORD), PAGE_EXECUTE_READWRITE);
		}
		codestart=(BYTE*)res+pNTHeader->OptionalHeader.BaseOfCode;
		length=pNTHeader->OptionalHeader.SizeOfCode;
		while (ScanPattern_8B35calladr(codestart,out_offset,length,&pos))
		{
			length-=pos+6-codestart;
			codestart=pos+6;
			Trampolines::VirtualProtect (pos+2, sizeof (DWORD), PAGE_EXECUTE_READWRITE);
			DWORD* blub0r=(DWORD*)(pos+2);
			*blub0r = (DWORD)new_addr; 
			Trampolines::VirtualProtect (pos+2, sizeof (DWORD), PAGE_EXECUTE_READWRITE);
		}

		return true;
	}
	const bool LibraryHandle::IATHook (const DWORD OriginalFunc, const DWORD HookFunc, void **const pOriginalFunc)
	{
		HMODULE ModHandle (m_module);
		DWORD CurAddr, CurPointer;
		bool Hooked (false);

		if (!ModHandle || !OriginalFunc || !HookFunc)
			return false;

		const DWORD base ((DWORD)ModHandle);
		IMAGE_NT_HEADERS *const pehdr (reinterpret_cast <IMAGE_NT_HEADERS *> (base + m_module.DOSHeader->e_lfanew));
		const DWORD IATanfang (base + pehdr->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IAT].VirtualAddress);
		const DWORD IATende (IATanfang + pehdr->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IAT].Size);

		CurAddr = IATanfang;

		while (CurAddr < IATende)
		{
			memcpy (&CurPointer, (void *)CurAddr, sizeof (DWORD));
//			CurPointer = CurAddr;

			if (CurPointer == OriginalFunc)
			{
				if (pOriginalFunc != NULL)
					*pOriginalFunc = (void *)CurPointer;
				DWORD old_attributes, old_attributes2;
				if (!VirtualProtect ((void *)CurAddr, sizeof (DWORD), PAGE_EXECUTE_READWRITE, &old_attributes))
					return false;
				memcpy ((void *)CurAddr, &HookFunc, sizeof (DWORD));
//				CurAddr = HookFunc;
				if (!VirtualProtect ((void *)CurAddr, sizeof (DWORD), old_attributes, &old_attributes2))
					return false;
				Hooked = true;
			}

			CurAddr += sizeof (DWORD);
		}
		return Hooked;
	}
	const bool LibraryHandle::InterceptDllCall (const char *const c_szDllName, const char *const c_szApiName, PVOID pApiNew, PVOID *const p_pApiOrg, PVOID pApiToChange/* = NULL*/)
	{
		HMODULE hLocalModule (m_module);
		DWORD dwAddressToIntercept;

		if (pApiToChange != NULL)
			dwAddressToIntercept = reinterpret_cast <DWORD> (pApiToChange);
		else
		{
			const HMODULE APIModule (GetModuleHandle (c_szDllName));

			if (APIModule == NULL)
				return false;

			dwAddressToIntercept = reinterpret_cast <DWORD> (GetProcAddress (APIModule, c_szApiName));
		}

		if (dwAddressToIntercept == 0u)
			return false;

		if (IsBadReadPtr (hLocalModule, sizeof (PIMAGE_NT_HEADERS)))
			return false;

		PIMAGE_DOS_HEADER pDOSHeader (reinterpret_cast <PIMAGE_DOS_HEADER> (hLocalModule));

		if (pDOSHeader->e_magic != IMAGE_DOS_SIGNATURE)
			return false;

		PIMAGE_NT_HEADERS pNTHeader (MakePtr <PIMAGE_NT_HEADERS> (pDOSHeader, pDOSHeader->e_lfanew));

		if (pNTHeader->Signature != IMAGE_NT_SIGNATURE)
			return false;

		PIMAGE_IMPORT_DESCRIPTOR pImportDesc (MakePtr <PIMAGE_IMPORT_DESCRIPTOR> (hLocalModule, pNTHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress));

		if (pImportDesc == reinterpret_cast <PIMAGE_IMPORT_DESCRIPTOR> (pNTHeader))
			return false;

		DWORD dwProtect;
		bool success (false);
		PIMAGE_THUNK_DATA pThunk;

		for (/* Empty */; pImportDesc->Name; ++pImportDesc)
			for (pThunk = MakePtr <PIMAGE_THUNK_DATA> (hLocalModule, pImportDesc->FirstThunk); pThunk->u1.Function > 0u; ++pThunk)
			{
				if (pThunk->u1.Function != dwAddressToIntercept)
					continue;

				if (!IsBadWritePtr (&pThunk->u1.Function, sizeof (pThunk->u1.Function)))
				{
					if (p_pApiOrg != NULL)
						*p_pApiOrg = reinterpret_cast <PVOID> (pThunk->u1.Function);

					pThunk->u1.Function = reinterpret_cast <DWORD> (pApiNew);
					success = true;
				}
				else if (::VirtualProtect (&pThunk->u1.Function, sizeof (pThunk->u1.Function), PAGE_EXECUTE_READWRITE, &dwProtect))
				{
					if (p_pApiOrg != NULL)
						*p_pApiOrg = reinterpret_cast <PVOID> (pThunk->u1.Function);

					pThunk->u1.Function = reinterpret_cast <DWORD> (pApiNew);
					success = true;

					const DWORD dwNewProtect (dwProtect);

					::VirtualProtect (&pThunk->u1.Function, sizeof (pThunk->u1.Function), dwNewProtect, &dwProtect);
				}
			}

		return success;
	}

	const bool LibraryHandle::exportchange (const char *const procname, const DWORD newaddr, DWORD *const oldaddr/* = NULL*/)
	{
		char *const base_addr (m_module.characterBuffer);
		IMAGE_DOS_HEADER *const doshdr (m_module.DOSHeader);

		if (doshdr->e_magic != IMAGE_DOS_SIGNATURE || doshdr->e_lfanew == 0)
			return false;

		IMAGE_NT_HEADERS *const pehdr (reinterpret_cast <IMAGE_NT_HEADERS *> (base_addr + doshdr->e_lfanew));

		if (pehdr == NULL || pehdr->Signature != IMAGE_NT_SIGNATURE)
			return false;

		if (pehdr->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress == 0u || !pehdr->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].Size)
			return false;

		IMAGE_EXPORT_DIRECTORY *const exports (reinterpret_cast <IMAGE_EXPORT_DIRECTORY *> (base_addr + pehdr->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress));
		WORD ordinal (0u);

		if (HIWORD (procname) == 0)
			ordinal = static_cast <WORD> (LOWORD (procname) - exports->Base);
		else
		{
			for (DWORD i (0u), *const names (reinterpret_cast <DWORD *> (base_addr + exports->AddressOfNames)); i < exports->NumberOfNames; ++i)
				if (strcmp (base_addr + names[i], procname) == 0)
				{
					ordinal = reinterpret_cast <WORD *> (base_addr + exports->AddressOfNameOrdinals)[i];

					goto Label_Continue;
				}

			return false;
		}

		Label_Continue:
			// Reliability check.
			InternalAssert (ordinal < exports->NumberOfFunctions);

			DWORD &originalAddress (reinterpret_cast <DWORD *> (base_addr + exports->AddressOfFunctions)[ordinal]);

			// Reliability check.
			InternalAssert (originalAddress > 0u);

			if (oldaddr != NULL)
				*oldaddr = reinterpret_cast <DWORD> (base_addr + originalAddress);

			if (newaddr == 0u)
				return true;

			if (!IsBadWritePtr (&originalAddress, sizeof (originalAddress)))
				originalAddress = newaddr - reinterpret_cast <DWORD> (base_addr);
			else
			{
				DWORD dwProtect;

				if (!::VirtualProtect (&originalAddress, sizeof (originalAddress), PAGE_EXECUTE_READWRITE, &dwProtect))
					return false;

				originalAddress = newaddr - reinterpret_cast <DWORD> (base_addr);

				const DWORD dwNewProtect (dwProtect);

				::VirtualProtect (&originalAddress, sizeof (originalAddress), dwNewProtect, &dwProtect);
			}

			return true;
	}

	const bool LibraryHandle::importchange (const char *const thedllname, const char *const functionname, const DWORD newaddr, DWORD *const oldaddr/* = NULL*/)
	{
		char *const base_addr (m_module.characterBuffer);
		IMAGE_DOS_HEADER *const doshdr (m_module.DOSHeader);

		if (doshdr->e_magic != IMAGE_DOS_SIGNATURE || doshdr->e_lfanew == 0)
			return false;

		IMAGE_NT_HEADERS *const pehdr (reinterpret_cast <IMAGE_NT_HEADERS *> (base_addr + doshdr->e_lfanew));

		if (pehdr->Signature != IMAGE_NT_SIGNATURE)
			return false;

		IMAGE_IMPORT_DESCRIPTOR *impdesc (reinterpret_cast <IMAGE_IMPORT_DESCRIPTOR *> (base_addr + pehdr->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress));

		if (pehdr->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress == 0u || pehdr->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size == 0u)
			return false;

		for (/* Empty */; impdesc->Name; ++impdesc)
		{
			if (impdesc->FirstThunk == 0u)
				break;

			const char *const dllname (base_addr + impdesc->Name);

			if (stricmp (dllname, thedllname) != 0)
				continue;

			DWORD test_ordinal;
			const char *test_procname;
			DWORD dwProtect;

			for
			(
				IMAGE_THUNK_DATA *imports (reinterpret_cast <IMAGE_THUNK_DATA *> (base_addr + impdesc->OriginalFirstThunk)), *thunks (reinterpret_cast <IMAGE_THUNK_DATA *> (base_addr + impdesc->FirstThunk));
				/* Empty */;
				++imports, ++thunks
			)
			{
				if (impdesc->OriginalFirstThunk > 0u)
				{
					if (imports->u1.Ordinal == 0u)
						break;

					test_ordinal = imports->u1.Ordinal;
					test_procname = reinterpret_cast <const char *> (reinterpret_cast <IMAGE_IMPORT_BY_NAME *> (base_addr + imports->u1.AddressOfData)->Name);
				}
				else
				{
					if (thunks->u1.Ordinal == 0u)
						break;

					test_ordinal = thunks->u1.Ordinal;
					test_procname = reinterpret_cast <const char *> (reinterpret_cast <IMAGE_IMPORT_BY_NAME *> (base_addr + thunks->u1.AddressOfData)->Name);
				}

				if (IMAGE_SNAP_BY_ORDINAL32 (test_ordinal) || strcmp (test_procname, functionname) != 0)
					continue;

				if (oldaddr != NULL)
					*oldaddr = thunks->u1.Function;

				if (newaddr == 0u)
					return true;

				if (!IsBadWritePtr (&thunks->u1.Function, sizeof (thunks->u1.Function)))
					thunks->u1.Function = newaddr;
				else
				{
					if (!::VirtualProtect (&thunks->u1.Function, sizeof (thunks->u1.Function), PAGE_EXECUTE_READWRITE, &dwProtect))
						return false;

					thunks->u1.Function = newaddr;

					const DWORD dwNewProtect (dwProtect);

					::VirtualProtect (&thunks->u1.Function, sizeof (thunks->u1.Function), dwNewProtect, &dwProtect);
				}

				return true;
			}
		}

		return false;
	}

	const bool LibraryHandle::importoffset (const char *const thedllname, const char *const functionname, DWORD &offset)
	{
		char *const base_addr (m_module.characterBuffer);
		IMAGE_DOS_HEADER *const doshdr (m_module.DOSHeader);

		if (doshdr->e_magic != IMAGE_DOS_SIGNATURE || doshdr->e_lfanew == 0)
			return false;

		IMAGE_NT_HEADERS *const pehdr (reinterpret_cast <IMAGE_NT_HEADERS *> (base_addr + doshdr->e_lfanew));

		if (pehdr->Signature != IMAGE_NT_SIGNATURE)
			return false;

		IMAGE_IMPORT_DESCRIPTOR *impdesc (reinterpret_cast <IMAGE_IMPORT_DESCRIPTOR *> (base_addr + pehdr->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress));

		if (pehdr->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress == 0u || pehdr->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size == 0u)
			return false;

		for (/* Empty */; impdesc->Name; ++impdesc)
		{
			if (impdesc->FirstThunk == 0u)
				break;

			const char *const dllname (base_addr + impdesc->Name);

			if (stricmp (dllname, thedllname) != 0)
				continue;

			DWORD test_ordinal;
			const char *test_procname;

			for
			(
				IMAGE_THUNK_DATA *imports (reinterpret_cast <IMAGE_THUNK_DATA *> (base_addr + impdesc->OriginalFirstThunk)), *thunks (reinterpret_cast <IMAGE_THUNK_DATA *> (base_addr + impdesc->FirstThunk));
				/* Empty */;
				++imports, ++thunks
			)
			{
				if (impdesc->OriginalFirstThunk > 0u)
				{
					if (imports->u1.Ordinal == 0u)
						break;

					test_ordinal = imports->u1.Ordinal;
					test_procname = reinterpret_cast <const char *> (reinterpret_cast <IMAGE_IMPORT_BY_NAME *> (base_addr + imports->u1.AddressOfData)->Name);
				}
				else
				{
					if (thunks->u1.Ordinal == 0u)
						break;

					test_ordinal = thunks->u1.Ordinal;
					test_procname = reinterpret_cast <const char *> (reinterpret_cast <IMAGE_IMPORT_BY_NAME *> (base_addr + thunks->u1.AddressOfData)->Name);
				}

				if (!IMAGE_SNAP_BY_ORDINAL32 (test_ordinal) && strcmp (test_procname, functionname) == 0)
				{
					offset = reinterpret_cast <DWORD> (&thunks->u1.Function);

					return true;
				}
			}
		}

		return false;
	}

	char *const LibraryHandle::lookup_export_name (const DWORD search_ordinal)
	{
		char *const base_addr (m_module.characterBuffer);
		IMAGE_DOS_HEADER *const doshdr (m_module.DOSHeader);

		if (doshdr->e_magic != IMAGE_DOS_SIGNATURE || doshdr->e_lfanew == 0)
			return NULL;

		IMAGE_NT_HEADERS *const pehdr (reinterpret_cast <IMAGE_NT_HEADERS *> (base_addr + doshdr->e_lfanew));

		if (pehdr->Signature != IMAGE_NT_SIGNATURE)
			return NULL;

		IMAGE_EXPORT_DIRECTORY *const exports (reinterpret_cast <IMAGE_EXPORT_DIRECTORY *> (base_addr + pehdr->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress));

		if (pehdr->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress == 0u || pehdr->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].Size == 0u)
			return NULL;

		for (DWORD index (0u); index < exports->NumberOfNames; ++index)
		{
			const WORD ordinal (reinterpret_cast <WORD *> (base_addr + exports->AddressOfNameOrdinals)[index]);

			if (ordinal == search_ordinal)
				return base_addr + reinterpret_cast <DWORD *> (base_addr + exports->AddressOfNames)[index];
		}

		return NULL;
	}

	struct SortNames_t
	{
		unsigned long  name;
		unsigned short nameOrdinal;
	};

	static inline int SortNamesList (const SortNames_t *const A, const SortNames_t *const B)
	{
		// Sort function for qsort.

		const char *const str_A (reinterpret_cast <const char *> (A->name));
		const char *const str_B (reinterpret_cast <const char *> (B->name));

		return strcmp (str_A, str_B);
	}

	const bool LibraryHandle::CombineModuleExportTables (LibraryHandle &moduleDestination)
	{
		// Combines this module and 'moduleDestination' export tables and replaces this module table with new one.

		// Get export tables
		IMAGE_EXPORT_DIRECTORY *const exportSource (GetExportTable ());
		const IMAGE_EXPORT_DIRECTORY *const exportDestination (moduleDestination.GetExportTable ());

		if (exportSource == NULL || exportDestination == NULL)
		{
			AddLogEntry (false, LogLevel_Critical, true, "Couldn't initialize dynamic link entities, exportSource: %i, exportDestination: %i. Exiting...", exportSource, exportDestination);

			return false;
		}

		unsigned long i, u, funcCount, nameCount, listFix;

		// setup new export table
		const unsigned long newNumberOfFunctions (exportSource->NumberOfFunctions + exportDestination->NumberOfFunctions);
		unsigned long newNumberOfNames           (exportSource->NumberOfNames     + exportDestination->NumberOfNames);

		// alloc lists
		unsigned long *const newFunctions (static_cast <unsigned long *> (VirtualAlloc (NULL, newNumberOfFunctions * sizeof (unsigned long), MEM_COMMIT, PAGE_READWRITE))/*new unsigned long[newNumberOfFunctions]*/);	/// THIS LEAKS!!!!!!!!
		SortNames_t   *const newSort      (new SortNames_t[newNumberOfNames]);

		// copy this module to new export
		for (funcCount = 0u; funcCount < exportSource->NumberOfFunctions; ++funcCount)
			newFunctions[funcCount] = RelativeVirtualAddressToVirtualAddress (m_module.pointer, reinterpret_cast <unsigned long *> (RelativeVirtualAddressToVirtualAddress (m_module.pointer, exportSource->AddressOfFunctions))[funcCount]);

		for (nameCount = 0u; nameCount < exportSource->NumberOfNames; ++nameCount)
		{
			// fix name address
			newSort[nameCount].name = RelativeVirtualAddressToVirtualAddress (m_module.pointer, reinterpret_cast <unsigned long *> (RelativeVirtualAddressToVirtualAddress (m_module.pointer, exportSource->AddressOfNames))[nameCount]);

			// ordinal is index to function list
			newSort[nameCount].nameOrdinal = reinterpret_cast <unsigned short *> (RelativeVirtualAddressToVirtualAddress (m_module.pointer, exportSource->AddressOfNameOrdinals))[nameCount];
		}

		// copy 'moduleDestination' to new export
		for (i = 0u; i < exportDestination->NumberOfFunctions; ++i)
			newFunctions[funcCount + i] = RelativeVirtualAddressToVirtualAddress (moduleDestination.GetPointer (), reinterpret_cast <unsigned long *> (RelativeVirtualAddressToVirtualAddress (moduleDestination.GetPointer (), exportDestination->AddressOfFunctions))[i]);

		for (i = listFix = 0u; i < exportDestination->NumberOfNames; ++i)
		{
			const char *const name (reinterpret_cast <const char *> (RelativeVirtualAddressToVirtualAddress (moduleDestination.GetPointer (), reinterpret_cast <unsigned long *> (RelativeVirtualAddressToVirtualAddress (moduleDestination.GetPointer (), exportDestination->AddressOfNames))[i])));

			// Check if name already in the list
			for (u = 0u; u < nameCount; ++u)
				if (stricmp (name, reinterpret_cast <const char *> (newSort[u].name)) == 0)
				{
					--listFix;

					break;
				}

			// already in the list.. skip
			if (u < nameCount)
				continue;

			newSort[nameCount + i + listFix].name = reinterpret_cast <unsigned long> (name);
			newSort[nameCount + i + listFix].nameOrdinal = static_cast <unsigned short> (funcCount) + reinterpret_cast <unsigned short *> (RelativeVirtualAddressToVirtualAddress (moduleDestination.GetPointer (), exportDestination->AddressOfNameOrdinals))[i];
		}

		// set new number
		newNumberOfNames = nameCount + i + listFix;

		// sort names list
		qsort (newSort, newNumberOfNames, sizeof (*newSort), reinterpret_cast <int (*) (const void *, const void *)> (&SortNamesList));

		// make 'newNames' and 'newNameOrdinals' lists (VirtualAlloc() so we dont waste heap memory to stuff that isn't freed)
		unsigned long  *const newNames        (static_cast <unsigned long *> (VirtualAlloc (NULL, newNumberOfNames * sizeof (unsigned long), MEM_COMMIT, PAGE_READWRITE)));	/// THIS LEAKS?!?!?!?!?!?!?!
		unsigned short *const newNameOrdinals (static_cast <unsigned short *> (VirtualAlloc (NULL, newNumberOfNames * sizeof (unsigned short), MEM_COMMIT, PAGE_READWRITE)));	/// THIS LEAKS?!?!?!?!?!?!?!

		// Store sorted list into two separated lists....
		for (i = 0u; i < newNumberOfNames; ++i)
		{
			newNames[i]        = newSort[i].name;
			newNameOrdinals[i] = newSort[i].nameOrdinal;
		}

		// Free allocated memory....
		delete [] newSort;

		// translate VAs to RVAs
		for (i = 0u; i < newNumberOfFunctions; ++i)
			newFunctions[i] = VirtualAddressToRelativeVirtualAddress (m_module.pointer, newFunctions[i]);

		for (i = 0u; i < newNumberOfNames; ++i)
		{
			newNames[i] = VirtualAddressToRelativeVirtualAddress (m_module.pointer, newNames[i]);
			newNameOrdinals[i] = static_cast <unsigned short> (VirtualAddressToRelativeVirtualAddress (m_module.pointer, newNameOrdinals[i]));
		}

		unsigned long oldProtectFlags;

		if (!VirtualProtect (exportSource, sizeof (*exportSource), PAGE_READWRITE, &oldProtectFlags))
		{
			AddLogEntry (false, LogLevel_Critical, true, "Couldn't initialize dynamic link entities, VirtualProtect() failed: %i. Exiting...", GetLastError ());

			return false;
		}

		exportSource->Base                  = 1;
		exportSource->NumberOfFunctions     = newNumberOfFunctions;
		exportSource->NumberOfNames         = newNumberOfNames;
		exportSource->AddressOfFunctions    = VirtualAddressToRelativeVirtualAddress (m_module.pointer, newFunctions);
		exportSource->AddressOfNames        = VirtualAddressToRelativeVirtualAddress (m_module.pointer, newNames);
		exportSource->AddressOfNameOrdinals = VirtualAddressToRelativeVirtualAddress (m_module.pointer, newNameOrdinals);

		VirtualProtect (exportSource, sizeof (*exportSource), oldProtectFlags, &oldProtectFlags);

		return true;
	}
}