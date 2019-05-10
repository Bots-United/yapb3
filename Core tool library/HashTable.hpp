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
// HashTable.hpp
//
// Class: HashTable
//
// Description: Represents Hash Table container.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef HASH_TABLE_INCLUDED
#define HASH_TABLE_INCLUDED

template <typename keyElementType, typename elementType, typename indexType = unsigned short> class HashTable
{
	//
	// Group: Private members.
	//
	private:
		struct HashItem_t
		{
			keyElementType m_key;
			indexType      m_index;

			inline HashItem_t (void)                                             : m_index (0u)                 { /*VOID*/ }
			inline HashItem_t (const keyElementType &key, const indexType index) : m_key (key), m_index (index) { /*VOID*/ }
		};

		indexType                              m_hashSize;
		DynamicArray <HashItem_t, indexType>  *m_table;
		DynamicArray <elementType, indexType>  m_symTable;

	//
	// Group: Typedef's.
	//
	public:
		typedef elementType ElementType_t;
		typedef indexType   IndexType_t;

	//
	// Group: Private functions.
	//
	private:
		inline elementType *const InternalGet (const keyElementType &keyName, const bool create = false) const
		{
			const indexType index (GetIndex (keyName, create));

			if (index == BadPosition)
				return NULL;

			return m_symTable + index;
		}

	//
	// Group: (Con/De)structors.
	//
	public:
		//
		// Function: HashTable
		//
		// Description: Default hash table container constructor.
		//
		// Parameters:
		//	hashSize - Initial hash size.
		//
		inline HashTable (const indexType hashSize = 36u) : m_hashSize (hashSize), m_table (new DynamicArray <HashItem_t, indexType>[hashSize])
		{
			// Reliability check.
			if (m_table == NULL)
				TerminateOnMalloc ();
		}

		//
		// Function: ~HashTable
		//
		// Description: Default hash table container destructor.
		//
		virtual inline ~HashTable (void)
		{
			RemoveAll ();

			delete [] m_table;
		}

	//
	// Group: Functions.
	//
	public:
		//
		// Function: IsEmpty
		//
		// Description: Checks whether container is empty.
		//
		// Returns: True if no elements in container, false otherwise.
		//
		inline const bool IsEmpty (void) const { return m_symTable.IsEmpty (); }

		//
		// Function: SetupHashTable
		//
		// Description: Setups the hash table.
		//
		// Parameters:
		//	hashSize - Initial hash size.
		//
		inline void SetupHashTable (const indexType hashSize)
		{
			m_hashSize = hashSize;
			m_table = new DynamicArray <HashItem_t, indexType>[hashSize];

			// Reliability check.
			if (m_table == NULL)
				TerminateOnMalloc ();
		}

		//
		// Function: IsExists
		//
		// Description: Checks whether element exists in container.
		//
		// Parameters:
		//	keyName - Key name of element, that should be checked.
		//
		// Returns: True if element exists, false otherwise.
		//
		inline const bool IsExists (const keyElementType &keyName) const { return InternalGet (keyName) != NULL; }

		//
		// Function: GetAllocatedSize
		//
		// Description: Gets the size of container.
		//
		// Returns: Number of elements in container.
		//
		inline const indexType GetAllocatedSize (void) const { return m_symTable.GetAllocatedSize (); }

		//
		// Function: Get
		//
		// Description: Gets the value, by it's index.
		//
		// Parameters:
		//	index - Index of element.
		//
		// Returns: Reference to element.
		//
		inline elementType &Get (const indexType index) { return m_symTable[index]; }

		//
		// Function: GetElements
		//
		// Description: Gets the all elements of container.
		//
		// Returns: Array of elements, containing inside container.
		//
		// See also: Class 'DynamicArray'.
		//
		inline DynamicArray <elementType, indexType> &GetElements (void) { return m_symTable; }

		//
		// Function: Find
		//
		// Description: Finds element by his key name.
		//
		// Parameters:
		//	keyName - Key name to be searched.
		//	element - Holder for element object.
		//
		// Returns: True if element found, false otherwise.
		//
		inline const bool Find (const keyElementType &keyName, elementType &element) const
		{
			elementType *const hashPointer = InternalGet (keyName);

			if (hashPointer != NULL)
			{
				element = *hashPointer;

				return true;
			}

			return false;
		}

		//
		// Function: Find
		//
		// Description: Finds element by his key name.
		//
		// Parameters:
		//	keyName - Key name to be searched.
		//	elementPointer - Holder for element pointer.
		//
		// Returns: True if element found, false otherwise.
		//
		inline const bool Find (const keyElementType &keyName, elementType *&elementPointer) const
		{
			elementType *const hashPointer = InternalGet (keyName);

			if (hashPointer != NULL)
			{
				elementPointer = hashPointer;

				return true;
			}

			return false;
		}

		//
		// Function: Remove
		//
		// Description: Removes element from container.
		//
		// Parameters:
		//	keyName - Key name of element, that should be removed.
		//
		// Returns: Removed element.
		//
		const elementType Remove (const keyElementType &keyName)
		{
			elementType removeResult;
			indexType hashID (Hash (keyName) % m_hashSize), index;
			DynamicArray <HashItem_t, indexType> &bucket (m_table[hashID]);

			for (indexType i (0u), j; i < bucket.GetElementNumber () (); ++i)
			{
				const HashItem_t &item (bucket[i]);

				if (item.m_key == keyName)
				{
					index = item.m_index;
					removeResult = m_symTable.Remove (index);

					bucket.Remove (i);

					for (hashID = 0u; hashID < m_hashSize; ++hashID)
					{
						&bucket = m_table[hashID];

						for (j = 0u; j < bucket.GetElementNumber () (); ++j)
						{
							const HashItem_t &item (bucket[j]);

							if (item.m_index > index)
								--item.m_index;
						}
					}

					return removeResult;
				}
			}

			return removeResult;
		}

		//
		// Function: GetIndex
		//
		// Description: Gets index of element.
		//
		// Parameters:
		//	keyName - Key of element.
		//	create - If true and no element found by a keyName, create new element.
		//
		// Returns: Either found index, created index, or -1 in case of error.
		//
		const indexType GetIndex (const keyElementType &keyName, const bool create = false)
		{
			DynamicArray <HashItem_t, indexType> &bucket (m_table[Hash (keyName) % m_hashSize]);

			for (indexType index (0u); index < bucket.GetElementNumber () (); ++index)
			{
				const HashItem_t &item (bucket[index]);

				if (item.m_key == keyName)
					return item.m_index;
			}

			if (create)
			{
				bucket += HashItem_t (keyName, bucket.GetElementNumber ());

				return bucket.GetLastElementIndex ();
			}

			return BadPosition;
		}

		//
		// Function: RemoveAll
		//
		// Description: Removes all elements from container.
		//
		inline void RemoveAll (void)
		{
			for (indexType index (0u); index < m_hashSize; ++index)
				m_table[index].RemoveAll ();

			m_symTable.RemoveAll ();
		}

	//
	// Group: Operators.
	//
	public:
		inline       elementType &operator [] (const keyElementType &keyName)       { return *InternalGet (keyName, true); }
		inline const elementType &operator [] (const keyElementType &keyName) const { return *InternalGet (keyName, true); }
/*
		inline HashTable &operator -= (const elementType &object)
		{
			m_table[Hash (keyName) % m_hashSize] -= HashItem_t (keyName, m_table[Hash (keyName) % m_hashSize].GetElementNumber ());

			return *this;
		}

		inline HashTable &operator += (const elementType &object)
		{
			m_table[Hash (keyName) % m_hashSize] += HashItem_t (keyName, m_table[Hash (keyName) % m_hashSize].GetElementNumber ());

			return *this;
		}*/
};

#endif	// ifndef HASH_TABLE_INCLUDED