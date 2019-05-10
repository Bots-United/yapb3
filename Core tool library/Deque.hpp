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
// Deque.hpp
//
// Class: Deque
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef DEQUE_INCLUDED
#define DEQUE_INCLUDED
#if 0
template <typename elementType, typename indexType = unsigned short> class Deque
{
	//
	// Group: Private special points.
	//
	private:
		static CLASS_GLOBAL const unsigned char SectorSize = 16u;

	//
	// Group: Private members.
	//
	private:
		DynamicArray <elementType *, indexType> m_sectors;		// pointer to array of pointers to blocks
		indexType                               m_itemCount;	// current length of sequence

	//
	// Group: Special points.
	//
	public:
		enum
		{
			ElementSize = sizeof (elementType),	// One deque element size.
			BadPosition = static_cast <indexType> (-1),	// Generic bad/missing length/position.
			MaximumPossibleSize = min/*Math::GetMinimumValueBetween <unsigned int>*/ (BadPosition, CoreToolLibrary::BadPosition / ElementSize / 2u)	// Maximum possible array size.
		};

	//
	// Group: (Con/De)structors.
	//
	public:
		inline Deque (void) : m_itemCount (0u), m_currentOffset (0u) { /*VOID*/ }

		inline Deque (const Deque &other)
		{
			for (indexType index = 0u; index < other.m_sectors.GetElementNumber (); ++index)
			{
				m_sectors += new elementType[SectorSize];

				for (indexType i = 0u; i < SectorSize; ++i)
					m_sectors.GetLastElement ()[i] = m_sectors[index][i];
			}

			m_itemCount = other.m_itemCount;
		}

		inline ~Deque (void)
		{
			// destroy the deque
			for (indexType index = 0u; index < m_sectors.GetElementNumber (); ++index)
				delete [] m_sectors[index];
		}

	//
	// Group: Functions.
	//
	public:
		//
		// Function: PushFront
		//
		// Description: Appends new element to the begin of array.
		//
		// Parameters:
		//	element - Object to append.
		//	count - Number of element to insert.
		//
		// Returns: True if operation succeeded, false otherwise.
		//
		indexType m_currentOffset;
		inline const bool PushFront (const elementType &element, const indexType count = 1u)
		{	// erase element at beginning
/*			if (m_itemCount / SectorSize >= m_sectors.GetElementNumber ())
			{
				// New sector
				m_sectors += new elementType[SectorSize];
			}

			++m_itemCount;

			operator [] (0u) = element;*/
			if (m_currentOffset % SectorSize == 0u && m_sectors.GetElementNumber/*AllocatedSize*/ () <= (m_itemCount + SectorSize) / SectorSize)
//			if (m_itemCount / SectorSize >= m_sectors.GetElementNumber ())
			{
				// New sector
				m_sectors += new elementType[SectorSize];
			}

			indexType _Newoff = m_currentOffset > 0u ? m_currentOffset : m_sectors.GetElementNumber/*GetAllocatedSize*/ () * SectorSize;
			const indexType _Block = --_Newoff / SectorSize;

			m_sectors[_Block][_Newoff % SectorSize] = element;
//			*(m_sectors[_Block] + _Newoff % SectorSize) = element;

//			AddLogEntry (false, LogLevel_Default, true, "Deque::PushFront(): m_sectors:\n{\tGetElementNumber() == %u,\n\tGetAllocatedSize() == %u\n},\nm_itemCount == %u,\nm_currentOffset == %u,\n_Block == %u,\n_Newoff == %u,\n_Newoff % SectorSize == %u,\n\nelement == %i.", m_sectors.GetElementNumber (), m_sectors.GetAllocatedSize (), m_itemCount + 1u, m_currentOffset, _Block, _Newoff, _Newoff % SectorSize, element);
			AddLogEntry (false, LogLevel_Default, true, "Deque::PushFront(): m_sectors:\n{\n\tGetElementNumber() == %u,\n\tGetAllocatedSize() == %u\n},\nm_itemCount == %u,\nm_currentOffset == %u,\n_Block == %u,\n_Newoff == %u,\n\nelement == %i.", m_sectors.GetElementNumber (), m_sectors.GetAllocatedSize (), m_itemCount + 1u, m_currentOffset, _Block, _Newoff, element);

			m_currentOffset = _Newoff;

			++m_itemCount;

			return true;
		}

		//
		// Function: PushBack
		//
		// Description: Appends new element to the end of array.
		//
		// Parameters:
		//	element - Object to append.
		//	count - Number of element to insert.
		//
		// Returns: True if operation succeeded, false otherwise.
		//
		inline const bool PushBack (const elementType &element, const indexType count = 1u)
		{	// insert element at end
			if (m_itemCount / SectorSize >= m_sectors.GetElementNumber ())
			{
				// New sector
				m_sectors += new elementType[SectorSize];
			}

			operator [] (m_itemCount++) = element;

			return true;
		}
/*
		inline elementType *const make_next (void)
		{
			if (m_itemCount / SectorSize >= m_sectors.GetElementNumber ())
			{
				// New sector
				m_sectors += new elementType[SectorSize];
			}

			return &operator [] (m_itemCount++);
		}
*/
		//
		// Function: PopFront
		//
		// Description: Pops first element from array.
		//
		// Returns: Object popped from the begin of array.
		//
		inline const elementType /*&*/PopFront (void)
		{
			--m_itemCount;

			return NULL;
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
			--m_itemCount;

			return NULL;
		}

		inline void PopAll (void)
		{
			m_itemCount = 0u;
		}

		inline void RemoveAll (void)
		{
			for (indexType index = 0u; index < m_sectors.GetElementNumber (); ++index)
				delete [] m_sectors[index];

			m_sectors.RemoveAll ();
		}

		//
		// Function: GetFirstElement
		//
		// Description: Gets the first (bottom) element from the array.
		//
		// Returns: First (bottom) random element reference.
		//
		inline elementType &GetFirstElement (void)
		{	// return first element of mutable sequence
			// Reliability check.
			InternalAssert (!IsEmpty ());

			return operator [] (m_itemCount - 1u);
		}

		//
		// Function: GetFirstElement
		//
		// Description: Gets the first (bottom) constant element from the array.
		//
		// Returns: First (bottom) random constant element reference.
		//
		inline const elementType &GetFirstElement (void) const
		{	// return first element of nonmutable sequence
			// Reliability check.
			InternalAssert (!IsEmpty ());

			return operator [] (m_itemCount - 1u);
		}

		inline elementType &GetSecondElement (void)
		{
			// Reliability check.
			InternalAssert (!IsEmpty ());

			return operator [] (m_itemCount - 2u);
		}

		inline const elementType &GetSecondElement (void) const
		{
			// Reliability check.
			InternalAssert (!IsEmpty ());

			return operator [] (m_itemCount - 2u);
		}

		//
		// Function: GetLastElement
		//
		// Description: Gets the last (top) element from the array.
		//
		// Returns: Last (top) random element reference.
		//
		inline elementType &GetLastElement (void)
		{	// return last element of mutable sequence
			// Reliability check.
			InternalAssert (!IsEmpty ());

			return operator [] (m_itemCount - 2u);
		}

		//
		// Function: GetLastElement
		//
		// Description: Gets the last (top) constant element from the array.
		//
		// Returns: Last (top) random constant element reference.
		//
		inline const elementType &GetLastElement (void) const
		{	// return last element of nonmutable sequence
			// Reliability check.
			InternalAssert (!IsEmpty ());

			return operator [] (m_itemCount - 2u);
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
			return sizeof (Deque) + ElementSize * GetAllocatedSize ();
		}

		//
		// Function: GetAllocatedSize
		//
		// Description: Gets allocated size of array.
		//
		// Returns: Number of allocated items.
		//
		inline const indexType GetAllocatedSize (void) const { return m_sectors.GetAllocatedSize (); }

		//
		// Function: GetElementNumber
		//
		// Description: Gets real number currently in array.
		//
		// Returns: Number of elements.
		//
		inline const indexType GetElementNumber (void) const { return m_itemCount; }

		//
		// Function: IsEmpty
		//
		// Description: Checks whether array is empty.
		//
		// Returns: True if array is empty, false otherwise.
		//
		inline const bool IsEmpty (void) const { return m_itemCount == 0u; }

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
/*
			// Destruct elements.
//			for (indexType index = 0u; index < count; (m_elements + (sourceIndex + index))->~elementType (), ++index);

			// Move elements down (for characters the MoveItems() trims last element).
			MoveItems (sourceIndex, sourceIndex + count);

			// Update array length.
			m_itemCount -= count;*/
			if (sourceIndex < m_itemCount - (sourceIndex + count))
			{	// closer to front
				std::copy_backward (begin (), _First, _Last);	// copy over hole

				for (/* Empty */; count > 0u; --count)
					PopFront ();	// pop copied elements
			}
			else
			{	// closer to back
				std::copy (_Last, end (), _First);	// copy over hole

				for (/* Empty */; count > 0u; --count)
					PopBack ();	// pop copied elements
			}
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

	//
	// Group: Operators.
	//
	public:
		inline Deque &operator = (const Deque &other)
		{
			if (this == &other)
				return *this;

			RemoveAll ();

			for (indexType index = 0u; index < other.m_sectors.GetElementNumber (); ++index)
			{
				m_sectors += new elementType[SectorSize];

				for (indexType i = 0u; i < SectorSize; ++i)
					m_sectors.GetLastElement ()[i] = m_sectors[index][i];
			}

			m_itemCount = other.m_itemCount;

			return *this;
		}

		inline Deque &operator += (const elementType &object)
		{
			PushBack (object);

			return *this;
		}

		inline elementType       &operator [] (const indexType index)
		{
			// Reliability check.
			InternalAssert (index < m_itemCount);

			return m_sectors[static_cast <const indexType> (index / SectorSize)][index % SectorSize];
		}

		inline const elementType &operator [] (const indexType index) const
		{
			// Reliability check.
			InternalAssert (index < m_itemCount);

			return m_sectors[static_cast <const indexType> (index / SectorSize)][index % SectorSize];
		}
};

