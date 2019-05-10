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
// File.hpp
//
// Class: File
//
// Description: Simple STDIO file wrapper class.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef FILE_INCLUDED
#define FILE_INCLUDED

class BaseFile
{
	public:
//		typedef void Handle_t;

		// Seek method constants
		enum SeekType_t
		{
			SeekType_Start   = SEEK_SET,	// Seek from start (0) of file buffer.
			SeekType_Current = SEEK_CUR,	// Seek from current position (GetCurrentPosition()) of file buffer.
			SeekType_End     = SEEK_END		// Seek backwards from end (GetSize()) of file buffer.
		};
		enum Constant_t
		{
			BadPosition = static_cast <unsigned int> (-1)
		};

	protected:
		DynamicString m_pathName;

	protected:
		inline BaseFile (void) : m_pathName () { /* VOID */ }
		inline BaseFile (const DynamicString &pathName) : m_pathName (pathName) { /* VOID */ }

		virtual inline ~BaseFile (void) { /* VOID */ }

	public:
		inline const DynamicString &GetPathName (void) const { return m_pathName; }

		inline const unsigned int GetRemainingSize (void) const { return GetSize () - GetCurrentPosition (); }

		//
		// Function: Read
		//
		// Description: Reads buffer from file stream in binary format.
		//
		// Parameters:
		//	bufferPointer - Holder for read buffer.
		//	size - Size of the buffer to read.
		//	count - Number of buffer chunks to read.
		//
		// Returns: Number of bytes red from file.
		//
		virtual inline const bool Read (void *const bufferPointer, const unsigned int size, const unsigned int count) const = 0;

		//
		// Function: Write
		//
		// Description: Writes binary buffer into file stream.
		//
		// Parameters:
		//	bufferPointer - Buffer holder, that should be written into file stream.
		//	size - Size of the buffer that should be written.
		//	count - Number of buffer chunks to write.
		//
		// Returns: Numbers of bytes written to file.
		//
		virtual inline const bool Write (const void *const bufferPointer, const unsigned int size, const unsigned int count) = 0;

		virtual inline const bool            IsEndOfFile        (void) const = 0;
		virtual inline const unsigned int    GetCurrentPosition (void) const = 0;
		virtual inline const bool            Seek               (const int offset, const SeekType_t seekType) = 0;
		virtual inline const int             GetCharacter       (void) const = 0;
		virtual inline const bool            Flush              (void) = 0;
		virtual inline const bool            Open               (const char *const filePath, const char *const mode = "rt") = 0;
		virtual inline       void            Close              (void) = 0;
		virtual inline const int             Print              (const char *const message) = 0;
		virtual inline const int             PrintFormat        (const char *const format, ...)
		{
			char tempMessageBuffer[1024u];

			// Concatenate all the arguments in one string....
			COLLECT_AND_STORE_ARGUMENTS_INTO_ONE_STRING (format, tempMessageBuffer, sizeof (tempMessageBuffer));

			return Print (tempMessageBuffer);
		}
		virtual inline const bool            PutCharacter       (const unsigned char character) = 0;
		virtual inline const bool            PutString          (const char *const string) = 0;
		virtual inline       void            Rewind             (void) = 0;
		virtual inline const unsigned int    GetSize            (void) const = 0;
		virtual inline const bool            IsValid            (void) const = 0;
//		virtual inline Handle_t *const       GetHandle          (void) const = 0;
		virtual inline const bool            GetStatistics      (FileStatistics_t &statistics) const { return false; }//= 0;	/// @todo !!!
		virtual inline const time_t          GetCreationTime    (void) const
		{
			// Reliability check.
			InternalAssert (IsValid ());

			FileStatistics_t statistics;

			// Check if statistics are valid:
			if (!GetStatistics (statistics))
				return 0;

			return statistics.st_ctime;	// Time of the creation of the file.
		}
		virtual inline const time_t          GetLastAccessTime  (void) const
		{
			// Reliability check.
			InternalAssert (IsValid ());

			FileStatistics_t statistics;

			// Check if statistics are valid:
			if (!GetStatistics (statistics))
				return 0;

			return statistics.st_atime;	// Time of the last file access.
		}
		virtual inline const time_t          GetLastChangeTime  (void) const
		{
			// Reliability check.
			InternalAssert (IsValid ());

			FileStatistics_t statistics;

			// Check if statistics are valid:
			if (!GetStatistics (statistics))
				return 0;

			return statistics.st_mtime;	// Time of the last modification of the file.
		}
		virtual inline unsigned char *const  GetData            (void) const
		{
			// Reliability check.
			InternalAssert (IsValid ());

			const unsigned int size (GetSize ());

			if (size == 0u)
				return NULL;

			// allocate space for this file's data in memory
			unsigned char *const data (new unsigned char[size]);

			// Now dump the file contents to memory....
			if (Read (data, size))
				return data;	/// @warning POSSIBLE MEMORY LEAK!!!

			delete [] data;

			return NULL;
		}
		virtual inline const bool ReadLine (DynamicString &buffer, const DynamicString::IndexType_t count) const { return false; }//= 0;	/// @todo !!!

		inline const bool Read (char *const string) const
		{
			unsigned short length;

			if (!Read (length))
				return false;

			// Reliability checks.
			InternalAssert (!IsNullString (string));

			string[0u] = '\0';

			if (length == 0u)
				return true;

			return Read (string, length);
		}
		inline const bool Read (DynamicString &string, const DynamicString::IndexType_t count) const
		{
			char *const tempBuffer (new char[count]);

			#if defined _DEBUG
				// Reliability check.
				if (tempBuffer == NULL)
				{
					AddLogEntry (true, LogLevel_Error, false, "Couldn't allocate %.2f kilobytes for temp buffer string!", count * sizeof (char) / 1024.0f);

					return false;
				}
			#endif	// if defined _DEBUG

			if (Read (tempBuffer, count, 1u))
			{
				tempBuffer[count - sizeof ('\0')] = '\0';

				string = tempBuffer;

				delete [] tempBuffer;

				return true;
			}

			delete [] tempBuffer;

			return false;
		}

		inline const bool Write (const char *const string)
		{
			return Write (string, IsNullString (string) ? 0u : GetStringLength <unsigned short> (string) + sizeof ('\0'));
		}
		inline const bool Write (const DynamicString &string, const DynamicString::IndexType_t count)
		{
			if (string.IsEmpty ())
			{
				// Reliability check.
				InternalAssert (count <= 1u);

				return Write <unsigned short> (0u);
			}

			// Reliability check.
			InternalAssert (count <= string.GetElementNumber () + sizeof ('\0'));

			// store string length followed by string itself
			return Write (count) && Write (string.GetData (), count, 1u);
		}

///		inline const bool ReadFile  (BaseFile &file)       const { return Read (file.GetData (), file.GetSize ()); }	/// @todo !!!
		inline const bool WriteFile (const BaseFile &file)       { return Write (file.GetData (), file.GetSize ()); }	/// @warning OTHER FILE TYPES THAN 'MemoryFile' MAY CAUSE MEMORY LEAKS WITH GetData!!!

