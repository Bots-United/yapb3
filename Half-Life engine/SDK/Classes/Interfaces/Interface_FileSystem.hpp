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
// FileSystem.hpp
//
// Class: FileSystem
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef HALF_LIFE_ENGINE_SDK_CLASSES_FILE_SYSTEM_INCLUDED
#define HALF_LIFE_ENGINE_SDK_CLASSES_FILE_SYSTEM_INCLUDED

class CSysModule;	// load/unload components (just a 'HMODULE')

//-----------------------------------------------------------------------------
// Enums used by the interface
//-----------------------------------------------------------------------------

enum FileSystemSeek_t
{
	FILESYSTEM_SEEK_HEAD,
	FILESYSTEM_SEEK_CURRENT,
	FILESYSTEM_SEEK_TAIL
};

enum FileWarningLevel_t
{
	// A problem!
	FILESYSTEM_WARNING = -1,

	// Don't print anything
	FILESYSTEM_WARNING_QUIET,

	// On shutdown, report names of files left unclosed
	FILESYSTEM_WARNING_REPORTUNCLOSED,

	// Report number of times a file was opened, closed
	FILESYSTEM_WARNING_REPORTUSAGE,

	// Report all open/close events to console (!slow!)
	FILESYSTEM_WARNING_REPORTALLACCESSES,

	// Report all open/close/read events to the console (!slower!)
	FILESYSTEM_WARNING_REPORTALLACCESSES_READ,

	// Report all open/close/read/write events to the console (!slower!)
	FILESYSTEM_WARNING_REPORTALLACCESSES_READWRITE
};

