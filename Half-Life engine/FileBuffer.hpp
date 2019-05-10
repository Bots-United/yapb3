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
// FileBuffer.hpp
//
// Class: FileBuffer
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef HALF_LIFE_ENGINE_FILE_BUFFER_INCLUDED
#define HALF_LIFE_ENGINE_FILE_BUFFER_INCLUDED

class FileBuffer : public MemoryFile
{
	//
	// Group: (Con/De)structors.
	//
	public:
		inline FileBuffer (const char *const filePath) : MemoryFile ()
		{
			m_pathName = filePath;
			m_buffer = Globals::g_halfLifeEngine->LoadFile (filePath, &m_size);
		}

		inline ~FileBuffer (void)
		{
			if (IsValid ())
				Close ();
		}

	//
	// Group: Functions.
	//
	public:
		inline const bool Open (const char *const filePath, const char *const/* mode*/ = "rt")
		{
			// this function works exactly the same as fopen(), but whereas fopen() opens a file and have
			// its access to it on disk, Open() completely loads the file into memory. The arguments are
			// identical, a pointer to the file path string and the access mode to it. Please not that the
			// access mode parameter has been supplied for keeping the similarity with fopen(), but the
			// file is still obviously open with read authorization.

			// Reliability check.
			InternalAssert (!IsValid ());

			m_pathName = filePath;
			m_buffer = Globals::g_halfLifeEngine->LoadFile (filePath, &m_size);

			return IsValid ();
		}

		inline void Close (void)
		{
			// this function frees the memory space in which has been stored the contents of a file opened by Open().

			// Reliability check.
			InternalAssert (IsValid ());

			m_pathName.MakeEmpty ();

			Globals::g_halfLifeEngine->FreeFile (m_buffer);

			m_buffer = NULL;
		}
};

#endif	// ifndef HALF_LIFE_ENGINE_FILE_BUFFER_INCLUDED