		/// @note Use below define in THIS and ALSO in derived classes!
		#define DECLARE_BASE_FILE_TEMPLATE_FUNCTIONS()																					\
			template <typename elementType> inline const bool Read (elementType *const elements, const unsigned int count = 1u) const	\
			{																															\
				return Read (elements, sizeof (elementType) * count, 1u);																\
			}																															\
			template </* char */> inline const bool Read (char *const string, const unsigned int count) const							\
			{																															\
				return Read (string, count, 1u);																						\
			}																															\
			template <typename elementType> inline const bool Read (elementType &element) const											\
			{																															\
				return Read (&element, sizeof (element), 1u);																			\
			}																															\
			template </* DynamicString */> inline const bool Read (DynamicString &string) const											\
			{																															\
				DynamicString::IndexType_t length;																						\
																																		\
				if (!Read (length))																										\
					return false;																										\
																																		\
				string.MakeEmpty ();																									\
																																		\
				if (length == 0u)																										\
					return true;																										\
																																		\
				return Read (string.GetData (), length);																				\
			}																															\
																																		\
			template <typename elementType> inline const bool Write (const elementType *const elements, const unsigned int count = 1u)	\
			{																															\
				return Write (elements, sizeof (elementType) * count, 1u);																\
			}																															\
			template </* char */> inline const bool Write (const char *const string, const unsigned int count)							\
			{																															\
				if (IsNullOrEmptyString (string))																						\
				{																														\
					/* Reliability check. */																							\
					InternalAssert (count <= 1u);																						\
																																		\
					return Write <unsigned short> (0u);																					\
				}																														\
																																		\
				/* Reliability check. */																								\
				InternalAssert (count <= GetStringLength <unsigned short> (string) + sizeof ('\0'));									\
																																		\
				/* store string length followed by string itself */																		\
				return Write (count) && Write (string, count, 1u);																		\
			}																															\
			template <typename elementType> inline const bool Write (const elementType &element)										\
			{																															\
				return Write (&element, sizeof (element), 1u);																			\
			}																															\
			template </* DynamicString */> inline const bool Write (const DynamicString &string)										\
			{																															\
				return Write (string.GetData (), string.GetElementNumber () + sizeof ('\0'));											\
			}

		DECLARE_BASE_FILE_TEMPLATE_FUNCTIONS ();

	//
	// Group: Operators.
	//
	public:
		inline operator const bool (void) const { return IsValid (); }
/*
		BaseFile &operator << (const DynamicString &string)
		{
			// Reliability check.
			InternalAssert (IsValid ());

			fputs (string, m_handle);

			return *this;
		}
		BaseFile &operator << (const char *const bufferPointer)
		{
			// Reliability check.
			InternalAssert (IsValid ());

			fputs (bufferPointer, m_handle);

			return *this;
		}
		BaseFile &operator << (const char &character)
		{
			// Reliability check.
			InternalAssert (IsValid ());

			fputc (character, m_handle);

			return *this;
		}
		BaseFile &operator << (const int value)
		{
			// Reliability check.
			InternalAssert (IsValid ());

			fprintf (m_handle, "%i", value);

			return *this;
		}
		const BaseFile &operator >> (DynamicString &string) const
		{
			// Reliability check.
			InternalAssert (IsValid ());

			char tempBuffer[1024u];

			fscanf (m_handle, "%s", tempBuffer);

			string = tempBuffer;

			return *this;
		}
		const BaseFile &operator >> (char *const bufferPointer) const
		{
			// Reliability check.
			InternalAssert (IsValid ());

			fscanf (m_handle, "%s", bufferPointer);

			return *this;
		}*/
};

// implementation of memory-loaded file read write class
/// @note This class is based on PMB RACC Bot "mfile.cpp".
class MemoryFile : public BaseFile
{
	protected:
//		BufferStream m_stream;	/// @todo !!!

		unsigned char        *m_buffer;				// buffer holding memory block

		unsigned int          m_size;				// size of the file buffer
		mutable unsigned int  m_currentPosition;	// read/write pointer

	public:
		// default constructor
		inline MemoryFile (void) : m_buffer (NULL), m_size (0u), m_currentPosition (0u) { /* VOID */ }

		// not default constructor
		inline MemoryFile (unsigned char *const buffer, const unsigned int size) : m_buffer (buffer/*! MAYBE WE SHOULD COPY THIS???? */), m_size (size), m_currentPosition (0u) { /* VOID */ }
/*! DO NOT USE, BECAUSE CALL OF GetData() MAY CAUSE MEMORY LEAK....
		// not default constructor
		inline MemoryFile (const BaseFile &file) : m_buffer (file.GetData ()), m_size (file.GetSize ()), m_currentPosition (file.GetCurrentPosition ()) { /* VOID *//* }
*/
		// default destructor
		virtual inline ~MemoryFile (void)
		{
			if (IsValid ())
				Close ();
		}
/*! DO NOT USE, BECAUSE CALL OF GetData() MAY CAUSE MEMORY LEAK....
	public:
		inline MemoryFile &operator = (const BaseFile &file)
		{
			// Reliability check.
			InternalAssert (&file != this);

			m_buffer = file.GetData ();
			m_size = file.GetSize ();
			m_currentPosition = file.GetCurrentPosition ();
		}
*/
	public:
		virtual inline const bool Read (void *const bufferPointer, const unsigned int size, const unsigned int count) const
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

		virtual inline const bool Write (const void *const bufferPointer, const unsigned int size, const unsigned int count)
		{
			// Reliability checks.
			InternalAssert (IsValid ());
			InternalAssert (bufferPointer != NULL);

			if (size == 0u || count == 0u)
				return false;

			const unsigned int totalWriteSize (size * count);
			const unsigned int remainingSize (GetRemainingSize ());

			// Check if we need to grow....
			if (totalWriteSize > remainingSize)
			{
				const unsigned int newSize (remainingSize + totalWriteSize);
				unsigned char *const newBuffer (new unsigned char[newSize]);	// Allocate a new buffer.

				// Reliability check.
				if (newBuffer == NULL)
				{
					AddLogEntry (true, LogLevel_Error, false, "Couldn't allocate %.2f kilobytes for file buffer!", newSize * sizeof (unsigned char) / 1024.0f);

					return false;
				}

				MemoryCopy (m_buffer, newBuffer, GetCurrentPosition ());	// Copy existing data up to current position.

				delete [] m_buffer;	// Deallocate the old buffer.

				m_buffer = newBuffer;	// Assign new buffer.
				m_size = newSize;	// Assign new size.
			}

			for (unsigned int index (0u); index < totalWriteSize; ++index, ++m_currentPosition)
				m_buffer[m_currentPosition] = static_cast <const unsigned char *const> (bufferPointer)[index];

			return true;
		}

		virtual inline const bool Open (const char *const/* filePath*/, const char *const/* mode*/ = "rt") { return false; }	/// @todo TODO!!!

		// destroy everything
		virtual inline void Close (void)
		{
			// this function frees the memory space in which has been stored the contents of a file opened by Open().

			// Reliability check.
			InternalAssert (IsValid ());

			delete [] m_buffer;	// free the memory-loaded file data space if needed

			Clear ();
		}

		// clear everything
		inline void Clear (void)
		{
			m_pathName.MakeEmpty ();

			m_buffer = NULL;	// nulls out its pointer
			m_currentPosition = 0u;	// initialize memory file pointer at first position
			m_size = 0u;
		}

		// sets the data
		inline void SetData (unsigned char *const buffer, const unsigned int size)
		{
			m_buffer = buffer;
			m_size = size;
			m_currentPosition = 0u;	// initialize memory file pointer at first position
		}

		// checks whether we exhausted (aka IsExhausted())
		virtual inline const bool IsEndOfFile (void) const
		{
			// this function works exactly the same as feof(), it returns a non-zero value if the
			// position of the memory-loaded file read pointer has reached the end of the file.

			// if end of file is reached...
			return m_currentPosition >= m_size;	// return a true value when end of file is reached, false means the end of file is not reached yet
		}

