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
// Stream.hpp
//
// Class: Stream
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef CORE_TOOL_LIBRARY_STREAM_INCLUDED
#define CORE_TOOL_LIBRARY_STREAM_INCLUDED

// implementation of memory-loaded file read write class
/// @note This class is based on PMB RACC Bot "mfile.cpp".
class BufferStream
{
	protected:
		unsigned char        *m_buffer;				// buffer holding memory block

		unsigned int          m_size;				// size of the buffer
		mutable unsigned int  m_currentPosition;	// read/write pointer
		unsigned int          m_itemCount;			// number of writed data bytes

	public:
		// default constructor
		inline BufferStream (void) : m_buffer (NULL), m_size (0u), m_currentPosition (0u), m_itemCount (0u) { /* VOID */ }

		// not default constructor
		inline BufferStream (unsigned char *const buffer, const unsigned int size) : m_buffer (buffer), m_size (size), m_currentPosition (0u), m_itemCount (0u) { /* VOID */ }

	public:
		inline const bool Read (void *const bufferPointer, const unsigned int size, const unsigned int count) const
		{
			// this function works exactly the same as fread(), but whereas fread() reads data from a
			// file on disk, Read() reads data from a text file that has been loaded into memory
			// using Open(). The arguments are identical, the destination buffer to be filled, the size
			// of the data to read and the number of blocks to read, and the file pointer. If the total
			// amount of data requested for reading exceeds the length of the file, only the allowed data
			// is read, and the function returns the number of blocks it read successfully.

			// Reliability checks.
			InternalAssert (IsValid ());
			InternalAssert (bufferPointer != NULL);

			if (size == 0u || count == 0u)
				return false;
/*
			// compute the maximum amount of data we're allowed to read
			const unsigned int blocksRead (Math::GetMinimumValueBetween (GetRemainingSize () / size, count));

			// read the specified amount of data in the memory-loaded file
			Copy (m_buffer + m_currentPosition, bufferPointer, blocksRead * size);
			m_currentPosition += blocksRead * size;	// update file pointer position

			return blocksRead;	// return the number of blocks we read
*/
			const unsigned int totalReadSize (size * count);

			if (totalReadSize > GetRemainingSize ())
				return false;

			for (unsigned int index (0u); index < totalReadSize; ++index, ++m_currentPosition)
				static_cast <unsigned char *const> (bufferPointer)[index] = m_buffer[m_currentPosition];

			return true;
		}

		inline const bool Write (const void *const bufferPointer, const unsigned int size, const unsigned int count)
		{
			// Reliability checks.
			InternalAssert (IsValid ());
			InternalAssert (bufferPointer != NULL);

			if (size == 0u || count == 0u)
				return false;

			const unsigned int totalWriteSize (size * count);

			// Check if we need to grow....
			if (totalWriteSize > GetRemainingSize ())
				return false;

			for (unsigned int index (0u); index < totalWriteSize; ++index, ++m_currentPosition)
				m_buffer[m_currentPosition] = static_cast <const unsigned char *const> (bufferPointer)[index];

			m_itemCount += totalWriteSize;

			return true;
		}

		// clear everything
		inline void Clear (void)
		{
			m_buffer = NULL;	// nulls out its pointer
			m_currentPosition = 0u;	// initialize memory file pointer at first position
			m_size = 0u;
			m_itemCount = 0u;
		}

		// sets the data
		inline void SetData (unsigned char *const buffer, const unsigned int size)
		{
			m_buffer = buffer;
			m_size = size;
			m_currentPosition = 0u;	// initialize memory file pointer at first position
			m_itemCount = 0u;	/// @note The input buffer can be not empty, so this variable can lie....
		}

		// checks whether we exhausted (aka IsExhausted())
		inline const bool IsEnd (void) const
		{
			// this function works exactly the same as feof(), it returns a non-zero value if the
			// position of the memory-loaded file read pointer has reached the end of the file.

			// if end of file is reached...
			return m_currentPosition >= m_size;	// return a true value when end of file is reached, false means the end of file is not reached yet
		}

		inline const unsigned int GetRemainingSize (void) const { return GetSize () - GetCurrentPosition (); }

		//
		// Function: GetCurrentPosition
		//
		// Description: Gets the current position in the opened file stream.
		//
		// Returns: Current position in the opened file stream.
		//
		inline const unsigned int GetCurrentPosition (void) const
		{
			// this function works exactly the same as ftell(), it returns the position of the memory-
			// loaded file read pointer

			return m_currentPosition;	// return the position of the read pointer in the memory-loaded file
		}

		inline const unsigned int GetElementNumber (void) const { return m_itemCount; }

		//
		// Function: IsValid
		//
		// Description: Checks whether file stream is valid.
		//
		// Returns: True if file stream valid, false otherwise.
		//
		inline const bool IsValid (void) const { return m_buffer != NULL; }

