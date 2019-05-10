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
// String.hpp
//
// Class: String
//
// Description: Reference counted string class.
//
/// @todo MAKE WORK WITH STRINGS WITH OTHER 'indexType's, BUT FOR NOW IT'S CRASHES!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef STRING_INCLUDED
#define STRING_INCLUDED

template <typename elementType, typename elementIndexType, typename indexType, const indexType elementsNumber> class ElementSet
{
	//
	// Group: Types definitions.
	//
	public:
		typedef elementType      ElementType_t;
		typedef elementIndexType ElementIndexType_t;
		typedef indexType        IndexType_t;

	//
	// Group: Special points.
	//
	public:
		enum
		{
			ElementsNumber = elementsNumber	// Array size.
		};

	protected:
		elementType m_set[elementsNumber];

	public:
		inline ElementSet (void) { /* VOID */ }
		inline ElementSet (const elementType *const set) { MemoryCopy (set, m_set, elementsNumber); }
		inline ElementSet (const ElementSet &other)      { MemoryCopy (other.m_set, m_set, elementsNumber); }

	public:
		inline ElementSet &operator = (const elementType *const set)
		{
			MemoryCopy (set, m_set, elementsNumber);

			return *this;
		}
		inline ElementSet &operator = (const ElementSet &other)
		{
			MemoryCopy (other.m_set, m_set, elementsNumber);

			return *this;
		}

		// casting operators
		inline operator       elementType *const (void)       { return m_set; }
		inline operator const elementType *const (void) const { return m_set; }

		// array access
		inline       elementType &operator [] (const elementIndexType index)       { return m_set[index]; }
		inline const elementType &operator [] (const elementIndexType index) const { return m_set[index]; }

	public:
		inline       elementType *const GetData (void)       { return m_set; }
		inline const elementType *const GetData (void) const { return m_set; }
};
// Purpose: Shared code for parsing / searching for characters in a string using lookup tables
class CharacterSet : public ElementSet <bool, char, unsigned char, static_cast <unsigned char> (-1)>
{
	//-----------------------------------------------------------------------------
	// Purpose: builds a simple lookup table of a group of important characters
	// Input  : *string - null terminated list of characters to flag
	//-----------------------------------------------------------------------------

	public:
		explicit inline CharacterSet (const ElementIndexType_t *const string) { Assign (string); }

	public:
		inline CharacterSet &operator = (const ElementIndexType_t *const string)
		{
			Assign (string);

			return *this;
		}

	public:
		inline void Reset (void)
		{
			for (IndexType_t index (0u); index < ElementsNumber; ++index)
				m_set[index] = false;
		}

		inline void Assign (const ElementIndexType_t *const string)
		{
			// Reliability check.
			InternalAssert (!IsNullOrEmptyString (string));

			IndexType_t index (0u);

			for (/* Empty */; string[index] != '\0'; ++index)
			{
				// Reliability check.
				InternalAssert (index < ElementsNumber);

				m_set[string[index]] = true;
			}

			memset (m_set + index, false, ElementsNumber - index);
		}
};