		//
		// Function: GetCurrentPosition
		//
		// Description: Gets the current position in the opened file stream.
		//
		// Returns: Current position in the opened file stream.
		//
		virtual inline const unsigned int GetCurrentPosition (void) const
		{
			// this function works exactly the same as ftell(), it returns the position of the memory-
			// loaded file read pointer

			return m_currentPosition;	// return the position of the read pointer in the memory-loaded file
		}

		//
		// Function: IsValid
		//
		// Description: Checks whether file stream is valid.
		//
		// Returns: True if file stream valid, false otherwise.
		//
		virtual inline const bool IsValid (void) const { return m_buffer != NULL; }

		virtual inline const int GetCharacter (void) const
		{
			// this function works exactly the same as fgetc(), but whereas fgetc() reads a character from
			// a file on disk, GetCharacter() reads it from a file that has been loaded into memory using
			// Open(). The function returns the character read as an integer, else it returns 'EOF' on
			// error or when the end of file has been reached.

			// Reliability check.
			InternalAssert (IsValid ());

			// Reliability check.
			if (IsEndOfFile ())
				return EOF;

			return m_buffer[m_currentPosition++];	// read one character from the memory-loaded file, increment the file pointer and return the character we read.
		}
		virtual inline const bool Flush (void) { return false; }	/// @todo TODO!!!
		virtual inline const int  Print        (const char *const/* message*/) { return 0; }	/// @todo TODO!!!
		virtual inline const bool PutCharacter (const unsigned char/* character*/) { return false; }	/// @todo TODO!!!
		virtual inline const bool PutString    (const char *const/* string*/) { return false; }	/// @todo TODO!!!
		virtual inline       void Rewind       (void) { /* VOID */ }	/// @todo TODO!!!

		// seeks memory stream
		virtual inline const bool Seek (const int offset, const SeekType_t seekType)
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
				case SeekType_Start:
				{
					const unsigned int unsignedPos (static_cast <const unsigned int> (offset));

					if (unsignedPos >= m_size)
						return false;	// don't allow setting the pointer too far

					m_currentPosition = unsignedPos;	// seek from start of file

					return true;	// returning true means the operation was successful
				}

				case SeekType_End:
				{
					const unsigned int unsignedPos (static_cast <const unsigned int> (offset));

					if (unsignedPos >= m_size)
						return false;	// don't allow setting the pointer too far

					m_currentPosition = m_size - unsignedPos;	// seek from end of file

					return true;	// returning true means the operation was successful
				}

				// SeekType_Current
				default:
					if (m_currentPosition + offset >= m_size)
						return false;	// don't allow setting the pointer too far

					m_currentPosition += offset;	// seek from current pointer location

					return true;	// returning true means the operation was successful
			}
		}
#if 0
		inline const int ReadBuffer (unsigned char *const buffer, const unsigned int howMany/*, const bool allocatedInputBuffer = false*/)
		{
			// Reliability check.
			InternalAssert (IsValid ());

			const unsigned int num (Math::GetMinimumValueBetween (howMany, GetRemainingSize ()));
/*
			if (allocatedInputBuffer)
			{
				if (buffer != NULL)
					delete [] buffer;

				buffer = new unsigned char[num];
			}
*/
			memcpy (buffer, m_buffer + m_currentPosition, num);

			m_currentPosition += num;

			return num;
		}

		// write's binary buffer into memory stream
		inline const bool WriteBuffer (unsigned char *const buffer, const unsigned int howMany)
		{
			// Reliability check.
			InternalAssert (IsValid ());

			if (howMany <= 0)
				return false;

			unsigned int storageNeeded (m_currentPosition + howMany);

			if (storageNeeded >= m_size)
			{
				static const unsigned char BlkSize (16u);

				storageNeeded += Math::GetMaximumValueBetween <unsigned int> (BlkSize, storageNeeded / 10u);
				storageNeeded = storageNeeded - (storageNeeded % BlkSize) + BlkSize;

				if (storageNeeded >= m_size)
					m_buffer = reinterpret_cast <unsigned char *> (realloc (m_buffer, storageNeeded));
			}

			memcpy (m_buffer + m_currentPosition, buffer, howMany);

			m_currentPosition += howMany;
			m_size = Math::GetMaximumValueBetween (m_size, m_currentPosition);

			return true;
		}
#endif	// if 0
		// read's line from memory stream
		inline const bool ReadLine (char *const lineBuffer, const unsigned int length)
		{
			// this function works exactly the same as fgets(), but whereas fgets() reads a line from a
			// file on disk, ReadLine() reads a line from a text file that has been loaded into memory
			// using Open(). The three arguments are identical, the line buffer to be filled, the size
			// of the data to read at each iteration of ReadLine(), and the file pointer.

			// Reliability checks.
			InternalAssert (IsValid ());
			InternalAssert (!IsNullString (lineBuffer));
			InternalAssert (length > 0u);

			if (IsEndOfFile ())
				return false;	// return if we've already reached the end of file

			unsigned int index;
			unsigned int startLineOffset (m_currentPosition);	// remember current offset as the start of the line
			unsigned int endLineOffset (m_size - 1u);

			// set the position of the last byte to read not to exceed the buffer size
			if (GetRemainingSize () > length - 1u)
				endLineOffset = m_currentPosition + length - 1u;

			// look for the position of the first newline we find, stop anyway if buffer is full
			while (m_currentPosition < endLineOffset)
			{
				// do we have a line feed?
				if (m_buffer[m_currentPosition] == '\n')
					endLineOffset = m_currentPosition;	// save end of line offset

				++m_currentPosition;	// get one character further
			}

			// have we actually read something?
			if (m_currentPosition == startLineOffset)
				return false;	// return if nothing was read

			// now we know where we must start the reading and where we must stop it

			// copy the data to the buffer, reading from file_pos to index
			for (index = startLineOffset; index <= endLineOffset; ++index)
				lineBuffer[index - startLineOffset] = m_buffer[index];

			// is there a CR+LF sequence at the end of that line?
			if (lineBuffer[index - startLineOffset - 2u] == '\r')
			{
				lineBuffer[index - startLineOffset - 2u] = '\n';	// turn it to a standard end of line

				--index;	// get back one position
			}

			// is there a carriage return or a line feed at the end of that line?
			if (lineBuffer[index - startLineOffset - 1u] == '\r' || lineBuffer[index - startLineOffset - 1u] == '\n')
				lineBuffer[index - startLineOffset - 1u] = '\n';	// turn it to a standard end of line

			lineBuffer[index - startLineOffset] = '\0';	// terminate string

			return true;	// and return the buffer filled out with what we've read
		}
#if 0
		// writes string into file
		template <const unsigned int MaxSize> inline void WriteString (const char *const format, ...)
		{
			// Reliability check.
			InternalAssert (IsValid ());

			static char buffer[MaxSize] = {'\0'/*, ...*/};

			// Concatenate all the arguments in one string....
			COLLECT_AND_STORE_ARGUMENTS_INTO_ONE_STRING (format, buffer, sizeof (buffer));

			WriteBuffer (reinterpret_cast <unsigned char *> (buffer), sizeof (buffer));
		}

		// writes string into file
		inline void WriteString (const char *const format, ...)
		{
			// Reliability check.
			InternalAssert (IsValid ());

			static char buffer[256u] = {'\0'/*, ...*/};

			// Concatenate all the arguments in one string....
			COLLECT_AND_STORE_ARGUMENTS_INTO_ONE_STRING (format, buffer, sizeof (buffer));

			WriteBuffer (reinterpret_cast <unsigned char *> (buffer), sizeof (buffer));
		}