template <typename elementType, typename indexType = unsigned short> class deque
{
	private:
		struct Iterator_t
		{
			elementType *current;
			elementType *leftBound;
			elementType *rightBound;
		};

		elementType *m_array;
		Iterator_t   m_first;
		Iterator_t   m_last;
		indexType    m_boundSize;

	public:
		inline deque (void) :
			m_boundSize (8u),
			m_array (new elementType[m_boundSize * 2u])
		{
			m_first.leftBound  = m_last.leftBound  = m_array;
			m_first.current    = m_last.current    = m_first.leftBound + m_boundSize;
			m_first.rightBound = m_last.rightBound = m_first.current + m_boundSize;
		}
		inline ~deque (void) { delete [] m_array; }

	private:
		inline void Reallocate (void)
		{
			const indexType size = GetElementNumber ();
			elementType *const temp = new elementType[size + m_boundSize * 2u];

			memcpy (temp + m_boundSize, m_first.current, size * ElementSize);

			delete [] m_array;

			m_array            = temp;
			m_first.leftBound  = m_array;
			m_first.current    = m_array + m_boundSize;
			m_first.rightBound = m_first.current + m_boundSize;
			m_last.current     = m_first.current + size;
			m_last.leftBound   = m_last.current - m_boundSize;
			m_last.rightBound  = m_last.current + m_boundSize;
		}

	public:
		inline       elementType &operator [] (const indexType index)       { return m_first.current[index]; }
		inline const elementType &operator [] (const indexType index) const { return m_first.current[index]; }

	public:
		inline const indexType GetElementNumber (void) const { return static_cast <indexType> (m_last.current - m_first.current); }

		inline void PushFront (const elementType &value)
		{
			if (m_first.current == m_first.leftBound)
				Reallocate ();

			*--m_first.current = value;
		}
		inline const elementType /*&*/PopFront (void)
		{
/*			if (m_first.current != m_first.rightBound)
				++m_first.current;
			else
				Reallocate ();*/
			InternalAssert (m_first.current < m_first.rightBound);

			return *m_first.current++;
		}
		inline void PushBack (const elementType &value)
		{
			if (m_last.current == m_last.rightBound)
				Reallocate ();

			*m_last.current++ = value;
		}
		inline const elementType /*&*/PopBack (void)
		{
/*			if (m_last.current != m_last.leftBound)
				--m_last.current;
			else
				Reallocate ();*/
			InternalAssert (m_last.current < m_last.leftBound);

			return *m_last.current--;
		}

		inline void SetBoundSize (const indexType value)
		{
			m_boundSize = value;

			Reallocate ();
		}
};
#endif	// if 0
template <typename elementType, typename indexType = unsigned short> class Deque
{
	//
	// Group: Private members.
	//
	private:
		// Rukia: front_vec is theoretically reversed; push_back becoslots push_front.
		//	e. 1 2 3 4 5 will look like this:
		//	front_vec: 3 2 1, back_vec 4 5
		DynamicArray <elementType, indexType> front_vec;
		DynamicArray <elementType, indexType> back_vec;

	//
	// Group: Special points.
	//
	public:
		enum
		{
			ElementSize = sizeof (elementType),	// One deque element size.
			BadPosition = static_cast <indexType> (-1),	// Generic bad/missing length/position.
			MaximumPossibleSize = min/*Math::GetMinimumValueBetween <unsigned int>*/ (BadPosition, CoreToolLibrary::BadPosition / ElementSize / 2u)	// Maximum possible array size.
		};

	//
	// Group: (Con/De)structors.
	//
	public:
		inline Deque (void) : front_vec (), back_vec () { /*VOID*/ }
		inline Deque (const Deque &other) : front_vec (other.front_vec), back_vec (other.back_vec) { /*VOID*/ }

	//
	// Group: Functions.
	//
	public:
		//
		// Function: PushFront
		//
		// Description: Appends new element to the begin of array.
		//
		// Parameters:
		//	element - Object to append.
		//	count - Number of element to insert.
		//
		// Returns: True if operation succeeded, false otherwise.
		//
		inline const bool PushFront (const elementType &element, const indexType count = 1u)
		{	// erase element at beginning
/*			if (m_itemCount / SectorSize >= m_sectors.GetElementNumber ())
			{
				// New sector
				m_sectors += new elementType[SectorSize];
			}

			++m_itemCount;

			operator [] (0u) = element;*/
			if (m_currentOffset % SectorSize == 0u && m_sectors.GetElementNumber/*AllocatedSize*/ () <= (m_itemCount + SectorSize) / SectorSize)
//			if (m_itemCount / SectorSize >= m_sectors.GetElementNumber ())
			{
				// New sector
				m_sectors += new elementType[SectorSize];
			}

			indexType _Newoff = m_currentOffset > 0u ? m_currentOffset : m_sectors.GetElementNumber/*GetAllocatedSize*/ () * SectorSize;
			const indexType _Block = --_Newoff / SectorSize;

			m_sectors[_Block][_Newoff % SectorSize] = element;
//			*(m_sectors[_Block] + _Newoff % SectorSize) = element;

//			AddLogEntry (false, LogLevel_Default, true, "Deque::PushFront(): m_sectors:\n{\tGetElementNumber() == %u,\n\tGetAllocatedSize() == %u\n},\nm_itemCount == %u,\nm_currentOffset == %u,\n_Block == %u,\n_Newoff == %u,\n_Newoff % SectorSize == %u,\n\nelement == %i.", m_sectors.GetElementNumber (), m_sectors.GetAllocatedSize (), m_itemCount + 1u, m_currentOffset, _Block, _Newoff, _Newoff % SectorSize, element);
			AddLogEntry (false, LogLevel_Default, true, "Deque::PushFront(): m_sectors:\n{\n\tGetElementNumber() == %u,\n\tGetAllocatedSize() == %u\n},\nm_itemCount == %u,\nm_currentOffset == %u,\n_Block == %u,\n_Newoff == %u,\n\nelement == %i.", m_sectors.GetElementNumber (), m_sectors.GetAllocatedSize (), m_itemCount + 1u, m_currentOffset, _Block, _Newoff, element);

			m_currentOffset = _Newoff;

			++m_itemCount;

			return true;
		}

		//
		// Function: PushBack
		//
		// Description: Appends new element to the end of array.
		//
		// Parameters:
		//	element - Object to append.
		//	count - Number of element to insert.
		//
		// Returns: True if operation succeeded, false otherwise.
		//
		inline const bool PushBack (const elementType &element, const indexType count = 1u)
		{	// insert element at end
			if (m_itemCount / SectorSize >= m_sectors.GetElementNumber ())
			{
				// New sector
				m_sectors += new elementType[SectorSize];
			}

			operator [] (m_itemCount++) = element;

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
			--m_itemCount;

			return NULL;
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
			--m_itemCount;

			return NULL;
		}

		inline void PopAll (void)
		{
			m_itemCount = 0u;
		}

		inline void MakeEmpty (void)
		{
			front_vec.MakeEmpty ();
			back_vec.MakeEmpty ();
		}

		inline void RemoveAll (void)
		{
			front_vec.RemoveAll ();
			back_vec.RemoveAll ();
		}

		// Destruct all the elements.
		inline void DestructAll (void)
		{
			front_vec.DestructAll ();
			back_vec.DestructAll ();
		}

		// Delete all the elements.
		inline void DeleteAll (void)
		{
			front_vec.DeleteAll ();
			back_vec.DeleteAll ();
		}

		// Delete all the elements then call Purge.
		inline void DeleteAndPurgeAll (void)
		{
			front_vec.DeleteAndPurgeAll ();
			back_vec.DeleteAndPurgeAll ();
		}

		inline void Swap (Deque &other)
		{
			front_vec.Swap (other.front_vec);
			back_vec.Swap (other.back_vec);
		}

		inline const bool IsValidIndex (const indexType position) const
		{
			return position < front_vec.GetElementNumber () || position - front_vec.GetElementNumber () < back_vec.GetElementNumber ();
		}

		//
		// Function: GetFirstElement
		//
		// Description: Gets the first (bottom) element from the array.
		//
		// Returns: First (bottom) random element reference.
		//
		inline elementType &GetFirstElement (void)
		{
			return front_vec.GetLastElement ();
		}

		//
		// Function: GetFirstElement
		//
		// Description: Gets the first (bottom) constant element from the array.
		//
		// Returns: First (bottom) random constant element reference.
		//
		inline const elementType &GetFirstElement (void) const
		{
			return front_vec.GetLastElement ();
		}

		//
		// Function: GetLastElement
		//
		// Description: Gets the last (top) element from the array.
		//
		// Returns: Last (top) random element reference.
		//
		inline elementType &GetLastElement (void)
		{
			return back_vec.GetLastElement ();
		}

		//
		// Function: GetLastElement
		//
		// Description: Gets the last (top) constant element from the array.
		//
		// Returns: Last (top) random constant element reference.
		//
		inline const elementType &GetLastElement (void) const
		{
			return back_vec.GetLastElement ();
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
			return sizeof (Deque) + ElementSize * GetAllocatedSize ();
		}

		//
		// Function: GetAllocatedSize
		//
		// Description: Gets allocated size of array.
		//
		// Returns: Number of allocated items.
		//
		inline const indexType GetAllocatedSize (void) const { return front_vec.GetAllocatedSize () + back_vec.GetAllocatedSize (); }

		//
		// Function: GetElementNumber
		//
		// Description: Gets real number currently in array.
		//
		// Returns: Number of elements.
		//
		inline const indexType GetElementNumber (void) const { return front_vec.GetElementNumber () + back_vec.GetElementNumber (); }

		//
		// Function: IsEmpty
		//
		// Description: Checks whether array is empty.
		//
		// Returns: True if array is empty, false otherwise.
		//
		inline const bool IsEmpty (void) const { return front_vec.IsEmpty () && back_vec.IsEmpty (); }

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
/*
			// Destruct elements.
//			for (indexType index = 0u; index < count; (m_elements + (sourceIndex + index))->~elementType (), ++index);

			// Move elements down (for characters the MoveItems() trims last element).
			MoveItems (sourceIndex, sourceIndex + count);

			// Update array length.
			m_itemCount -= count;*/
			if (sourceIndex < m_itemCount - (sourceIndex + count))
			{	// closer to front
				std::copy_backward (begin (), _First, _Last);	// copy over hole

				for (/* Empty */; count > 0u; --count)
					PopFront ();	// pop copied elements
			}
			else
			{	// closer to back
				std::copy (_Last, end (), _First);	// copy over hole

				for (/* Empty */; count > 0u; --count)
					PopBack ();	// pop copied elements
			}
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

	//
	// Group: Operators.
	//
	public:
		inline Deque &operator = (const Deque &right)
		{
			front_vec = copy.right;
			back_vec = copy.right;

			return *this;
		}

		inline Deque &operator += (const elementType &object)
		{
			PushBack (object);

			return *this;
		}

		inline elementType       &operator [] (const indexType index)
		{
			// Reliability check.
			InternalAssert (index < m_itemCount);

			return m_sectors[static_cast <const indexType> (index / SectorSize)][index % SectorSize];
		}

		inline const elementType &operator [] (const indexType index) const
		{
			// Reliability check.
			InternalAssert (index < m_itemCount);

			return m_sectors[static_cast <const indexType> (index / SectorSize)][index % SectorSize];
		}
};

