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
// SaveRestoreData.hpp
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef SAVE_RESTORE_DATA_INCLUDED
#define SAVE_RESTORE_DATA_INCLUDED

class CSaveRestoreBuffer
{
	public:
		CSaveRestoreBuffer( void );
		CSaveRestoreBuffer( SaveRestoreData_t *pdata );
		virtual ~CSaveRestoreBuffer( void );

		int			EntityIndex( Structures::EntityVariables_t *pevLookup );
		int			EntityIndex( Edict *pentLookup );
		int			EntityIndex( EOFFSET eoLookup );
		int			EntityIndex( BaseEntity *pEntity );

		int			EntityFlags( int entityIndex, int flags ) { return EntityFlagsSet( entityIndex, 0 ); }
		int			EntityFlagsSet( int entityIndex, int flags );

		Edict		*EntityFromIndex( int entityIndex );

		unsigned short	TokenHash( const char *pszToken );

	protected:
		SaveRestoreData_t		*m_pdata;
		void		BufferRewind( int size );
		unsigned int	HashString( const char *pszToken );

	private:
		// effc++ rule 11
		void		operator = (CSaveRestoreBuffer &);
		CSaveRestoreBuffer (const CSaveRestoreBuffer &);
};

class CSave : public CSaveRestoreBuffer
{
	public:
		CSave( SaveRestoreData_t *pdata ) : CSaveRestoreBuffer( pdata ) { /* Empty */ }

		void	WriteShort( const char *pname, const short *value, int count );
		void	WriteInt( const char *pname, const int *value, int count );		// Save an int
		void	WriteFloat( const char *pname, const float *value, int count );	// Save a float
		void	WriteTime( const char *pname, const float *value, int count );	// Save a float (timevalue)
		void	WriteData( const char *pname, int size, const char *pdata );		// Save a binary data block
		void	WriteString( const char *pname, const char *pstring );			// Save a null-terminated string
		void	WriteString( const char *pname, const int *stringId, int count );	// Save a null-terminated string (engine string)
		void	WriteVector( const char *pname, const Math::Vector3D &value );				// Save a vector
		void	WriteVector( const char *pname, const float *value, int count );	// Save a vector
		void	WritePositionVector( const char *pname, const Math::Vector3D &value );		// Offset for landmark if necessary
		void	WritePositionVector( const char *pname, const float *value, int count );	// array of pos vectors
		void	WriteFunction( const char *pname, const int *value, int count );		// Save a function pointer
		int		WriteEntVars( const char *pname, Structures::EntityVariables_t *pev );		// Save Structures::EntityVariables_t (Structures::EntityVariables_t)
		int		WriteFields( const char *pname, void *pBaseData, TYPEDESCRIPTION *pFields, int fieldCount );

	private:
		int		DataEmpty( const char *pdata, int size );
		void	BufferField( const char *pname, int size, const char *pdata );
		void	BufferString( char *pdata, int len );
		void	BufferData( const char *pdata, int size );
		void	BufferHeader( const char *pname, int size );
};

struct HEADER
{
	unsigned short  size;
	unsigned short  token;
	char           *pData;
};

class CRestore : public CSaveRestoreBuffer
{
	public:
		CRestore( SaveRestoreData_t *pdata ) : CSaveRestoreBuffer( pdata ), m_global(0), m_precache(TRUE) { /* Empty */ }
		int		ReadEntVars( const char *pname, Structures::EntityVariables_t *pev );		// Structures::EntityVariables_t
		int		ReadFields( const char *pname, void *pBaseData, TYPEDESCRIPTION *pFields, int fieldCount );
		int		ReadField( void *pBaseData, TYPEDESCRIPTION *pFields, int fieldCount, int startField, int size, char *pName, void *pData );
		int		ReadInt( void );
		short	ReadShort( void );
		int		ReadNamedInt( const char *pName );
		char	*ReadNamedString( const char *pName );
		int		Empty( void ) { return (m_pdata == NULL) || ((m_pdata->pCurrentData-m_pdata->pBaseData)>=m_pdata->bufferSize); }
		inline	void SetGlobalMode( int global ) { m_global = global; }
		void	PrecacheMode( BOOL mode ) { m_precache = mode; }

	private:
		char	*BufferPointer( void );
		void	BufferReadBytes( char *pOutput, int size );
		void	BufferSkipBytes( int bytes );
		int		BufferSkipZString( void );
		int		BufferCheckZString( const char *string );

		void	BufferReadHeader( HEADER *pheader );

		int		m_global;		// Restoring a global entity?
		BOOL	m_precache;
};

#define MAX_ENTITYARRAY 64

//#define ARRAYSIZE(p)		(sizeof(p)/sizeof(p[0]))

#define IMPLEMENT_SAVERESTORE(derivedClass,baseClass) \
	int derivedClass::Save( CSave &save )\
	{\
		if ( !baseClass::Save(save) )\
			return 0;\
		return save.WriteFields( #derivedClass, this, m_SaveData, ARRAYSIZE(m_SaveData) );\
	}\
	int derivedClass::Restore( CRestore &restore )\
	{\
		if ( !baseClass::Restore(restore) )\
			return 0;\
		return restore.ReadFields( #derivedClass, this, m_SaveData, ARRAYSIZE(m_SaveData) );\
	}


enum GLOBALESTATE { GLOBAL_OFF, GLOBAL_ON, GLOBAL_DEAD };

struct globalentity_t
{
	char			name[64u];
	char			levelName[32u];
	GLOBALESTATE	state;
	globalentity_t	*pNext;
};

class CGlobalState
{
	public:
						CGlobalState(void);
		void			Reset( void );
		void			ClearStates( void );
		void			EntityAdd( string_t globalname, string_t mapName, GLOBALESTATE state );
		void			EntitySetState( string_t globalname, GLOBALESTATE state );
		void			EntityUpdate( string_t globalname, string_t mapname );
		const globalentity_t	*EntityFromTable( string_t globalname );
		GLOBALESTATE	EntityGetState( string_t globalname );
		int				EntityInTable( string_t globalname ) { return (Find( globalname ) != NULL) ? 1 : 0; }
		int				Save( CSave &save );
		int				Restore( CRestore &restore );
		static TYPEDESCRIPTION m_SaveData[];

		void			DumpGlobals( void );

	private:
		globalentity_t	*Find( string_t globalname );
		globalentity_t	*m_pList;
		int				m_listCount;
		// effc++ rule 11
		void		operator = (CGlobalState &);
		CGlobalState (const CGlobalState &);
};

extern CGlobalState gGlobalState;

#endif	// ifndef SAVE_RESTORE_DATA_INCLUDED