#endif	// if 0
		inline unsigned char *const       GetData (void) const { return m_buffer; }	// get's the raw data
		virtual inline const unsigned int GetSize (void) const { return m_size; }	// get's the raw size

		DECLARE_BASE_FILE_TEMPLATE_FUNCTIONS ();
};

class STDIOFile : public BaseFile
{
	//
	// Group: Type definitions.
	//
	public:
		typedef FILE Handle_t;

	//
	// Group: Private members.
	//
	private:
		Handle_t *m_handle;	// Pointer to C file stream.

	//
	// Group: (Con/De)structors.
	//
	public:
		//
		// Function: STDIOFile
		//
		// Description: Default file class, constructor.
		//
		inline STDIOFile (void) : m_handle (NULL) { /* VOID */ }

		//
		// Function: STDIOFile
		//
		// Description: Default file class, constructor, with file opening.
		//
		// Parameters:
		//	filePath - String containing filename.
		//	mode - String containing open mode for file.
		//
		inline STDIOFile (const char *const filePath, const char *const mode = "rt") :
			BaseFile (filePath),

			m_handle (fopen (filePath, mode))
		{ /* VOID */ }

		//
		// Function: ~STDIOFile
		//
		// Description: Default file class, destructor.
		//
		virtual inline ~STDIOFile (void)
		{
			if (IsValid ())
				Close ();
		}

	//
	// Group: Functions.
	//
	public:
		//
		// Function: IsExists
		//
		// Description: Query if a file exists.
		//
		// Parameters:
		//	filePath - Filename of the file.
		//	mode - String containing open mode for file.
		//
		// Returns: True if file exists, false if it does not.
		//
		static inline const bool IsExists (const char *const filename, const char *const mode)
		{
			// This function tests if a file exists by attempting to open it, and returns true if the file exists and can be opened, false otherwise.

			// Try to open the file, have we got a valid file pointer in return?, if so close it and return true - otherwise assume it doesn't exist.
			return STDIOFile (filename, mode).IsValid ();
		}
		static inline const bool IsExists (const char *const pathname)
		{
			// This function tests if a file exists, and returns TRUE if it is the case, FALSE otherwise.

			// Standard C library stuff, courtesy of Whistler.
			return access (pathname, 0) == 0;
		}

		static inline const bool Delete (const char *const filename)
		{
			return unlink (filename) == 0;
		}

		static inline const bool Rename (const char *const existingFilename, const char *const newFilename)
		{
			return MoveFileA (existingFilename, newFilename) == TRUE;
		}

		inline const int GetCLibraryHandle (void) const
		{
			// For more information see: "http://www.codeproject.com/Articles/1044/A-Handy-Guide-To-Handling-Handles"

			// Reliability check.
			InternalAssert (IsValid ());

			return _fileno (m_handle);
		}
		inline const HANDLE GetOSHandle (void) const
		{
			// For more information see: "http://www.codeproject.com/Articles/1044/A-Handy-Guide-To-Handling-Handles"

			// Reliability check.
			InternalAssert (IsValid ());

			const int CLibraryHandle (GetCLibraryHandle ());

			// Reliability check.
			InternalAssert (CLibraryHandle != -1);

			return reinterpret_cast <HANDLE> (_get_osfhandle (CLibraryHandle));
		}

		inline const bool GetStatistics (FileStatistics_t &statistics) const
		{
			// Reliability check.
			InternalAssert (IsValid ());

			const int CLibraryHandle (GetCLibraryHandle ());

			// Reliability check.
			InternalAssert (CLibraryHandle != -1);

			return _fstat (CLibraryHandle, &statistics) == 0;	// Check if statistics are valid.
		}

		inline const bool GetFinalPathName (char *const pathName, const unsigned short maximumPathNameLength/* = MAX_PATH - usually */, const unsigned int flags = FILE_NAME_NORMALIZED) const
		{
			// Reliability check.
			InternalAssert (IsValid ());

			const HANDLE handle (GetOSHandle ());

			// Reliability check.
			InternalAssert (handle != INVALID_HANDLE_VALUE);

			const unsigned int writtenLength (GetFinalPathNameByHandle (handle, pathName, maximumPathNameLength, flags));

			return writtenLength > 0 && writtenLength < maximumPathNameLength;
		}
		inline const bool GetFinalPathName (DynamicString &pathName) const
		{
			char tempPathName[MAX_PATH];

			if (!GetFinalPathName (tempPathName, sizeof (tempPathName)))
				return false;

			pathName = tempPathName;

			return true;
		}
		inline DynamicString GetFinalPathName (void) const
		{
			DynamicString tempPathName;

			GetFinalPathName (tempPathName);

			return tempPathName;
		}

		//
		// Function: Open
		//
		// Description: Opens file and gets it's size.
		//
		// Parameters:
		//	filePath - String containing filename.
		//	mode - String containing open mode for file.
		//
		// Returns: True if operation succeeded, false otherwise.
		//
		inline const bool Open (const char *const filePath, const char *const mode = "rt")
		{
			// Reliability check.
			InternalAssert (!IsValid ());

			m_pathName = filePath;
			m_handle = fopen (filePath, mode);

			return IsValid ();
		}

		//
		// Function: Close
		//
		// Description: Closes file, and destroys STDIO file object.
		//
		inline void Close (void)
		{
			// Reliability check.
			InternalAssert (IsValid ());

			m_pathName.MakeEmpty ();

			fclose (m_handle);

			m_handle = NULL;
		}

		//
		// Function: IsEndOfFile
		//
		// Description: Checks whether we reached end of file.
		//
		// Returns: True if reached, false otherwise.
		//
		inline const bool IsEndOfFile (void) const
		{
			// Reliability check.
			InternalAssert (IsValid ());

			return feof (m_handle) != 0;
		}

		//
		// Function: Flush
		//
		// Description: Flushes file stream.
		//
		// Returns: True if operation succeeded, false otherwise.
		//
		inline const bool Flush (void)
		{
			// Reliability check.
			InternalAssert (IsValid ());

			return fflush (m_handle) == 0;
		}