template <typename elementType, typename indexType = unsigned short> class deque
{
	private:
		struct Iterator_t
		{
			elementType *current;
			elementType *leftBound;
			elementType *rightBound;
		};

		elementType *m_array;
		Iterator_t   m_first;
		Iterator_t   m_last;
		indexType    m_boundSize;

	//
	// Group: Special points.
	//
	public:
		enum
		{
			ElementSize = sizeof (elementType),	// One deque element size.
			BadPosition = static_cast <indexType> (-1),	// Generic bad/missing length/position.
			MaximumPossibleSize = min/*Math::GetMinimumValueBetween <unsigned int>*/ (BadPosition, CoreToolLibrary::BadPosition / ElementSize / 2u)	// Maximum possible array size.
		};

	public:
		inline deque (void) :
			m_boundSize (8u),
			m_array (new elementType[m_boundSize * 2u])
		{
			m_first.leftBound  = m_last.leftBound  = m_array;
			m_first.current    = m_last.current    = m_first.leftBound + m_boundSize;
			m_first.rightBound = m_last.rightBound = m_first.current + m_boundSize;
		}
		inline ~deque (void) { delete [] m_array; }

	private:
		inline void Reallocate (void)
		{
			const indexType size (GetElementNumber ());
			elementType *const newBuffer (new elementType[size + m_boundSize * 2u]);

			memcpy (newBuffer + m_boundSize, m_first.current, size * ElementSize);

			delete [] m_array;

			m_array            = newBuffer;
			m_first.leftBound  = m_array;
			m_first.current    = m_array + m_boundSize;
			m_first.rightBound = m_first.current + m_boundSize;
			m_last.current     = m_first.current + size;
			m_last.leftBound   = m_last.current - m_boundSize;
			m_last.rightBound  = m_last.current + m_boundSize;
		}

	public:
		inline       elementType &operator [] (const indexType index)       { return m_first.current[index]; }
		inline const elementType &operator [] (const indexType index) const { return m_first.current[index]; }

	public:
		inline const indexType GetElementNumber (void) const { return static_cast <indexType> (m_last.current - m_first.current); }

		inline void PushFront (const elementType &value)
		{
			if (m_first.current == m_first.leftBound)
				Reallocate ();

			*--m_first.current = value;
		}
		inline const elementType /*&*/PopFront (void)
		{
//			if (m_first.current == m_first.rightBound)
//				Reallocate ();
			InternalAssert (m_first.current < m_first.rightBound);

			return *m_first.current++;
		}
		inline void PushBack (const elementType &value)
		{
			if (m_last.current == m_last.rightBound)
				Reallocate ();

			*m_last.current++ = value;
		}
		inline const elementType /*&*/PopBack (void)
		{
//			if (m_last.current == m_last.leftBound)
//				Reallocate ();
			InternalAssert (m_last.current < m_last.leftBound);

			return *m_last.current--;
		}

		inline void SetBoundSize (const indexType value)
		{
			m_boundSize = value;

			Reallocate ();
		}
};

#endif	// ifndef DEQUE_INCLUDED