template <typename indexType/* = unsigned short*/> class DynamicArray <char, indexType>
{/*
	friend DynamicArray <char, unsigned char>;	// ShortDynamicString
	friend DynamicArray <char, unsigned short>;	// DynamicString
	friend DynamicArray <char, unsigned int>;	// LongDynamicString
*/
	//
	// Group: Protected members.
	//
	protected:
		char      *m_elements;		// The actual array of data.
		indexType  m_allocatedSize;	// Allocated size.
		indexType  m_itemCount;		// ¹ of elements (upperBound - 1).

	//
	// Group: Special points.
	//
	public:
		enum
		{
			ElementSize = sizeof (char),	// One string element size.
			BadPosition = static_cast <indexType> (-1),	// Generic bad/missing length/position.
			MaximumPossibleSize = min/*Math::GetMinimumValueBetween <unsigned int>*/ (BadPosition, CoreToolLibrary::BadPosition/* / ElementSize*/ / 2u)	// Maximum possible string size.
		};

	//
	// Group: Types definitions.
	//
	public:
		typedef char      ElementType_t;
		typedef indexType IndexType_t;

		typedef const int (*const SortComparatorFunction_t) (const char *const entry1, const char *const entry2);

	//
	// Group: (Con/De)structors.
	//
	public:
		//
		// Function: DynamicArray
		//
		// Description: Initializes a element buffer with given by the size.
		//
		// Parameters:
		//	size - Initial size to assign.
		//
		explicit inline DynamicArray (const indexType size = sizeof ('\0')/*16u-STL-String, 32u-HL2 Engine*/) : m_elements (new char[size]), m_allocatedSize (size), m_itemCount (0u)
		{
			// Reliability check.
			InternalAssert (size >= 1u && size <= MaximumPossibleSize);

			#if defined _DEBUG
				// Reliability check.
				if (m_elements == NULL)
				{
					m_allocatedSize = 0u;

					AddLogEntry (true, LogLevel_Error, false, "Couldn't allocate %.2f kilobytes for string buffer!", size * sizeof (ElementType_t) / 1024.0f);

					return;
				}
			#endif	// if defined _DEBUG

			// Terminate the string.
			m_elements[0u] = '\0';
		}

		//
		// Function: DynamicArray
		//
		// Description: Initializes a array buffer with given by the input element in amount of 'count'.
		//
		// Parameters:
		//	input - Element to assign.
		//	count - Number of elements to use for the content (i.e., it's length).
		//
		inline DynamicArray (const char input, const indexType count = 1u) : m_elements (NULL), m_allocatedSize (0u), m_itemCount (0u)
		{
			Assign (input, count);
		}

		//
		// Function: DynamicArray
		//
		// Description: Initializes a array buffer with given by the buffer.
		//
		// Parameters:
		//	bufferPointer - Array buffer to assign.
		//	count - Number of elements to use for the content (i.e., it's length).
		//
		inline DynamicArray (const char *const bufferPointer, const indexType count) : m_elements (NULL), m_allocatedSize (0u), m_itemCount (0u)
		{
			Assign (bufferPointer, count);
		}

		//
		// Function: DynamicArray
		//
		// Description: Initializes a array buffer with given by the buffer.
		//
		// Parameters:
		//	bufferPointer - Array buffer to assign.
		//
		inline DynamicArray (const char *const bufferPointer) : m_elements (NULL), m_allocatedSize (0u), m_itemCount (0u)
		{
			Assign (bufferPointer);
		}

		//
		// Function: DynamicArray
		//
		// Description: Array copying constructor.
		//
		// Parameters:
		//	other - Other array that should be assigned to this one.
		//	startIndex - Starting position of the subarray of the input array that forms the new content.
		//	count - Number of elements to use for the content (i.e., it's length).
		//
		/*template <typename indexType2> */inline DynamicArray (const DynamicArray/* <char, indexType2>*/ &other, const indexType/*2*/ startIndex, const indexType/*2*/ count) : m_elements (NULL), m_allocatedSize (0u), m_itemCount (0u)
		{
			Assign/* <indexType2>*/ (other, startIndex, count);
		}

		//
		// Function: DynamicArray
		//
		// Description: Array copying constructor.
		//
		// Parameters:
		//	other - Other array that should be assigned to this one.
		//	startIndex - Starting position of the subarray of the input array that forms the new content.
		//
		/*template <typename indexType2> */inline DynamicArray (const DynamicArray/* <char, indexType2>*/ &other, const indexType/*2*/ startIndex = 0u) : m_elements (NULL), m_allocatedSize (0u), m_itemCount (0u)
		{
			Assign/* <indexType2>*/ (other, startIndex);
		}

		//
		// Function: ~DynamicArray
		//
		// Description: Default array class destructor.
		//
		virtual inline ~DynamicArray (void)
		{
			// Destroy the array buffer.
			delete [] m_elements;
		}

	//
	// Group: Private functions.
	//
	private:
		//
		// Function: Initialize
		//
		// Description: Initializes string buffer.
		//
		// Parameters:
		//	length - Initial length of string.
		//
/*		inline void Initialize (const indexType length)
		{
			const indexType freeSize (m_allocatedSize - m_itemCount - 1u);

			if (length <= freeSize)
				return;

			const indexType delta (m_allocatedSize > 64u ? m_allocatedSize / 2u : m_allocatedSize > 8u ? 16u : 4u);

			SetSize (m_allocatedSize + (freeSize + delta < length ? length - freeSize : delta));
		}

		//
		// Function: InsertSpace
		//
		// Description: Inserts space at specified location, with specified length.
		//
		// Parameters:
		//	index - Location to insert space.
		//	size - Size of space insert.
		//
		inline void InsertSpace (const indexType index, const indexType size)
		{
			// Reliability check.
			InternalAssert (index <= m_itemCount);

			Initialize (size);

			MoveItems (index + size, index);
		}
*/
	//
	// Group: Protected functions.
	//
	protected:
		//
		// Function: MoveItems
		//
		// Description: Moves items inside array pointer.
		//
		// Parameters:
		//	destinationIndex - Destination index.
		//	sourceIndex - Source index.
		//
		inline void MoveItems (const indexType destinationIndex, const indexType sourceIndex)
		{
			// Reliability check.
			InternalAssert (destinationIndex <= m_allocatedSize && sourceIndex <= m_itemCount);

			if (destinationIndex != sourceIndex)
				memmove (m_elements + destinationIndex, m_elements + sourceIndex, (m_itemCount - sourceIndex + 1u) * ElementSize);
		}

		//
		// Function: IsInside
		//
		// Description: Test if 'bufferPointer' points inside array.
		//
		// Parameters:
		//	bufferPointer - Element buffer to check for.
		//
		// Returns: True if 'bufferPointer' points inside array, false otherwise.
		//
		inline const bool IsInside (const char *const bufferPointer) const
		{
			return bufferPointer >= m_elements && bufferPointer <= m_elements + m_itemCount;	// Don't ask.
		}

	//
	// Group: Functions.
	//
	public:
		//
		// Function: SetSize
		//
		// Description: Sets the size of the array.
		//
		// Parameters:
		//	newSize - Size to what array should be resized.
		//	keepData - Keep exiting data, while resizing array or not.
		//
		// Returns: True if operation succeeded, false otherwise.
		//
		inline const bool SetSize (const indexType newSize)
		{
			// Reliability check.
			InternalAssert (newSize > 0u);
			InternalAssert (newSize <= MaximumPossibleSize);

			// Is no needed to resize?
			if (newSize == m_allocatedSize)
				return true;

			// Allocate a new buffer.
			char *const newBuffer (new char[newSize]);

			#if defined _DEBUG
				// Reliability check.
				if (newBuffer == NULL)
				{
					AddLogEntry (true, LogLevel_Error, false, "Couldn't allocate %.2f kilobytes for string buffer!", newSize * sizeof (ElementType_t) / 1024.0f);

					return false;
				}
			#endif	// if defined _DEBUG

			if (!IsEmpty ())
			{
				if (m_itemCount >= newSize)
					m_itemCount = newSize - sizeof ('\0');	// Trim item count to new size.

				for (indexType index (0u); index < m_itemCount; newBuffer[index] = m_elements[index], ++index);	// Copy existing elements.
//				for (indexType index (0u); index < m_itemCount; new (newBuffer + index) elementType (m_elements[index]), ++index);	// Copy existing elements.
			}

			delete [] m_elements;	// Deallocate the old buffer.

			m_elements = newBuffer;	// Assign new buffer.
			m_allocatedSize = newSize;

			return true;
		}
		inline const bool SetSizeWithoutKeepingData (const indexType newSize)
		{
			// Reliability check.
			InternalAssert (newSize > 0u);
			InternalAssert (newSize <= MaximumPossibleSize);

			// Is no needed to resize?
			if (newSize == m_allocatedSize)
				return true;

			// Allocate a new buffer.
			char *const newBuffer (new char[newSize]);

			#if defined _DEBUG
				// Reliability check.
				if (newBuffer == NULL)
				{
					AddLogEntry (true, LogLevel_Error, false, "Couldn't allocate %.2f kilobytes for string buffer!", newSize * sizeof (ElementType_t) / 1024.0f);

					return false;
				}
			#endif	// if defined _DEBUG

			delete [] m_elements;	// Destroy the old buffer.

			m_elements = newBuffer;	// Assign new buffer.
			m_allocatedSize = newSize;

			return true;
		}

		//
		// Function: GetMemoryUsage
		//
		// Description: Gets the using by this dynamic array memory.
		//
		// Returns: How much memory this dynamic array is using.
		//
		inline const unsigned int GetMemoryUsage (void) const
		{
			return sizeof (DynamicArray) + /*ElementSize * */m_allocatedSize;
		}

		//
		// Function: GetAllocatedSize
		//
		// Description: Gets allocated size of array.
		//
		// Returns: Number of allocated items.
		//
		inline const indexType GetAllocatedSize (void) const { return m_allocatedSize; }

		//
		// Function: GetElementNumber
		//
		// Description: Gets real number currently in array.
		//
		// Returns: Number of elements.
		//
		inline const indexType GetElementNumber (void) const { return m_itemCount; }

		//
		// Function: ReleaseBuffer
		//
		// Description: Terminates the string with null character.
		//
		inline void ReleaseBuffer (void) { ReleaseBuffer (GetStringLength <indexType> (m_elements)); }

		//
		// Function: ReleaseBuffer
		//
		// Description: Terminates the string with null character with specified buffer end.
		//
		// Parameters:
		//	newLength - End of buffer.
		//
		inline void ReleaseBuffer (const indexType newLength)
		{
			// Reliability check.
			InternalAssert (newLength < m_allocatedSize);

			if (m_itemCount <= newLength)
				return;

			m_elements[m_itemCount = newLength] = '\0';	// Update length and terminate the string....
		}

		inline const bool IsDigit (void) const
		{
			// Reliability check.
			InternalAssert (!IsEmpty ());

			for (indexType index (0u); index < m_itemCount; ++index)
				if (!IsDigitCharacter (m_elements[index]))
					return false;

			return true;
		}
		inline const bool IsAlpha (void) const
		{
			// Reliability check.
			InternalAssert (!IsEmpty ());

			for (indexType index (0u); index < m_itemCount; ++index)
				if (!IsAlphaCharacter (m_elements[index]))
					return false;

			return true;
		}

		//
		// Function: GetField
		//
		// Description:
		//	This function gets and returns a particuliar field in a 'string' where several fields are
		//	concatenated. Fields can be words, or groups of words between quotes; separators may be
		//	white space or tabs. A purpose of this function is to provide bots with the same Cmd_Argv()
		//	convenience the engine provides to real clients. This way the handling of real client
		//	commands and bot client commands is exactly the same, just have a look in engine.cpp
		//	for the hooking of pfnCmd_Argc(), pfnCmd_Args() and pfnCmd_Argv(), which redirects the call
		//	either to the actual engine functions (when the caller is a real client), either on
		//	our function here, which does the same thing, when the caller is a bot.
		//
		// Parameters:
		//	fieldID - Field index to get from.
		//
		// Returns: Wanted field string.
		//
		template <typename indexType2> inline const DynamicArray &GetField (indexType2 fieldID) const
		{
			// Reliability check.
			InternalAssert (!IsEmpty ());

			static DynamicArray field;

			// Reset the field string, as it's static.
			field.MakeEmpty ();

			// While we have not reached end of line....
			for (indexType index (0u), fieldStart, fieldStop; index < m_itemCount; --fieldID)
			{
				// Ignore spaces or tabs.
				for (/* Empty */; index < m_itemCount && (m_elements[index] == ' ' || m_elements[index] == '\t'); ++index);

				// Is this field multi-word between quotes or single word?
				if (m_elements[index] == '"')
				{
					// Move one step further to bypass the quote and save field start position.
					fieldStart = ++index;

					// Reach end of field.
					for (/* Empty */; index < m_itemCount && m_elements[index] != '"'; ++index);

					// Move one step further to bypass the quote and save field stop position.
					fieldStop = index++;
				}
				else
				{
					// Save field start position.
					fieldStart = index;

					// Reach end of field.
					for (/* Empty */; index < m_itemCount && m_elements[index] != ' ' && m_elements[index] != '\t'; ++index);

					// Save field stop position.
					fieldStop = index;
				}

				// Is this field we just processed the wanted one?
				if (fieldID == 0u)
				{
					// Store the field value in a string.
					field = GetSubstring (fieldStart, fieldStop - fieldStart);

					// And stop parsing.
					break;
				}

				// We have parsed one field more....
			}

			// Returns the wanted field.
			return field;
		}

		//
		// Function: MakeEmpty
		//
		// Description: Empties the string.
		//
		inline void MakeEmpty (void)
		{
			m_elements[m_itemCount = 0u] = '\0';	// Update  buffer length and terminate the string.
		}

		//
		// Function: GetSubstring
		//
		// Description: Gets the substring by specified bounds. (startIndex, startIndex + count).
		//
		// Parameters:
		//	startIndex - Start index to get from.
		//	count - Number of characters to get.
		//
		// Returns: Tokenized string.
		//
		inline const DynamicArray GetSubstring (const indexType startIndex, const indexType count) const
		{
			return DynamicArray (*this, startIndex, count);
		}

		//
		// Function: GetSubstring
		//
		// Description: Gets the substring by specified bounds. (startIndex, startIndex + m_itemCount).
		//
		// Parameters:
		//	startIndex - Start index to get from.
		//
		// Returns: Tokenized string.
		//
		inline const DynamicArray GetSubstring (const indexType startIndex = 0u) const
		{
			return DynamicArray (*this, startIndex);
		}

		//
		// Function: GetLeftSubstring
		//
		// Description: Gets the string from left side.
		//
		// Parameters:
		//	count - Number of characters to get.
		//
		// Returns: Tokenized string.
		//
		inline const DynamicArray GetLeftSubstring (const indexType count) const { return GetSubstring (0u, count); }

		//
		// Function: GetRightSubstring
		//
		// Description: Gets the string from right side.
		//
		// Parameters:
		//	count - Number of characters to get.
		//
		// Returns: Tokenized string.
		//
		inline const DynamicArray GetRightSubstring (const indexType count) const
		{
			// Reliability check.
			InternalAssert (count <= m_itemCount);

			return GetSubstring (m_itemCount - count, count);
		}

		//
		// Function: ToUpper
		//
		// Description: Gets the string in upper case.
		//
		// Parameters:
		//	startIndex - Start index to get upper from.
		//	count - Number of characters to get upper.
		//
		// Returns: Upped sting.
		//
		inline const DynamicArray ToUpper (indexType startIndex, const indexType count) const
		{
			// Checks for bounds....
			InternalAssert (startIndex < m_itemCount && count <= m_itemCount - startIndex);

			DynamicArray result;

			if (result.SetSizeWithoutKeepingData (count + 1u))
			{
				for (/* Empty */; result.m_itemCount < count; ++result.m_itemCount, ++startIndex)
					result.m_elements[result.m_itemCount] = static_cast <char> (toupper (m_elements[startIndex]));

				// Terminate the string.
				result.m_elements[result.m_itemCount] = '\0';
			}

			return result;
		}

		//
		// Function: ToUpper
		//
		// Description: Gets the string in upper case.
		//
		// Parameters:
		//	startIndex - Start index to get upper from.
		//
		// Returns: Upped sting.
		//
		inline const DynamicArray ToUpper (const indexType startIndex = 0u) const
		{
			return ToUpper (startIndex, m_itemCount - startIndex);
		}

		//
		// Function: MakeUpper
		//
		// Description: Converts string to upper case.
		//
		// Parameters:
		//	startIndex - Start index to make upper from.
		//	count - Number of characters to make upper.
		//
		// Returns: Upped sting.
		//
		inline DynamicArray &MakeUpper (indexType startIndex, indexType count)
		{
			// Checks for bounds....
			InternalAssert (startIndex < m_itemCount && count <= m_itemCount - startIndex);

			for (/* Empty */; count > 0u; m_elements[startIndex] = static_cast <char> (toupper (m_elements[startIndex])), ++startIndex, --count);

			return *this;
		}

		//
		// Function: MakeUpper
		//
		// Description: Converts string to upper case.
		//
		// Parameters:
		//	startIndex - Start index to make upper from.
		//
		// Returns: Upped sting.
		//
		inline DynamicArray &MakeUpper (const indexType startIndex = 0u)
		{
			return MakeUpper (startIndex, m_itemCount - startIndex);
		}

		//
		// Function: ToLower
		//
		// Description: Gets the string in lower case.
		//
		// Parameters:
		//	startIndex - Start index to get lower from.
		//	count - Number of characters to get lower.
		//
		// Returns: Lowered sting.
		//
		inline const DynamicArray ToLower (indexType startIndex, const indexType count) const
		{
			// Checks for bounds....
			InternalAssert (startIndex < m_itemCount && count <= m_itemCount - startIndex);

			DynamicArray result;

			if (result.SetSizeWithoutKeepingData (count + 1u))
			{
				for (/* Empty */; result.m_itemCount < count; ++result.m_itemCount, ++startIndex)
					result.m_elements[result.m_itemCount] = static_cast <char> (tolower (m_elements[startIndex]));

				// Terminate the string.
				result.m_elements[result.m_itemCount] = '\0';
			}

			return result;
		}

		//
		// Function: ToLower
		//
		// Description: Gets the string in lower case.
		//
		// Parameters:
		//	startIndex - Start index to get lower from.
		//
		// Returns: Lowered sting.
		//
		inline const DynamicArray ToLower (const indexType startIndex = 0u) const
		{
			return ToLower (startIndex, m_itemCount - startIndex);
		}

		//
		// Function: MakeLower
		//
		// Description: Converts string to lower case.
		//
		// Parameters:
		//	startIndex - Start index to make lower from.
		//	count - Number of characters to make lower.
		//
		// Returns: Lowered sting.
		//
		inline DynamicArray &MakeLower (indexType startIndex, indexType count)
		{
			// Checks for bounds....
			InternalAssert (startIndex < m_itemCount && count <= m_itemCount - startIndex);

			for (/* Empty */; count > 0u; m_elements[startIndex] = static_cast <char> (tolower (m_elements[startIndex])), ++startIndex, --count);

			return *this;
		}

		//
		// Function: MakeLower
		//
		// Description: Converts string to lower case.
		//
		// Parameters:
		//	startIndex - Start index to make lower from.
		//
		// Returns: Lowered sting.
		//
		inline DynamicArray &MakeLower (const indexType startIndex = 0u)
		{
			return MakeLower (startIndex, m_itemCount - startIndex);
		}

		//
		// Function: GetReversed
		//
		// Description: Reverses a subrange of characters in string.
		//
		// Parameters:
		//	startIndex - Start index to get reversed from.
		//	count - Number of characters to get reversed.
		//
		// Returns: Reversed string.
		//
		inline const DynamicArray GetReversed (indexType startIndex, indexType count) const
		{
			// Checks for bounds....
			InternalAssert (startIndex < m_itemCount && count <= m_itemCount - startIndex);

			DynamicArray result;

			if (result.SetSizeWithoutKeepingData (count + 1u))
			{
				for (indexType destinationIndex (startIndex + count); count > 0u; result.m_elements[result.m_itemCount++] = m_elements[--destinationIndex], result.m_elements[--count] = m_elements[startIndex++]);

				// Terminate the string.
				result.m_elements[result.m_itemCount] = '\0';
			}

			return result;
		}

		//
		// Function: GetReversed
		//
		// Description: Reverses a subrange of characters in string.
		//
		// Parameters:
		//	startIndex - Start index to get reversed from.
		//
		// Returns: Reversed string.
		//
		inline const DynamicArray GetReversed (const indexType startIndex = 0u) const
		{
			return GetReversed (startIndex, m_itemCount - startIndex);
		}

		//
		// Function: ToReverse
		//
		// Description: Reverses the string.
		//
		// Parameters:
		//	startIndex - Start index to reverse from.
		//	count - Number of characters to reverse.
		//
		// Returns: Reversed string.
		//
		inline const DynamicArray ToReverse (indexType startIndex, indexType count) const
		{
			// Checks for bounds....
			InternalAssert (startIndex < m_itemCount && count <= m_itemCount - startIndex);

			DynamicArray result (*this);

			if (!result.IsEmpty () && count > 0u)
				for (count += startIndex; startIndex < --count; result.m_elements[startIndex] = m_elements[count], result.m_elements[count] = m_elements[startIndex++]);

			return result;
		}

		//
		// Function: ToReverse
		//
		// Description: Reverses the string.
		//
		// Parameters:
		//	startIndex - Start index to reverse from.
		//
		// Returns: Reversed string.
		//
		inline const DynamicArray ToReverse (const indexType startIndex = 0u) const
		{
			return ToReverse (startIndex, m_itemCount - startIndex);
		}

		//
		// Function: Reverse
		//
		// Description: Converts string into reverse order.
		//
		// Parameters:
		//	startIndex - Start index to make reversed from.
		//	count - Number of characters to make reversed.
		//
		// Returns: Reversed string.
		//
		inline DynamicArray &Reverse (indexType startIndex, indexType count)
		{
			// Checks for bounds....
			InternalAssert (startIndex < m_itemCount && count <= m_itemCount - startIndex);

			if (count > 0u)
				for (count += startIndex; startIndex < --count; ++startIndex)
					SwapElements (m_elements[startIndex], m_elements[count]);

			return *this;
		}

		//
		// Function: Reverse
		//
		// Description: Converts string into reverse order.
		//
		// Parameters:
		//	startIndex - Start index to make reversed from.
		//
		// Returns: Reversed string.
		//
		inline DynamicArray &Reverse (const indexType startIndex = 0u)
		{
			return Reverse (startIndex, m_itemCount - startIndex);
		}

		inline const char  CompareWithCaseCheck (const char character) const
		{
			// Reliability check.
			InternalAssert (character != '\0');

			InternalAssert ((&character)[1u] == '\0');
/*
			const char tempBuffer[2u] = {character, '\0'};

			return static_cast <char> (strcmp (m_elements, tempBuffer));*/
			return static_cast <char> (strcmp (m_elements, &character));
		}
		inline const short CompareWithoutCaseCheck (const char character) const
		{
			// Reliability check.
			InternalAssert (character != '\0');

			InternalAssert ((&character)[1u] == '\0');
/*
			const char tempBuffer[2u] = {character, '\0'};

			return static_cast <short> (stricmp (m_elements, tempBuffer));*/
			return static_cast <short> (stricmp (m_elements, &character));
		}
		//
		// Function: Compare
		//
		// Description: Compares string with other string.
		//
		// Parameters:
		//	string - String to compare with.
		//	caseCheck - Checks whether with/without case check.
		//
		// Returns: Zero if they are equal.
		//
		inline const char CompareWithCaseCheck (const char *const bufferPointer) const
		{
			// Reliability check.
			InternalAssert (bufferPointer != NULL);

			return static_cast <char> (strcmp (m_elements, bufferPointer));
		}
		/*template <typename indexType2> */inline const char CompareWithCaseCheck (const DynamicArray/* <char, indexType2>*/ &string) const
		{
			return static_cast <char> (strcmp (m_elements, string));
		}
		inline const short CompareWithoutCaseCheck (const char *const bufferPointer) const
		{
			// Reliability check.
			InternalAssert (bufferPointer != NULL);

			return static_cast <short> (stricmp (m_elements, bufferPointer));
		}
		/*template <typename indexType2> */inline const short CompareWithoutCaseCheck (const DynamicArray/* <char, indexType2>*/ &string) const
		{
			return static_cast <short> (stricmp (m_elements, string));
		}

		//
		// Function: Compare
		//
		// Description: Compares string with other string.
		//
		// Parameters:
		//	string - String to compare with.
		//	count - Number of characters to compare.
		//	caseCheck - Checks whether with/without case check.
		//
		// Returns: Zero if they are equal.
		//
		inline const short CompareWithCaseCheck (const char *const bufferPointer, const indexType count) const
		{
			// Reliability check.
			InternalAssert (bufferPointer != NULL);

			return static_cast <short> (strncmp (m_elements, bufferPointer, count));
		}
		inline const short CompareWithoutCaseCheck (const char *const bufferPointer, const indexType count) const
		{
			// Reliability check.
			InternalAssert (bufferPointer != NULL);

			return static_cast <short> (strnicmp (m_elements, bufferPointer, count));
		}
		inline const short CompareWithCaseCheck (const DynamicArray &string, const indexType count) const
		{
			return static_cast <short> (strncmp (m_elements, string, count));
		}
		inline const short CompareWithoutCaseCheck (const DynamicArray &string, const indexType count) const
		{
			return static_cast <short> (strnicmp (m_elements, string, count));
		}

		//
		// Function: Compare
		//
		// Description: Compares string with other string.
		//
		// Parameters:
		//	startIndex - Start index to search from.
		//	string - String to compare with.
		//	caseCheck - Checks whether with/without case check.
		//
		// Returns: Zero if they are equal.
		//
		inline const char CompareWithCaseCheck (const indexType startIndex, const char *const bufferPointer) const
		{
			// Reliability check.
			InternalAssert (startIndex <= m_itemCount);

			return static_cast <char> (strcmp (m_elements + startIndex, bufferPointer));
		}
		inline const char CompareWithCaseCheck (const indexType startIndex, const DynamicArray &string) const
		{
			// Reliability check.
			InternalAssert (startIndex <= m_itemCount);

			return static_cast <char> (strcmp (m_elements + startIndex, string));
		}
		inline const short CompareWithoutCaseCheck (const indexType startIndex, const DynamicArray &string) const
		{
			// Reliability check.
			InternalAssert (startIndex <= m_itemCount);

			return static_cast <short> (stricmp (m_elements + startIndex, string));
		}

		//
		// Function: Compare
		//
		// Description: Compares string with other string.
		//
		// Parameters:
		//	startIndex - Start index to search from.
		//	string - String to compare with.
		//	count - Number of characters to compare.
		//	caseCheck - Checks whether with/without case check.
		//
		// Returns: Zero if they are equal.
		//
		inline const short CompareWithCaseCheck (const indexType startIndex, const indexType count, const DynamicArray &string) const
		{
			// Reliability check.
			InternalAssert (startIndex <= m_itemCount);

			return static_cast <short> (strncmp (m_elements + startIndex, string, count));
		}
		inline const short CompareWithoutCaseCheck (const indexType startIndex, const indexType count, const DynamicArray &string) const
		{
			// Reliability check.
			InternalAssert (startIndex <= m_itemCount);

			return static_cast <short> (strnicmp (m_elements + startIndex, string, count));
		}

		//
		// Function: Collate
		//
		// Description: Collate the string.
		//
		// Parameters:
		//	string - String to collate.
		//
		// Returns: One on success.
		//
		inline const int Collate (const DynamicArray &string) const { return strcoll (m_elements, string.m_elements); }

		//
		// Function: Find
		//
		// Description: Find the character.
		//
		// Parameters:
		//	input - Character to search for.
		//	startIndex - Start index to search from.
		//	caseCheck - Checks whether with/without case check.
		//
		// Returns: Index of found character.
		//
		inline const indexType Find (const char input, indexType startIndex = 0u, const bool caseCheck = true) const
		{
			// Checks for bounds....
			InternalAssert (input != '\0');
			InternalAssert (startIndex < m_itemCount);

			return Find (input, startIndex, m_itemCount - startIndex, caseCheck);
		}
		inline const indexType Find (char input, indexType startIndex, indexType count, const bool caseCheck = true) const
		{
			// Checks for bounds....
			InternalAssert (input != '\0');
			InternalAssert (startIndex < m_itemCount && count <= m_itemCount - startIndex);

			count += startIndex;

			// Room for match, look for it....
			if (caseCheck)
			{
				for (/* Empty */; startIndex < count; ++startIndex)
					if (m_elements[startIndex] == input)
						return startIndex;	// Found a match.
			}
			else
			{
				for (input = static_cast <char> (tolower (input)); startIndex < count; ++startIndex)
					if (tolower (m_elements[startIndex]) == input)
						return startIndex;	// Found a match.
			}

			return BadPosition;	// No match.
		}

		inline const bool GetLineFromFile (FILE *const file, const unsigned short count = 256u)
		{
			// Reliability checks.
			InternalAssert (file != NULL);
			InternalAssert (count > 0u);

			if (!SetSizeWithoutKeepingData (count))
				return false;

			const bool isOk (fgets (m_elements, count, file) != NULL);

			m_itemCount = GetStringLength <indexType> (m_elements);

			return isOk;
		}

		//
		// Function: IsMemberWithCaseCheck
		//
		// Description: Finds an character.
		//
		// Parameters:
		//	character - Object to search for.
		//
		// Returns: True if character is founded, false otherwise.
		//
		inline const bool IsMemberWithCaseCheck (const char character) const
		{
			return Find (character) != BadPosition;
		}

		//
		// Function: IsMemberWithoutCaseCheck
		//
		// Description: Finds an character.
		//
		// Parameters:
		//	character - Object to search for.
		//
		// Returns: True if character is founded, false otherwise.
		//
		inline const bool IsMemberWithoutCaseCheck (const char character) const
		{
			return Find (character, 0u, false) != BadPosition;
		}

		//
		// Function: Find
		//
		// Description: Tries to find string from specified index.
		//
		// Parameters:
		//	string - String to search for.
		//	startIndex - Index to start search from.
		//	count - Length of sequence of characters to search for.
		//	caseCheck - Checks whether with/without case check.
		//
		// Returns: Position of found string.
		//
		inline const indexType Find (const DynamicArray &string, indexType startIndex, const indexType count, const bool caseCheck = true) const
		{
			// Reliability check.
			InternalAssert (!string.IsEmpty () && startIndex < m_itemCount && count <= string.m_itemCount && count > 0u);
/*
			if (count == 0u)
				return startIndex;	// Null string always matches (if inside string).
*/
			// Room for match, look for it....
			if (caseCheck)
			{
				for (indexType index; startIndex + count <= m_itemCount; ++startIndex)
					for (index = 0u; m_elements[startIndex + index] == string.m_elements[index]; /* Empty */)
						if (++index == count)
							return startIndex;	// Found a match.
			}
			else
			{
				for (indexType index; startIndex + count <= m_itemCount; ++startIndex)
					for (index = 0u; tolower (m_elements[startIndex + index]) == tolower (string.m_elements[index]); /* Empty */)
						if (++index == count)
							return startIndex;	// Found a match.
			}

			return BadPosition;	// No match.
		}

		//
		// Function: IsMemberWithCaseCheck
		//
		// Description: Finds an string.
		//
		// Parameters:
		//	string - String to search for.
		//
		// Returns: True if string is founded, false otherwise.
		//
		inline const bool IsMemberWithCaseCheck (const DynamicArray &string) const
		{
			return Find (string) != BadPosition;
		}

		//
		// Function: IsMemberWithoutCaseCheck
		//
		// Description: Finds an string.
		//
		// Parameters:
		//	string - String to search for.
		//
		// Returns: True if string is founded, false otherwise.
		//
		inline const bool IsMemberWithoutCaseCheck (const DynamicArray &string) const
		{
			return Find (string, false) != BadPosition;
		}

		//
		// Function: Find
		//
		// Description: Tries to find string from specified index.
		//
		// Parameters:
		//	startIndex - Index to start search from.
		//	count - Length of sequence of characters to search for.
		//	string - String to search for.
		//	startIndex2 - Index to start search from.
		//	count2 - Length of sequence of characters to search for.
		//	caseCheck - Checks whether with/without case check.
		//
		// Returns: Position of found string.
		//
		inline const indexType Find (indexType startIndex, indexType count, const DynamicArray &string, const indexType startIndex2, const indexType count2, const bool caseCheck = true) const
		{
			// Reliability check.
			InternalAssert (startIndex < m_itemCount && count <= m_itemCount/* - startIndex*/ && !string.IsEmpty () && startIndex2 < string.m_itemCount && count2 <= string.m_itemCount/* - startIndex2*/ && count >= count2 && count > 0u && count2 > 0u);
/*
			if (count == 0u || count2 == 0u)
				return startIndex;	// Null string always matches (if inside string).
*/
			count += startIndex;

			// Room for match, look for it....
			if (caseCheck)
			{
				for (indexType index; startIndex < count && startIndex + count2 <= m_itemCount/*count*/; ++startIndex)
					for (index = 0u; startIndex2 + index <= count2 && m_elements[startIndex + index] == string.m_elements[startIndex2 + index]; /* Empty */)
						if (++index == count2)
							return startIndex;	// Found a match.
			}
			else
			{
				for (indexType index; startIndex < count && startIndex + count2 <= m_itemCount/*count*/; ++startIndex)
					for (index = 0u; startIndex2 + index <= count2 && tolower (m_elements[startIndex + index]) == tolower (string.m_elements[startIndex2 + index]); /* Empty */)
						if (++index == count2)
							return startIndex;	// Found a match.
			}

			return BadPosition;	// No match.
		}

		//
		// Function: Find
		//
		// Description: Tries to find string from specified index.
		//
		// Parameters:
		//	string - String to search for.
		//	startIndex - Index to start search from.
		//	caseCheck - Checks whether with/without case check.
		//
		// Returns: Position of found string.
		//
/*		inline const indexType Find (const DynamicArray &string, indexType startIndex = 0u, const bool caseCheck = true) const
		{
			return Find (string, startIndex, string.m_itemCount, caseCheck);
		}
*/
		//
		// Function: Find
		//
		// Description: Tries to find string from specified index.
		//
		// Parameters:
		//	string - String to search for.
		//	caseCheck - Checks whether with/without case check.
		//
		// Returns: Position of found string.
		//
		inline const indexType Find (const DynamicArray &string, const bool caseCheck = true) const
		{
			return Find (string, 0u, string.m_itemCount, caseCheck);
		}

		//
		// Function: ReverseFind
		//
		// Description: Tries to find string from specified index. (look for [bufferPointer, bufferPointer + count) beginning before 'startIndex').
		//
		// Parameters:
		//	string - String to search for.
		//	startIndex - Index to start search from.
		//	count - Length of sequence of characters to search for.
		//	caseCheck - Checks whether with/without case check.
		//
		// Returns: Position of found string.
		//
		inline const indexType ReverseFind (const DynamicArray &string, indexType startIndex, indexType count, const bool caseCheck = true) const
		{
			// Reliability check.
			InternalAssert (!string.IsEmpty () && startIndex <= m_itemCount && count <= string.m_itemCount && count > 0u);
/*
			if (count == 0u)
				return startIndex < m_itemCount ? startIndex : m_itemCount;	// Null always matches.
*/
			if (startIndex == m_itemCount)
				--startIndex;	// Ignore last '\0' character.

			--count;	// Ignore last character.

			// Room for match, look for it....
			if (caseCheck)
			{
				for (indexType index; startIndex - count < m_itemCount; --startIndex)
					for (index = count; m_elements[startIndex - (count - index)] == string.m_elements[index]; --index)
						if (index == 0u)
							return startIndex - count;	// Found a match.
			}
			else
			{
				for (indexType index; startIndex - count < m_itemCount; --startIndex)
					for (index = count; tolower (m_elements[startIndex - (count - index)]) == tolower (string.m_elements[index]); --index)
						if (index == 0u)
							return startIndex - count;	// Found a match.
			}

			return BadPosition;	// No match.
		}

		inline const indexType rfind (const char *const bufferPointer, indexType startIndex, const indexType count) const
		{	// look for [bufferPointer, bufferPointer + count) beginning before 'startIndex'.
			// Reliability check.
			InternalAssert ((count == 0u || bufferPointer != NULL) && count > 0u);
/*
			if (count == 0u)
				return startIndex < m_itemCount ? startIndex : m_itemCount;	// Null always matches
*/
			if (count <= m_itemCount)
			{
				// room for match, look for it

//				const char *_Uptr = m_elements + (startIndex < m_itemCount - count ? startIndex : m_itemCount - count);

//				for (/* Empty */; /* Empty */; --_Uptr)
/*				{
					if (*_Uptr == *bufferPointer && memcmp (_Uptr, bufferPointer, count) == 0)
						return _Uptr - m_elements;	// found a match
					else if (_Uptr == m_elements)
						return BadPosition;	// at beginning, no more chance for match
				}*/
				if (startIndex > m_itemCount - count)
					startIndex = m_itemCount - count;

				for (/*startIndex = Math::GetMinimumValueBetween (startIndex, m_itemCount - count)/*startIndex < m_itemCount - count ? startIndex : m_itemCount - count*/; startIndex < BadPosition; --startIndex)
					if (m_elements[startIndex] == *bufferPointer && memcmp (m_elements + startIndex, bufferPointer, count) == 0)
						return startIndex;	// Found a match.
			}

			return BadPosition;	// No match.
		}

		//
		// Function: ReverseFind
		//
		// Description: Tries to find character in reverse order.
		//
		// Parameters:
		//	character - Character to search for.
		//	startIndex - Start index to search from.
		//	caseCheck - Checks whether with/without case check.
		//
		// Returns: Position of found character.
		//
		inline const indexType ReverseFind (char character, indexType startIndex, const bool caseCheck = true) const
		{
			// Reliability check.
			InternalAssert (character != '\0');
			InternalAssert (!IsEmpty ());

			if (caseCheck)
			{
				for (/* Empty */; startIndex <= m_itemCount; --startIndex)
					if (m_elements[startIndex] == character)
						return startIndex;	// Found a match.
			}
			else
			{
				for (character = static_cast <char> (tolower (character)); startIndex <= m_itemCount; --startIndex)
					if (tolower (m_elements[startIndex]) == character)
						return startIndex;	// Found a match.
			}

			return BadPosition;	// No match.
		}
		inline const indexType ReverseFindWithCaseCheck (const char character) const { return ReverseFind (character, GetElementNumber ()); }

		//
		// Function: FindOneOf
		//
		// Description: Find one of occurrences of string (Is equal to strcspn() function).
		//
		// Parameters:
		//	string - String to search for.
		//	startIndex - Index to start search from.
		//	caseCheck - Checks whether with/without case check.
		//
		// Returns: 'BadPosition' in case of nothing is found, start of string in buffer otherwise.
		//
		inline const indexType FindOneOf (const DynamicArray &string, indexType startIndex = 0u, const bool caseCheck = true) const
		{
			// Reliability check.
			InternalAssert (!string.IsEmpty ());

//			for (/* Empty */; startIndex < m_itemCount; ++startIndex)
//				if (string.Find (m_elements[startIndex], 0u, caseCheck) != BadPosition)
//					return startIndex;

			if (caseCheck)
			{
				for (indexType index; startIndex < m_itemCount; ++startIndex)
					for (index = 0u; index < string.m_itemCount; ++index)
						if (m_elements[startIndex] == string.m_elements[index])
							return startIndex;	// Found a match.
			}
			else
			{
				for (indexType index; startIndex < m_itemCount; ++startIndex)
					for (index = 0u; index < string.m_itemCount; ++index)
						if (tolower (m_elements[startIndex]) == tolower (string.m_elements[index]))
							return startIndex;	// Found a match.
			}

			return BadPosition;	// No match.
		}
		inline const indexType FindOneOf (const DynamicArray &string, indexType startIndex, indexType count, const bool caseCheck = true) const
		{
			// Reliability check.
			InternalAssert (!string.IsEmpty () && startIndex < m_itemCount && count <= m_itemCount - startIndex && count > 0u);
/*
			if (count == 0u)
				return startIndex;	// Null string always matches (if inside string).
*/
			count += startIndex;

//			for (/* Empty */; startIndex < count; ++startIndex)
//				if (string.Find (m_elements[startIndex], 0u, caseCheck) != BadPosition)
//					return startIndex;

			if (caseCheck)
			{
				for (indexType index; startIndex < count; ++startIndex)
					for (index = 0u; index < string.m_itemCount; ++index)
						if (m_elements[startIndex] == string.m_elements[index])
							return startIndex;	// Found a match.
			}
			else
			{
				for (indexType index; startIndex < count; ++startIndex)
					for (index = 0u; index < string.m_itemCount; ++index)
						if (tolower (m_elements[startIndex]) == tolower (string.m_elements[index]))
							return startIndex;	// Found a match.
			}

			return BadPosition;	// No match.
		}
		inline const indexType ReverseFindOneOf (const DynamicArray &string, indexType startIndex, const bool caseCheck = true) const
		{
			// Reliability check.
			InternalAssert (!string.IsEmpty ());

//			for (/* Empty */; startIndex <= m_itemCount; --startIndex)
//				if (string.ReverseFind (m_elements[startIndex], string.GetElementNumber (), caseCheck) != BadPosition)
//					return startIndex;

			if (caseCheck)
			{
				for (indexType index; startIndex <= m_itemCount; --startIndex)
					for (index = 0u; index < string.m_itemCount; ++index)
						if (m_elements[startIndex] == string.m_elements[index])
							return startIndex;	// Found a match.
			}
			else
			{
				for (indexType index; startIndex <= m_itemCount; --startIndex)
					for (index = 0u; index < string.m_itemCount; ++index)
						if (tolower (m_elements[startIndex]) == tolower (string.m_elements[index]))
							return startIndex;	// Found a match.
			}

			return BadPosition;	// No match.
		}
		inline const indexType ReverseFindOneOfWithoutCaseCheck (const DynamicArray &string) const
		{
			return ReverseFindOneOf (string, GetElementNumber (), false);
		}
		inline const indexType ReverseFindOneOfWithCaseCheck (const DynamicArray &string) const
		{
			return ReverseFindOneOf (string, GetElementNumber (), true);
		}

		//
		// Function: FindOneDiscrepancyOf
		//
		// Description: Find one of discrepancys of string (Is equal to strspn() function).
		//
		// Parameters:
		//	string - String to search for.
		//	startIndex - Index to start search from.
		//	caseCheck - Checks whether with/without case check.
		//
		// Returns: 'BadPosition' in case of nothing is found, start of string in buffer otherwise.
		//
		inline const indexType FindOneDiscrepancy (char character, indexType startIndex = 0u, const bool caseCheck = true) const
		{
			// Reliability check.
			InternalAssert (character != '\0');

			if (caseCheck)
			{
				for (/* Empty */; startIndex < m_itemCount; ++startIndex)
					if (m_elements[startIndex] != character)
						return startIndex;	// Found a match.
			}
			else
			{
				for (character = static_cast <char> (tolower (character)); startIndex < m_itemCount; ++startIndex)
					if (tolower (m_elements[startIndex]) != character)
						return startIndex;	// Found a match.
			}

			return BadPosition;	// No match.
		}
		inline const indexType FindOneDiscrepancy (char character, indexType startIndex, indexType count, const bool caseCheck = true) const
		{
			// Reliability check.
			InternalAssert (character != '\0' && startIndex < m_itemCount && count <= m_itemCount - startIndex && count > 0u);
/*
			if (count == 0u)
				return startIndex;	// Null string always matches (if inside string).
*/
			count += startIndex;

			if (caseCheck)
			{
				for (/* Empty */; startIndex < count; ++startIndex)
					if (m_elements[startIndex] != character)
						return startIndex;	// Found a match.
			}
			else
			{
				for (character = static_cast <char> (tolower (character)); startIndex < count; ++startIndex)
					if (tolower (m_elements[startIndex]) != character)
						return startIndex;	// Found a match.
			}

			return BadPosition;	// No match.
		}
		inline const indexType FindOneDiscrepancyOf (const DynamicArray &string, indexType startIndex = 0u, const bool caseCheck = true) const
		{
			// Reliability check.
			InternalAssert (!string.IsEmpty () && startIndex < m_itemCount);

//			for (/* Empty */; startIndex < m_itemCount; ++startIndex)
//				if (string.Find (m_elements[startIndex], 0u, caseCheck) == BadPosition)
//					return startIndex;

			if (caseCheck)
			{
				for (indexType index; startIndex < m_itemCount; ++startIndex)
					for (index = 0u; index < string.m_itemCount && m_elements[startIndex] != string.m_elements[index]; /* Empty */)
						if (++index == string.m_itemCount)
							return startIndex;	// Found a match.
			}
			else
			{
				for (indexType index; startIndex < m_itemCount; ++startIndex)
				{
					const char lowerCharacter (static_cast <char> (tolower (m_elements[startIndex])));

					for (index = 0u; index < string.m_itemCount && lowerCharacter != tolower (string.m_elements[index]); /* Empty */)
						if (++index == string.m_itemCount)
							return startIndex;	// Found a match.
				}
			}

			return BadPosition;	// No match.
		}
		inline const indexType FindOneDiscrepancyOf (const DynamicArray &string, indexType startIndex, indexType count, const bool caseCheck = true) const
		{
			// Reliability check.
			InternalAssert (!string.IsEmpty () && startIndex < m_itemCount && count <= m_itemCount - startIndex && count > 0u);
/*
			if (count == 0u)
				return startIndex;	// Null string always matches (if inside string).
*/
			count += startIndex;

//			for (/* Empty */; startIndex < count; ++startIndex)
//				if (string.Find (m_elements[startIndex], 0u, caseCheck) == BadPosition)
//					return startIndex;

			if (caseCheck)
			{
				for (indexType index; startIndex < count; ++startIndex)
					for (index = 0u; index < string.m_itemCount && m_elements[startIndex] != string.m_elements[index]; /* Empty */)
						if (++index == string.m_itemCount)
							return startIndex;	// Found a match.
			}
			else
			{
				for (indexType index; startIndex < count; ++startIndex)
				{
					const char lowerCharacter (static_cast <char> (tolower (m_elements[startIndex])));

					for (index = 0u; index < string.m_itemCount && lowerCharacter != tolower (string.m_elements[index]); /* Empty */)
						if (++index == string.m_itemCount)
							return startIndex;	// Found a match.
				}
			}

			return BadPosition;	// No match.
		}
		inline const indexType ReverseFindOneDiscrepancyOf (const DynamicArray &string, indexType startIndex, const bool caseCheck = true) const
		{
			// Reliability check.
			InternalAssert (!string.IsEmpty () && startIndex < m_itemCount);

//			for (/* Empty */; startIndex <= m_itemCount; --startIndex)
//				if (string.ReverseFind (m_elements[startIndex], string.GetElementNumber (), caseCheck) == BadPosition)
//					return startIndex;

			if (caseCheck)
			{
				for (indexType index; startIndex <= m_itemCount; --startIndex)
					for (index = 0u; index < string.m_itemCount && m_elements[startIndex] != string.m_elements[index]; /* Empty */)
						if (++index == string.m_itemCount)
							return startIndex;	// Found a match.
			}
			else
			{
				for (indexType index; startIndex <= m_itemCount; --startIndex)
				{
					const char lowerCharacter (static_cast <char> (tolower (m_elements[startIndex])));

					for (index = 0u; index < string.m_itemCount && lowerCharacter != tolower (string.m_elements[index]); /* Empty */)
						if (++index == string.m_itemCount)
							return startIndex;	// Found a match.
				}
			}

			return BadPosition;	// No match.
		}
		inline const indexType ReverseFindOneDiscrepancyOf (const DynamicArray &string, indexType startIndex, indexType count, const bool caseCheck = true) const
		{
			// Reliability check.
			InternalAssert (!string.IsEmpty () && startIndex < m_itemCount && count <= m_itemCount - startIndex && count > 0u);
/*
			if (count == 0u)
				return startIndex < m_itemCount ? startIndex : m_itemCount;	// Null always matches.
*/
			if (startIndex == m_itemCount)
				--startIndex;	// Ignore last '\0' character.

			--count;

//			for (/* Empty */; startIndex - count < m_itemCount; --startIndex)
//				if (string.ReverseFind (m_elements[startIndex], string.GetElementNumber (), caseCheck) == BadPosition)
//					return startIndex;

			if (caseCheck)
			{
				for (indexType index; startIndex - count < m_itemCount; --startIndex)
					for (index = 0u; index < string.m_itemCount && m_elements[startIndex] != string.m_elements[index]; /* Empty */)
						if (++index == string.m_itemCount)
							return startIndex;	// Found a match.
			}
			else
			{
				for (indexType index; startIndex - count < m_itemCount; --startIndex)
				{
					const char lowerCharacter (static_cast <char> (tolower (m_elements[startIndex])));

					for (index = 0u; index < string.m_itemCount && lowerCharacter != tolower (string.m_elements[index]); /* Empty */)
						if (++index == string.m_itemCount)
							return startIndex;	// Found a match.
				}
			}

			return BadPosition;	// No match.
		}

		//
		// Function: TrimLeft
		//
		// Description: Trims string from left side.
		//
		// Returns: Trimmed string.
		//
		inline DynamicArray &TrimLeft (void)
		{
			#if defined _DEBUG
				// Reliability check.
				if (IsEmpty ())
					return *this;
			#endif	// if defined _DEBUG

			indexType count (0u);

			for (/* Empty */; count < m_itemCount && IsTrimmingCharacter (m_elements[count]); ++count);

			if (count > 0u)
				Remove (0u, count);

			return *this;
		}
		inline DynamicArray &TrimLeftFrom (const indexType sourceIndex)
		{
			// Reliability check.
			InternalAssert (!IsEmpty ());
			InternalAssert (sourceIndex < m_itemCount);

			indexType destinationIndex (sourceIndex);

			while (IsTrimmingCharacter (m_elements[destinationIndex]) && ++destinationIndex < m_itemCount);

			if (destinationIndex > sourceIndex)
				Remove (sourceIndex, destinationIndex - sourceIndex/* convert to count */);

			return *this;
		}

		//
		// Function: TrimLeft
		//
		// Description: Trims specified character at the left of the string.
		//
		// Parameters:
		//	character - Character to trim.
		//
		inline DynamicArray &TrimLeft (const char character)
		{
			#if defined _DEBUG
				// Reliability check.
				if (IsEmpty () || character == '\0')
					return *this;
			#endif	// if defined _DEBUG

			indexType count (0u);

			for (/* Empty */; count < m_itemCount && m_elements[count] == character; ++count);

			if (count > 0u)
				Remove (0u, count);

			return *this;
		}
		inline DynamicArray &TrimLeft (const DynamicArray &separator)
		{
			// Reliability check.
			InternalAssert (!IsEmpty () && !separator.IsEmpty ())

			indexType count (0u);

			for (/* Empty */; count < m_itemCount && (separator & m_elements[count]); ++count);

			if (count > 0u)
				Remove (0u, count);

			return *this;
		}
		inline DynamicArray &TrimLeft (const CharacterSet &set)
		{
			// Reliability check.
			InternalAssert (!IsEmpty ())

			indexType count (0u);

			for (/* Empty */; count < m_itemCount && set[m_elements[count]]; ++count);

			if (count > 0u)
				Remove (0u, count);

			return *this;
		}

		//
		// Function: TrimRight
		//
		// Description: Trims string from right side.
		//
		// Returns: Trimmed string.
		//
		inline DynamicArray &TrimRight (void)
		{
			#if defined _DEBUG
				// Reliability check.
				if (IsEmpty ())
					return *this;
			#endif	// if defined _DEBUG

			indexType sourceIndex (m_itemCount - 1u), count (0u);

			for (/* Empty */; sourceIndex < m_itemCount && IsTrimmingCharacter (m_elements[sourceIndex]); --sourceIndex, ++count);

			if (count > 0u)
				Remove (sourceIndex + 1u, count);

			return *this;
		}
		inline DynamicArray &TrimRightFrom (indexType sourceIndex)
		{
			// Reliability check.
			InternalAssert (!IsEmpty ());
			InternalAssert (--sourceIndex < m_itemCount);

			indexType destinationIndex (sourceIndex);

			while (IsTrimmingCharacter (m_elements[destinationIndex]) && --destinationIndex < sourceIndex);

			if (destinationIndex != sourceIndex)
			{
				++destinationIndex;

				Remove (destinationIndex, sourceIndex + 1u - destinationIndex/* convert to count */);
			}

			return *this;
		}

		//
		// Function: TrimRight
		//
		// Description:
		//	Trims specified character at the right of the string.
		//
		// Parameters:
		//	character - Character to trim.
		//
		inline DynamicArray &TrimRight (const char character)
		{
			#if defined _DEBUG
				// Reliability check.
				if (IsEmpty () || character == '\0')
					return *this;
			#endif	// if defined _DEBUG

			indexType sourceIndex (m_itemCount - 1u), count (0u);

			for (/* Empty */; sourceIndex < m_itemCount && m_elements[sourceIndex] == character; --sourceIndex, ++count);

			if (count > 0u)
				Remove (sourceIndex + 1u, count);

			return *this;
		}
		inline DynamicArray &TrimRight (const DynamicArray &separator)
		{
			// Reliability check.
			InternalAssert (!IsEmpty () && !separator.IsEmpty ());

			indexType sourceIndex (m_itemCount - 1u), count (0u);

			for (/* Empty */; sourceIndex < m_itemCount && (separator & m_elements[sourceIndex]); --sourceIndex, ++count);

			if (count > 0u)
				Remove (sourceIndex + 1u, count);

			return *this;
		}

		//
		// Function: Trim
		//
		// Description: Trims string from both sides.
		//
		// Returns: Trimmed string.
		//
		inline DynamicArray &Trim (void) { return TrimLeft ().TrimRight (); }

		inline DynamicArray &Trim (const char character) { return TrimLeft (character).TrimRight (character); }
		inline DynamicArray &Trim (const DynamicArray &separator) { return TrimLeft (separator).TrimRight (separator); }

		//
		// Function: TrimQuotes
		//
		// Description: Trims trailing quotes.
		//
		// Returns: Trimmed string.
		//
		inline DynamicArray &TrimQuotes (void) { return TrimLeft ('"').TrimLeft ('\'').TrimRight ('"').TrimRight ('\''); }

		//
		// Function: GetValue <elementType>
		//
		// Description: Gets the string as specified element type, if possible.
		//
		// Parameters:
		//	startIndex - Starting position of the string to get value from.
		//
		// Returns: Specified type value of string.
		//
		template <typename elementType> inline const elementType GetValue (const indexType startIndex = 0u) const
		{
			// Reliability check.
			InternalAssert (startIndex <= m_itemCount);

			return static_cast <elementType> (atoi (m_elements + startIndex));
		}
		//
		// Function: GetValue <bool>
		//
		// Description: Gets the string as boolean, if possible.
		//
		// Parameters:
		//	startIndex - Starting position of the string to get value from.
		//
		// Returns: Boolean value of string.
		//
		template </* bool */> inline const bool GetValue (const indexType startIndex/* = 0u*/) const
		{
			// Reliability check.
			InternalAssert (startIndex <= m_itemCount);

			if (CompareWithoutCaseCheck (startIndex, "true") == 0)
				return true;
			else if (CompareWithoutCaseCheck (startIndex, "false") == 0)
				return false;
			else
				return atoi (m_elements + startIndex) == 1;
		}
		//
		// Function: GetValue <int>
		//
		// Description: Gets the string as integer, if possible.
		//
		// Parameters:
		//	startIndex - Starting position of the string to get value from.
		//
		// Returns: Integer value of string.
		//
		template </* int */> inline const int GetValue (const indexType startIndex/* = 0u*/) const
		{
			// Reliability check.
			InternalAssert (startIndex <= m_itemCount);

			return atoi (m_elements + startIndex);
		}
		//
		// Function: GetValue <double>
		//
		// Description: Gets the string as double, if possible.
		//
		// Parameters:
		//	startIndex - Starting position of the string to get value from.
		//
		// Returns: Double value of string.
		//
		template </* double */> inline const double GetValue (const indexType startIndex/* = 0u*/) const
		{
			// Reliability check.
			InternalAssert (startIndex <= m_itemCount);

			return atof (m_elements + startIndex);
		}
		//
		// Function: GetValue <float>
		//
		// Description: Gets the string as float, if possible.
		//
		// Parameters:
		//	startIndex - Starting position of the string to get value from.
		//
		// Returns: Float value of string.
		//
		template </* float */> inline const float GetValue (const indexType startIndex/* = 0u*/) const
		{
			// Reliability check.
			InternalAssert (startIndex <= m_itemCount);

			return static_cast <float> (atof (m_elements + startIndex));
		}

		inline const bool Insert (const indexType startIndex, const char character, indexType count)
		{	// Insert count * character at 'startIndex'.
			// Reliability check ('startIndex' off end or result too long).
			InternalAssert (startIndex <= m_itemCount && MaximumPossibleSize - m_itemCount > count);

			// Make room and insert new stuff.

			// Reliability check.
			if (!SetSize (m_itemCount + count + 1u))
				return false;

			MoveItems (startIndex + count, startIndex);	// Empty out hole.

			// Update the buffer length and terminate the string.
			m_elements[m_itemCount += count] = '\0';

			for (/* Empty */; count > 0u; m_elements[startIndex + --count] = character);	// Fill hole.

			return true;
		}
		inline const bool Insert (const char character, const indexType count)
		{	// Insert count * character at 0.
			return Insert (0u, character, count);
		}

		inline const bool Insert (const indexType startIndex, const char *const bufferPointer, indexType count)
		{	// Insert [bufferPointer, bufferPointer + count) at 'startIndex'.
			if (IsInside (bufferPointer))
				return Insert (startIndex, *this, static_cast <indexType> (bufferPointer - m_elements), count);	// Substring.

			// Reliability check ('startIndex' off end or result too long).
			InternalAssert (startIndex <= m_itemCount && (count == 0u || bufferPointer != NULL) && count <= GetStringLength <indexType> (bufferPointer) && MaximumPossibleSize - m_itemCount > count);

			// Make room and insert new stuff.

			// Reliability check.
			if (!SetSize (m_itemCount + count + 1u))
				return false;

			MoveItems (startIndex + count, startIndex);	// Empty out hole.

			// Update the buffer length and terminate the string.
			m_elements[m_itemCount += count] = '\0';

			for (/* Empty */; count > 0u; m_elements[startIndex + --count] = bufferPointer[count]);	// Fill hole.

			return true;
		}
		inline const bool Insert (const char *const bufferPointer, const indexType count)
		{	// Insert [bufferPointer, bufferPointer + count) at 0.
			return Insert (0u, bufferPointer, count);
		}

		inline const bool Insert (const indexType startIndex, const DynamicArray &string, const indexType startIndex2, indexType count)
		{	// Insert string [startIndex2, startIndex2 + count) at 'startIndex'.
			// Reliability check ('startIndex' or 'startIndex2' off end or result too long).
			InternalAssert (startIndex <= m_itemCount && startIndex2 <= string.m_itemCount && count <= string.m_itemCount - startIndex2 && MaximumPossibleSize - m_itemCount > count);

			// Make room and insert new stuff.

			// Reliability check.
			if (!SetSize (m_itemCount + count + 1u))
				return false;

			MoveItems (startIndex + count, startIndex);	// Empty out hole.

			if (this == &string)
			{
				MoveItems (startIndex, startIndex < startIndex2 ? startIndex2 + count : startIndex2);	// Substring.

				m_elements[m_itemCount += count] = '\0';	// Update the buffer length and terminate the string.
			}
			else
			{
				m_elements[m_itemCount += count] = '\0';	// Update the buffer length and terminate the string.

				for (/* Empty */; count > 0u; m_elements[startIndex + --count] = string.m_elements[startIndex2 + count]);	// Fill hole.
			}

			return true;
		}
		inline const bool Insert (const DynamicArray &string, const indexType startIndex, const indexType count)
		{	// Insert string [startIndex, startIndex + count) at 0.
			return Insert (0u, string, startIndex, count);
		}

		//
		// Function: Insert
		//
		// Description: Inserts string at specified index.
		//
		// Parameters:
		//	sourceIndex - Position to insert string.
		//	string - Text to insert.
		//
		inline const bool Insert (const indexType sourceIndex, const DynamicArray &string)
		{
/*			#if defined _DEBUG
				// Reliability check.
				if (string.IsEmpty () || sourceIndex > m_itemCount)
					return false;
			#endif	// if defined _DEBUG

//			InsertSpace (sourceIndex, string.m_itemCount);
			SetSize (m_itemCount + string.m_itemCount + 1u);

			for (indexType index (0u); index < string.m_itemCount; m_elements[sourceIndex + index] = string[index], ++index);

			if (sourceIndex == m_itemCount)
				m_elements[m_itemCount + string.m_itemCount] = '\0';

			m_itemCount += string.m_itemCount;
*/
			return Insert (sourceIndex, string, 0u, string.m_itemCount);
		}

		//
		// Function: Replace
		//
		// Description: Replaces old character with new one.
		//
		// Parameters:
		//	oldCharacter - Old character to replace.
		//	newCharacter - New character to replace with.
		//	startIndex - Index to start replace from.
		//	caseCheck - Checks whether with/without case check.
		//
		// Returns: Number of characters replaced.
		//
		inline const indexType Replace (char oldCharacter, const char newCharacter, indexType startIndex = 0u, const bool caseCheck = true) const
		{
			#if defined _DEBUG
				// Reliability check.
				if (/*oldCharacter == '\0' || */oldCharacter == newCharacter)
					return 0u;
			#endif	// if defined _DEBUG

			indexType replacedCharactersCount (0u);

			if (caseCheck)
			{
				for (/* Empty */; startIndex < m_itemCount; ++startIndex)
					if (m_elements[startIndex] == oldCharacter)
					{
						m_elements[startIndex] = newCharacter;

						++replacedCharactersCount;
					}
			}
			else
			{
				for (oldCharacter = static_cast <char> (tolower (oldCharacter)); startIndex < m_itemCount; ++startIndex)
					if (tolower (m_elements[startIndex]) == oldCharacter)
					{
						m_elements[startIndex] = newCharacter;

						++replacedCharactersCount;
					}
			}

			return replacedCharactersCount;
		}

		inline const bool Replace (const indexType startIndex, const indexType count, const indexType count2, const char character)
		{	// replace [startIndex, startIndex + count) with 'count2' * 'character'.
			// Reliability check. ('startIndex' off end or result too long)
			InternalAssert (startIndex <= m_itemCount && count <= m_itemCount - startIndex && MaximumPossibleSize - count2 > m_itemCount - count);

			if (count2 < count)
				MoveItems (startIndex + count2, startIndex + count);	// Smaller hole, move tail up.

			if (count2 > 0u || count > 0u)
			{
				const indexType newItemCount (m_itemCount + count2 - count);

				// Reliability check.
				if (!SetSize (newItemCount + 1u))
					return false;

				// Make room and rearrange.

				if (count < count2)
					MoveItems (startIndex + count2, startIndex + count);	// Move tail down.

				for (indexType index (0u); index < count2; m_elements[startIndex + index] = character, ++index);	// Fill hole.

				m_elements[m_itemCount = newItemCount] = '\0';	// Update the buffer length and terminate the string.
			}

			return true;
		}

		inline const bool Replace (const indexType startIndex, const indexType count, const char *const bufferPointer, const indexType count2)
		{	// replace [startIndex, startIndex + count) with [bufferPointer, bufferPointer + count2)
			if (IsInside (bufferPointer))
				return Replace (startIndex, count, *this, static_cast <indexType> (bufferPointer - m_elements), count2);	// Substring, replace carefully.

			// Reliability check.
			InternalAssert (startIndex <= m_itemCount && count <= m_itemCount - startIndex && (count2 == 0u || (bufferPointer != NULL || count2 <= GetStringLength <indexType> (bufferPointer))) && MaximumPossibleSize - count2 > m_itemCount - count);

			if (count2 < count)
				MoveItems (startIndex + count2, startIndex + count);	// Smaller hole, move tail up.

			if (count2 > 0u || count > 0u)
			{
				const indexType newItemCount (m_itemCount + count2 - count);

				// Reliability check.
				if (!SetSize (newItemCount + 1u))
					return false;

				// Make room and rearrange.

				if (count < count2)
					MoveItems (startIndex + count2, startIndex + count);	// Move tail down.

				for (indexType index (0u); index < count2; m_elements[startIndex + index] = bufferPointer[index], ++index);	// Fill hole.

				m_elements[m_itemCount = newItemCount] = '\0';	// Update the buffer length and terminate the string.
			}

			return true;
		}

		inline const bool Replace (const indexType startIndex, const indexType count, const DynamicArray &newString, const indexType startIndex2, const indexType count2)
		{	// replace (startIndex, startIndex + count) with newString (startIndex2, startIndex2 + count2).
			// Reliability check.
			InternalAssert (startIndex <= m_itemCount && newString.m_itemCount >= startIndex2 && count <= m_itemCount - startIndex && count2 <= newString.m_itemCount - startIndex2 && MaximumPossibleSize - count2 > m_itemCount - count);

			const indexType newSize (m_itemCount + count2 - count);

			// Reliability check.
			if (newSize > m_itemCount && !SetSize (newSize + 1u))
				return false;

			if (this != &newString)
			{
				// No overlap, just move down and copy in new stuff.

				MoveItems (startIndex + count2, startIndex + count);	// Empty hole.

				for (indexType index (0u); index < count2; m_elements[startIndex + index] = newString.m_elements[startIndex2 + index], ++index);	// Fill hole.
			}
			else if (count2 <= count)
			{
				// Hole doesn't get larger, just copy in substring.

				MoveItems (startIndex, startIndex2);	// Fill hole.
				MoveItems (startIndex + count2, startIndex + count);	// Move tail down.
			}
			else if (startIndex2 <= startIndex)
			{
				// Hole gets larger, substring begins before hole.

				MoveItems (startIndex + count2, startIndex + count);	// Move tail down.
				MoveItems (startIndex, startIndex2);	// Fill hole.
			}
			else if (startIndex + count <= startIndex2)
			{
				// Hole gets larger, substring begins after hole.

				MoveItems (startIndex + count2, startIndex + count);	// Move tail down.
				MoveItems (startIndex, startIndex2 + count2 - count);	// Fill hole.
			}
			else
			{
				// Hole gets larger, substring begins in hole.

				MoveItems (startIndex, startIndex2);	// Fill old hole.
				MoveItems (startIndex + count2, startIndex + count);	// Move tail down.
				MoveItems (startIndex + count, startIndex2 + count2);	// Fill rest of new hole.
			}

			m_elements[m_itemCount = newSize] = '\0';	// Update the buffer length and terminate the string.

			return true;
		}
		inline const bool Replace (const indexType startIndex, const DynamicArray &newString)
		{
			return Replace (startIndex, m_itemCount - startIndex, newString, 0u, newString.m_itemCount);
		}

		//
		// Function: ReplaceAll
		//
		// Description: Replaces string in other string.
		//
		// Parameters:
		//	oldString - Old string to replace.
		//	newString - New string to replace with.
		//	startIndex - Index to start replace from.
		//	count - Length of the section to be replaced within the string. If this is longer than the actual length of the string, the function replaces the entire segction between 'startIndex' and the end of the string.
		//	caseCheck - Checks whether with/without case check.
		//
		// Returns: Number of occurrences replaced.
		//
		inline const indexType ReplaceAll (const DynamicArray &oldString, const DynamicArray &newString, indexType startIndex, const indexType count, const bool caseCheck = true)
		{
			// Reliability check.
			InternalAssert (!oldString.IsEmpty () && oldString != newString);

			indexType replacedOccurrencesCount (0u);

//			for (/* Empty */; startIndex < m_itemCount; startIndex += newString.m_itemCount, ++replacedOccurrencesCount)
/*			{
				startIndex = Find (oldString, startIndex, oldString.m_itemCount, caseCheck);

				if (startIndex == BadPosition)
					break;

				Remove (startIndex, oldString.m_itemCount);
				Insert (startIndex, newString);
			}*/
			for (startIndex = Find (oldString, startIndex, oldString.m_itemCount, caseCheck); startIndex < m_itemCount; Remove (startIndex, oldString.m_itemCount), Insert (startIndex, newString), startIndex = Find (oldString, startIndex + newString.m_itemCount, oldString.m_itemCount, caseCheck), ++replacedOccurrencesCount);
//			for (startIndex = Find (oldString, startIndex, oldString.m_itemCount, caseCheck); startIndex < m_itemCount; Replace (startIndex, oldString.m_itemCount, newString, 0u, newString.m_itemCount), startIndex = Find (oldString, startIndex + newString.m_itemCount, oldString.m_itemCount, caseCheck), ++replacedOccurrencesCount);

			return replacedOccurrencesCount;
		}

		//
		// Function: ReplaceAll
		//
		// Description: Replaces string in other string.
		//
		// Parameters:
		//	oldString - Old string to replace.
		//	newString - New string to replace with.
		//	startIndex - Index to start replace from.
		//	caseCheck - Checks whether with/without case check.
		//
		// Returns: Number of occurrences replaced.
		//
/*		inline const indexType ReplaceAll (const DynamicArray &oldString, const DynamicArray &newString, indexType startIndex = 0u, const bool caseCheck = true)
		{
			return ReplaceAll (oldString, newString, startIndex, newString.m_itemCount, caseCheck);
		}
*/
		//
		// Function: ReplaceAll
		//
		// Description: Replaces string in other string.
		//
		// Parameters:
		//	oldString - Old string to replace.
		//	newString - New string to replace with.
		//	caseCheck - Checks whether with/without case check.
		//
		// Returns: Number of occurrences replaced.
		//
		inline const indexType ReplaceAll (const DynamicArray &oldString, const DynamicArray &newString, const bool caseCheck = true)
		{
			return ReplaceAll (oldString, newString, 0u, newString.m_itemCount, caseCheck);
		}

		//
		// Function: Contains
		//
		// Description: Checks whether string contains something.
		//
		// Parameters:
		//	string - String to check.
		//	caseCheck - Checks whether with/without case check.
		//
		// Returns: True if string exists, false otherwise.
		//
		inline const bool Contains (const DynamicArray &string, const bool caseCheck = true) const
		{
			return Find (string, 0u, string.m_itemCount, caseCheck) != BadPosition;
		}

		//
		// Function: Hash
		//
		// Description: Gets the string hash.
		//
		// Returns: Hash of the string.
		//
		inline const unsigned int Hash (void) const
		{
			unsigned int hash (0ul);

			for (indexType index (0u); index < m_itemCount; ++index)
				hash = (hash << 5ul) + hash + m_elements[index];

			return hash;
		}

		//
		// Function: Split
		//
		// Description: Splits string using string separator.
		//
		// Parameters:
		//	separator - Separator to split with.
		//
		// Returns: Array of slitted strings.
		//
		template <typename indexType2> inline const DynamicArray <DynamicArray, indexType2> Split (const char separator) const
		{
			return Split <indexType2> (0u, separator);
		}
		template <typename indexType2> inline const DynamicArray <DynamicArray, indexType2> SplitWithNumberOfTimes (indexType startIndex, const char separator, indexType number) const
		{
			// Reliability check.
			InternalAssert (separator != '\0' && startIndex < m_itemCount && number > 0u);

			DynamicArray <DynamicArray, indexType2> holder;

			if ((startIndex = FindOneDiscrepancy (separator, startIndex)) == BadPosition)
				return holder;

			indexType tokenLength;

			do
			{
				if ((tokenLength = Find (separator, startIndex)) == BadPosition)
					return holder += GetSubstring (startIndex, m_itemCount - startIndex);

				holder += GetSubstring (startIndex, tokenLength - startIndex);
			} while (--number > 0u && (startIndex = FindOneDiscrepancy (separator, ++tokenLength)) != BadPosition);

			return holder;
		}
		template <typename indexType2> inline const DynamicArray <DynamicArray, indexType2> Split (indexType startIndex, const char separator) const
		{
			// Reliability check.
			InternalAssert (separator != '\0' && startIndex < m_itemCount);

			DynamicArray <DynamicArray, indexType2> holder;

			if ((startIndex = FindOneDiscrepancy (separator, startIndex)) == BadPosition)
				return holder;

			indexType tokenLength;

			do
			{
				if ((tokenLength = Find (separator, startIndex)) == BadPosition)
					return holder += GetSubstring (startIndex, m_itemCount - startIndex);

				holder += GetSubstring (startIndex, tokenLength - startIndex);
			} while ((startIndex = FindOneDiscrepancy (separator, ++tokenLength)) != BadPosition);

			return holder;
		}
		template <typename indexType2> inline const DynamicArray <DynamicArray, indexType2> Split (indexType startIndex, const indexType count, const char separator) const
		{
			// Reliability check.
			InternalAssert (separator != '\0' && startIndex < m_itemCount && count <= m_itemCount - startIndex && count > 0u);

			DynamicArray <DynamicArray, indexType2> holder;

			if ((startIndex = FindOneDiscrepancy (separator, startIndex, count)) == BadPosition)
				return holder;

			indexType tokenLength;

			do
			{
				if ((tokenLength = Find (separator, startIndex, count)) == BadPosition)
					return holder += GetSubstring (startIndex, Math::GetMinimumValueBetween <indexType> (count, m_itemCount - startIndex));

				holder += GetSubstring (startIndex, tokenLength - startIndex);
			} while ((startIndex = FindOneDiscrepancy (separator, ++tokenLength, count)) != BadPosition);
/*
			DynamicArray <DynamicArray, indexType2> holder;
			const indexType end (startIndex + count);
			indexType tokenLength;

			for (startIndex = FindOneDiscrepancy (separator, startIndex, count), tokenLength = Find (separator, startIndex, count); startIndex < tokenLength; holder += GetSubstring (startIndex, tokenLength - startIndex), startIndex = tokenLength, startIndex = FindOneDiscrepancy (separator, startIndex, count), tokenLength = Find (separator, startIndex, count))
				if (tokenLength == BadPosition)
					tokenLength = end;
*/
			return holder;
		}
		template <typename indexType2> inline const DynamicArray <DynamicArray, indexType2> Split (const DynamicArray &separator) const
		{
			return Split <indexType2> (0u, separator);
		}
		template <typename indexType2> inline const DynamicArray <DynamicArray, indexType2> Split (indexType startIndex, const DynamicArray &separator) const
		{
			// Reliability check.
			InternalAssert (!separator.IsEmpty () && startIndex < m_itemCount);

			DynamicArray <DynamicArray, indexType2> holder;

			if ((startIndex = FindOneDiscrepancyOf (separator, startIndex)) == BadPosition)
				return holder;

			indexType tokenLength;

			do
			{
				if ((tokenLength = FindOneOf (separator, startIndex)) == BadPosition)
					return holder += GetSubstring (startIndex, m_itemCount - startIndex);

				holder += GetSubstring (startIndex, tokenLength - startIndex);
			} while ((startIndex = FindOneDiscrepancyOf (separator, ++tokenLength)) != BadPosition);

			return holder;
		}
		template <typename indexType2> inline const DynamicArray <DynamicArray, indexType2> Split (indexType startIndex, const indexType count, const DynamicArray &separator) const
		{
			// Reliability check.
			InternalAssert (!separator.IsEmpty () && startIndex < m_itemCount && count <= m_itemCount - startIndex && count > 0u);

			DynamicArray <DynamicArray, indexType2> holder;

			if ((startIndex = FindOneDiscrepancyOf (separator, startIndex, count)) == BadPosition)
				return holder;

			indexType tokenLength;

			do
			{
				if ((tokenLength = FindOneOf (separator, startIndex, count)) == BadPosition)
					return holder += GetSubstring (startIndex, Math::GetMinimumValueBetween <indexType> (count, m_itemCount - startIndex));

				holder += GetSubstring (startIndex, tokenLength - startIndex);
			} while ((startIndex = FindOneDiscrepancyOf (separator, ++tokenLength, count)) != BadPosition);
/*
			DynamicArray <DynamicArray, indexType2> holder;
			const indexType end (startIndex + count);
			indexType tokenLength;

			for (startIndex = FindOneDiscrepancyOf (separator, startIndex, count), tokenLength = FindOneOf (separator, startIndex, count); startIndex < tokenLength; holder += GetSubstring (startIndex, tokenLength - startIndex), startIndex = tokenLength, startIndex = FindOneDiscrepancyOf (separator, startIndex, count), tokenLength = FindOneOf (separator, startIndex, count))
				if (tokenLength == BadPosition)
					tokenLength = end;
*/
			return holder;
		}
/*		template <typename indexType2> inline const DynamicArray <DynamicArray, indexType2> SplitQuotes (indexType startIndex, const indexType count, const DynamicArray &separator) const
		{
			// Reliability check.
			InternalAssert (!separator.IsEmpty () && startIndex < m_itemCount && count <= m_itemCount - startIndex && count > 0u);

			/// @todo !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		}
*/
		//
		// Function: GetData
		//
		// Description: Gets the pointer to all element in array.
		//
		// Returns: Pointer to object list.
		//
		inline char *const GetData (void) { return m_elements; }

		//
		// Function: GetData
		//
		// Description: Gets the constant pointer to all element in array.
		//
		// Returns: Constant pointer to object list.
		//
		inline const char *const GetData (void) const { return m_elements; }

		//
		// Function: IsEmpty
		//
		// Description: Checks whether array is empty.
		//
		// Returns: True if array is empty, false otherwise.
		//
		inline const bool IsEmpty (void) const { return m_itemCount == 0u; }

		//
		// Function: FreeExtra
		//
		// Description: Frees unused space.
		//
		// Returns: True if element is empty, false otherwise.
		//
		inline const bool FreeExtra (void)
		{
			// Is no needed to free?
			if (IsEmpty ())
				return true;

			const indexType newAllocatedSize (m_itemCount + sizeof ('\0'));

			// Is no needed to free?
			if (m_allocatedSize == newAllocatedSize)
				return true;

			char *const buffer (new char[newAllocatedSize]);

			// Reliability check.
			if (buffer == NULL)
				return false;

			for (indexType index (0u); index < m_itemCount; buffer[index] = m_elements[index], ++index);	// Copy data.

			buffer[m_itemCount] = '\0';	// Terminate the string.

			delete [] m_elements;

			m_elements = buffer;
			m_allocatedSize = newAllocatedSize;

			return true;
		}

		inline const bool SetFromPointer (char *const bufferPointer, const indexType count)
		{
			// Reliability check.
			InternalAssert (IsEmpty ());

			m_elements = bufferPointer;
			m_allocatedSize = m_itemCount = count;

			++m_allocatedSize;
		}

		//
		// Function: Assign
		//
		// Description: Assigns the number, pointed by 'count' of elements to existing buffer.
		//
		// Parameters:
		//	input - Element to assign.
		//	count - Number of elements to use for the content (i.e., it's length).
		//
		// Returns: True if element is assigned, false otherwise.
		//
		inline const bool Assign (const char input, const indexType count = 1u)
		{	// assign count * input.
			// Reliability check (Result too long).
			InternalAssert (count < MaximumPossibleSize);

			// Make room and assign new stuff.

			// Reliability check.
			if (!SetSizeWithoutKeepingData (count + 1u))
				return false;

			for (m_itemCount = 0u; m_itemCount < count; ++m_itemCount)
				m_elements[m_itemCount] = input;	// Fill hole.

			m_elements[m_itemCount] = '\0';	// Terminate the string.

			return true;
		}

		//
		// Function: Assign
		//
		// Description: Assigns the array to existing buffer. (bufferPointer, bufferPointer + count)
		//
		// Parameters:
		//	bufferPointer - Array buffer to assign.
		//	count - Number of elements to use for the content (i.e., it's length).
		//
		// Returns: True if input array is assigned, false otherwise.
		//
		inline const bool Assign (const char *const bufferPointer, const indexType count)
		{
			if (IsInside (bufferPointer))
				return Assign (*this, static_cast <indexType> (bufferPointer - m_elements), count);	// Subarray.

			// Reliability check.
			InternalAssert ((count == 0u || bufferPointer != NULL) && count <= GetStringLength <indexType> (bufferPointer));

			// Make room and assign new stuff.

			// Reliability check.
			if (!SetSizeWithoutKeepingData (count + 1u))
				return false;

			for (m_itemCount = 0u; m_itemCount < count; m_elements[m_itemCount] = bufferPointer[m_itemCount], ++m_itemCount);

			m_elements[m_itemCount] = '\0';	// Terminate the string.

			return true;
		}

		//
		// Function: Assign
		//
		// Description: Assigns the array to existing buffer. (bufferPointer, bufferPointer + GetStringLength (bufferPointer))
		//
		// Parameters:
		//	bufferPointer - Array buffer to assign.
		//
		// Returns: True if input array is assigned, false otherwise.
		//
		inline const bool Assign (const char *const bufferPointer)
		{
			// Reliability check.
			InternalAssert (bufferPointer != NULL);

			return Assign (bufferPointer, GetStringLength <indexType> (bufferPointer));
		}

		//
		// Function: Assign
		//
		// Description: Assigns the array to existing buffer. (startIndex, startIndex + count)
		//
		// Parameters:
		//	inputArray - Array buffer to assign.
		//	startIndex - Starting position of the subarray of the input array that forms the new content.
		//	count - Number of elements to use for the content (i.e., it's length).
		//
		// Returns: True if input array is assigned, false otherwise.
		//
		/*template <typename indexType2> */inline const bool Assign (const DynamicArray/* <char, indexType2>*/ &inputArray, const indexType/*2*/ startIndex, const indexType/*2*/ count)
		{
			// Reliability check ('startIndex' off end).
			InternalAssert (startIndex <= inputArray.m_itemCount && count <= inputArray.m_itemCount - startIndex);

			if (/*static_cast <void *> (*/this/*)*/ == /*static_cast <const void *const> (*/&inputArray/*)*/)
			{
				// Substring.

				Remove (0u, /*static_cast <indexType> (*/startIndex/*)*/);
				Remove (/*static_cast <indexType> (*/startIndex + count/*)*/);

				return true;
			}
			else if (SetSizeWithoutKeepingData (/*static_cast <indexType> (*/count + 1u/*)*/))	// Make room and assign new stuff.
			{
				for (m_itemCount = 0u; m_itemCount < count; m_elements[m_itemCount] = inputArray.m_elements[startIndex + m_itemCount], ++m_itemCount);

				// Terminate the string.
				m_elements[m_itemCount] = '\0';

				return true;
			}

			return false;
		}

		//
		// Function: Assign
		//
		// Description: Assigns the array to existing buffer. (startIndex, inputArray.m_itemCount - startIndex)
		//
		// Parameters:
		//	inputArray - Array buffer to assign.
		//	startIndex - Starting position of the subarray of the input array that forms the new content.
		//
		// Returns: True if input array is assigned, false otherwise.
		//
		/*template <typename indexType2> */inline const bool Assign (const DynamicArray/* <char, indexType2>*/ &inputArray, const indexType/*2*/ startIndex = 0u)
		{
			return Assign/* <indexType2>*/ (inputArray, startIndex, inputArray.m_itemCount - startIndex);
		}

		//
		// Function: Append
		//
		// Description: Appends the character to existing buffer.
		//
		// Parameters:
		//	character - Character to append.
		//	count - Number of characters to append.
		//
		inline const bool Append (const char character, indexType count = 1u)
		{
			// Reliability check (Result too long).
			InternalAssert (MaximumPossibleSize - m_itemCount > count);

			// Make room and append new stuff using assign.

			// Reliability check.
			if (!SetSize (m_itemCount + count + 1u))
				return false;

			for (/* Empty */; count > 0u; ++m_itemCount, --count)
				m_elements[m_itemCount] = character;	// Fill hole.

			m_elements[m_itemCount] = '\0';	// Terminate the string.

			return true;
		}

		//
		// Function: Append
		//
		// Description: Appends the string to existing buffer. [bufferPointer, bufferPointer + count)
		//
		// Parameters:
		//	bufferPointer - String buffer to append.
		//	count - Number of characters to append.
		//
		inline const bool Append (const char *bufferPointer, indexType count)
		{
			if (IsInside (bufferPointer))
				return Append (*this, static_cast <indexType> (bufferPointer - m_elements), count);	// Substring.

			// Reliability check (Result too long or 'count' out of size).
			InternalAssert ((count == 0u || bufferPointer != NULL) && count <= GetStringLength <indexType> (bufferPointer) && MaximumPossibleSize - m_itemCount > count && m_itemCount + count >= m_itemCount);

			// Make room and append new stuff.

			// Reliability check.
			if (!SetSize (m_itemCount + count + 1u))
				return false;

			for (/* Empty */; count > 0u; ++m_itemCount, ++bufferPointer, --count)
				m_elements[m_itemCount] = *bufferPointer;	// Fill hole.

			m_elements[m_itemCount] = '\0';	// Terminate the string.

			return true;
		}

		//
		// Function: Append
		//
		// Description: Appends the string to existing buffer. [bufferPointer, bufferPointer + GetStringLength (bufferPointer))
		//
		// Parameters:
		//	bufferPointer - String buffer to append.
		//
		inline const bool Append (const char *const bufferPointer)
		{
			// Reliability check.
			InternalAssert (bufferPointer != NULL);

			return Append (bufferPointer, GetStringLength <indexType> (bufferPointer));
		}

		//
		// Function: Append
		//
		// Description: Appends the string to existing buffer. [startIndex, startIndex + count)
		//
		// Parameters:
		//	inputArray - String buffer to append.
		//	startIndex - Start index to append.
		//	count - Number of characters to append.
		//
		/*template <typename indexType2> */inline const bool Append (const DynamicArray/* <char, indexType2>*/ &inputArray, indexType/*2*/ startIndex, indexType/*2*/ count)
		{
			// Reliability check ('startIndex' off end or result too long or 'count' out of size).
			InternalAssert (startIndex <= inputArray.m_itemCount && count <= inputArray.m_itemCount - startIndex && MaximumPossibleSize - m_itemCount > count && m_itemCount + count >= m_itemCount);

			// Make room and append new stuff.

			// Reliability check.
			if (!SetSize (m_itemCount + count + 1u))
				return false;

			for (/* Empty */; count > 0u; ++m_itemCount, ++startIndex, --count)
				m_elements[m_itemCount] = inputArray.m_elements[startIndex];	// Fill hole.

			m_elements[m_itemCount] = '\0';	// Terminate the string.

			return true;
		}

		//
		// Function: Append
		//
		// Description: Appends the string to existing buffer. [startIndex, inputArray.m_itemCount - startIndex)
		//
		// Parameters:
		//	inputArray - String buffer to append.
		//	startIndex - Start index to append.
		//
		/*template <typename indexType2> */inline const bool Append (const DynamicArray/* <char, indexType2>*/ &inputArray, const indexType/*2*/ startIndex = 0u)
		{
			return Append/* <indexType2>*/ (inputArray, startIndex, inputArray.m_itemCount - startIndex);
		}

		//
		// Function: AppendFormat
		//
		// Description: Appends the formatted string to existing buffer.
		//
		// Parameters:
		//	format - Formatted string buffer to append.
		//
		inline void AppendFormat (const char *const format, ...)
		{
			char tempBuffer[1024u];

			// Concatenate all the arguments in one string....
			COLLECT_AND_STORE_ARGUMENTS_INTO_ONE_STRING (format, tempBuffer, sizeof (tempBuffer));

			Append (tempBuffer);
		}
		inline void AppendFormat (const indexType count, const char *const format, ...)
		{
			char *const tempBuffer (new char[count]);

			// Reliability check.
			if (tempBuffer == NULL)
			{
				AddLogEntry (true, LogLevel_Error, false, "Couldn't allocate %.2f kilobytes for string buffer!", count * sizeof (ElementType_t) / 1024.0f);

				return;
			}

			// Concatenate all the arguments in one string....
			COLLECT_AND_STORE_ARGUMENTS_INTO_ONE_STRING (format, tempBuffer, count);

			Append (tempBuffer, count);
		}

		//
		// Function: AssignFormat
		//
		// Description: Assigns the formatted string to existing buffer.
		//
		// Parameters:
		//	format - Formatted string buffer to assign.
		//
		inline void AssignFormat (const char *const format, ...)
		{
			char tempBuffer[1024u];

			// Concatenate all the arguments in one string....
			COLLECT_AND_STORE_ARGUMENTS_INTO_ONE_STRING (format, tempBuffer, sizeof (tempBuffer));

			Assign (tempBuffer);
		}
		inline void AssignFormat (const indexType count, const char *const format, ...)
		{
			char *const tempBuffer (new char[count]);

			// Reliability check.
			if (tempBuffer == NULL)
			{
				AddLogEntry (true, LogLevel_Error, false, "Couldn't allocate %.2f kilobytes for string buffer!", count * sizeof (ElementType_t) / 1024.0f);

				return;
			}

			// Concatenate all the arguments in one string....
			COLLECT_AND_STORE_ARGUMENTS_INTO_ONE_STRING (format, tempBuffer, count);

			Assign (tempBuffer, count);
		}

		//
		// Function: Remove
		//
		// Description: Deletes elements from array. (sourceIndex, sourceIndex + count)
		//
		// Parameters:
		//	sourceIndex - Starting position of elements to remove.
		//	count - Number of elements to remove.
		//
		// Returns: True if the deleting passed successfully, false otherwise.
		//
		inline void Remove (const indexType sourceIndex, const indexType count)
		{
			// Checks for bounds....
			InternalAssert (sourceIndex <= m_itemCount && count <= m_itemCount - sourceIndex);

			// Move elements down (for characters the MoveItems() trims last element).
			MoveItems (sourceIndex, sourceIndex + count);

			// Update array length.
			m_itemCount -= count;
		}

		//
		// Function: Remove
		//
		// Description: Deletes elements from array. (sourceIndex, m_itemCount - sourceIndex)
		//
		// Parameters:
		//	sourceIndex - Starting position of elements to remove.
		//
		// Returns: True if the deleting passed successfully, false otherwise.
		//
		inline void Remove (const indexType sourceIndex = 0u)
		{
			Remove (sourceIndex, m_itemCount - sourceIndex);	// Shrink to nothing.
		}

		//
		// Function: DeleteLeft
		//
		// Description: Deletes elements from left side of array.
		//
		// Parameters:
		//	count - Number of characters to remove.
		//
		inline void DeleteLeft (const indexType count) { Remove (0u, count); }

		//
		// Function: DeleteRight
		//
		// Description: Deletes elements from right side of array.
		//
		// Parameters:
		//	count - Number of characters to remove.
		//
		inline void DeleteRight (const indexType count)
		{
			// Reliability check.
			InternalAssert (count <= m_itemCount);

			Remove (m_itemCount - count, count);
		}

		inline void RemoveWithCaseCheck (const char character)
		{
			InternalAssert (!IsEmpty () && character != '\0');

			for (indexType index (0u); index < m_itemCount; /* Empty */)
			{
				if (m_elements[index] == character)
					Remove (index, 1u);
				else
					++index;
			}
		}

		inline void RemoveWithoutCaseCheck (char character)
		{
			InternalAssert (!IsEmpty () && character != '\0');

			character = static_cast <char> (tolower (character));

			for (indexType index (0u); index < m_itemCount; /* Empty */)
			{
				if (tolower (m_elements[index]) == character)
					Remove (index, 1u);
				else
					++index;
			}
		}

		inline void RemoveWithCaseCheck (const DynamicArray &separator)
		{
			InternalAssert (!IsEmpty () && !separator.IsEmpty ());

			for (indexType index (0u); index < m_itemCount; /* Empty */)
			{
				if (separator.IsMemberWithCaseCheck (m_elements[index]))
					Remove (index, 1u);
				else
					++index;
			}
		}

		inline void RemoveWithoutCaseCheck (const DynamicArray &separator)
		{
			InternalAssert (!IsEmpty () && !separator.IsEmpty ());

			for (indexType index (0u); index < m_itemCount; /* Empty */)
			{
				if (separator.IsMemberWithoutCaseCheck (m_elements[index]))
					Remove (index, 1u);
				else
					++index;
			}
		}

		//
		// Function: Sort
		//
		// Description: Sorts the array.
		//
		// Parameters:
		//	comparator - Function to compare two elements.
		//
		inline void Sort (const SortComparatorFunction_t comparator)
		{
			if (m_itemCount < 2u)
				return;

			// Sort remaining arguments using Quicksort algorithm:
			qsort (m_elements, m_itemCount, ElementSize, reinterpret_cast <int (*) (const void *const, const void *const)> (comparator));
		}

		//
		// Function: Sort
		//
		// Description: Sorts the array.
		//
		// Parameters:
		//	comparator - Function to compare two elements.
		//	sourceIndex - Starting position of elements to sort.
		//
		inline void Sort (const SortComparatorFunction_t comparator, const indexType sourceIndex)
		{
			// Checks for bounds....
			InternalAssert (sourceIndex < m_itemCount);

			Sort (sourceIndex, m_itemCount - sourceIndex, comparator);
		}

		//
		// Function: Sort
		//
		// Description: Sorts the array.
		//
		// Parameters:
		//	comparator - Function to compare two elements.
		//	sourceIndex - Starting position of elements to sort.
		//	count - Number of elements to sort.
		//
		inline void Sort (const SortComparatorFunction_t comparator, const indexType sourceIndex, const indexType count)
		{
			// Checks for bounds....
			InternalAssert (sourceIndex < m_itemCount && count <= m_itemCount - sourceIndex);

			if (count < 2u)
				return;

			// Sort remaining arguments using Quicksort algorithm:
			qsort (m_elements + sourceIndex, count, ElementSize, reinterpret_cast <int (*) (const void *const, const void *const)> (comparator));
		}

		//
		// Function: GetBuffer
		//
		// Description: Gets the buffer with specified length.
		//
		// Parameters:
		//	minLength - Length to retrieve.
		//
		// Returns: Pointer to string buffer.
		//
/*		inline const char *const GetBuffer (const indexType minLength)
		{
			if (minLength >= m_allocatedSize)
				SetSize (minLength + 1u);

			return m_elements;
		}

		//
		// Function: GetBufferSetLength
		//
		// Description: Gets the buffer with specified length, and terminates string with that length.
		//
		// Parameters:
		//	minLength - Length to retrieve.
		//
		// Returns: Pointer to string buffer.
		//
		inline const char *const GetBufferSetLength (const indexType length)
		{
			const char *const buffer (GetBuffer (length));

			ReleaseBuffer (length);

			return buffer;
		}
*/
	//
	// Group: Operators.
	//
	public:
		// Following five operators are equal's GetData() function.
		inline operator       char *const    (void)       { return m_elements; }
		inline operator const char *const    (void) const { return m_elements; }
		inline operator       char *const    (void) const { return m_elements; }	// Needed too :{O (Half-Life engine so stupid....)

		inline       char *const operator () (void)       { return m_elements; }
		inline const char *const operator () (void) const { return m_elements; }

		// Element access...

		//
		// Function: operator []
		//
		// Description: Gets element from specified index.
		//
		// Parameters:
		//	index - Element index to retrieve.
		//
		// Returns: Element object.
		//
		inline char       &operator [] (const indexType index)
		{
			// Reliability check.
			InternalAssert (index <= m_itemCount);

			return m_elements[index];
		}
		inline const char &operator [] (const indexType index) const
		{
			// Reliability check.
			InternalAssert (index <= m_itemCount);

			return m_elements[index];
		}

		inline operator                           const short (void) const                                      { return GetValue <short> (); }
		inline operator                                 short (void)                                            { return GetValue <short> (); }

		inline operator                  const unsigned short (void) const                                      { return GetValue <unsigned short> (); }
		inline operator                        unsigned short (void)                                            { return GetValue <unsigned short> (); }

		inline operator                             const int (void) const                                      { return GetValue <int> (); }
		inline operator                                   int (void)                                            { return GetValue <int> (); }

		inline operator                    const unsigned int (void) const                                      { return GetValue <unsigned int> (); }
		inline operator                          unsigned int (void)                                            { return GetValue <unsigned int> (); }

		inline operator                           const float (void) const                                      { return GetValue <float> (); }
		inline operator                                 float (void)                                            { return GetValue <float> (); }

		inline operator                          const double (void) const                                      { return GetValue <double> (); }
		inline operator                                double (void)                                            { return GetValue <double> (); }

		inline DynamicArray *const operator                -> (void) const                                      { return const_cast <DynamicArray *const> (this); }

		/*template <typename indexType2> */inline const DynamicArray operator                  + (const DynamicArray/* <char, indexType2>*/ &otherString) const
		{
			DynamicArray result (*this);

			result/*.operator*/ +=/* <indexType2> (*/otherString/*)*/;

			return result;
		}

		inline const DynamicArray operator                  + (const char character) const
		{
			DynamicArray result (*this);

			result += character;

			return result;
		}
		friend inline const DynamicArray operator           + (const char character, const DynamicArray &holder)
		{
			DynamicArray result (character);

			result += holder;

			return result;
		}

		inline const DynamicArray operator                  + (const char *const string) const
		{
			DynamicArray result (*this);

			result += string;

			return result;
		}
		friend inline const DynamicArray operator           + (const char *const string, const DynamicArray &holder)
		{
			DynamicArray result (string);

			result += holder;

			return result;
		}

		inline const bool operator                          & (const char input)                                            const { return IsMemberWithCaseCheck (input); }
		inline const bool operator                          & (const DynamicArray &otherString)                             const { return IsMemberWithCaseCheck (otherString); }

		inline const bool operator                          < (const char *const bufferPointer)                             const { return CompareWithCaseCheck (bufferPointer) < 0; }
		inline const bool operator                          < (const DynamicArray &otherString)                             const { return CompareWithCaseCheck (otherString) < 0; }
		inline const bool operator                          > (const char *const bufferPointer)                             const { return CompareWithCaseCheck (bufferPointer) > 0; }
		inline const bool operator                          > (const DynamicArray &otherString)                             const { return CompareWithCaseCheck (otherString) > 0; }

		// Equality.
		inline const bool operator                         == (const char character)                                        const { return CompareWithCaseCheck (character) == 0; }
		inline const bool operator                         == (const char *const bufferPointer)                             const { return CompareWithCaseCheck (bufferPointer) == 0; }
		friend inline const bool operator                  == (const char *const bufferPointer, const DynamicArray &string)       { return string == bufferPointer; }
		inline const bool operator                         == (const DynamicArray &otherString)                             const { return CompareWithCaseCheck (otherString) == 0; }

		// Inequality.
		inline const bool operator                         != (const char character)                                        const { return CompareWithCaseCheck (character) != 0; }
		inline const bool operator                         != (const char *const bufferPointer)                             const { return CompareWithCaseCheck (bufferPointer) != 0; }
		friend inline const bool operator                  != (const char *const bufferPointer, const DynamicArray &string)       { return string != bufferPointer; }
		inline const bool operator                         != (const DynamicArray &otherString)                             const { return CompareWithCaseCheck (otherString) != 0; }

		//
		// Function: operator =
		//
		// Description: Assigns the character to existing buffer.
		//
		// Parameters:
		//	input - Character to assign.
		//
		// Returns: Assigned character.
		//
		inline DynamicArray &operator                       = (const char input)
		{
			Assign (input);

			return *this;
		}

		//
		// Function: operator =
		//
		// Description: Assigns the string to existing buffer.
		//
		// Parameters:
		//	bufferPointer - String buffer to assign.
		//
		// Returns: Assigned buffer pointer.
		//
		inline DynamicArray &operator                       = (const char *const bufferPointer)
		{
			Assign (bufferPointer);

			return *this;
		}

		//
		// Function: operator =
		//
		// Description: Assigns the string to existing buffer.
		//
		// Parameters:
		//	inputArray - String buffer to assign.
		//
		// Returns: Assigned string.
		//
		/*template <typename indexType2> */inline DynamicArray &operator                       = (const DynamicArray/* <char, indexType2>*/ &inputArray)
		{
			Assign/* <indexType2>*/ (inputArray);

			return *this;
		}

		//
		// Function: operator +=
		//
		// Description: Appends the character to existing buffer.
		//
		// Parameters:
		//	input - Character to append.
		//
		// Returns: String with new input character.
		//
		inline DynamicArray &operator                      += (const char input)
		{
			Append (input);

			return *this;
		}

		//
		// Function: operator +=
		//
		// Description: Appends the string to existing buffer.
		//
		// Parameters:
		//	bufferPointer - String buffer to append.
		//
		// Returns: String with new buffer pointer.
		//
		inline DynamicArray &operator                      += (const char *const bufferPointer)
		{
			Append (bufferPointer);

			return *this;
		}

		//
		// Function: operator +=
		//
		// Description: Appends the array to existing buffer.
		//
		// Parameters:
		//	inputArray - Array buffer to append.
		//
		// Returns: Array with new input array.
		//
		/*template <typename indexType2> */inline DynamicArray &operator                      += (const DynamicArray/* <char, indexType2>*/ &inputArray)
		{
			Append/* <indexType2>*/ (inputArray);

			return *this;
		}
};

typedef DynamicArray <char, unsigned char> ShortDynamicString;
typedef DynamicArray <char>                DynamicString;
typedef DynamicArray <char, unsigned int>  LongDynamicString;
/*! @todo !!!
typedef StaticArray <char, unsigned char> ShortStaticString;
typedef StaticArray <char>                StaticString;
typedef StaticArray <char, unsigned int>  LongStaticString;
*/
#endif	// ifndef STRING_INCLUDED