		//
		// Function: GetCharacter
		//
		// Description: Pops one character from the file stream.
		//
		// Returns: Popped from stream character, or EOF if fail.
		//
		inline const int GetCharacter (void) const
		{
			// Reliability check.
			InternalAssert (IsValid ());

			return fgetc (m_handle);
		}
/*
		//
		// Function: ReadLine
		//
		// Description: Gets the single line, from the non-binary stream.
		//
		// Parameters:
		//	buffer - Pointer to buffer, that should receive string.
		//	count - Maximum size of buffer.
		//
		// Returns: Pointer to string containing popped line.
		//
		inline const char *const ReadLine (char *buffer, const unsigned short count) const
		{
			// Reliability check.
			InternalAssert (IsValid ());

			return fgets (buffer, count, m_handle);
		}
*/
		//
		// Function: ReadLine
		//
		// Description: Gets the line from file stream, and stores it inside string class.
		//
		// Parameters:
		//	buffer - String buffer, that should receive line.
		//	count - Maximum size of buffer.
		//
		// Returns: True if operation succeeded, false otherwise.
		//
		inline const bool ReadLine (DynamicString &buffer, const DynamicString::IndexType_t count) const
		{
			// Reliability check.
			InternalAssert (IsValid ());
			InternalAssert (count > 0u);
/*
			char *const tempBuffer (new char[count]);

			buffer.MakeEmpty ();

			// Reliability check.
			if (tempBuffer == NULL)
			{
				AddLogEntry (true, LogLevel_Error, false, "Couldn't allocate %.2f kilobytes for temp buffer string!", count * sizeof (char) / 1024.0f);

				return false;
			}

			if (fgets (tempBuffer, count, m_handle) != NULL)
			{
				buffer = tempBuffer;

				delete [] tempBuffer;

				return true;
			}

			delete [] tempBuffer;

			return false;*/
			return buffer.GetLineFromFile (m_handle, count);
//			return fgets (buffer, count, m_handle) != NULL;
		}
		//
		// Function: ReadLine
		//
		// Description: Reads an entire line from the file.
		//
		// Parameters:
		//	buffer - String buffer, that should receive line.
		//
		// Returns: True if operation succeeded, false otherwise.
		//
		inline const bool ReadLine (DynamicString &buffer)
		{
			buffer.MakeEmpty ();

			// while end of file is not reached
			for
			(
				char tempCharacter (static_cast <const char> (GetCharacter ()));

				tempCharacter != '\n';	// Is end of line?

				tempCharacter = static_cast <const char> (GetCharacter ())
			)
			{
				// Is end of file?
				if (tempCharacter == EOF)
					return buffer.IsEmpty () == false;

				buffer += tempCharacter;
			}

			return true;
		}
		inline const bool GetLeftTrimmedLine (DynamicString &buffer)
		{
			buffer.MakeEmpty ();

			char tempCharacter;

			// skip any leading blanks
			while (IsTrimmingCharacter (tempCharacter = static_cast <const char> (GetCharacter ())));

			// Is end of file?
			if (tempCharacter == EOF)
				return false;

			buffer += tempCharacter;

			// while end of file is not reached
			while ((tempCharacter = static_cast <const char> (GetCharacter ())) != '\n')
			{
				// Is end of file?
				if (tempCharacter == EOF)
					return true;

				buffer += tempCharacter;
			}
		}
		inline const bool GetTrimmedLine (DynamicString &buffer)
		{
			if (GetLeftTrimmedLine (buffer) == false)
				return false;

			buffer.TrimRight ();

			return true;
		}

		//
		// Function: Print
		//
		// Description: Puts formatted buffer, into stream.
		//
		// Parameters:
		//	message - String to write.
		//
		// Returns: Number of bytes, that was written.
		//
		inline const int Print (const char *const message)
		{
			// Reliability check.
			InternalAssert (IsValid ());

			return fprintf (m_handle, message);
		}
		//
		// Function: PrintFormat
		//
		// Description: Puts formatted buffer, into stream.
		//
		// Parameters:
		//	format - String to write.
		//
		// Returns: Number of bytes, that was written.
		//
		inline const int PrintFormat (const char *const format, ...)
		{
			// Reliability check.
			InternalAssert (IsValid ());

			va_list argumentPointer;

			// concatenate all the arguments in one string
			va_start (argumentPointer, format);

			const int written (vfprintf (m_handle, format, argumentPointer));

			va_end (argumentPointer);

			return written;
		}

		//
		// Function: PutCharacter
		//
		// Description: Puts character into file stream.
		//
		// Parameters:
		//	character - Character that should be put into stream.
		//
		// Returns: Character that was putted into the stream.
		//
		inline const bool PutCharacter (const unsigned char character)
		{
			// Reliability check.
			InternalAssert (IsValid ());

			return fputc (character, m_handle) != EOF;
		}

		//
		// Function: PutString
		//
		// Description: Puts buffer into the file stream.
		//
		// Parameters:
		//	string - Buffer that should be put, into stream.
		//
		// Returns: True if operation succeeded, false otherwise.
		//
		inline const bool PutString (const char *const string)
		{
			// Reliability check.
			InternalAssert (IsValid ());

			return fputs (string, m_handle) != EOF;
		}

		//
		// Function: Read
		//
		// Description: Reads buffer from file stream in binary format.
		//
		// Parameters:
		//	bufferPointer - Holder for read buffer.
		//	size - Size of the buffer to read.
		//	count - Number of buffer chunks to read.
		//
		// Returns: Number of bytes red from file.
		//
		inline const bool Read (void *const bufferPointer, const unsigned int size, const unsigned int count) const
		{
			// Reliability checks.
			InternalAssert (IsValid ());
			InternalAssert (bufferPointer != NULL);

			return fread (bufferPointer, size, count, m_handle) == count;
		}

		//
		// Function: Write
		//
		// Description: Writes binary buffer into file stream.
		//
		// Parameters:
		//	bufferPointer - Buffer holder, that should be written into file stream.
		//	size - Size of the buffer that should be written.
		//	count - Number of buffer chunks to write.
		//
		// Returns: Numbers of bytes written to file.
		//
		inline const bool Write (const void *const bufferPointer, const unsigned int size, const unsigned int count)
		{
			// Reliability checks.
			InternalAssert (IsValid ());
			InternalAssert (bufferPointer != NULL);

			return fwrite (bufferPointer, size, count, m_handle) == count;
		}

		//
		// Function: Seek
		//
		// Description: Seeks file stream with specified parameters.
		//
		// Parameters:
		//	offset - Offset where cursor should be set.
		//	seekType - Type of offset set.
		//
		// Returns: True if operation success, false otherwise.
		//
		inline const bool Seek (const int offset, const SeekType_t seekType)
		{
			// Reliability check.
			InternalAssert (IsValid ());

			return fseek (m_handle, offset, seekType) == 0;
		}

		//
		// Function: Rewind
		//
		// Description: Rewinds the file stream.
		//
		inline void Rewind (void)
		{
			// Reliability check.
			InternalAssert (IsValid ());

			rewind (m_handle);
		}

		//
		// Function: GetSize
		//
		// Description: Gets the file size of opened file stream.
		//
		// Returns: Number of bytes in file.
		//
		inline const unsigned int GetSize (void) const
		{
			// Reliability check.
			InternalAssert (IsValid ());

			const unsigned int currentPosition (ftell (m_handle));

			// Get the filesize.
			fseek (m_handle, 0ul, SeekType_End);

			// Get the filesize.
			const unsigned int fullSize (ftell (m_handle));

			fseek (m_handle, currentPosition, SeekType_Start);

			return fullSize;
		}

		//
		// Function: GetCurrentPosition
		//
		// Description: Gets the current position in the opened file stream.
		//
		// Returns: Current position in the opened file stream.
		//
		inline const unsigned int GetCurrentPosition (void) const
		{
			// Reliability check.
			InternalAssert (IsValid ());

			return ftell (m_handle);
		}

		//
		// Function: IsValid
		//
		// Description: Checks whether file stream is valid.
		//
		// Returns: True if file stream valid, false otherwise.
		//
		inline const bool IsValid (void) const { return m_handle != NULL; }

		//
		// Function: GetHandle
		//
		// Description: Gets the pointer to file stream.
		//
		// Returns: Pointer to file stream.
		//
		inline Handle_t *const GetHandle (void) { return m_handle; }

		//
		// Function: GetHandle
		//
		// Description: Gets the constant pointer to file stream.
		//
		// Returns: Constant pointer to file stream.
		//
		inline const Handle_t *const GetHandle (void) const { return m_handle; }

