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
// FileSystemFile.hpp
//
// Class: FileSystemFile
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef HALF_LIFE_ENGINE_FILE_SYSTEM_FILE_INCLUDED
#define HALF_LIFE_ENGINE_FILE_SYSTEM_FILE_INCLUDED

class FileSystemFile : public BaseFile
{
	//
	// Group: Type definitions.
	//
	public:
		typedef SDK::Classes::Interface::FileSystem::FileHandle_t Handle_t;

	//
	// Group: Private members.
	//
	private:
		Handle_t *m_handle;

	//
	// Group: (Con/De)structors.
	//
	public:
		inline FileSystemFile (const char *const filePath, const char *const options, const char *const pathID = NULL) :
			BaseFile (filePath),

			m_handle (Globals::g_halfLifeEngine->GetFileSystem ()->Open (filePath, options, pathID))
		{ /* VOID */ }
		inline FileSystemFile (Handle_t *const fileHandle = FILESYSTEM_INVALID_HANDLE) : m_handle (fileHandle)
		{
			// Reliability check.
			InternalAssert (IsValid ());
		}

		inline ~FileSystemFile (void)
		{
			if (IsValid ())
				Close ();
		}

	//
	// Group: Functions.
	//
	public:
		inline const bool Read (void *const bufferPointer, const unsigned int size, const unsigned int count) const
		{
			// Reliability checks.
			InternalAssert (IsValid ());
			InternalAssert (bufferPointer != NULL);

			for (unsigned int counter (0u); counter < count; ++counter)
				if (static_cast <unsigned int> (Globals::g_halfLifeEngine->GetFileSystem ()->Read (bufferPointer, size, m_handle)) != size)
					return false;

			return true;
		}
		inline const bool Write (const void *const bufferPointer, const unsigned int size, const unsigned int count)
		{
			// Reliability checks.
			InternalAssert (IsValid ());
			InternalAssert (bufferPointer != NULL);

			for (unsigned int counter (0u); counter < count; ++counter)
				if (static_cast <unsigned int> (Globals::g_halfLifeEngine->GetFileSystem ()->Write (bufferPointer, size, m_handle)) != size)
					return false;

			return true;
		}

		inline const bool            IsEndOfFile        (void) const
		{
			// Reliability check.
			InternalAssert (IsValid ());

			return Globals::g_halfLifeEngine->GetFileSystem ()->EndOfFile (m_handle);
		}
		inline const unsigned int    GetCurrentPosition (void) const
		{
			// Reliability check.
			InternalAssert (IsValid ());

			return Globals::g_halfLifeEngine->GetFileSystem ()->Tell (m_handle);
		}
		inline const bool            Seek               (const int offset, const SeekType_t seekType)
		{
			// Reliability checks.
			InternalAssert (IsValid ());
			CompileTimeAssert
			(
				SeekType_Start   == SDK::Classes::Interface::FILESYSTEM_SEEK_HEAD &&
				SeekType_Current == SDK::Classes::Interface::FILESYSTEM_SEEK_CURRENT &&
				SeekType_End     == SDK::Classes::Interface::FILESYSTEM_SEEK_TAIL
			);

			Globals::g_halfLifeEngine->GetFileSystem ()->Seek (m_handle, offset, static_cast <SDK::Classes::Interface::FileSystemSeek_t> (seekType));

			return true;
		}
		inline const int             GetCharacter       (void) const
		{
			// Reliability check.
			InternalAssert (IsValid ());

			return '\0';	/// @todo !!!
		}
		inline const bool            Flush              (void)
		{
			// Reliability check.
			InternalAssert (IsValid ());

			Globals::g_halfLifeEngine->GetFileSystem ()->Flush (m_handle);

			return true;
		}
		inline const bool            Open               (const char *const filePath, const char *const mode = "rt")
		{
			// Reliability check.
			InternalAssert (!IsValid ());

			return Open (filePath, mode, NULL);
		}
		inline const bool            Open               (const char *const filePath, const char *const mode, const char *const pathID)
		{
			// Reliability check.
			InternalAssert (!IsValid ());

			m_pathName = filePath;
			m_handle = Globals::g_halfLifeEngine->GetFileSystem ()->Open (filePath, mode, pathID);

			return IsValid ();
		}
		inline       void            Close              (void)
		{
			// Reliability check.
			InternalAssert (IsValid ());

			m_pathName.MakeEmpty ();

			Globals::g_halfLifeEngine->GetFileSystem ()->Close (m_handle);

			m_handle = NULL;
		}
		inline const int             Print              (const char *const message)
		{
			// Reliability check.
			InternalAssert (IsValid ());

			return 0;	/// @todo !!!
		}
		inline const int             PrintFormat        (const char *const format, ...)
		{
			// Reliability check.
			InternalAssert (IsValid ());

			return 0;	/// @todo !!!
		}
		inline const bool            PutCharacter       (const unsigned char character)
		{
			// Reliability check.
			InternalAssert (IsValid ());

			return false;	/// @todo !!!
		}
		inline const bool            PutString          (const char *const string)
		{
			// Reliability check.
			InternalAssert (IsValid ());

			return false;	/// @todo !!!
		}
		inline       void            Rewind             (void)
		{
			// Reliability check.
			InternalAssert (IsValid ());

			Seek (0, SeekType_Start);
		}

		inline const bool IsValid (void) const { return m_handle != NULL; }

		inline       Handle_t *const GetHandle (void)       { return m_handle; }
		inline const Handle_t *const GetHandle (void) const { return m_handle; }

		inline const bool GetStatistics (FileStatistics_t &statistics) const
		{
			// Reliability checks.
			InternalAssert (IsValid ());
			InternalAssert (!GetPathName ().IsEmpty ());

/// @warning DO NOT USE ME, BECAUSE STEAM AND STDIO FILESYSTEMS HAVE DIFFERENT VTABLE OFFSETS FOR "FS_*" FUNCTIONS!!!!			return Globals::g_halfLifeEngine->GetFileSystem ()->GetStatistics (GetPathName (), statistics);	// Check if statistics are valid.
			return false;
		}

		inline const unsigned int GetSize (void) const
		{
			// Reliability check.
			InternalAssert (IsValid ());

			return Globals::g_halfLifeEngine->GetFileSystem ()->Size (m_handle);
		}

	//
	// Group: Operators.
	//
	public:
		inline operator       Handle_t *const (void)       { return m_handle; }
		inline operator const Handle_t *const (void) const { return m_handle; }

		inline       Handle_t *const operator -> (void)       { return m_handle; }
		inline const Handle_t *const operator -> (void) const { return m_handle; }
};

#endif	// ifndef HALF_LIFE_ENGINE_FILE_SYSTEM_FILE_INCLUDED