//-----------------------------------------------------------------------------
// Main file system interface
//-----------------------------------------------------------------------------
#if 0
// This is the minimal interface that can be implemented to provide access to a named set of files.
abstract_class IBaseFileSystem
{
	//-----------------------------------------------------------------------------
	// Forward declarations
	//-----------------------------------------------------------------------------
	public:
		#pragma pack (push, 1)	// Structures must be packed (byte-aligned) (Very important!)
			class BaseFileHandle
			{
				public:
/*! off=0(0) */					FILE *m_pFile;
/*! off=4(1) */					bool  m_bPack;

				public:
					inline BaseFileHandle (void) : m_pFile (NULL), m_bPack (false) { /* VOID */ }
			};	// sizeof (BaseFileHandle) == 5 (1.25)
		#pragma pack (pop)	// Reset default packing.

		// Reliability check.
		CompileTimeAssert (sizeof (BaseFileHandle) == 5u);

		typedef BaseFileHandle FileHandle_t;

		#define FILESYSTEM_INVALID_HANDLE	static_cast <HalfLifeEngine::SDK::Classes::Interface::IBaseFileSystem::FileHandle_t *const> (NULL)

		#define BASEFILESYSTEM_INTERFACE_VERSION		"VBaseFileSystem003"

	public:
		// if pathID is NULL, all paths will be searched for the file
		virtual FileHandle_t *const Open (const char *const filename, const char *const options, const char *const pathID = NULL) = 0;
		virtual void			Close (FileHandle_t *const file) = 0;

		virtual int				Read (void *const output, const int size, FileHandle_t *const file) = 0;
		virtual int				Write (const void *const input, const int size, FileHandle_t *const file) = 0;

		virtual void			Seek (FileHandle_t *const file, const int pos, const FileSystemSeek_t seekType) = 0;
		virtual unsigned int	Tell (FileHandle_t *const file) = 0;
		virtual unsigned int	Size (FileHandle_t *const file) = 0;
		virtual unsigned int	Size (const char *const filename, const char *const pathID = NULL) = 0;

		virtual long			GetFileTime (const char *const filename, const char *const pathID = NULL) = 0;	// Returns time of the last modification of the file.

		virtual void			Flush (FileHandle_t *const file) = 0;
		virtual bool			Precache (const char *const filename, const char *const pathID) = 0;

		virtual bool			FileExists (const char *const filename, const char *const pathID = NULL) = 0;
		virtual bool			IsFileWritable (char const *const filename, const char *const pathID = NULL) = 0;
};
#endif	// if 0
abstract_class IFileSystem : public Interface::Base//IBaseFileSystem - @note NOT EXISTS IN HL 1 ENGINE ?!?!?!?!??!?!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
{
	//-----------------------------------------------------------------------------
	// Forward declarations
	//-----------------------------------------------------------------------------
	public:
		#pragma pack (push, 1)	// Structures must be packed (byte-aligned) (Very important!)
			class BaseFileHandle
			{
				public:
/*! off=0(0) */					FILE *m_pFile;
/*! off=4(1) */					bool  m_bPack;

				public:
					inline BaseFileHandle (void) : m_pFile (NULL), m_bPack (false) { /* VOID */ }
			};	// sizeof (BaseFileHandle) == 5 (1.25)
		#pragma pack (pop)	// Reset default packing.

		// Reliability check.
		CompileTimeAssert (sizeof (BaseFileHandle) == 5u);

		typedef BaseFileHandle FileHandle_t;

		#define FILESYSTEM_INVALID_HANDLE	static_cast <HalfLifeEngine::SDK::Classes::Interface::IFileSystem::FileHandle_t *const> (NULL)

		DECLARE_TYPED_ENUMERATION (int, FileFindHandle_t)
		{
			FILESYSTEM_INVALID_FIND_HANDLE = -1
		};

		#define FILESYSTEM_INTERFACE_VERSION			"VFileSystem009"

	//
	// Group: Pure virtual functions.
	//
	public:
/*! off=4(1) */		virtual void			Mount   (void) = 0;
/*! off=8(2) */		virtual void			Unmount (void) = 0;

		// Remove all search paths (including write path?)
/*! off=12(3) */		virtual void			RemoveAllSearchPaths (void) = 0;

		// Add paths in priority order (mod dir, game dir, ....)
		// If one or more .pak files are in the specified directory, then they are
		//  added after the file system path
		// If the path is the relative path to a .bsp file, then any previous .bsp file
		//  override is cleared and the current .bsp is searched for an embedded PAK file
		//  and this file becomes the highest priority search path (i.e., it's looked at first
		//   even before the mod's file system path).
/*! off=16(4) */		virtual void			AddSearchPath (const char *const path, const char *const pathID) = 0;
/*! off=20(5) */		virtual bool			RemoveSearchPath (const char *const path) = 0;

		// Deletes a file (on the WritePath)
/*! off=24(6) */		virtual void			RemoveFile (const char *const relativePath, const char *const pathID) = 0;

		// this isn't implementable on STEAM as is.
/*! off=28(7) */		virtual void			CreateDirHierarchy (const char *const path, const char *const pathID) = 0;

/*! off=32(8) */		virtual bool			FileExists (const char *const filename) = 0;

		// File I/O and info
/*! off=36(9) */		virtual bool			IsDirectory (const char *const filename) = 0;

		// if pathID is NULL, all paths will be searched for the file
/*! off=40(10) */		virtual FileHandle_t *const Open (const char *const filename, const char *const options, const char *const pathID = NULL) = 0;
/*! off=44(11) */		virtual void			Close (FileHandle_t *const file) = 0;

/*! off=48(12) */		virtual void			Seek (FileHandle_t *const file, const int pos, const FileSystemSeek_t seekType) = 0;
/*! off=52(13) */		virtual unsigned int	Tell (FileHandle_t *const file) = 0;
/*! off=56(14) */		virtual unsigned int	Size (FileHandle_t *const file) = 0;
/*! off=60(15) */		virtual unsigned int	Size (const char *const filename) = 0;

/*! off=64(16) */		virtual long			GetFileTime (const char *const filename) = 0;	// Returns time of the last modification of the file.

/*! off=68(17) */		virtual void			FileTimeToString (char *const strip, const int maxCharsIncludingTerminator, const long fileTime) = 0;

/*! off=72(18) */		virtual bool			IsOk (FileHandle_t *const file) = 0;

/*! off=76(19) */		virtual void			Flush (FileHandle_t *const file) = 0;

/*! off=80(20) */		virtual bool			EndOfFile (FileHandle_t *const file) = 0;

/*! off=84(21) */		virtual int				Read (void *const output, const int size, FileHandle_t *const file) = 0;
/*! off=88(22) */		virtual int				Write (const void *const input, const int size, FileHandle_t *const file) = 0;

/*! off=92(23) */		virtual char			*ReadLine (char *const output, const int maxChars, FileHandle_t *const file) = 0;
/*! off=96(24) */		virtual int				FPrintf (FileHandle_t *const file, char *const format, ...) = 0;

/*! off=100(25) */		virtual int				GetReadBuffer (FileHandle_t *const file, int *, bool) = 0;
/*! off=104(26) */		virtual void			ReleaseReadBuffer (FileHandle_t *const file, void *) = 0;

		// FindFirst/FindNext
/*! off=108(27) */		virtual const char		*FindFirst (const char *const wildCard, FileFindHandle_t *handle, const char * = NULL) = 0;
/*! off=112(28) */		virtual const char		*FindNext (FileFindHandle_t handle) = 0;
/*! off=116(29) */		virtual bool			FindIsDirectory (FileFindHandle_t handle) = 0;
/*! off=120(30) */		virtual void			FindClose (FileFindHandle_t handle) = 0;

/*! off=124(31) */		virtual void			GetLocalCopy (const char *const filename) = 0;

		// return string length of local path EXCLUDING '\0' terminator.
/*! off=128(32) */		virtual const char		*GetLocalPath (const char *const filename, char *const localPath, const int localPathMaximumCharacters) = 0;

/*! off=132(33) */		virtual char			*ParseFile (const char *const inputFileBuffer, char *const outputFileBuffer, bool *const quoteFound = NULL) = 0;

		// Returns true on success (based on current list of search paths, otherwise false if it can't be resolved)
/*! off=136(34) */		virtual bool			FullPathToRelativePath (const char *const fullpath, char *const relative) = 0;

		// Gets the current working directory
/*! off=140(35) */		virtual bool			GetCurrentDirectory (char *const directory, const int maxlen) = 0;

		// Dump to printf/OutputDebugString the list of files that have not been closed
/*! off=144(36) */		virtual void			PrintOpenedFiles (void) = 0;

/*! off=148(37) */		virtual void			SetWarningFunc (void (*const pfnWarning) (const char *const format, ...)) = 0;
/*! off=152(38) */		virtual void			SetWarningLevel (FileWarningLevel_t level) = 0;

/*! off=156(39) */		virtual void			LogLevelLoadStarted (const char *const name) = 0;
/*! off=160(40) */		virtual void			LogLevelLoadFinished (const char *const name) = 0;

		// HintResourceNeed() is not to be confused with resource precaching.
/*! off=164(41) */		virtual int				HintResourceNeed (const char *const hintlist, const int forgetEverything) = 0;

/*! off=168(42) */		virtual int				PauseResourcePreloading (void) = 0;
/*! off=172(43) */		virtual int				ResumeResourcePreloading (void) = 0;

/*! off=176(44) */		virtual int				SetVBuf (FileHandle_t *const stream, char *const buffer, const int mode, const long size) = 0;
/*! off=180(45) */		virtual void			GetInterfaceVersion (char *const p, const int maxlen) = 0;
/*! off=184(46) */		virtual bool			IsFileImmediatelyAvailable (const char *const filename) = 0;

/*! off=188(47) */		virtual int				WaitForResources (const char *const) = 0;
/*! off=192(48) */		virtual int				GetWaitForResourcesProgress (int, float * = NULL, bool * = NULL) = 0;
/*! off=196(49) */		virtual void			CancelWaitForResources (int) = 0;

/*! off=200(50) */		virtual const bool		SteamGetAppUpdateStats (const unsigned int uAppId) const = 0;	// NOT EXISTS ON LINUX?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?!?

	//
	// Group: Functions.
	//
	public:
		inline const int Unlink (const char *const filename)
		{
			char localPath[512u];

			GetLocalPath (filename, localPath, sizeof (localPath));

			return unlink (localPath);
		}
		inline /*HMODULE*/CSysModule *const LoadLibrary (const char *const libraryFilename)
		{
			GetLocalCopy (libraryFilename);

			return LoadLibrary (libraryFilename);
		}
};