		inline unsigned char *const GetData (void) const
		{
			/// @warning: DATA IS UNLOCKED!!!

			// Reliability check.
			InternalAssert (IsValid ());

			// Check if the file buffer is not initialized yet....
			if (m_handle->_cnt <= 0)
			{
				// Reliability checks.
				InternalAssert (m_handle->_base == NULL);
				InternalAssert (m_handle->_ptr == NULL);

				_filbuf (m_handle);	// Initialize the file buffer....

				// Reliability checks.
				InternalAssert (m_handle->_base != NULL);
				InternalAssert (m_handle->_ptr != NULL);
				InternalAssert (m_handle->_base < m_handle->_ptr);

				// Step back.... (beacause function _filbuf() increments FILE::_ptr, decrements FILE::_cnt and returns first element of data)
				m_handle->_ptr = m_handle->_base;

				#if defined _UNICODE
					m_handle->_cnt += sizeof (wchar_t);
				#else	// if defined _UNICODE
					++m_handle->_cnt;
				#endif	// if !defined _UNICODE
			}

			return reinterpret_cast <unsigned char *> (m_handle->_base);
		}

		DECLARE_BASE_FILE_TEMPLATE_FUNCTIONS ();

	//
	// Group: Operators.
	//
	public:
		inline operator       Handle_t *const (void)       { return m_handle; }
		inline operator const Handle_t *const (void) const { return m_handle; }

		inline       Handle_t *const operator -> (void)       { return m_handle; }
		inline const Handle_t *const operator -> (void) const { return m_handle; }

		const STDIOFile &operator << (const DynamicString &string) const
		{
			// Reliability check.
			InternalAssert (IsValid ());

			fputs (string, m_handle);

			return *this;
		}
		const STDIOFile &operator << (const char *const bufferPointer) const
		{
			// Reliability check.
			InternalAssert (IsValid ());

			fputs (bufferPointer, m_handle);

			return *this;
		}
		const STDIOFile &operator << (const char &character) const
		{
			// Reliability check.
			InternalAssert (IsValid ());

			fputc (character, m_handle);

			return *this;
		}
		const STDIOFile &operator << (const int value) const
		{
			// Reliability check.
			InternalAssert (IsValid ());

			fprintf (m_handle, "%i", value);

			return *this;
		}
		const STDIOFile &operator >> (DynamicString &string) const
		{
			// Reliability check.
			InternalAssert (IsValid ());

			char tempBuffer[1024u];

			fscanf (m_handle, "%s", tempBuffer);

			string = tempBuffer;

			return *this;
		}
		const STDIOFile &operator >> (char *const bufferPointer) const
		{
			// Reliability check.
			InternalAssert (IsValid ());

			fscanf (m_handle, "%s", bufferPointer);

			return *this;
		}
};

class FileList : public DynamicArray <DynamicString>
{
	private:
		static inline const int Compare (const DynamicString *const string1, const DynamicString *const string2)	///< compare function
		{
			// Compare all of both strings:
			return string1->CompareWithCaseCheck (*string2);
		}

	public:
		inline void Sort (void)
		{
			DynamicArray <DynamicString>::Sort (Compare);
		}

		inline void ScanDirectory (const DynamicString &path, const DynamicString &extention = "*")
		{
			RemoveAll ();

			WIN32_FIND_DATA findData;
			const HANDLE fileHandle (FindFirstFile (path + "\\*." + extention, &findData));

			if (fileHandle == INVALID_HANDLE_VALUE)
				return;

			do
			{
				PushBack ((findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ? DynamicString ('\\') + findData.cFileName : findData.cFileName);
			} while (FindNextFile (fileHandle, &findData));

			FindClose (fileHandle);
		}
};

class SoundData
{
	//
	// Group: Protected constants.
	//
	protected:
		enum Type_t
		{
			Type_Wave
		};

	public:
		virtual inline ~SoundData (void) { /* VOID */ }

	//
	// Group: Private functions.
	//
	private:
		virtual inline const Type_t GetType (void) const = 0;

	public:
		virtual inline const bool  IsWave      (void)                            const { return GetType () == Type_Wave; }
		virtual inline const bool  Merge       (const SoundData &otherSoundData/*!, @todo ADD CUSTOMIZATION VARIABLES!!! */) = 0;
		virtual inline const bool  Merge       (const BaseFile &otherSoundFile/*!, @todo ADD CUSTOMIZATION VARIABLES!!! */) = 0;
		virtual inline const bool  Mix         (const SoundData &otherSoundData/*!, @todo ADD CUSTOMIZATION VARIABLES!!! */) = 0;
		virtual inline const bool  Mix         (const BaseFile &otherSoundFile/*!, @todo ADD CUSTOMIZATION VARIABLES!!! */) = 0;
		virtual inline void        SetVolume   (const float newVolume/*!, @todo ADD CUSTOMIZATION VARIABLES!!! */) = 0;
		virtual inline const float GetDuration (void) const = 0;
		virtual inline const bool  IsValid     (void) const = 0;

	public:
		inline SoundData &operator += (const SoundData &otherSoundData)
		{
			Merge (otherSoundData);

			return *this;
		}
		inline SoundData &operator += (const BaseFile &otherSoundFile)
		{
			Merge (otherSoundFile);

			return *this;
		}
};
class WaveSoundData : public SoundData
{
	protected:
		// current sound state
		WaveFileHeader_t  m_header;
		size_t            m_size;	// sound unpacked size (for bounds checking)
		unsigned char    *m_data;	// sound pointer (see 'm_header.format.tag' for details)

	public:
		inline WaveSoundData (void) : m_data (NULL), m_size (0u) { memset (&m_header, 0u, sizeof (m_header)); }
		inline WaveSoundData (const BaseFile &file)
		{
			// Reliability check.
			InternalAssert (file.IsValid ());

			if (!file.Read (m_header))
				return;

			m_size = m_header.dataChunkLength;

			// Reliability check.
			if (m_size == 0u)
				return;

			m_data = new unsigned char[m_size];

			// Load the data
			if (!file.Read (m_data, m_size))
			{
				Delete ();

				m_data = NULL;

				return;
			}

			Initialize ();
		}
		inline WaveSoundData (const unsigned char *const buffer, const unsigned short size) :
			m_header (*reinterpret_cast <const WaveFileHeader_t *const> (buffer)),
			m_size (m_header.dataChunkLength),
			m_data (new unsigned char[m_size])
		{
			// Reliability checks.
			InternalAssert (buffer != NULL);
			InternalAssert (size >= sizeof (m_header) + m_size);

//			MemoryCopy (buffer, &m_header, sizeof (m_header));
			MemoryCopy (buffer, m_data, m_size);	// Load the data

			Initialize ();
		}
		inline WaveSoundData (const WaveSoundData &soundData) :
			m_header (soundData.m_header),
			m_size (m_header.dataChunkLength),
			m_data (new unsigned char [m_size])
		{
			// Load the data
			MemoryCopy (soundData.m_data, m_data, m_size);
		}

		virtual inline ~WaveSoundData (void) { Delete (); }

	//
	// Group: Private functions.
	//
	private:
		inline const Type_t GetType (void) const { return Type_Wave; }