		inline const int GetCharacter (void) const
		{
			// this function works exactly the same as fgetc(), but whereas fgetc() reads a character from
			// a file on disk, GetCharacter() reads it from a file that has been loaded into memory using
			// Open(). The function returns the character read as an integer, else it returns 'EOF' on
			// error or when the end of file has been reached.

			// Reliability check.
			InternalAssert (IsValid ());

			// Reliability check.
			if (IsEnd ())
				return EOF;

			return m_buffer[m_currentPosition++];	// read one character from the memory-loaded file, increment the file pointer and return the character we read.
		}
		inline const bool Flush (void) { return false; }	/// @todo TODO!!!
		inline const bool PutCharacter (const unsigned char/* character*/) { return false; }	/// @todo TODO!!!
		inline const bool PutString    (const char *const/* string*/) { return false; }	/// @todo TODO!!!
		inline       void Rewind       (void) { /* VOID */ }	/// @todo TODO!!!

		// seeks memory stream
		inline const bool Seek (const int offset, const BaseFile::SeekType_t seekType)
		{
			// this function works exactly the same as fseek(), it sets the position of the memory-
			// loaded file read pointer to the wanted offset, relatively to the specified offset mode
			// which can be SeekType_Start (from start of file), SeekType_End (from end of file) or SeekType_Current (from
			// current pointer position).

			// Reliability check.
			InternalAssert (IsValid ());

			// given the offset mode, set the pointer at the right location
			switch (seekType)
			{
				case BaseFile::SeekType_Start:
				{
					const unsigned int unsignedPos (static_cast <const unsigned int> (offset));

					if (unsignedPos >= m_size)
						return false;	// don't allow setting the pointer too far

					m_currentPosition = unsignedPos;	// seek from start of file

					return true;	// returning true means the operation was successful
				}

				case BaseFile::SeekType_End:
				{
					const unsigned int unsignedPos (static_cast <const unsigned int> (offset));

					if (unsignedPos >= m_size)
						return false;	// don't allow setting the pointer too far

					m_currentPosition = m_size - unsignedPos;	// seek from end of file

					return true;	// returning true means the operation was successful
				}

				// BaseFile::SeekType_Current
				default:
					if (m_currentPosition + offset >= m_size)
						return false;	// don't allow setting the pointer too far

					m_currentPosition += offset;	// seek from current pointer location

					return true;	// returning true means the operation was successful
			}
		}

		inline unsigned char *const GetData (void) const { return m_buffer; }	// get's the raw data
		inline const unsigned int   GetSize (void) const { return m_size; }	// get's the raw size

		template <typename elementType> inline const bool Read (elementType *const elements, const unsigned int count = 1u) const
		{
			return Read (elements, sizeof (elementType) * count, 1u);
		}
		template </* char */> inline const bool Read (char *const string, const unsigned int count) const
		{
			return Read (string, count, 1u);
		}
		template <typename elementType> inline const bool Read (elementType &element) const
		{
			return Read (&element, sizeof (element), 1u);
		}
		template </* DynamicString */> inline const bool Read (DynamicString &string) const
		{
			DynamicString::IndexType_t length;

			if (!Read (length))
				return false;

			string.MakeEmpty ();

			if (length == 0u)
				return true;

			return Read (string.GetData (), length);
		}

		template <typename elementType> inline const bool Write (const elementType *const elements, const unsigned int count = 1u)
		{
			return Write (elements, sizeof (elementType) * count, 1u);
		}
		template </* char */> inline const bool Write (const char *const string, const unsigned int count)
		{
			if (IsNullOrEmptyString (string))
			{
				// Reliability check.
				InternalAssert (count <= 1u);

				return Write <unsigned short> (0u);
			}

			// Reliability check.
			InternalAssert (count <= GetStringLength <unsigned short> (string) + sizeof ('\0'));

			// store string length followed by string itself
			return Write (count) && Write (string, count, 1u);
		}
		template <typename elementType> inline const bool Write (const elementType &element)
		{
			return Write (&element, sizeof (element), 1u);
		}
		template </* DynamicString */> inline const bool Write (const DynamicString &string)
		{
			return Write (string.GetData (), string.GetElementNumber () + sizeof ('\0'));
		}
};

//-----------------------------------------------------------------------------
// This is useful if you just want a buffer to write into on the stack.
//-----------------------------------------------------------------------------
template <const unsigned int size> class StaticBufferStream : public BufferStream
{
	public:
		enum { MaximumPossibleSize = size };

	private:
		unsigned char m_staticData[size];

	public:
		inline StaticBufferStream (void) : BufferStream (m_staticData, size) { /* VOID */ }
};

class DynamicBufferStream : public BufferStream
{
	public:
		/// @todo !!!
};

#endif	// ifndef CORE_TOOL_LIBRARY_STREAM_INCLUDED