abstract_class BaseFileSystem : public IFileSystem
{
	/// @warning DO NOT USE ANY FUNCTIONS OF THIS CLASS!!!
#if 0
	public:
		// IMPLEMENTATION DETAILS FOR CBaseFileSystem
		struct FindData_t
		{
			WIN32_FIND_DATA findData;
			int currentSearchPathID;
			CUtlVector<char> wildCardString;
			HANDLE findHandle;
		};

		class CFileHandle
		{
			public:
/*! off=0(0) */				FILE *m_pFile;
/*! off=4(1) */				bool  m_bPack;
/*! off=8(2) */				int   m_nStartOffset;
/*! off=12(3) */				int   m_nLength;
/*! off=16(4) */				long  m_nFileTime;

			public:
				inline CFileHandle (void) : m_pFile (NULL), m_bPack (false), m_nStartOffset (0), m_nLength (0), m_nFileTime (0) { /* VOID */ }
		};	// sizeof (CFileHandle) == 20 (5)

		// Reliability check.
		CompileTimeAssert (sizeof (CFileHandle) == 20u);

		enum { MAX_FILES_IN_PACK = 32768 };

		class CPackFileEntry
		{
		public:
			CUtlSymbol			m_Name;
			int					m_nPosition;
			int					m_nLength;
		};

		class CSearchPath
		{
		public:
			CUtlSymbol			m_Path;
			CUtlSymbol			m_PathID;

			bool				m_bIsMapPath;
			bool				m_bIsPackFile;
			long				m_lPackFileTime;
			CFileHandle			*m_hPackFile;
			int					m_nNumPackFiles;

			CUtlRBTree< CPackFileEntry, int > m_PackFiles;

			static CBaseFileSystem*	m_fs;

		public:
								CSearchPath( void );
								~CSearchPath( void );

			static bool PackFileLessFunc( CPackFileEntry const& src1, CPackFileEntry const& src2 );
			const char* GetPathString() const;
			const char* GetPathIDString() const;
		};	// sizeof (CSearchPath) == 56 (14)

		//-----------------------------------------------------------------------------
		// Purpose: For tracking unclosed files
		// NOTE:  The symbol table could take up memory that we don't want to eat here.
		// In that case, we shouldn't store them in a table, or we should store them as locally allocates stings
		//  so we can control the size
		//-----------------------------------------------------------------------------
		class COpenedFile
		{
		public:
			FILE		*m_pFile;
			char		*m_pName;

		public:
						COpenedFile( void );
						~COpenedFile( void );

						COpenedFile( const COpenedFile& src );

			bool operator==( const COpenedFile& src ) const;

			void		SetName( char const *name );
			char const	*GetName( void );
		};
#endif	// if 0
};