	protected:
		inline const bool Merge8 (const WaveSoundData &otherSoundData)
		{
			return m_header.format.channels == 1u ? Merge8Mono (otherSoundData) : Merge8Stereo (otherSoundData);
		}
		inline const bool Merge8Mono (const WaveSoundData &otherSoundData)
		{
			if (otherSoundData.GetHeader ().format.bitsPerSample != GetHeader ().format.bitsPerSample || otherSoundData.GetHeader ().format.sampleRate != GetHeader ().format.sampleRate)
				return false;

			unsigned char *const commonData (new unsigned char[m_size + otherSoundData.m_size]);

			if (commonData == NULL)
				return false;

			MemoryCopy (m_data, commonData, m_size);

			delete [] m_data;

			MemoryCopy (otherSoundData.m_data, commonData + m_size, otherSoundData.m_size);

			m_data = commonData;

			m_size += otherSoundData.m_size;
			m_header.dataChunkLength += m_header.dataChunkLength;

			return true;
		}
		inline const bool Merge8Stereo (const WaveSoundData &otherSoundData) { return false; }	/// @todo !!!
		inline const bool Merge16 (const WaveSoundData &otherSoundData)
		{
			return m_header.format.channels == 1u ? Merge16Mono (otherSoundData) : Merge16Stereo (otherSoundData);
		}
		inline const bool Merge16Mono (const WaveSoundData &otherSoundData) { return false; }	/// @todo !!!
		inline const bool Merge16Stereo (const WaveSoundData &otherSoundData) { return false; }	/// @todo !!!

		inline const bool Mix8 (const WaveSoundData &otherSoundData)
		{
			return m_header.format.channels == 1u ? Mix8Mono (otherSoundData) : Mix8Stereo (otherSoundData);
		}
		inline const bool Mix8Mono (const WaveSoundData &otherSoundData) { return false; }	/// @todo !!!
		inline const bool Mix8Stereo (const WaveSoundData &otherSoundData) { return false; }	/// @todo !!!
		inline const bool Mix16 (const WaveSoundData &otherSoundData)
		{
			return m_header.format.channels == 1u ? Mix16Mono (otherSoundData) : Mix16Stereo (otherSoundData);
		}
		inline const bool Mix16Mono (const WaveSoundData &otherSoundData) { return false; }	/// @todo !!!
		inline const bool Mix16Stereo (const WaveSoundData &otherSoundData) { return false; }	/// @todo !!!

		inline void SetVolume8 (const float newVolume)
		{
			if (m_header.format.channels == 1u)
				SetVolume8Mono (newVolume);
			else
				SetVolume8Stereo (newVolume);
		}
		inline void SetVolume8Mono (const float newVolume)
		{
			const unsigned short volume (static_cast <unsigned short> (newVolume * 256u));

			for (size_t index (0u); index < m_size; ++index)
				m_data[index] = static_cast <char> (Math::GetClampedValueInRange <int> ((m_data[index] * volume) >> 8u/* static_cast <int> (m_data[index] * newVolume) */, -256, 255));
		}
		inline void SetVolume8Stereo (const float newVolume) { /*! @todo !!! */ }
		inline void SetVolume16 (const float newVolume)
		{
			if (m_header.format.channels == 1u)
				SetVolume16Mono (newVolume);
			else
				SetVolume16Stereo (newVolume);
		}
		inline void SetVolume16Mono (const float newVolume)
		{
			short *tempShortData (reinterpret_cast <short *> (m_data));
			const short *const tempShortDataEnd (tempShortData + m_size / sizeof (*tempShortData));
			const unsigned short volume (static_cast <unsigned short> (newVolume * 256u));

			for (/* Empty */; tempShortData < tempShortDataEnd; ++tempShortData)
				*tempShortData = static_cast <short> (Math::GetClampedValueInRange <int> ((*tempShortData * volume) >> 8u/* static_cast <int> (*tempShortData * newVolume) */, -32768, 32767));
		}
		inline void SetVolume16Stereo (const float newVolume) { /*! @todo !!! */ }

		inline void ConvertToSigned (void)
		{
			// Do any sample conversion for 8 bit PCM, convert to signed because the mixing routine assumes this.

			if (m_header.format.bitsPerSample == 8u)
				ConvertToSigned8 ();	// now convert 8-bit sounds to signed
			else
				ConvertToSigned16 ();	// now convert 16-bit sounds to signed
		}
		inline void ConvertToSigned8 (void)
		{
			// Convert unsigned data to signed.

			for (size_t index (0u); index < m_size; ++index)
				reinterpret_cast <char *> (m_data)[index] = static_cast <char> (static_cast <int> (m_data[index] - 128));
		}
		inline void ConvertToSigned16 (void)
		{
			// Convert unsigned data to signed.

			for (size_t index (0u); index < m_size; ++index)
			{
				reinterpret_cast <char *> (m_data)[index * 2u + 0u] = static_cast <char> (static_cast <int> (m_data[index * 2u + 0u] - 128));
				reinterpret_cast <char *> (m_data)[index * 2u + 1u] = static_cast <char> (static_cast <int> (m_data[index * 2u + 1u] - 128));
			}
/*			const size_t nSamples (m_size / sizeof (short));
			short *const pDest (reinterpret_cast <short *> (m_data));
			for (size_t i (0u); i < nSamples; ++i)
			{
				const int val = *((unsigned short*)&pDest[i]) - (1 << 15);
				pDest[i] = (short)val;
			}*/
		}
		inline void ConvertToUnsigned (void)
		{
			if (m_header.format.bitsPerSample == 8u)
				ConvertToUnsigned8 ();	// now convert 8-bit sounds to unsigned
			else
				ConvertToUnsigned16 ();	// now convert 16-bit sounds to unsigned
		}
		inline void ConvertToUnsigned8 (void)
		{
			// Convert signed data to unsigned.

			for (size_t index (0u); index < m_size; ++index)
				reinterpret_cast <unsigned char *> (m_data)[index] = static_cast <unsigned char> (static_cast <int> (m_data[index] + 128));
		}
		inline void ConvertToUnsigned16 (void)
		{
			// Convert signed data to unsigned.

			for (size_t index (0u); index < m_size; ++index)
			{
				reinterpret_cast <unsigned char *> (m_data)[index * 2u + 0u] = static_cast <unsigned char> (static_cast <int> (m_data[index * 2u + 0u] + 128));
				reinterpret_cast <unsigned char *> (m_data)[index * 2u + 1u] = static_cast <unsigned char> (static_cast <int> (m_data[index * 2u + 1u] + 128));
			}
/*			const size_t nSamples (m_size / sizeof (short));
			short *const pDest (reinterpret_cast <short *> (m_data));
			for (size_t i (0u); i < nSamples; ++i)
			{
				const int val = *((short*)&pDest[i]) + (1 << 15);
				pDest[i] = (unsigned short)val;
			}*/
		}

	public:
		inline const bool Merge (const SoundData &otherSoundData)
		{
			if (!IsWave ())
				return false;

			const WaveSoundData &otherWaveSoundData (static_cast <const WaveSoundData &> (otherSoundData));

			return m_header.format.bitsPerSample == 8u ? Merge8 (otherWaveSoundData) : Merge16 (otherWaveSoundData);
		}
		inline const bool Merge (const BaseFile &otherSoundFile)
		{
			// Reliability check.
			InternalAssert (otherSoundFile.IsValid ());

			WaveSoundData otherSoundData (otherSoundFile);

			if (!otherSoundData.IsValid ())
				return false;

			return Merge (otherSoundData);
		}
		inline const bool Mix (const SoundData &otherSoundData)
		{
			if (!IsWave ())
				return false;

			const WaveSoundData &otherWaveSoundData (static_cast <const WaveSoundData &> (otherSoundData));

			return m_header.format.bitsPerSample == 8u ? Mix8 (otherWaveSoundData) : Mix16 (otherWaveSoundData);
		}
		inline const bool Mix (const BaseFile &otherSoundFile)
		{
			// Reliability check.
			InternalAssert (otherSoundFile.IsValid ());

			WaveSoundData otherSoundData (otherSoundFile);

			if (!otherSoundData.IsValid ())
				return false;

			return Mix (otherSoundData);
		}
		inline void SetVolume (const float newVolume)
		{
			// Fast case - nothing changes.
			if (newVolume == 1.0f)
				return;

			if (m_header.format.bitsPerSample == 8u)
				SetVolume8 (newVolume);
			else
				SetVolume16 (newVolume);
		}

