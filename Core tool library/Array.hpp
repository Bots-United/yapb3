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
// Array.hpp
//
// Class: Array
//
// Description: Universal template array container.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef ARRAY_INCLUDED
#define ARRAY_INCLUDED

template <typename elementType, typename indexType = unsigned short> class DynamicArray
{
	//
	// Group: Protected members.
	//
	protected:
		elementType *m_elements;		// The actual array of data. (buffer holding memory block)
		indexType    m_allocatedSize;	// Allocated size.
		indexType    m_itemCount;		// ¹ of elements (upperBound - 1).

	//
	// Group: Special points.
	//
	public:
		static const indexType BadPosition = static_cast <indexType> (-1);	// Generic bad/missing length/position.
		enum
		{
			ElementSize = sizeof (elementType),	// One array element size.
//			BadPosition = static_cast <indexType> (-1),	// Generic bad/missing length/position.
			MaximumPossibleSize = min/*Math::GetMinimumValueBetween <unsigned int>*/ (BadPosition, CoreToolLibrary::BadPosition / ElementSize / 2u)	// Maximum possible array size.
		};

	//
	// Group: Types definitions.
	//
	public:
		typedef elementType ElementType_t;
		typedef indexType   IndexType_t;

		enum BinaryFindCompareResult_t
		{
			Result_Equal,

			Result_Less,
			Result_More
		};
		typedef const BinaryFindCompareResult_t (*const BinaryFindComparatorFunction_t) (const elementType &entry1, const elementType &entry2);
		typedef const int (*const SortComparatorFunction_t) (const elementType *const entry1, const elementType *const entry2);

	//
	// Group: (Con/De)structors.
	//
	public:
		//
		// Function: DynamicArray
		//
		// Description: Default array constructor.
		//
		inline DynamicArray (void) : m_elements (NULL), m_allocatedSize (0u), m_itemCount (0u) { /* VOID */ }

		//
		// Function: DynamicArray
		//
		// Description: Initializes a element buffer with given by the size.
		//
		// Parameters:
		//	size - Initial size to assign.
		//
		explicit inline DynamicArray (const indexType size) : m_elements (new elementType[size]), m_allocatedSize (size), m_itemCount (0u)
		{
			// Reliability check.
			InternalAssert (size >= 1u && size <= MaximumPossibleSize);

			#if defined _DEBUG
				// Reliability check.
				if (m_elements == NULL)
				{
					AddLogEntry (true, LogLevel_Error, false, "Couldn't allocate %.2f kilobytes for array!", size * ElementSize / 1024.0f);

					m_allocatedSize = 0u;
				}
			#endif	// if defined _DEBUG
		}

		//
		// Function: DynamicArray
		//
		// Description: Initializes a element buffer with given by the size.
		//
		// Parameters:
		//	size - Initial size to assign.
		//	count - Initial number of default elements to assign.
		//
/*		explicit inline DynamicArray (const indexType size, const indexType count) : m_elements (new elementType[size]), m_allocatedSize (size), m_itemCount (0u)
		{
			// Reliability check.
			InternalAssert (size >= 1u && size <= MaximumPossibleSize);

			// Reliability check.
			if (m_elements == NULL)
			{
				m_allocatedSize = 0u;

				return;
			}

			// Construct elements.
			PushFront (count, false);
		}
*/
		//
		// Function: DynamicArray
		//
		// Description: Initializes a array buffer with given by the input element in amount of 'count'.
		//
		// Parameters:
		//	input - Element to assign.
		//	count - Number of elements to use for the content (i.e., it's length).
		//
		inline DynamicArray (const elementType &input, const indexType count = 1u) : m_elements (NULL), m_allocatedSize (0u), m_itemCount (0u)
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
		inline DynamicArray (const elementType *const bufferPointer, const indexType count/*, const bool doCopy = true*/) : m_elements (NULL), m_allocatedSize (0u), m_itemCount (0u)
		{
//			if (doCopy)
				Assign (bufferPointer, count);
//			else
//				SetFromPointer (const_cast <elementType *const> (bufferPointer), count);
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
		inline DynamicArray (const DynamicArray &other, const indexType startIndex, const indexType count) : m_elements (NULL), m_allocatedSize (0u), m_itemCount (0u)
		{
			Assign (other, startIndex, count);
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
		inline DynamicArray (const DynamicArray &other, const indexType startIndex = 0u) : m_elements (NULL), m_allocatedSize (0u), m_itemCount (0u)
		{
			Assign (other, startIndex);
		}

		//
		// Function: ~DynamicArray
		//
		// Description: Default array destructor.
		//
		virtual inline ~DynamicArray (void)
		{
			// Destruct elements.
//			for (/* Empty */; m_itemCount > 0u; Destruct (m_elements + --m_itemCount));

			// Destroy the array buffer.
			delete [] m_elements;
		}

	//
	// Group: Private functions.
	//
	private:
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
				UninitializedMemoryNonScalarMove (m_elements + destinationIndex, m_elements + sourceIndex, m_itemCount - sourceIndex);
		}

	//
	// Group: Functions.
	//
	public:
		inline const indexType GetElementIndex (const elementType &element) const { return GetElementIndex (&element); }
		inline const indexType GetElementIndex (const elementType *const element) const
		{
			// Reliability check.
			InternalAssert (IsInside (element));

			return static_cast <indexType> ((reinterpret_cast <unsigned int> (element) - reinterpret_cast <unsigned int> (m_elements)) / ElementSize);
		}

		//
		// Function: IsInside
		//
		// Description: Test if 'element' points inside array.
		//
		// Parameters:
		//	element - Element pointer to check for.
		//
		// Returns: True if 'element' points inside array, false otherwise.
		//
		inline const bool IsInside (const elementType *const element) const
		{
			return element >= m_elements && element < m_elements + m_itemCount;	// Don't ask.
		}
#if 0
		inline const indexType GetGrowSize (const indexType wantedSize) const
		{
			// Reliability check.
			InternalAssert (wantedSize > 0u);
			InternalAssert (wantedSize <= MaximumPossibleSize);
/*ORIGINAL YAPB GROW METHOD
			// heuristically determine growth when checkSize == 0 (this avoids heap fragmentation in many situations)
			indexType checkSize (Math::GetClampedValueInRange (m_itemCount / 8u, 4u, 1024u) + m_itemCount);

			if (checkSize < wantedSize)
				checkSize = wantedSize;*/
/*
			// Rukia: We will keep the size of the vector in multiple of 2's via this method.
			//	Take log2(wantedSize), round up, find 2^that, grow to there.
			//	Why? To maintain certain requirements for O(1) amortized on certain operations.
			const unsigned int growSize (static_cast <unsigned int> (Math::powf (2.0f, Math::ceilf (Math::logf (wantedSize / Math::logf (2.0f))))));

			return growSize <= MaximumPossibleSize ? static_cast <indexType> (growSize) : MaximumPossibleSize;
/*
			unsigned int growSize (m_allocatedSize == 0u ? 8u : m_allocatedSize);

			while (growSize < wantedSize)
				growSize *= 2u;

			return growSize <= MaximumPossibleSize ? static_cast <indexType> (growSize) : MaximumPossibleSize;*/
/*
			unsigned int growSize;

			if (m_growSize > 0u)
				growSize = (1u + ((wantedSize - 1u) / m_growSize)) * m_growSize;
			else
			{
				if (m_allocatedSize == 0u)
				{
					// Compute an allocation which is at least as big as a cache line...
					growSize = (31u + ElementSize) / ElementSize;
				}
				else
					growSize = m_allocatedSize;

				while (growSize < wantedSize)
					growSize *= 2u;
			}

			return growSize <= MaximumPossibleSize ? static_cast <indexType> (growSize) : MaximumPossibleSize;*/
			return wantedSize;
		}
		inline const bool Shrink (const indexType wantedSize)
		{
			// Is no needed to resize?
			if (wantedSize >= m_allocatedSize)
				return true;

			return SetSize (GetGrowSize (wantedSize));
		}
		inline const bool Shrink (void)
		{
			// Rukia: If we are less than a third full, shrink the buffer.
			//	Why not less than half? (push;pop) results in a growth, than a shrink. Bad performance.
			if (m_itemCount * 3u < m_allocatedSize)
			{
				// Rukia: So what should we shrink it to?
				//	m_itemCount * 2 should work nicely. Stomp will force it all the way down if needed.
				return Grow (m_itemCount * 2u);
			}

			return true;
		}
		inline const bool Scale (const indexType wantedSize)
		{
			// Is no needed to resize?
			if (wantedSize == m_allocatedSize)
				return true;

			return SetSize (GetGrowSize (wantedSize));
		}
		inline const bool Grow (const indexType wantedSize)
		{
			// Is no needed to resize?
			if (wantedSize <= m_allocatedSize)
				return true;

			return SetSize (GetGrowSize (wantedSize));
		}
#endif	// if 0
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
			elementType *const newBuffer (new elementType[newSize]);

			#if defined _DEBUG
				// Reliability check.
				if (newBuffer == NULL)
				{
					AddLogEntry (true, LogLevel_Error, false, "Couldn't allocate %.2f kilobytes for array!", newSize * ElementSize / 1024.0f);

					return false;
				}
			#endif	// if defined _DEBUG

//			const indexType oldItemCount (m_itemCount);

			if (!IsEmpty ())
			{
				if (m_itemCount > newSize)
					m_itemCount = newSize;	// Trim item count to new size.

				for (indexType index (0u); index < m_itemCount; newBuffer[index] = m_elements[index], ++index);	// Copy existing elements.
//				for (indexType index (0u); index < m_itemCount; new (newBuffer + index) elementType (m_elements[index]), ++index);	// Copy existing elements.

				// Destroy the old array....
//				for (indexType index (0u); index < oldItemCount; (m_elements + index)->~elementType (), ++index);	// Destruct old buffer.
//				for (indexType index (0u); index < oldItemCount; ++index)	// Destruct old buffer.
//					Destruct (m_elements + index);
			}

			delete [] m_elements;	// Deallocate the old buffer.

			m_elements = newBuffer;	// Assign new buffer.
			m_allocatedSize = newSize;	// Assign new size.

			return true;
		}
		inline const bool SetSizeWithoutKeepingData (const indexType newSize)
		{
			// Reliability checks.
//			InternalAssert (newSize > 0u);
			InternalAssert (newSize <= MaximumPossibleSize);

			// Is no needed to resize?
			if (newSize == m_allocatedSize)
				return true;

			// Reliability check.
			InternalAssert (newSize > 0u);

			// Allocate a new buffer.
			elementType *const newBuffer (new elementType[newSize]);

			#if defined _DEBUG
				// Reliability check.
				if (newBuffer == NULL)
				{
					AddLogEntry (true, LogLevel_Error, false, "Couldn't allocate %.2f kilobytes for array!", newSize * ElementSize / 1024.0f);

					return false;
				}
			#endif	// if defined _DEBUG

			// Destroy and deallocate old array....

//			for (indexType index (0u); index < m_itemCount; (m_elements + index)->~elementType (), ++index);	// Destruct old buffer.
//			for (indexType index (0u); index < m_itemCount; ++index)	// Destruct old buffer.
//				Destruct (m_elements + index);

			delete [] m_elements;	// Destroy the old buffer.

			m_elements = newBuffer;	// Assign new buffer.
			m_allocatedSize = newSize;	// Assign new size.

			return true;
		}

		//
		// Function: MakeEmpty
		//
		// Description: Empties the array without memory deallocation.
		//
		inline void MakeEmpty (void)
		{
			// Destruct elements.
//			for (/* Empty */; m_itemCount > 0u; Destruct (m_elements + --m_itemCount));

			m_itemCount = 0u;
		}

		//
		// Function: RemoveAll
		//
		// Description: Destroys array object, and all elements.
		//
		inline void RemoveAll (void)
		{
			// Destruct elements.
//			for (/* Empty */; m_itemCount > 0u; Destruct (m_elements + --m_itemCount));

			delete [] m_elements;

			m_elements = NULL;
			m_allocatedSize = m_itemCount = 0u;
		}

		// Destruct all the elements.
		inline void DestructAll (void)
		{
			for (indexType index (0u); index < m_itemCount; ++index)
				Destruct (m_elements + index);
		}

		// Delete all the elements.
		inline void DeleteAll (void)
		{
			// Free all used memory....
			for (indexType index (0u); index < m_itemCount; ++index)
				delete m_elements[index];
		}

		// Delete all the elements then call Purge.
		inline void DeleteAndPurgeAll (void)
		{
			// Free all used memory....
			DeleteAll ();
			RemoveAll ();
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
			return sizeof (DynamicArray) + ElementSize * m_allocatedSize;
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
		// Function: Insert
		//
		// Description: Inserts number of element at specified index.
		//
		// Parameters:
		//	startIndex - Index where element should be inserted.
		//	object - Element to insert.
		//	count - Number of element to insert.
		//
		// Returns: True if operation succeeded, false otherwise.
		//
		inline const bool Insert (const indexType startIndex, const elementType &object, indexType count = 1u)
		{
			// Reliability check ('startIndex' off end or result too long).
			InternalAssert (startIndex <= m_itemCount && MaximumPossibleSize - m_itemCount >= count);

			// Reliability check.
			if (!SetSize (m_itemCount + count))
			{
				AddLogEntry (true, LogLevel_Error, false, "Couldn't allocate %.2f kilobytes in array for inserting %s!", (m_itemCount + count) * ElementSize / 1024.0f, count == 1u ? "new element" : "%u elements", count);

				return false;
			}

//			if (startIndex < m_itemCount)
				MoveItems (startIndex + count, startIndex);	// Empty out hole.

			// Update current elements number....
			m_itemCount += count;

			for (/* Empty */; count > 0u; m_elements[startIndex + --count] = object);	// Copy data.

			return true;
		}

		//
		// Function: Insert
		//
		// Description: Inserts number of elements at specified index.
		//
		// Parameters:
		//	startIndex - Index where elements should be inserted.
		//	bufferPointer - Array buffer to insert.
		//	count - Number of elements to insert.
		//
		// Returns: True if operation succeeded, false otherwise.
		//
		inline const bool Insert (const indexType startIndex, const elementType *const bufferPointer, indexType count)
		{
			if (IsInside (bufferPointer))
				return Insert (startIndex, *this, static_cast <indexType> (bufferPointer - m_elements), count);	// Subarray.

			// Reliability check ('startIndex' off end or result too long).
			InternalAssert (startIndex <= m_itemCount && (count == 0u || bufferPointer != NULL) && MaximumPossibleSize - m_itemCount >= count);

			// Reliability check.
			if (!SetSize (m_itemCount + count))
			{
				AddLogEntry (true, LogLevel_Error, false, "Couldn't allocate %.2f kilobytes in array for inserting %s!", (m_itemCount + count) * ElementSize / 1024.0f, count == 1u ? "new element" : "%u elements", count);

				return false;
			}

//			if (startIndex < m_itemCount)
				MoveItems (startIndex + count, startIndex);	// Empty out hole.

			// Update current elements number....
			m_itemCount += count;

			for (/* Empty */; count > 0u; m_elements[startIndex + --count] = bufferPointer[count]);	// Copy data.
//			for (/* Empty */; count > 0u; new (m_elements + (startIndex + --count)) elementType (bufferPointer[count]));	// Copy data.

			return true;
		}

		//
		// Function: Insert
		//
		// Description: Inserts number of elements at specified index.
		//
		// Parameters:
		//	startIndex - Index where elements should be inserted.
		//	array - Array buffer to insert.
		//	count - Number of elements to insert.
		//
		// Returns: True if operation succeeded, false otherwise.
		//
		inline const bool Insert (const indexType startIndex, const DynamicArray &array, const indexType startIndex2, indexType count)
		{	// insert array [startIndex2, startIndex2 + count) at startIndex.
			// Reliability check ('startIndex' or 'startIndex2' off end or result too long).
			InternalAssert (startIndex <= m_itemCount && startIndex2 <= array.m_itemCount && count <= array.m_itemCount - startIndex2 && MaximumPossibleSize - m_itemCount >= count);

			// Reliability check.
			if (!SetSize (m_itemCount + count))
			{
				AddLogEntry (true, LogLevel_Error, false, "Couldn't allocate %.2f kilobytes in array for inserting %s!", (m_itemCount + count) * ElementSize / 1024.0f, count == 1u ? "new element" : "%u elements", count);

				return false;
			}

			// Make room and insert new stuff.

//			if (startIndex < m_itemCount)
				MoveItems (startIndex + count, startIndex);	// Empty out hole.

			if (this == &array)
			{
				MoveItems (startIndex, startIndex < startIndex2 ? startIndex2 + count : startIndex2);	// Subarray.

				// Update current elements number....
				m_itemCount += count;
			}
			else
			{
				// Update current elements number....
				m_itemCount += count;

				for (/* Empty */; count > 0u; m_elements[startIndex + --count] = array.m_elements[startIndex2 + count]);	// Fill hole.
//				for (/* Empty */; count > 0u; new (m_elements + (startIndex + --count)) elementType (array.m_elements[startIndex2 + count]));	// Fill hole.
			}

			return true;
		}

		//
		// Function: PushFrontDefaultElement
		//
		// Description: Appends new element to the end of array.
		//
		// Parameters:
		//	count - Number of element to insert.
		//
		// Returns: True if operation succeeded, false otherwise.
		//
		inline const bool PushFrontDefaultElement (const indexType count = 1u)
		{
			return PushFront (elementType (), count);
//			return Insert (0u, elementType (), count);
		}

		//
		// Function: PushFront
		//
		// Description: Appends new element to the begin of array.
		//
		// Parameters:
		//	object - Object to append.
		//	count - Number of element to insert.
		//
		// Returns: True if operation succeeded, false otherwise.
		//
		inline const bool PushFront (const elementType &object, /*const */indexType count = 1u)
		{
			// Reliability check (Result too long).
			InternalAssert (MaximumPossibleSize - m_itemCount >= count);

			// Make room and append new stuff using assign.

			// Reliability check.
			if (!SetSize (m_itemCount + count))
			{
				AddLogEntry (true, LogLevel_Error, false, "Couldn't allocate %.2f kilobytes in array for inserting %s!", (m_itemCount + count) * ElementSize / 1024.0f, count == 1u ? "new element" : "%u elements", count);

				return false;
			}

			MoveItems (count, 0u);	// Empty out hole.

			// Update the buffer length.
			m_itemCount += count;

			for (/* Empty */; count > 0u; m_elements[--count] = object);	// Fill hole.

			return true;
//			return Insert (0u, object, count);
		}

		//
		// Function: PushFront
		//
		// Description: Appends number of elements to the begin of array.
		//
		// Parameters:
		//	objects - Pointer to objects list.
		//	count - Number of elements to insert.
		//
		// Returns: True if operation succeeded, false otherwise.
		//
		inline const bool PushFront (const elementType *const objects, /*const */indexType count)
		{
			if (IsInside (objects))
				return PushBack (*this, static_cast <indexType> (objects - m_elements), count);	// Subarray.

			// Reliability check (Result too long or 'count' out of size).
			InternalAssert ((count == 0u || objects != NULL) && MaximumPossibleSize - m_itemCount >= count);

			// Make room and append new stuff.

			// Reliability check.
			if (!SetSize (m_itemCount + count))
			{
				AddLogEntry (true, LogLevel_Error, false, "Couldn't allocate %.2f kilobytes in array for inserting %s!", (m_itemCount + count) * ElementSize / 1024.0f, count == 1u ? "new element" : "%u elements", count);

				return false;
			}

			MoveItems (count, 0u);	// Empty out hole.

			// Update the buffer length.
			m_itemCount += count;

			for (/* Empty */; count > 0u; m_elements[--count] = objects[count]);	// Fill hole.

			return true;
//			return Insert (0u, objects, count);
		}

		//
		// Function: PushFront
		//
		// Description: Inserts other array reference into the begin of our array.
		//
		// Parameters:
		//	other - Other array elements list.
		//	startIndex - Start index to insert.
		//	count - Number of elements to insert.
		//
		// Returns: True if operation succeeded, false otherwise.
		//
		inline const bool PushFront (const DynamicArray &other, const indexType startIndex, /*const */indexType count)
		{
			// Reliability check ('startIndex' off end or result too long or 'count' out of size).
			InternalAssert (startIndex <= other.m_itemCount && count <= other.m_itemCount - startIndex && MaximumPossibleSize - m_itemCount >= count);

			// Make room and append new stuff.

			// Reliability check.
			if (!SetSize (m_itemCount + count))
			{
				AddLogEntry (true, LogLevel_Error, false, "Couldn't allocate %.2f kilobytes in array for inserting %s!", (m_itemCount + count) * ElementSize / 1024.0f, count == 1u ? "new element" : "%u elements", count);

				return false;
			}

			MoveItems (count, 0u);	// Empty out hole.

			// Update the buffer length.
			m_itemCount += count;

			for (/* Empty */; count > 0u; m_elements[--count] = other.m_elements[startIndex + count]);	// Fill hole.

			return true;
//			return Insert (0u, other, startIndex, count);
		}

		//
		// Function: PushFront
		//
		// Description: Inserts other array reference into the begin of our array.
		//
		// Parameters:
		//	other - Other array elements list.
		//	startIndex - Start index to insert.
		//
		// Returns: True if operation succeeded, false otherwise.
		//
		inline const bool PushFront (const DynamicArray &other, const indexType startIndex = 0u)
		{
			return PushFront (other, startIndex, other.m_itemCount - startIndex);
//			return Insert (0u, other, startIndex, other.m_itemCount - startIndex);
		}

		//
		// Function: PushBackDefaultElement
		//
		// Description: Appends new element to the end of array.
		//
		// Parameters:
		//	count - Number of element to insert.
		//
		// Returns: True if operation succeeded, false otherwise.
		//
		inline const bool PushBackDefaultElement (const indexType count = 1u)
		{
			return PushBack (elementType (), count);
//			return Insert (m_itemCount, elementType (), count);
		}

		//
		// Function: PushBack
		//
		// Description: Appends new element to the end of array.
		//
		// Parameters:
		//	object - Object to append.
		//	count - Number of element to insert.
		//
		// Returns: True if operation succeeded, false otherwise.
		//
		inline const bool PushBack (const elementType &object, /*const */indexType count = 1u)
		{
			// Reliability check (Result too long).
			InternalAssert (MaximumPossibleSize - m_itemCount >= count);

			// Make room and append new stuff using assign.

			// Reliability check.
			if (!SetSize (m_itemCount + count))
			{
				AddLogEntry (true, LogLevel_Error, false, "Couldn't allocate %.2f kilobytes in array for inserting %s!", (m_itemCount + count) * ElementSize / 1024.0f, count == 1u ? "new element" : "%u elements", count);

				return false;
			}

			for (/* Empty */; count > 0u; ++m_itemCount, --count)
				m_elements[m_itemCount] = object;	// Fill hole.

			return true;
//			return Insert (m_itemCount, object, count);
		}

		//
		// Function: PushBack
		//
		// Description: Appends number of elements to the end of array.
		//
		// Parameters:
		//	objects - Pointer to objects list.
		//	count - Number of elements to insert.
		//
		// Returns: True if operation succeeded, false otherwise.
		//
		inline const bool PushBack (const elementType * /*const */objects, /*const */indexType count = 1u)
		{
			if (IsInside (objects))
				return PushBack (*this, static_cast <indexType> (objects - m_elements), count);	// Subarray.

			// Reliability check (Result too long or 'count' out of size).
			InternalAssert ((count == 0u || objects != NULL) && MaximumPossibleSize - m_itemCount >= count);

			// Make room and append new stuff.

			// Reliability check.
			if (!SetSize (m_itemCount + count))
			{
				AddLogEntry (true, LogLevel_Error, false, "Couldn't allocate %.2f kilobytes in array for inserting %s!", (m_itemCount + count) * ElementSize / 1024.0f, count == 1u ? "new element" : "%u elements", count);

				return false;
			}

			for (/* Empty */; count > 0u; ++m_itemCount, ++objects, --count)
				m_elements[m_itemCount] = *objects;	// Fill hole.

			return true;
//			return Insert (m_itemCount, objects, count);
		}

		//
		// Function: PushBack
		//
		// Description: Inserts other array reference into the end of our array.
		//
		// Parameters:
		//	other - Other array elements list.
		//	startIndex - Start index to insert.
		//	count - Number of elements to insert.
		//
		// Returns: True if operation succeeded, false otherwise.
		//
		inline const bool PushBack (const DynamicArray &other, /*const */indexType startIndex, /*const */indexType count)
		{
			// Reliability check ('startIndex' off end or result too long or 'count' out of size).
			InternalAssert (startIndex <= other.m_itemCount && count <= other.m_itemCount - startIndex && MaximumPossibleSize - m_itemCount >= count);

			// Make room and append new stuff.

			// Reliability check.
			if (!SetSize (m_itemCount + count))
			{
				AddLogEntry (true, LogLevel_Error, false, "Couldn't allocate %.2f kilobytes in array for inserting %s!", (m_itemCount + count) * ElementSize / 1024.0f, count == 1u ? "new element" : "%u elements", count);

				return false;
			}

			for (/* Empty */; count > 0u; ++m_itemCount, ++startIndex, --count)
				m_elements[m_itemCount] = other.m_elements[startIndex];	// Fill hole.

			return true;
//			return Insert (m_itemCount, other, startIndex, count);
		}

		//
		// Function: PushBack
		//
		// Description: Inserts other array reference into the end of our array.
		//
		// Parameters:
		//	other - Other array elements list.
		//	startIndex - Start index to insert.
		//
		// Returns: True if operation succeeded, false otherwise.
		//
		inline const bool PushBack (const DynamicArray &other, const indexType startIndex = 0u)
		{
			return PushBack (other, startIndex, other.m_itemCount - startIndex);
//			return Insert (m_itemCount, other, startIndex, other.m_itemCount - startIndex);
		}

		//
		// Function: GetReversed
		//
		// Description: Reverses a subrange of elements in array.
		//
		// Parameters:
		//	startIndex - Start index to get reversed from.
		//	count - Number of elements to get reversed.
		//
		// Returns: Reversed array.
		//
		inline const DynamicArray GetReversed (indexType startIndex, indexType count) const
		{
			// Checks for bounds....
			InternalAssert (startIndex < m_itemCount && count <= m_itemCount - startIndex);

			DynamicArray result;

			if (result.SetSizeWithoutKeepingData (count))
				for (indexType destinationIndex (startIndex + count); count > 0u; result.m_elements[result.m_itemCount++] = m_elements[--destinationIndex], result.m_elements[--count] = m_elements[startIndex++]);

			return result;
		}

		//
		// Function: GetReversed
		//
		// Description: Reverses a subrange of elements in array.
		//
		// Parameters:
		//	startIndex - Start index to get reversed from.
		//
		// Returns: Reversed array.
		//
		inline const DynamicArray GetReversed (const indexType startIndex = 0u) const
		{
			return GetReversed (startIndex, m_itemCount - startIndex);
		}

		//
		// Function: ToReverse
		//
		// Description: Reverses the array.
		//
		// Parameters:
		//	startIndex - Start index to reverse from.
		//	count - Number of elements to reverse.
		//
		// Returns: Reversed array.
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
		// Description: Reverses the array.
		//
		// Parameters:
		//	startIndex - Start index to reverse from.
		//
		// Returns: Reversed array.
		//
		inline const DynamicArray ToReverse (const indexType startIndex = 0u) const
		{
			return ToReverse (startIndex, m_itemCount - startIndex);
		}

		//
		// Function: Reverse
		//
		// Description: Reverses a subrange of elements in array.
		//
		// Parameters:
		//	startIndex - Start index to reverse.
		//	count - Number of elements to reverse.
		//
		// Returns: Reversed array.
		//
		inline DynamicArray &Reverse (indexType startIndex, indexType count)
		{
			// Checks for bounds....
			InternalAssert (startIndex < m_itemCount && count <= m_itemCount - startIndex);

			if (count > 0u)
//				for (count += startIndex; startIndex < --count; ++startIndex)
//					SwapElements (m_elements[startIndex], m_elements[count]);
				MemoryReverse (m_elements + startIndex, count);

			return *this;
		}

		//
		// Function: Reverse
		//
		// Description: Reverses a subrange of elements in array.
		//
		// Parameters:
		//	startIndex - Start index to reverse.
		//
		// Returns: Reversed array.
		//
		inline DynamicArray &Reverse (const indexType startIndex = 0u)
		{
			return Reverse (startIndex, m_itemCount - startIndex);
		}

		void Assign (DynamicArray &&right)
		{	// assign by moving right
			InternalAssert (this != &right);	// Same object? - assert!

			// clear this and steal from right

			// Destruct elements.
//			for (/* Empty */; m_itemCount > 0u; Destruct (m_elements + --m_itemCount));

			delete [] m_elements;

			m_elements = right.m_elements;
			m_itemCount = right.m_itemCount;

			right.m_elements = NULL;
			right.m_itemCount = 0u;
		}

		void Swap (DynamicArray &&right)
		{	// exchange contents with movable right
			InternalAssert (this != &right);	// Same object? - assert!

			// Swap with emptied container....
			Assign (&&right);
		}

		inline void Swap (DynamicArray &right)
		{	// exchange contents with 'right'
			InternalAssert (this != &right);	// Same object? - assert!

			// Same allocator, swap control information....
			SwapElements (m_elements, right.m_elements);
			SwapElements (m_itemCount, right.m_itemCount);
		}

		//
		// Function: GetData
		//
		// Description: Gets the pointer to all elements in array.
		//
		// Returns: Pointer to object list.
		//
		inline elementType *const GetData (void) { return m_elements; }

		//
		// Function: GetData
		//
		// Description: Gets the constant pointer to all element in array.
		//
		// Returns: Constant pointer to object list.
		//
		inline const elementType *const GetData (void) const { return m_elements; }

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
			if (IsEmpty () || m_allocatedSize == m_itemCount)
				return true;

			elementType *const buffer (new elementType[m_itemCount]);

			// Reliability check.
			if (buffer == NULL)
				return false;

			for (indexType index (0u); index < m_itemCount; buffer[index] = m_elements[index], ++index);	// Copy data.
//			for (indexType index (0u); index < m_itemCount; new (buffer + index) elementType (m_elements[index]), ++index);	// Copy data.

			delete [] m_elements;

			m_elements = buffer;
			m_allocatedSize = m_itemCount;

			return true;
		}

		//
		// Function: PopFront
		//
		// Description: Pops first element from array.
		//
		// Returns: Object popped from the begin of array.
		//
		inline const elementType /*&*/PopFront (void)
		{
			// Reliability check.
			InternalAssert (!IsEmpty ());

			const elementType /*&*/element (GetFirstElement ());

			Remove (GetFirstElementIndex (), 1u);

			return element;
		}

		//
		// Function: PopFront
		//
		// Description: Pops number of elements from begin of array.
		//
		// Parameters:
		//	count - Number of element to pop.
		//
		inline void PopFront (const indexType count)
		{
			// Reliability check.
			InternalAssert (!IsEmpty ());

			Remove (GetFirstElementIndex (), count);
		}

		inline void DeleteFront (void)
		{
			// Reliability check.
			InternalAssert (!IsEmpty ());

			Delete (GetFirstElementIndex (), 1u);
		}

		//
		// Function: PopBack
		//
		// Description: Pops last element from array.
		//
		// Returns: Object popped from the end of array.
		//
		inline const elementType /*&*/PopBack (void)
		{
			// Reliability check.
			InternalAssert (!IsEmpty ());

			const elementType /*&*/element (GetLastElement ());

			Remove (GetLastElementIndex (), 1u);

			return element;
		}

		//
		// Function: PopBack
		//
		// Description: Pops number of elements from end of array.
		//
		// Parameters:
		//	count - Number of element to pop.
		//
		inline void PopBack (const indexType count)
		{
			// Reliability check.
			InternalAssert (!IsEmpty ());

			Remove (m_itemCount - count, count);
		}

		inline void DeleteBack (void)
		{
			// Reliability check.
			InternalAssert (!IsEmpty ());

			Delete (GetLastElementIndex (), 1u);
		}

		//
		// Function: Pop
		//
		// Description: Pops first found given element from array.
		//
		// Parameters:
		//	element - Object to remove for.
		//
		// Returns: Object popped from the end of array.
		//
		inline const bool Pop (const elementType &element)
		{
			const indexType elementPosition (Find (element));

			if (elementPosition == BadPosition)
				return false;

			// Else....

			Remove (elementPosition, 1u);

			return true;
		}
		template <typename indexType2> inline void Pop (const DynamicArray <elementType, indexType2> &other)
		{
			// Reliability checks.
			CompileTimeAssert (sizeof (indexType) >= sizeof (indexType2));
			InternalAssert (this != &other);
			InternalAssert (GetElementNumber () <= other.GetElementNumber ());

			for (indexType2 index (0u); index < other.GetElementNumber (); ++index)
				Pop (other[index]);
		}

		//
		// Function: GetRandomElement
		//
		// Description: Gets the random element from the array.
		//
		// Returns: Random element reference.
		//
		inline elementType &GetRandomElement (void)
		{
			// Reliability check.
			InternalAssert (!IsEmpty ());

			return m_elements[g_randomNumberGenerator.GetValueBetween <indexType> (0u, m_itemCount - 1u)];
		}

		//
		// Function: GetRandomElement
		//
		// Description: Gets the random constant element from the array.
		//
		// Returns: Random constant element reference.
		//
		inline const elementType &GetRandomElement (void) const
		{
			// Reliability check.
			InternalAssert (!IsEmpty ());

			return m_elements[g_randomNumberGenerator.GetValueBetween <indexType> (0u, m_itemCount - 1u)];
		}

		//
		// Function: GetRandomElementIndex
		//
		// Description: Gets the random element index from the array.
		//
		// Returns: Random element index reference.
		//
		inline const indexType GetRandomElementIndex (void) const
		{
			// Reliability check.
			InternalAssert (!IsEmpty ());

			return g_randomNumberGenerator.GetValueBetween <indexType> (0u, m_itemCount - 1u);
		}

		//
		// Function: GetFirstElement
		//
		// Description: Gets the first (bottom) element from the array.
		//
		// Returns: First (bottom) element reference.
		//
		inline elementType &GetFirstElement (void)
		{
			// Reliability check.
			InternalAssert (!IsEmpty ());

			return m_elements[0u];
		}

		//
		// Function: GetFirstElement
		//
		// Description: Gets the first (bottom) constant element from the array.
		//
		// Returns: First (bottom) constant element reference.
		//
		inline const elementType &GetFirstElement (void) const
		{
			// Reliability check.
			InternalAssert (!IsEmpty ());

			return m_elements[0u];
		}

		//
		// Function: GetFirstElementIndex
		//
		// Description: Gets the first (bottom) element index.
		//
		// Returns: First (bottom) element index.
		//
		inline const indexType GetFirstElementIndex (void) const
		{
			// Reliability check.
			InternalAssert (!IsEmpty ());

			return 0u;
		}

		//
		// Function: GetLastElement
		//
		// Description: Gets the last (top) element from the array.
		//
		// Returns: Last (top) element reference.
		//
		inline elementType &GetLastElement (void)
		{
			// Reliability check.
			InternalAssert (!IsEmpty ());

			return m_elements[m_itemCount - 1u];
		}

		//
		// Function: GetLastElement
		//
		// Description: Gets the last (top) constant element from the array.
		//
		// Returns: Last (top) constant element reference.
		//
		inline const elementType &GetLastElement (void) const
		{
			// Reliability check.
			InternalAssert (!IsEmpty ());

			return m_elements[m_itemCount - 1u];
		}

		//
		// Function: GetLastElementIndex
		//
		// Description: Gets the last (top) element index.
		//
		// Returns: Last (top) element index.
		//
		inline const indexType GetLastElementIndex (void) const
		{
			// Reliability check.
			InternalAssert (!IsEmpty ());

			return m_itemCount - 1u;
		}

		//
		// Function: Find
		//
		// Description: Finds an element (element needs operator == defined).
		//
		// Parameters:
		//	element - Object to search for.
		//
		// Returns: Index of found object.
		//
		inline const indexType Find (const elementType &element, const indexType startIndex = 0u) const
		{
			return Find (element, startIndex, m_itemCount - startIndex);
		}
		inline const indexType Find (const elementType &element, indexType startIndex, indexType count) const
		{
			// Reliability check.
			InternalAssert (startIndex <= m_itemCount && count <= m_itemCount - startIndex);

			for (count += startIndex; startIndex < count; ++startIndex)
				if (m_elements[startIndex] == element)
					return startIndex;

			return BadPosition;
		}

		inline const indexType BinaryFind (const elementType &element, const BinaryFindComparatorFunction_t comparatorFunction, const indexType startIndex = 0u) const
		{
			// Reliability check.
			InternalAssert (!IsEmpty ());

			return BinaryFind (element, comparatorFunction, startIndex, m_itemCount - startIndex);
		}
		inline const indexType BinaryFind (const elementType &element, const BinaryFindComparatorFunction_t comparatorFunction, indexType startIndex, int count) const
		{
			// Reliability check.
			InternalAssert (startIndex < m_itemCount && count > 0 && count <= m_itemCount - startIndex);

			indexType middle;

			count += startIndex;

			do
			{
				// To start, find the subscript of the middle position.
				middle = static_cast <indexType> ((startIndex + count) / 2u);

				const BinaryFindCompareResult_t result (comparatorFunction (m_elements[middle], element));

				if (result == Result_Equal)
					return middle;	// found it. Return position

				if (result == Result_Less)	// If the number is < key, decrease position by one.
					startIndex = middle + 1u;	// repeat search in top half.
				else	// (Else 'result == Result_More') If the key is < number, increase position by one.
					count = middle - 1;	// repeat search in bottom half.
			} while (startIndex <= count);

			// failed to find key
			return BadPosition;
		}

		//
		// Function: IsMember
		//
		// Description: Finds an element (element needs operator == defined).
		//
		// Parameters:
		//	element - Object to search for.
		//
		// Returns: True if element is founded, false otherwise.
		//
		inline const bool IsMember (const elementType &element) const
		{
			return Find (element) != BadPosition;
		}

		inline const bool SetFromPointer (elementType *const bufferPointer, const indexType count)
		{
			// Reliability check.
			InternalAssert (IsEmpty ());

			m_elements = bufferPointer;
			m_allocatedSize = m_itemCount = count;
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
		inline const bool Assign (const elementType &input, const indexType count = 1u)
		{	// assign count * input.
			// Reliability check (Result too long).
			InternalAssert (count < MaximumPossibleSize);

			// Make room and assign new stuff.

			// Reliability check.
			if (!SetSizeWithoutKeepingData (count))
			{
				AddLogEntry (true, LogLevel_Error, false, "Couldn't allocate %.2f kilobytes in array for assigning %s!", count * ElementSize / 1024.0f, count == 1u ? "new element" : "%u elements", count);

				return false;
			}

			for (m_itemCount = 0u; m_itemCount < count; m_elements[m_itemCount] = input, ++m_itemCount);	// Fill hole.
//			for (m_itemCount = 0u; m_itemCount < count; new (m_elements + m_itemCount) elementType (input), ++m_itemCount);	// Fill hole.

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
		inline const bool Assign (const elementType *const bufferPointer, const indexType count)
		{
			if (IsInside (bufferPointer))
				return Assign (*this, static_cast <indexType> (bufferPointer - m_elements), count);	// Subarray.

			// Reliability check.
			InternalAssert (count == 0u || bufferPointer != NULL);

			// Make room and assign new stuff.

			// Reliability check.
			if (!SetSizeWithoutKeepingData (count))
			{
				AddLogEntry (true, LogLevel_Error, false, "Couldn't allocate %.2f kilobytes in array for assigning %s!", count * ElementSize / 1024.0f, count == 1u ? "new element" : "%u elements", count);

				return false;
			}

			for (m_itemCount = 0u; m_itemCount < count; m_elements[m_itemCount] = bufferPointer[m_itemCount], ++m_itemCount);
//			for (m_itemCount = 0u; m_itemCount < count; new (m_elements + m_itemCount) elementType (bufferPointer[m_itemCount]), ++m_itemCount);

			return true;
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
		inline const bool Assign (const DynamicArray &inputArray, const indexType startIndex, const indexType count)
		{
			// Reliability check ('startIndex' off end).
			InternalAssert (startIndex <= inputArray.m_itemCount && count <= inputArray.m_itemCount - startIndex);

			if (this == &inputArray)
			{
				// Subarray.

				Remove (0u, startIndex);
				Remove (startIndex + count);

				return true;
			}
			else if (SetSizeWithoutKeepingData (count))	// Make room and assign new stuff.
			{
				for (m_itemCount = 0u; m_itemCount < count; m_elements[m_itemCount] = inputArray.m_elements[startIndex + m_itemCount], ++m_itemCount);	// Copy data.
//				for (m_itemCount = 0u; m_itemCount < count; new (m_elements + m_itemCount) elementType (inputArray.m_elements[startIndex + m_itemCount]), ++m_itemCount);	// Copy data.

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
		inline const bool Assign (const DynamicArray &inputArray, const indexType startIndex = 0u)
		{
			return Assign (inputArray, startIndex, inputArray.m_itemCount - startIndex);
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
			InternalAssert (sourceIndex < m_itemCount && count <= m_itemCount - sourceIndex);

			// Destruct elements.
//			for (indexType index (0u); index < count; ++index)
//				Destruct (m_elements + (sourceIndex + index));

			// Move elements down.
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
			Remove (sourceIndex, m_itemCount - sourceIndex);
		}

		// Delete the element.
		inline void Delete (const indexType sourceIndex, const indexType count)
		{
			// Checks for bounds....
			InternalAssert (sourceIndex < m_itemCount && count <= m_itemCount - sourceIndex);

			const indexType steps (sourceIndex + count);

			// Free used memory....
			for (indexType index (sourceIndex); index < steps; ++index)
				delete m_elements[index];

			Remove (sourceIndex, count);
		}
		inline void Delete (const indexType sourceIndex = 0u)
		{
			Delete (sourceIndex, m_itemCount - sourceIndex);
		}

		inline const bool FindAndDeleteElement (const elementType &element)
		{
			const indexType elementPosition (Find (element));

			if (elementPosition == BadPosition)
				return false;

			// Else....

			Delete (elementPosition, 1u);

			return true;
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
			// Reliability check.
			InternalAssert (m_itemCount >= 2u);

			// Sort remaining arguments using quick sort algorithm:
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
			InternalAssert (sourceIndex < m_itemCount && count <= m_itemCount - sourceIndex && count >= 2u);

			// Sort remaining arguments using Quicksort algorithm:
			qsort (m_elements + sourceIndex, count, ElementSize, reinterpret_cast <int (*) (const void *const, const void *const)> (comparator));
		}

	//
	// Group: Operators.
	//
	public:
		//
		// Function: operator =
		//
		// Description: Assigns the element to existing buffer.
		//
		// Parameters:
		//	input - Element to assign.
		//
		// Returns: Assigned element.
		//
		inline DynamicArray &operator =  (const elementType &input)
		{
			Assign (input);

			return *this;
		}

		//
		// Function: operator =
		//
		// Description: Reassignes current array with specified one.
		//
		// Parameters:
		//	other - Other array that should be assigned.
		//
		// Returns: Reassigned array.
		//
		inline DynamicArray &operator =  (const DynamicArray &other)
		{
			Assign (other);

			return *this;
		}

		inline DynamicArray &operator -= (const elementType &object)
		{
			Pop (object);

			return *this;
		}
		template <typename indexType2> inline DynamicArray &operator -= (const DynamicArray <elementType, indexType2> &other)
		{
			Pop (other);

			return *this;
		}

		inline DynamicArray &operator += (const elementType &object)
		{
			PushBack (object);

			return *this;
		}

		inline DynamicArray &operator += (const DynamicArray &other)
		{
			PushBack (other);

			return *this;
		}

		// Following five operators are equal's DynamicArray::GetData() function.
		inline operator       elementType *const    (void)       { return m_elements; }
		inline operator       elementType *const    (void) const { return m_elements; }	// Needed too :{O (Half-Life engine so stupid....)
		inline operator const elementType *const    (void) const { return m_elements; }

		inline       elementType *const operator () (void)       { return m_elements; }
		inline const elementType *const operator () (void) const { return m_elements; }

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
		inline       elementType &operator [] (const indexType index)
		{
			// Reliability check.
			InternalAssert (index < m_itemCount);

			return m_elements[index];
		}
		inline const elementType &operator [] (const indexType index) const
		{
			// Reliability check.
			InternalAssert (index < m_itemCount);

			return m_elements[index];
		}
/*
		// Same as [], but resize the string if 'index' doesn't exist yet.
		inline       elementType &operator () (const indexType index)
		{
			if (index >= m_allocatedSize)
				SetSize (index + 1u);

			if (index >= m_itemCount)
				m_itemCount = index + 1u;

			return m_elements[index];
		}
*/
		// Equality operation.
		inline const bool operator == (const DynamicArray &right) const
		{
			// If arrays are various objects we carry out comparison.
			if (this != &right)
			{
				// If number of array elements not equally,
				if (m_itemCount != right.m_itemCount)
					return false;	// arrays are not equal.

				// Else we carry out step by step comparison.
				for (indexType index (0u); index < m_itemCount; ++index)
					if (m_elements[index] != right.m_elements[index])
						return false;	// arrays are not equal.
			}

			// Otherwise we return true (since any array is equal to itself).
			return true;
		}

		// Inequality operation.
		inline const bool operator != (const DynamicArray &right) const
		{
			return !(*this == right);
		}

		inline const bool operator < (const DynamicArray &right) const
		{
			if (this == &right)
				return false;	// arrays are not equal.

			const indexType length (Math::GetMinimumValueBetween (m_itemCount, right.m_itemCount));

			for (indexType index (0u); index < length; ++index)
			{
				if (m_elements[index] < right.m_elements[index])
					return true;	// arrays are equal.

				if (m_elements[index] > right.m_elements[index])
					return false;	// arrays are not equal.
			}

			// They are the same, judge by length.
			return m_itemCount < right.m_itemCount;
		}
		inline const bool operator > (const DynamicArray &right) const
		{
			return right < *this;
		}
		inline const bool operator >= (const DynamicArray &right) const
		{
			return !(*this < right);
		}
		inline const bool operator <= (const DynamicArray &right) const
		{
			return !(right < *this);
		}
};

template <typename elementType, typename indexType, const indexType elementsNumber> class StaticArray
{
	//
	// Group: Protected members.
	//
	protected:
		elementType m_elements[elementsNumber];	// The actual array of data. (fixed-size array of elements of type 'elementType')
		indexType   m_itemCount;				// ¹ of elements (upperBound - 1).

	//
	// Group: Special points.
	//
	public:
		enum
		{
			ElementSize = sizeof (elementType),	// One array element size.
			BadPosition = static_cast <indexType> (-1),	// Generic bad/missing length/position.
			MaximumPossibleSize = elementsNumber	// Maximum possible array size.
		};

	//
	// Group: Types definitions.
	//
	public:
		typedef elementType ElementType_t;
		typedef indexType   IndexType_t;

		enum BinaryFindCompareResult_t
		{
			Result_Equal,

			Result_Less,
			Result_More
		};
		typedef const BinaryFindCompareResult_t (*const BinaryFindComparatorFunction_t) (const elementType &entry1, const elementType &entry2);
		typedef const int (*const SortComparatorFunction_t) (const elementType *const entry1, const elementType *const entry2);

	//
	// Group: (Con/De)structors.
	//
	public:
		//
		// Function: StaticArray
		//
		// Description: Default array constructor.
		//
		inline StaticArray (void) : m_itemCount (0u) { /* VOID */ }

		//
		// Function: StaticArray
		//
		// Description: Initializes a array buffer with given by the input element in amount of 'count'.
		//
		// Parameters:
		//	input - Element to assign.
		//	count - Number of elements to use for the content (i.e., it's length).
		//
		inline StaticArray (const elementType &input, const indexType count = 1u) : m_itemCount (0u)
		{
			Assign (input, count);
		}

		//
		// Function: StaticArray
		//
		// Description: Initializes a array buffer with given by the buffer.
		//
		// Parameters:
		//	bufferPointer - StaticArray buffer to assign.
		//	count - Number of elements to use for the content (i.e., it's length).
		//
		inline StaticArray (const elementType *const bufferPointer, const indexType count) : m_itemCount (0u)
		{
			Assign (bufferPointer, count);
		}

		//
		// Function: StaticArray
		//
		// Description: StaticArray copying constructor.
		//
		// Parameters:
		//	other - Other array that should be assigned to this one.
		//	startIndex - Starting position of the subarray of the input array that forms the new content.
		//	count - Number of elements to use for the content (i.e., it's length).
		//
		inline StaticArray (const StaticArray &other, const indexType startIndex, const indexType count) : m_itemCount (0u)
		{
			Assign (other, startIndex, count);
		}

		//
		// Function: StaticArray
		//
		// Description: StaticArray copying constructor.
		//
		// Parameters:
		//	other - Other array that should be assigned to this one.
		//	startIndex - Starting position of the subarray of the input array that forms the new content.
		//
		inline StaticArray (const StaticArray &other, const indexType startIndex = 0u) : m_itemCount (0u)
		{
			Assign (other, startIndex);
		}

		//
		// Function: ~StaticArray
		//
		// Description: Default array destructor.
		//
		virtual inline ~StaticArray (void)
		{
			// Destruct elements.
//			for (/* Empty */; m_itemCount > 0u; Destruct (m_elements + --m_itemCount));
		}

	//
	// Group: Private functions.
	//
	private:
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
			InternalAssert (destinationIndex <= elementsNumber && sourceIndex <= m_itemCount);

			if (destinationIndex != sourceIndex)
				UninitializedMemoryNonScalarMove (m_elements + destinationIndex, m_elements + sourceIndex, m_itemCount - sourceIndex);
		}

	//
	// Group: Functions.
	//
	public:
		inline const indexType GetElementIndex (const elementType &element) const { return GetElementIndex (&element); }
		inline const indexType GetElementIndex (const elementType *const element) const
		{
			// Reliability check.
			InternalAssert (IsInside (element));

			return static_cast <indexType> ((reinterpret_cast <unsigned int> (element) - reinterpret_cast <unsigned int> (m_elements)) / ElementSize);
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
		inline const bool IsInside (const elementType *const bufferPointer) const
		{
			return bufferPointer >= m_elements && bufferPointer < m_elements + m_itemCount;	// Don't ask.
		}

		//
		// Function: MakeEmpty
		//
		// Description: Empties the array without memory deallocation.
		//
		inline void MakeEmpty (void)
		{
			RemoveAll ();	// Shrink to nothing.
		}

		//
		// Function: RemoveAll
		//
		// Description: Destroys array object, and all elements.
		//
		inline void RemoveAll (void)
		{
			// Destruct elements.
//			for (/* Empty */; m_itemCount > 0u; Destruct (m_elements + --m_itemCount));

			m_itemCount = 0u;
		}

		// Destruct all the elements.
		inline void DestructAll (void)
		{
			for (indexType index (0u); index < m_itemCount; ++index)
				Destruct (m_elements + index);
		}

		// Delete all the elements.
		inline void DeleteAll (void)
		{
			// Free all used memory....
			for (indexType index (0u); index < m_itemCount; ++index)
				delete m_elements[index];
		}

		// Delete all the elements then call Purge.
		inline void DeleteAndPurgeAll (void)
		{
			// Free all used memory....
			DeleteAll ();
			RemoveAll ();
		}

		//
		// Function: GetMemoryUsage
		//
		// Description: Gets the using by this dynamic array memory.
		//
		// Returns: How much memory this dynamic array is using.
		//
		static inline const unsigned int GetMemoryUsage (void)
		{
			return sizeof (StaticArray) + ElementSize * elementsNumber;
		}

		//
		// Function: GetAllocatedSize
		//
		// Description: Gets allocated size of array.
		//
		// Returns: Number of allocated items.
		//
		static inline const indexType GetAllocatedSize (void) { return elementsNumber; }

		//
		// Function: GetElementNumber
		//
		// Description: Gets real number currently in array.
		//
		// Returns: Number of elements.
		//
		inline const indexType GetElementNumber (void) const { return m_itemCount; }

		inline void SetElementNumber (const indexType newElementNumber)
		{
			// Reliability check.
			InternalAssert (newElementNumber <= GetAllocatedSize ());

			m_itemCount = newElementNumber;
		}
		inline void IncreaseElementNumber (const indexType count = 1u)
		{
			// Reliability check.
			InternalAssert (m_itemCount + count <= GetAllocatedSize ());

			m_itemCount += count;
		}
		inline void DecreaseElementNumber (const indexType count = 1u)
		{
			// Reliability check.
			InternalAssert (m_itemCount >= count);

			m_itemCount -= count;
		}

		//
		// Function: Insert
		//
		// Description: Inserts number of element at specified index.
		//
		// Parameters:
		//	startIndex - Index where element should be inserted.
		//	object - Element to insert.
		//	count - Number of element to insert.
		//
		// Returns: True if operation succeeded, false otherwise.
		//
		inline const bool Insert (const indexType startIndex, const elementType &object, indexType count = 1u)
		{
			// Reliability check ('startIndex' off end or result too long).
			InternalAssert (startIndex <= m_itemCount && MaximumPossibleSize - m_itemCount >= count);

//			if (startIndex < m_itemCount)
				MoveItems (startIndex + count, startIndex);	// Empty out hole.

			// Update current elements number....
			m_itemCount += count;

			for (/* Empty */; count > 0u; m_elements[startIndex + --count] = object);	// Copy data.

			return true;
		}

		//
		// Function: Insert
		//
		// Description: Inserts number of elements at specified index.
		//
		// Parameters:
		//	startIndex - Index where elements should be inserted.
		//	bufferPointer - StaticArray buffer to insert.
		//	count - Number of elements to insert.
		//
		// Returns: True if operation succeeded, false otherwise.
		//
		inline const bool Insert (const indexType startIndex, const elementType *const bufferPointer, indexType count)
		{
			if (IsInside (bufferPointer))
				return Insert (startIndex, *this, static_cast <indexType> (bufferPointer - m_elements), count);	// Subarray.

			// Reliability check ('startIndex' off end or result too long).
			InternalAssert (startIndex <= m_itemCount && (count == 0u || bufferPointer != NULL) && MaximumPossibleSize - m_itemCount >= count);

//			if (startIndex < m_itemCount)
				MoveItems (startIndex + count, startIndex);	// Empty out hole.

			// Update current elements number....
			m_itemCount += count;

			for (/* Empty */; count > 0u; m_elements[startIndex + --count] = bufferPointer[count]);	// Copy data.
//			for (/* Empty */; count > 0u; new (m_elements + (startIndex + --count)) elementType (bufferPointer[count]));	// Copy data.

			return true;
		}

		//
		// Function: Insert
		//
		// Description: Inserts number of elements at specified index.
		//
		// Parameters:
		//	startIndex - Index where elements should be inserted.
		//	array - StaticArray buffer to insert.
		//	count - Number of elements to insert.
		//
		// Returns: True if operation succeeded, false otherwise.
		//
		inline const bool Insert (const indexType startIndex, const StaticArray &array, const indexType startIndex2, indexType count)
		{	// insert array [startIndex2, startIndex2 + count) at startIndex.
			// Reliability check ('startIndex' or 'startIndex2' off end or result too long).
			InternalAssert (startIndex <= m_itemCount && startIndex2 <= array.m_itemCount && count <= array.m_itemCount - startIndex2 && MaximumPossibleSize - m_itemCount >= count);

			// Insert new stuff.

//			if (startIndex < m_itemCount)
				MoveItems (startIndex + count, startIndex);	// Empty out hole.

			if (this == &array)
			{
				MoveItems (startIndex, startIndex < startIndex2 ? startIndex2 + count : startIndex2);	// Subarray.

				// Update current elements number....
				m_itemCount += count;
			}
			else
			{
				// Update current elements number....
				m_itemCount += count;

				for (/* Empty */; count > 0u; m_elements[startIndex + --count] = array.m_elements[startIndex2 + count]);	// Fill hole.
//				for (/* Empty */; count > 0u; new (m_elements + (startIndex + --count)) elementType (array.m_elements[startIndex2 + count]));	// Fill hole.
			}

			return true;
		}

		//
		// Function: PushFrontDefaultElement
		//
		// Description: Appends new element to the end of array.
		//
		// Parameters:
		//	count - Number of element to insert.
		//
		// Returns: True if operation succeeded, false otherwise.
		//
		inline const bool PushFrontDefaultElement (const indexType count = 1u)
		{
			return PushFront (elementType (), count);
//			return Insert (0u, elementType (), count);
		}

		//
		// Function: PushFront
		//
		// Description: Appends new element to the begin of array.
		//
		// Parameters:
		//	object - Object to append.
		//	count - Number of element to insert.
		//
		// Returns: True if operation succeeded, false otherwise.
		//
		inline const bool PushFront (const elementType &object, /*const */indexType count = 1u)
		{
			// Reliability check (Result too long).
			InternalAssert (MaximumPossibleSize - m_itemCount >= count);

			// Append new stuff using assign.

			MoveItems (count, 0u);	// Empty out hole.

			// Update the buffer length.
			m_itemCount += count;

			for (/* Empty */; count > 0u; m_elements[--count] = object);	// Fill hole.

			return true;
//			return Insert (0u, object, count);
		}

		//
		// Function: PushFront
		//
		// Description: Appends number of elements to the begin of array.
		//
		// Parameters:
		//	objects - Pointer to objects list.
		//	count - Number of elements to insert.
		//
		// Returns: True if operation succeeded, false otherwise.
		//
		inline const bool PushFront (const elementType *const objects, /*const */indexType count)
		{
			if (IsInside (objects))
				return PushBack (*this, static_cast <indexType> (objects - m_elements), count);	// Subarray.

			// Reliability check (Result too long or 'count' out of size).
			InternalAssert ((count == 0u || objects != NULL) && MaximumPossibleSize - m_itemCount >= count);

			// Append new stuff.

			MoveItems (count, 0u);	// Empty out hole.

			// Update the buffer length.
			m_itemCount += count;

			for (/* Empty */; count > 0u; m_elements[--count] = objects[count]);	// Fill hole.

			return true;
//			return Insert (0u, objects, count);
		}

		//
		// Function: PushFront
		//
		// Description: Inserts other array reference into the begin of our array.
		//
		// Parameters:
		//	other - Other array elements list.
		//	startIndex - Start index to insert.
		//	count - Number of elements to insert.
		//
		// Returns: True if operation succeeded, false otherwise.
		//
		inline const bool PushFront (const StaticArray &other, const indexType startIndex, /*const */indexType count)
		{
			// Reliability check ('startIndex' off end or result too long or 'count' out of size).
			InternalAssert (startIndex <= other.m_itemCount && count <= other.m_itemCount - startIndex && MaximumPossibleSize - m_itemCount >= count);

			// Append new stuff.

			MoveItems (count, 0u);	// Empty out hole.

			// Update the buffer length.
			m_itemCount += count;

			for (/* Empty */; count > 0u; m_elements[--count] = other.m_elements[startIndex + count]);	// Fill hole.

			return true;
//			return Insert (0u, other, startIndex, count);
		}

		//
		// Function: PushFront
		//
		// Description: Inserts other array reference into the begin of our array.
		//
		// Parameters:
		//	other - Other array elements list.
		//	startIndex - Start index to insert.
		//
		// Returns: True if operation succeeded, false otherwise.
		//
		inline const bool PushFront (const StaticArray &other, const indexType startIndex = 0u)
		{
			return PushFront (other, startIndex, other.m_itemCount - startIndex);
//			return Insert (0u, other, startIndex, other.m_itemCount - startIndex);
		}

		//
		// Function: PushBackDefaultElement
		//
		// Description: Appends new element to the end of array.
		//
		// Parameters:
		//	count - Number of element to insert.
		//
		// Returns: True if operation succeeded, false otherwise.
		//
		inline const bool PushBackDefaultElement (const indexType count = 1u)
		{
			return PushBack (elementType (), count);
//			return Insert (m_itemCount, elementType (), count);
		}

		//
		// Function: PushBack
		//
		// Description: Appends new element to the end of array.
		//
		// Parameters:
		//	object - Object to append.
		//	count - Number of element to insert.
		//
		// Returns: True if operation succeeded, false otherwise.
		//
		inline const bool PushBack (const elementType &object, /*const */indexType count = 1u)
		{
			// Reliability check (Result too long).
			InternalAssert (MaximumPossibleSize - m_itemCount >= count);

			// Append new stuff using assign.

			for (/* Empty */; count > 0u; ++m_itemCount, --count)
				m_elements[m_itemCount] = object;	// Fill hole.

			return true;
//			return Insert (m_itemCount, object, count);
		}

		//
		// Function: PushBack
		//
		// Description: Appends number of elements to the end of array.
		//
		// Parameters:
		//	objects - Pointer to objects list.
		//	count - Number of elements to insert.
		//
		// Returns: True if operation succeeded, false otherwise.
		//
		inline const bool PushBack (const elementType * /*const */objects, /*const */indexType count = 1u)
		{
			if (IsInside (objects))
				return PushBack (*this, static_cast <indexType> (objects - m_elements), count);	// Subarray.

			// Reliability check (Result too long or 'count' out of size).
			InternalAssert ((count == 0u || objects != NULL) && MaximumPossibleSize - m_itemCount >= count);

			// Append new stuff.

			for (/* Empty */; count > 0u; ++m_itemCount, ++objects, --count)
				m_elements[m_itemCount] = *objects;	// Fill hole.

			return true;
//			return Insert (m_itemCount, objects, count);
		}

		//
		// Function: PushBack
		//
		// Description: Inserts other array reference into the end of our array.
		//
		// Parameters:
		//	other - Other array elements list.
		//	startIndex - Start index to insert.
		//	count - Number of elements to insert.
		//
		// Returns: True if operation succeeded, false otherwise.
		//
		inline const bool PushBack (const StaticArray &other, /*const */indexType startIndex, /*const */indexType count)
		{
			// Reliability check ('startIndex' off end or result too long or 'count' out of size).
			InternalAssert (startIndex <= other.m_itemCount && count <= other.m_itemCount - startIndex && MaximumPossibleSize - m_itemCount >= count);

			// Append new stuff.

			for (/* Empty */; count > 0u; ++m_itemCount, ++startIndex, --count)
				m_elements[m_itemCount] = other.m_elements[startIndex];	// Fill hole.

			return true;
//			return Insert (m_itemCount, other, startIndex, count);
		}

		//
		// Function: PushBack
		//
		// Description: Inserts other array reference into the end of our array.
		//
		// Parameters:
		//	other - Other array elements list.
		//	startIndex - Start index to insert.
		//
		// Returns: True if operation succeeded, false otherwise.
		//
		inline const bool PushBack (const StaticArray &other, const indexType startIndex = 0u)
		{
			return PushBack (other, startIndex, other.m_itemCount - startIndex);
//			return Insert (m_itemCount, other, startIndex, other.m_itemCount - startIndex);
		}

		//
		// Function: GetReversed
		//
		// Description: Reverses a subrange of elements in array.
		//
		// Parameters:
		//	startIndex - Start index to get reversed from.
		//	count - Number of elements to get reversed.
		//
		// Returns: Reversed array.
		//
		inline const StaticArray GetReversed (indexType startIndex, indexType count) const
		{
			// Checks for bounds....
			InternalAssert (startIndex < m_itemCount && count <= m_itemCount - startIndex);

			StaticArray result;

			for (indexType destinationIndex (startIndex + count); count > 0u; result.m_elements[result.m_itemCount++] = m_elements[--destinationIndex], result.m_elements[--count] = m_elements[startIndex++]);

			return result;
		}

		//
		// Function: GetReversed
		//
		// Description: Reverses a subrange of elements in array.
		//
		// Parameters:
		//	startIndex - Start index to get reversed from.
		//
		// Returns: Reversed array.
		//
		inline const StaticArray GetReversed (const indexType startIndex = 0u) const
		{
			return GetReversed (startIndex, m_itemCount - startIndex);
		}

		//
		// Function: ToReverse
		//
		// Description: Reverses the array.
		//
		// Parameters:
		//	startIndex - Start index to reverse from.
		//	count - Number of elements to reverse.
		//
		// Returns: Reversed array.
		//
		inline const StaticArray ToReverse (indexType startIndex, indexType count) const
		{
			// Checks for bounds....
			InternalAssert (startIndex < m_itemCount && count <= m_itemCount - startIndex);

			StaticArray result (*this);

			if (!result.IsEmpty () && count > 0u)
				for (count += startIndex; startIndex < --count; result.m_elements[startIndex] = m_elements[count], result.m_elements[count] = m_elements[startIndex++]);

			return result;
		}

		//
		// Function: ToReverse
		//
		// Description: Reverses the array.
		//
		// Parameters:
		//	startIndex - Start index to reverse from.
		//
		// Returns: Reversed array.
		//
		inline const StaticArray ToReverse (const indexType startIndex = 0u) const
		{
			return ToReverse (startIndex, m_itemCount - startIndex);
		}

		//
		// Function: Reverse
		//
		// Description: Reverses a subrange of elements in array.
		//
		// Parameters:
		//	startIndex - Start index to reverse.
		//	count - Number of elements to reverse.
		//
		// Returns: Reversed array.
		//
		inline StaticArray &Reverse (indexType startIndex, indexType count)
		{
			// Checks for bounds....
			InternalAssert (startIndex < m_itemCount && count <= m_itemCount - startIndex);

			if (count > 0u)
//				for (count += startIndex; startIndex < --count; ++startIndex)
//					SwapElements (m_elements[startIndex], m_elements[count]);
				MemoryReverse (m_elements + startIndex, count);

			return *this;
		}

		//
		// Function: Reverse
		//
		// Description: Reverses a subrange of elements in array.
		//
		// Parameters:
		//	startIndex - Start index to reverse.
		//
		// Returns: Reversed array.
		//
		inline StaticArray &Reverse (const indexType startIndex = 0u)
		{
			return Reverse (startIndex, m_itemCount - startIndex);
		}

		inline void Swap (StaticArray &right)
		{	// exchange contents with 'right'
			InternalAssert (this != &right);	// Same object? - assert!

			// Same allocator, swap control information....
			SwapBuffers (m_elements, right.m_elements, m_itemCount);
			SwapElements (m_itemCount, right.m_itemCount);
		}

		//
		// Function: GetData
		//
		// Description: Gets the pointer to all elements in array.
		//
		// Returns: Pointer to object list.
		//
		inline elementType *const GetData (void) { return m_elements; }

		//
		// Function: GetData
		//
		// Description: Gets the constant pointer to all element in array.
		//
		// Returns: Constant pointer to object list.
		//
		inline const elementType *const GetData (void) const { return m_elements; }

		//
		// Function: IsEmpty
		//
		// Description: Checks whether array is empty.
		//
		// Returns: True if array is empty, false otherwise.
		//
		inline const bool IsEmpty (void) const { return m_itemCount == 0u; }

		//
		// Function: PopFront
		//
		// Description: Pops first element from array.
		//
		// Returns: Object popped from the begin of array.
		//
		inline const elementType /*&*/PopFront (void)
		{
			// Reliability check.
			InternalAssert (!IsEmpty ());

			const elementType /*&*/element (GetFirstElement ());

			Remove (GetFirstElementIndex (), 1u);

			return element;
		}

		//
		// Function: PopFront
		//
		// Description: Pops number of elements from begin of array.
		//
		// Parameters:
		//	count - Number of element to pop.
		//
		inline void PopFront (const indexType count)
		{
			// Reliability check.
			InternalAssert (!IsEmpty ());

			Remove (GetFirstElementIndex (), count);
		}

		inline void DeleteFront (void)
		{
			// Reliability check.
			InternalAssert (!IsEmpty ());

			Delete (GetFirstElementIndex (), 1u);
		}

		//
		// Function: PopBack
		//
		// Description: Pops last element from array.
		//
		// Returns: Object popped from the end of array.
		//
		inline const elementType /*&*/PopBack (void)
		{
			// Reliability check.
			InternalAssert (!IsEmpty ());

			const elementType /*&*/element (GetLastElement ());

			Remove (GetLastElementIndex (), 1u);

			return element;
		}

		//
		// Function: PopBack
		//
		// Description: Pops number of elements from end of array.
		//
		// Parameters:
		//	count - Number of element to pop.
		//
		inline void PopBack (const indexType count)
		{
			// Reliability check.
			InternalAssert (!IsEmpty ());

			Remove (m_itemCount - count, count);
		}

		inline void DeleteBack (void)
		{
			// Reliability check.
			InternalAssert (!IsEmpty ());

			Delete (GetLastElementIndex (), 1u);
		}

		//
		// Function: Pop
		//
		// Description: Pops first found given element from array.
		//
		// Parameters:
		//	element - Object to remove for.
		//
		// Returns: Object popped from the end of array.
		//
		inline const bool Pop (const elementType &element)
		{
			const indexType elementPosition (Find (element));

			if (elementPosition == BadPosition)
				return false;

			// Else....

			Remove (elementPosition, 1u);

			return true;
		}
		template <typename indexType2, indexType2 elementsNumber2> inline void Pop (const StaticArray <elementType, indexType2, elementsNumber2> &other)
		{
			// Reliability checks.
			CompileTimeAssert (elementsNumber >= elementsNumber2);
			InternalAssert (this != &other);
			InternalAssert (GetElementNumber () <= other.GetElementNumber ());

			for (indexType2 index (0u); index < other.GetElementNumber (); ++index)
				Pop (other[index]);
		}

		//
		// Function: GetRandomElement
		//
		// Description: Gets the random element from the array.
		//
		// Returns: Random element reference.
		//
		inline elementType &GetRandomElement (void)
		{
			// Reliability check.
			InternalAssert (!IsEmpty ());

			return m_elements[g_randomNumberGenerator.GetValueBetween <indexType> (0u, m_itemCount - 1u)];
		}

		//
		// Function: GetRandomElement
		//
		// Description: Gets the random constant element from the array.
		//
		// Returns: Random constant element reference.
		//
		inline const elementType &GetRandomElement (void) const
		{
			// Reliability check.
			InternalAssert (!IsEmpty ());

			return m_elements[g_randomNumberGenerator.GetValueBetween <indexType> (0u, m_itemCount - 1u)];
		}

		//
		// Function: GetRandomElementIndex
		//
		// Description: Gets the random element index from the array.
		//
		// Returns: Random element index reference.
		//
		inline const indexType GetRandomElementIndex (void) const
		{
			// Reliability check.
			InternalAssert (!IsEmpty ());

			return g_randomNumberGenerator.GetValueBetween <indexType> (0u, m_itemCount - 1u);
		}

		//
		// Function: GetFirstElement
		//
		// Description: Gets the first (bottom) element from the array.
		//
		// Returns: First (bottom) element reference.
		//
		inline elementType &GetFirstElement (void)
		{
			// Reliability check.
			InternalAssert (!IsEmpty ());

			return m_elements[0u];
		}

		//
		// Function: GetFirstElement
		//
		// Description: Gets the first (bottom) constant element from the array.
		//
		// Returns: First (bottom) constant element reference.
		//
		inline const elementType &GetFirstElement (void) const
		{
			// Reliability check.
			InternalAssert (!IsEmpty ());

			return m_elements[0u];
		}

		//
		// Function: GetFirstElementIndex
		//
		// Description: Gets the first (bottom) element index.
		//
		// Returns: First (bottom) element index.
		//
		inline const indexType GetFirstElementIndex (void) const
		{
			// Reliability check.
			InternalAssert (!IsEmpty ());

			return 0u;
		}

		//
		// Function: GetLastElement
		//
		// Description: Gets the last (top) element from the array.
		//
		// Returns: Last (top) element reference.
		//
		inline elementType &GetLastElement (void)
		{
			// Reliability check.
			InternalAssert (!IsEmpty ());

			return m_elements[m_itemCount - 1u];
		}

		//
		// Function: GetLastElement
		//
		// Description: Gets the last (top) constant element from the array.
		//
		// Returns: Last (top) constant element reference.
		//
		inline const elementType &GetLastElement (void) const
		{
			// Reliability check.
			InternalAssert (!IsEmpty ());

			return m_elements[m_itemCount - 1u];
		}

		//
		// Function: GetLastElementIndex
		//
		// Description: Gets the last (top) element index.
		//
		// Returns: Last (top) element index.
		//
		inline const indexType GetLastElementIndex (void) const
		{
			// Reliability check.
			InternalAssert (!IsEmpty ());

			return m_itemCount - 1u;
		}

		//
		// Function: Find
		//
		// Description: Finds an element (element needs operator == defined).
		//
		// Parameters:
		//	element - Object to search for.
		//
		// Returns: Index of found object.
		//
		inline const indexType Find (const elementType &element, const indexType startIndex = 0u) const
		{
			return Find (element, startIndex, m_itemCount - startIndex);
		}
		inline const indexType Find (const elementType &element, indexType startIndex, indexType count) const
		{
			// Reliability check.
			InternalAssert (startIndex <= m_itemCount && count <= m_itemCount - startIndex);

			count += startIndex;

			for (/* Empty */; startIndex < count; ++startIndex)
				if (m_elements[startIndex] == element)
					return startIndex;

			return BadPosition;
		}

		inline const indexType BinaryFind (const elementType &element, const BinaryFindComparatorFunction_t comparatorFunction, const indexType startIndex = 0u) const
		{
			// Reliability check.
			InternalAssert (!IsEmpty ());

			return BinaryFind (element, comparatorFunction, startIndex, m_itemCount - startIndex);
		}
		inline const indexType BinaryFind (const elementType &element, const BinaryFindComparatorFunction_t comparatorFunction, indexType startIndex, int count) const
		{
			// Reliability check.
			InternalAssert (startIndex < m_itemCount && count > 0 && count <= m_itemCount - startIndex);

			indexType middle;

			count += startIndex;

			do
			{
				// To start, find the subscript of the middle position.
				middle = static_cast <indexType> ((startIndex + count) / 2u);

				const BinaryFindCompareResult_t result (comparatorFunction (m_elements[middle], element));

				if (result == Result_Equal)
					return middle;	// found it. Return position

				if (result == Result_Less)	// If the number is < key, decrease position by one.
					startIndex = middle + 1u;	// repeat search in top half.
				else	// (Else 'result == Result_More') If the key is < number, increase position by one.
					count = middle - 1;	// repeat search in bottom half.
			} while (startIndex <= count);

			// failed to find key
			return BadPosition;
		}

		//
		// Function: IsMember
		//
		// Description: Finds an element (element needs operator == defined).
		//
		// Parameters:
		//	element - Object to search for.
		//
		// Returns: True if element is founded, false otherwise.
		//
		inline const bool IsMember (const elementType &element) const
		{
			return Find (element) != BadPosition;
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
		inline const bool Assign (const elementType &input, const indexType count = 1u)
		{	// assign count * input.
			// Reliability check (Result too long).
			InternalAssert (count < MaximumPossibleSize);

			// Assign new stuff.

			for (m_itemCount = 0u; m_itemCount < count; m_elements[m_itemCount] = input, ++m_itemCount);	// Fill hole.
//			for (m_itemCount = 0u; m_itemCount < count; new (m_elements + m_itemCount) elementType (input), ++m_itemCount);	// Fill hole.

			return true;
		}

		//
		// Function: Assign
		//
		// Description: Assigns the array to existing buffer. (bufferPointer, bufferPointer + count)
		//
		// Parameters:
		//	bufferPointer - StaticArray buffer to assign.
		//	count - Number of elements to use for the content (i.e., it's length).
		//
		// Returns: True if input array is assigned, false otherwise.
		//
		inline const bool Assign (const elementType *const bufferPointer, const indexType count)
		{
			if (IsInside (bufferPointer))
				return Assign (*this, static_cast <indexType> (bufferPointer - m_elements), count);	// Subarray.

			// Reliability check.
			InternalAssert (count == 0u || bufferPointer != NULL);

			// Assign new stuff.

			for (m_itemCount = 0u; m_itemCount < count; m_elements[m_itemCount] = bufferPointer[m_itemCount], ++m_itemCount);
//			for (m_itemCount = 0u; m_itemCount < count; new (m_elements + m_itemCount) elementType (bufferPointer[m_itemCount]), ++m_itemCount);

			return true;
		}

		//
		// Function: Assign
		//
		// Description: Assigns the array to existing buffer. (startIndex, startIndex + count)
		//
		// Parameters:
		//	inputArray - StaticArray buffer to assign.
		//	startIndex - Starting position of the subarray of the input array that forms the new content.
		//	count - Number of elements to use for the content (i.e., it's length).
		//
		// Returns: True if input array is assigned, false otherwise.
		//
		inline const bool Assign (const StaticArray &inputArray, const indexType startIndex, const indexType count)
		{
			// Reliability check ('startIndex' off end).
			InternalAssert (startIndex <= inputArray.m_itemCount && count <= inputArray.m_itemCount - startIndex);

			if (this == &inputArray)
			{
				// Subarray.

				Remove (0u, startIndex);
				Remove (startIndex + count);
			}
			else	// Assign new stuff.
			{
				for (m_itemCount = 0u; m_itemCount < count; m_elements[m_itemCount] = inputArray.m_elements[startIndex + m_itemCount], ++m_itemCount);	// Copy data.
//				for (m_itemCount = 0u; m_itemCount < count; new (m_elements + m_itemCount) elementType (inputArray.m_elements[startIndex + m_itemCount]), ++m_itemCount);	// Copy data.
			}

			return true;
		}

		//
		// Function: Assign
		//
		// Description: Assigns the array to existing buffer. (startIndex, inputArray.m_itemCount - startIndex)
		//
		// Parameters:
		//	inputArray - StaticArray buffer to assign.
		//	startIndex - Starting position of the subarray of the input array that forms the new content.
		//
		// Returns: True if input array is assigned, false otherwise.
		//
		inline const bool Assign (const StaticArray &inputArray, const indexType startIndex = 0u)
		{
			return Assign (inputArray, startIndex, inputArray.m_itemCount - startIndex);
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
			InternalAssert (sourceIndex < m_itemCount && count <= m_itemCount - sourceIndex);

			// Destruct elements.
//			for (indexType index (0u); index < count; ++index)
//				Destruct (m_elements + (sourceIndex + index));

			// Move elements down.
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
			Remove (sourceIndex, m_itemCount - sourceIndex);
		}

		// Delete the element.
		inline void Delete (const indexType sourceIndex, const indexType count)
		{
			// Checks for bounds....
			InternalAssert (sourceIndex < m_itemCount && count <= m_itemCount - sourceIndex);

			const indexType steps (sourceIndex + count);

			// Free used memory....
			for (indexType index (sourceIndex); index < steps; ++index)
				delete m_elements[index];

			Remove (sourceIndex, count);
		}
		inline void Delete (const indexType sourceIndex = 0u)
		{
			Delete (sourceIndex, m_itemCount - sourceIndex);
		}

		inline const bool FindAndDeleteElement (const elementType &element)
		{
			const indexType elementPosition (Find (element));

			if (elementPosition == BadPosition)
				return false;

			// Else....

			Delete (elementPosition, 1u);

			return true;
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
			// Reliability check.
			InternalAssert (m_itemCount >= 2u);

			// Sort remaining arguments using quick sort algorithm:
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
			InternalAssert (sourceIndex < m_itemCount && count <= m_itemCount - sourceIndex && count >= 2u);

			// Sort remaining arguments using Quicksort algorithm:
			qsort (m_elements + sourceIndex, count, ElementSize, reinterpret_cast <int (*) (const void *const, const void *const)> (comparator));
		}

	//
	// Group: Operators.
	//
	public:
		//
		// Function: operator =
		//
		// Description: Assigns the element to existing buffer.
		//
		// Parameters:
		//	input - Element to assign.
		//
		// Returns: Assigned element.
		//
		inline StaticArray &operator =  (const elementType &input)
		{
			Assign (input);

			return *this;
		}

		//
		// Function: operator =
		//
		// Description: Reassignes current array with specified one.
		//
		// Parameters:
		//	other - Other array that should be assigned.
		//
		// Returns: Reassigned array.
		//
		inline StaticArray &operator =  (const StaticArray &other)
		{
			Assign (other);

			return *this;
		}

		inline StaticArray &operator -= (const elementType &object)
		{
			Pop (object);

			return *this;
		}
		template <typename indexType2, indexType2 elementsNumber2> inline StaticArray &operator -= (const StaticArray <elementType, indexType2, elementsNumber2> &other)
		{
			Pop (other);

			return *this;
		}

		inline StaticArray &operator += (const elementType &object)
		{
			PushBack (object);

			return *this;
		}

		inline StaticArray &operator += (const StaticArray &other)
		{
			PushBack (other);

			return *this;
		}

		// Following five operators are equal's StaticArray::GetData() function.
		inline operator       elementType *const    (void)       { return m_elements; }
		inline operator       elementType *const    (void) const { return m_elements; }	// Needed too :{O (Half-Life engine so stupid....)
		inline operator const elementType *const    (void) const { return m_elements; }

		inline       elementType *const operator () (void)       { return m_elements; }
		inline const elementType *const operator () (void) const { return m_elements; }

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
		inline       elementType &operator [] (const indexType index)
		{
			// Reliability check.
			InternalAssert (index < m_itemCount);

			return m_elements[index];
		}
		inline const elementType &operator [] (const indexType index) const
		{
			// Reliability check.
			InternalAssert (index < m_itemCount);

			return m_elements[index];
		}

		// Equality operation.
		inline const bool operator == (const StaticArray &right) const
		{
			// If arrays are various objects we carry out comparison.
			if (this != &right)
			{
				// If number of array elements not equally,
				if (m_itemCount != right.m_itemCount)
					return false;	// arrays are not equal.

				// Else we carry out step by step comparison.
				for (indexType index (0u); index < m_itemCount; ++index)
					if (m_elements[index] != right.m_elements[index])
						return false;	// arrays are not equal.
			}

			// Otherwise we return true (since any array is equal to itself).
			return true;
		}

		// Inequality operation.
		inline const bool operator != (const StaticArray &right) const
		{
			return !(*this == right);
		}

		inline const bool operator < (const StaticArray &right) const
		{
			if (this == &right)
				return false;	// arrays are not equal.

			const indexType length (Math::GetMinimumValueBetween (m_itemCount, right.m_itemCount));

			for (indexType index (0u); index < length; ++index)
			{
				if (m_elements[index] < right.m_elements[index])
					return true;	// arrays are equal.

				if (m_elements[index] > right.m_elements[index])
					return false;	// arrays are not equal.
			}

			// They are the same, judge by length.
			return m_itemCount < right.m_itemCount;
		}
		inline const bool operator > (const StaticArray &right) const
		{
			return right < *this;
		}
		inline const bool operator >= (const StaticArray &right) const
		{
			return !(*this < right);
		}
		inline const bool operator <= (const StaticArray &right) const
		{
			return !(right < *this);
		}
};

#endif	// ifndef ARRAY_INCLUDED