abstract_class FileSystem_Stdio : public BaseFileSystem
{
	public:
		class CFileHandle : public BaseFileHandle
		{
			public:
/*! off=8(2) */				int  m_nStartOffset;
/*! off=12(3) */				int  m_nLength;
/*! off=16(4) */				long m_nFileTime;	// Last modification time.

			public:
				inline CFileHandle (void) : BaseFileHandle (), m_nStartOffset (0), m_nLength (0), m_nFileTime (0) { /* VOID */ }
		};	// sizeof (CFileHandle) == 20 (5)

		// Reliability checks.
		CompileTimeAssert (sizeof (CFileHandle) == 20u);
		CompileTimeAssert (GET_OFFSET_OF_MEMBER_VARIABLE (CFileHandle, m_nStartOffset) == 8u);

	//----------------------------------------------------------------------------
	// Purpose: Functions implementing basic file system behavior.
	//----------------------------------------------------------------------------
	protected:
/*! off=204(51) */		virtual FILE *FS_fopen (const char *filename, const char *options) = 0;
/*! off=208(52) */		virtual void FS_fclose (FILE *fp) = 0;
/*! off=212(53) */		virtual void FS_fseek (FILE *fp, long pos, int seekType) = 0;
/*! off=216(54) */		virtual long FS_ftell (FILE *fp) = 0;
/*! off=220(55) */		virtual int FS_feof (FILE *fp) = 0;
/*! off=224(56) */		virtual size_t FS_fread (void *dest, size_t count, size_t size, FILE *fp) = 0;
/*! off=228(57) */		virtual size_t FS_fwrite (const void *src, size_t count, size_t size, FILE *fp) = 0;
/*! off=232(58) */		virtual size_t FS_vfprintf (FILE *fp, const char *fmt, va_list list) = 0;
/*! off=236(59) */		virtual int FS_ferror (FILE *fp) = 0;
/*! off=240(60) */		virtual int FS_fflush (FILE *fp) = 0;
/*! off=244(61) */		virtual char *FS_fgets (char *dest, int destSize, FILE *fp) = 0;
/*! off=248(62) */		virtual int FS_stat (const char *path, FileStatistics_t *buf) = 0;
/*! off=252(63) */		virtual HANDLE FS_FindFirstFile(char *findname, WIN32_FIND_DATA *dat) = 0;
/*! off=256(64) */		virtual bool FS_FindNextFile(HANDLE handle, WIN32_FIND_DATA *dat) = 0;
/*! off=260(65) */		virtual bool FS_FindClose(HANDLE handle) = 0;

	public:
/*! off=264(66) */		virtual const bool IsThreadSafe (void) const = 0;

	// ADDED BY EDWARD:
	public:
		inline const bool GetStatistics (const char *const pathName, FileStatistics_t &statistics)
		{
			return FS_stat (pathName, &statistics) == 0;	// Check if statistics are valid.
		}
};
abstract_class FileSystem_Steam : public BaseFileSystem
{
	public:
		class CFileHandle : public BaseFileHandle
		{
			public:
/*! off=5(1.25) */				bool      m_isErrorOnReadingOccurs;	/// @todo CHECK MY NAME!!!
/*! off=8(2) */				long long m_nStartOffset;
/*! off=16(4) */				long long m_nLength;
/*! off=24(6) */				long long m_nFileTime;	// Last modification time.

			public:
				inline CFileHandle (void) : BaseFileHandle (), m_nStartOffset (0), m_nLength (0), m_nFileTime (0) { /* VOID */ }
		};	// sizeof (CFileHandle) == 32 (8)

		// Reliability checks.
		CompileTimeAssert (sizeof (CFileHandle) == 32u);
		CompileTimeAssert (GET_OFFSET_OF_MEMBER_VARIABLE (CFileHandle, m_isErrorOnReadingOccurs) == 5u);
		CompileTimeAssert (GET_OFFSET_OF_MEMBER_VARIABLE (CFileHandle, m_nStartOffset) == 8u);

	//----------------------------------------------------------------------------
	// Purpose: Functions implementing basic file system behavior.
	//----------------------------------------------------------------------------
	protected:
/*! off=204(51) */		virtual const bool UNKNOWN1 (const char *const a2, char *a3/*! MAYBE: const char *const pathID = NULL */) = 0;
/*! off=208(52) */		virtual FileFindHandle_t OpenFile_UNKNOWN2 (const char *filename, const char *const options, char *a4/*! MAYBE: const char *const pathID = NULL */) = 0;

/*! off=212(53) */		virtual FILE *FS_fopen (const char *filename, const char *options) = 0;
/*! off=216(54) */		virtual void FS_fclose (FILE *fp) = 0;
/*! off=220(55) */		virtual void FS_fseek (FILE *fp, long pos, int seekType) = 0;
/*! off=224(56) */		virtual long FS_ftell (FILE *fp) = 0;
/*! off=228(57) */		virtual int FS_feof (FILE *fp) = 0;
/*! off=232(58) */		virtual size_t FS_fread (void *dest, size_t count, size_t size, FILE *fp) = 0;
/*! off=236(59) */		virtual size_t FS_fwrite (const void *src, size_t count, size_t size, FILE *fp) = 0;
/*! off=240(60) */		virtual size_t FS_vfprintf (FILE *fp, const char *fmt, va_list list) = 0;
/*! off=244(61) */		virtual int FS_ferror (FILE *fp) = 0;
/*! off=248(62) */		virtual int FS_fflush (FILE *fp) = 0;
/*! off=252(63) */		virtual char *FS_fgets (char *dest, int destSize, FILE *fp) = 0;
/*! off=256(64) */		virtual int FS_stat (const char *path, FileStatistics_t *buf) = 0;
/*! off=260(65) */		virtual HANDLE FS_FindFirstFile(char *findname, WIN32_FIND_DATA *dat) = 0;
/*! off=264(66) */		virtual bool FS_FindNextFile(HANDLE handle, WIN32_FIND_DATA *dat) = 0;
/*! off=268(67) */		virtual bool FS_FindClose(HANDLE handle) = 0;

	public:
/*! off=272(68) */		virtual const bool IsThreadSafe (void) const = 0;

	// ADDED BY EDWARD:
	public:
		inline const bool GetStatistics (const char *const pathName, FileStatistics_t &statistics)
		{
			return FS_stat (pathName, &statistics) == 0;	// Check if statistics are valid.
		}
};

typedef BaseFileSystem FileSystem;	// Can be 'FileSystem_Stdio' or 'FileSystem_Steam'.
/*abstract_class FileSystem : public BaseFileSystem
{
	public:
		// Common functions....
};
*/
#endif	// ifndef HALF_LIFE_ENGINE_SDK_CLASSES_FILE_SYSTEM_INCLUDED