		inline const bool IsValid (void) const { return m_data != NULL; }
		inline void Delete (void) { delete [] m_data; }
		inline       WaveFileHeader_t &GetHeader (void)       { return m_header; }
		inline const WaveFileHeader_t &GetHeader (void) const { return m_header; }
		inline       unsigned char *const GetSamples (void)       { return m_data; }
		inline const unsigned char *const GetSamples (void) const { return m_data; }
		inline const size_t GetSamplesSize (void) const { return m_size; }
		inline const float GetDuration (void) const
		{
			// now return the duration
			return static_cast <float> (GetHeader ().dataChunkLength) / ((GetHeader ().format.bitsPerSample * GetHeader ().format.channels) / 8u) / GetHeader ().format.sampleRate;	// SOURCE ENGINE
//			return static_cast <float> (GetHeader ().dataChunkLength) / static_cast <float> (GetHeader ().format.bytesPerSecond);	// ORIGINAL
/*
			const float secondLength (static_cast <float> (GetHeader ().dataChunkLength) / static_cast <float> (GetHeader ().format.bytesPerSecond));

			if (!Math::IsZero (secondLength))
				return secondLength;

			char ch[32u];

			_snprintf (ch, sizeof (ch), "0.%u", GetHeader ().dataChunkLength);

			// else return milli second length.
			return static_cast <float> (atof (ch));*/
		}

		inline void Initialize (void)
		{
			if (m_header.format.bitsPerSample == 8u)
				ConvertToSigned8 ();	// now convert 8-bit sounds to signed
		}

		inline const bool ResampleInternal (const unsigned short outSampleRate = 8000u, const unsigned char outBitsPerSample = 16u)
		{
			// We need convert sound to signed even if nothing to resample.

			const unsigned char outWidth (static_cast <unsigned char> (outBitsPerSample / 8u));	// resolution - bum bits divided by 8 (8 bit is 1, 16 bit is 2)
			const unsigned char width (static_cast <unsigned char> (m_header.format.bitsPerSample / 8u));	// resolution - bum bits divided by 8 (8 bit is 1, 16 bit is 2)
			const unsigned int samples (m_header.dataChunkLength / width / m_header.format.channels);	// total samplecount in sound
			int srcsample;
			int i, sample, sample2, samplefrac, fracstep;
			const float stepscale (static_cast <float> (m_header.format.sampleRate) / outSampleRate);	// this is usually 0.5, 1, or 2
			const int outcount (static_cast <int> (samples / stepscale));

			m_header.dataChunkLength = m_size = outcount * outWidth * m_header.format.channels;

			unsigned char *const tempbuffer (new unsigned char[m_size]);
/*
			samples = outcount;
			if (sound.loopstart != -1)
				sound.loopstart = static_cast <int> (sound.loopstart / stepscale);
*/
			// resample / decimate to the current source rate
			if (stepscale == 1.0f && width == 1u && outWidth == 1u)
			{
				// fast special case
				ConvertToSigned8 ();
			}
			else
			{
				// general case
				samplefrac = 0;
				fracstep = static_cast <int> (stepscale * 256);

				if (m_header.format.channels == 2u)
				{
					for (i = 0; i < outcount; ++i)
					{
						srcsample = samplefrac >> 8;
						samplefrac += fracstep;

						if (width == 2u)
						{
							sample = reinterpret_cast <short *> (m_data)[srcsample * 2u + 0u];
							sample2 = reinterpret_cast <short *> (m_data)[srcsample * 2u + 1u];
						}
						else
						{
							sample = static_cast <int> (static_cast <char> (m_data[srcsample * 2u + 0u])) << 8;
							sample2 = static_cast <int> (static_cast <char> (m_data[srcsample * 2u + 1u])) << 8;
						}

						if (outWidth == 2u)
						{
							reinterpret_cast <short *> (tempbuffer)[i * 2u + 0u] = static_cast <short> (sample);
							reinterpret_cast <short *> (tempbuffer)[i * 2u + 1u] = static_cast <short> (sample2);
						}
						else
						{
							reinterpret_cast <char *> (tempbuffer)[i * 2u + 0u] = static_cast <char> (sample >> 8);
							reinterpret_cast <char *> (tempbuffer)[i * 2u + 1u] = static_cast <char> (sample2 >> 8);
						}
					}
				}
				else for (i = 0; i < outcount; ++i)
				{
					srcsample = samplefrac >> 8;
					samplefrac += fracstep;

					if (width == 2u)
						sample = reinterpret_cast <short *> (m_data)[srcsample];
					else
						sample = static_cast <int> (static_cast <char> (m_data[srcsample])) << 8;

					if (outWidth == 2u)
						reinterpret_cast <short *> (tempbuffer)[i] = static_cast <short> (sample);
					else
						reinterpret_cast <char *> (tempbuffer)[i] = static_cast <char> (sample >> 8);
				}
			}

			m_header.format.sampleRate = outSampleRate;
			m_header.format.bitsPerSample = outBitsPerSample;
			m_header.format.bytesPerSample = m_header.format.channels * m_header.format.bitsPerSample / 8u;
			m_header.format.bytesPerSecond = m_header.format.sampleRate * m_header.format.bytesPerSample;

			delete [] m_data;
			m_data = tempbuffer;

			return true;
		}

	public:
		inline WaveSoundData &operator = (const BaseFile &file)
		{
			// Reliability check.
			InternalAssert (file.IsValid ());

			Delete ();

			file.Read (m_header);

			m_size = m_header.dataChunkLength;
			m_data = new unsigned char[m_size];

			// Load the data
			file.Read (m_data, m_size);

			Initialize ();

			return *this;
		}
		inline WaveSoundData &operator = (const WaveSoundData &soundData)
		{
			Delete ();

			m_header = soundData.m_header;
			m_size = m_header.dataChunkLength;
			m_data = new unsigned char [m_size];

			// Load the data
			MemoryCopy (soundData.m_data, m_data, m_size);

			return *this;
		}
};
/*! @warning NOT WORK!!!
template <typename FileType, typename SoundDataType> class SoundFile : public FileType, public SoundDataType
{
	public:
		inline SoundFile (const char *const filePath, const char *const mode = "rb") :
			FileType (filePath, mode)
		{
			// Reliability check.
			InternalAssert (FileType::IsValid ());

			/// @note We can't initialize 'SoundDataType' class in initialize list because this pointer is not provided there :(
//			SoundDataType (static_cast <FileType &> (*this));
			SoundDataType::operator = (static_cast <FileType &> (*this));

			// Reliability check.
			InternalAssert (IsValid ());
		}

	public:
		inline const bool IsValid (void) const { return FileType::IsValid () && SoundDataType::IsValid (); }
		inline const bool Open    (const char *const filePath, const char *const mode = "rb")
		{
			if (!FileType::Open (filePath, mode))
				return false;

			return SoundDataType::operator = (static_cast <FileType &> (*this)).IsValid ();
		}
		inline       void Close   (void)
		{
			FileType::Close ();
			SoundDataType::Delete ();	/// @todo FULL RESET!!!
		}
};
typedef SoundFile <STDIOFile, WaveSoundData> STDIOWaveFile;
*/
#endif	// ifndef FILE_INCLUDED