// ---------------------------------------------------------------------------------------------------------------------------------
//                                        _
//                                       | |
//  _ __ ___   _ __ ___    __ _  _ __    | |__   ____   ____
// | '_ ` _ \ | '_ ` _ \  / _` || '__|   | '_ \ |  _ \ |  _ \
// | | | | | || | | | | |( (_| || |    _ | | | || |_) )| |_) )
// |_| |_| |_||_| |_| |_| \__, ||_|   (_)|_| |_||  __/ |  __/
//                         __/ |                | |    | |
//                        |___/                 |_|    |_|
//
// Memory manager & tracking software
//
// Best viewed with 8-character tabs and (at least) 132 columns
//
// ---------------------------------------------------------------------------------------------------------------------------------
//
// Restrictions & freedoms pertaining to usage and redistribution of this software:
//
//  * This software is 100% free
//  * If you use this software (in part or in whole) you must credit the author.
//  * This software may not be re-distributed (in part or in whole) in a modified
//    form without clear documentation on how to obtain a copy of the original work.
//  * You may not use this software to directly or indirectly cause harm to others.
//  * This software is provided as-is and without warrantee. Use at your own risk.
//
// For more information, visit HTTP://www.FluidStudios.com
//
// ---------------------------------------------------------------------------------------------------------------------------------
// Originally created on 12/22/2000 by Paul Nettle
//
// Copyright 2000, Fluid Studios, Inc., all rights reserved.
// ---------------------------------------------------------------------------------------------------------------------------------

#if defined _MSC_VER && _MSC_VER > 1000
#	pragma once
#endif	// if defined _MSC_VER && _MSC_VER > 1000

#ifndef MEMORY_MANAGER_INCLUDED
#define MEMORY_MANAGER_INCLUDED

// ---------------------------------------------------------------------------------------------------------------------------------
// External constants
// ---------------------------------------------------------------------------------------------------------------------------------
enum MemoryAllocationType_t
{
	MemoryAllocationType_Unknown,

	MemoryAllocationType_New,
	MemoryAllocationType_NewArray,
	MemoryAllocationType_Malloc,
	MemoryAllocationType_Calloc,
	MemoryAllocationType_Realloc,

	MemoryAllocationType_Delete,
	MemoryAllocationType_DeleteArray,
	MemoryAllocationType_Free,

	MemoryAllocationType_Total
};

// ---------------------------------------------------------------------------------------------------------------------------------
// Types
// ---------------------------------------------------------------------------------------------------------------------------------
struct AllocUnit_t
{
	size_t                  actualSize;
	size_t                  reportedSize;
	void                   *actualAddress;
	void                   *reportedAddress;
	char                    sourceFile[40u];
	char                    sourceFunc[128u];
	unsigned int            sourceLine;
	MemoryAllocationType_t  allocationType;
	bool                    breakOnDealloc;
	bool                    breakOnRealloc;
	unsigned int            allocationNumber;
	AllocUnit_t            *next;
	AllocUnit_t            *prev;
};

struct MemoryStatistics_t
{
	unsigned int totalReportedMemory;
	unsigned int totalActualMemory;
	unsigned int peakReportedMemory;
	unsigned int peakActualMemory;
	unsigned int accumulatedReportedMemory;
	unsigned int accumulatedActualMemory;
	unsigned int accumulatedAllocUnitCount;
	unsigned int totalAllocUnitCount;
	unsigned int peakAllocUnitCount;
};

// ---------------------------------------------------------------------------------------------------------------------------------
// Used by the macros
// ---------------------------------------------------------------------------------------------------------------------------------
void m_setOwner (const char *const file, const unsigned int line, const char *const func);

// ---------------------------------------------------------------------------------------------------------------------------------
// Allocation breakpoints
// ---------------------------------------------------------------------------------------------------------------------------------
bool &m_breakOnRealloc (const void *const reportedAddress);
bool &m_breakOnDealloc (const void *const reportedAddress);

// ---------------------------------------------------------------------------------------------------------------------------------
// The meat of the memory tracking software
// ---------------------------------------------------------------------------------------------------------------------------------
void *const m_allocator   (const char *const sourceFile, const unsigned int sourceLine, const char *const sourceFunc, const MemoryAllocationType_t allocationType, const size_t reportedSize);
void *const m_reallocator (const char *const sourceFile, const unsigned int sourceLine, const char *const sourceFunc, const MemoryAllocationType_t reallocationType, const size_t reportedSize, const void *const reportedAddress);
void        m_deallocator (const char *const sourceFile, const unsigned int sourceLine, const char *const sourceFunc, const MemoryAllocationType_t deallocationType, const void *const reportedAddress);

// ---------------------------------------------------------------------------------------------------------------------------------
// Utilitarian functions
// ---------------------------------------------------------------------------------------------------------------------------------
AllocUnit_t *const m_findAllocUnit         (const void *const reportedAddress);
const bool         m_validateAddress       (const void *const reportedAddress);
const bool         m_validateAllocUnit     (const AllocUnit_t *const allocUnit);
const bool         m_validateAllAllocUnits (void);

// ---------------------------------------------------------------------------------------------------------------------------------
// Unused RAM calculations
// ---------------------------------------------------------------------------------------------------------------------------------
const unsigned int m_calcUnused    (const AllocUnit_t *const allocUnit);
const unsigned int m_calcAllUnused (void);

// ---------------------------------------------------------------------------------------------------------------------------------
// Logging and reporting
// ---------------------------------------------------------------------------------------------------------------------------------
void                      m_dumpAllocUnit       (const AllocUnit_t *const allocUnit, const char *const prefix = "");
void                      m_dumpMemoryReport    (const char *const filename = "memreport.log", const bool overwrite = true);
const MemoryStatistics_t &m_getMemoryStatistics (void);

// ---------------------------------------------------------------------------------------------------------------------------------
// Variations of global operators new & delete
// ---------------------------------------------------------------------------------------------------------------------------------
void *operator new       (size_t reportedSize);
void *operator new []    (size_t reportedSize);
void *operator new       (size_t reportedSize, const char *const sourceFile, const int sourceLine);
void *operator new []    (size_t reportedSize, const char *const sourceFile, const int sourceLine);
void  operator delete    (void *const reportedAddress);
void  operator delete [] (void *const reportedAddress);

#endif	// ifndef MEMORY_MANAGER_INCLUDED

#include <Fluid_Studios_Memory_Manager/nommgr.hpp>

// ---------------------------------------------------------------------------------------------------------------------------------
// Macros -- "Kids, please don't try this at home. We're trained professionals here." :)
// ---------------------------------------------------------------------------------------------------------------------------------
#define new						(m_setOwner   (__FILE__, __LINE__, __FUNCTION__), false) ? NULL : new
#define delete					(m_setOwner   (__FILE__, __LINE__, __FUNCTION__), false) ? ("", 0u, "") : delete
#define malloc(size)			m_allocator   (__FILE__, __LINE__, __FUNCTION__, MemoryAllocationType_Malloc, size)
#define calloc(count, size)		m_allocator   (__FILE__, __LINE__, __FUNCTION__, MemoryAllocationType_Calloc, count * size)
#define realloc(pointer, size)	m_reallocator (__FILE__, __LINE__, __FUNCTION__, MemoryAllocationType_Realloc, size, pointer)
#define free(pointer)			m_deallocator (__FILE__, __LINE__, __FUNCTION__, MemoryAllocationType_Free, pointer)

// ---------------------------------------------------------------------------------------------------------------------------------
// mmgr.hpp - End of file
// ---------------------------------------------------------------------------------------------------------------------------------