//===== Copyright © 1996-2005, Valve Corporation, All rights reserved. ======//
//
// Purpose: Defines a symbol table
//
// $Header: $
// $NoKeywords: $
//===========================================================================//

#ifndef UTLSYMBOL_H
#define UTLSYMBOL_H

#ifdef _WIN32
#pragma once
#endif
/*
//-----------------------------------------------------------------------------
// forward declarations
//-----------------------------------------------------------------------------
class CUtlSymbolTable;
*/
//-----------------------------------------------------------------------------
// This is a symbol, which is a easier way of dealing with strings.
//-----------------------------------------------------------------------------
typedef unsigned short UtlSymId_t;

#define UTL_INVAL_SYMBOL  ((UtlSymId_t)~0)

class CUtlSymbol
{
	protected:
		UtlSymId_t m_Id;

	public:
		// constructor, destructor
		inline CUtlSymbol (void) : m_Id (UTL_INVAL_SYMBOL) {}
		inline CUtlSymbol (const UtlSymId_t id) : m_Id (id) {}
//		inline CUtlSymbol (const char *pStr);
		inline CUtlSymbol (const CUtlSymbol &sym) : m_Id (sym.m_Id) {}

		// operator =
		inline CUtlSymbol &operator = (const CUtlSymbol &src) { m_Id = src.m_Id; return *this; }

		// operator ==
		inline const bool operator == (const CUtlSymbol &src) const { return m_Id == src.m_Id; }
//		inline const bool operator == (const char *pStr) const;

		// Is valid?
		inline const bool IsValid (void) const { return m_Id != UTL_INVAL_SYMBOL; }

		// Gets at the symbol
		inline operator const UtlSymId_t (void) const { return m_Id; }
#if 0
		// Gets the string associated with the symbol
		const char *String(void) const;

		// Modules can choose to disable the static symbol table so to prevent accidental use of them.
		static void DisableStaticSymbolTable(void);

	protected:
		// Initializes the symbol table
		static void Initialize(void);

		// returns the current symbol table
		static CUtlSymbolTable* CurrTable(void);

		// The standard global symbol table
		static CUtlSymbolTable* s_pSymbolTable; 

		static bool s_bAllowStaticSymbolTable;

		friend class CCleanupUtlSymbolTable;
#endif	// if 0
};	// sizeof (CUtlSymbol) == 2 (0.5)

#endif // UTLSYMBOL_H