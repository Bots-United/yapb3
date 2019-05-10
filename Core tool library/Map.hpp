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
// Map.hpp
//
// Class: Map
//
// Description: Represents associative map container.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef MAP_INCLUDED
#define MAP_INCLUDED

template <typename keyElementType, typename elementType, typename indexType = unsigned short> class Map
{
	//
	// Group: Members.
	//
	public:
		struct MapItem_t
		{
			keyElementType key;
			elementType    element;
		};

	//
	// Group: Private members.
	//
	private:
		struct HashItem_t
		{
			//
			// Group: Members.
			//
			public:
				unsigned int  m_index;
				HashItem_t   *m_next;

			//
			// Group: Functions.
			//
			public:
				inline HashItem_t (void)                                             : m_next (NULL)                  { /*VOID*/ }
				inline HashItem_t (const unsigned int index, HashItem_t *const next) : m_index (index), m_next (next) { /*VOID*/ }
		};

		unsigned int               m_hashSize;
		HashItem_t               **m_table;
		DynamicArray <MapItem_t>   m_mapTable;

	//
	// Group: Typedef's.
	//
	public:
		typedef keyElementType KeyType_t;
		typedef elementType    ElementType_t;
		typedef indexType      IndexType_t;

	//
	// Group: (Con/De)structors.
	//
	public:
		//
		// Function: Map
		//
		// Description: Default constructor for map container.
		//
		// Parameters:
		//	hashSize - Initial hash size.
		//
		inline Map (const unsigned int hashSize = 36u) : m_hashSize (hashSize), m_table (new HashItem_t *[hashSize])
		{
			// Reliability check.
			if (m_table == NULL)
				TerminateOnMalloc ();

			for (unsigned int index = 0u; index < hashSize; ++index)
				m_table[index] = NULL;
		}

		//
		// Function: ~Map
		//
		// Description: Default map container destructor.
		//
		// Parameters:
		//	hashSize - Initial hash size.
		//
		virtual inline ~Map (void)
		{
			RemoveAll ();

			delete [] m_table;
		}

	//
	// Group: Functions.
	//
	public:
		//
		// Function: IsExists
		//
		// Description: Checks whether specified element exists in container.
		//
		// Parameters:
		//	keyName - Key that should be looked up.
		//
		// Returns: True if key exists, false otherwise.
		//
		inline const bool IsExists (const keyElementType &keyName) const { return GetIndex (keyName) != BadPosition; }

		//
		// Function: SetupMap
		//
		// Description: Initializes map, if not initialized automatically.
		//
		// Parameters:
		//	hashSize - Initial hash size.
		//
		inline void SetupMap (const unsigned int hashSize)
		{
			m_hashSize = hashSize;
			m_table = new HashItem_t *[hashSize];

			// Reliability check.
			if (m_table == NULL)
				TerminateOnMalloc ();

			for (unsigned int index = 0u; index < hashSize; ++index)
				m_table[index] = NULL;
		}

		//
		// Function: IsEmpty
		//
		// Description: Checks whether map container is currently empty.
		//
		// Returns: True if no elements exists, false otherwise.
		//
		inline const bool IsEmpty (void) const { return m_mapTable.GetAllocatedSize () == 0u; }

		//
		// Function: GetSize
		//
		// Description: Retrieves size of the map container.
		//
		// Returns: Number of elements currently in map container.
		//
		inline const unsigned int GetSize (void) const { return m_mapTable.GetAllocatedSize (); }

		//
		// Function: GetKey
		//
		// Description: Gets the key object, by it's index.
		//
		// Parameters:
		//	index - Index of key.
		//
		// Returns: Object containing the key.
		//
		inline keyElementType &GetKey (const unsigned int index) { return m_mapTable[index].key; }

		//
		// Function: GetKey
		//
		// Description: Gets the constant key object, by it's index.
		//
		// Parameters:
		//	index - Index of key.
		//
		// Returns: Constant object containing the key.
		//
		inline const keyElementType &GetKey (const unsigned int index) const { return m_mapTable[index].key; }

		// Function: GetValue
		//
		// Description: Gets the element object, by it's index.
		//
		// Parameters:
		//	index - Index of element.
		//
		// Returns: Object containing the element.
		//
		inline elementType &GetValue (const unsigned int index) { return m_mapTable[index].element; }

		//
		// Function: GetValue
		//
		// Description: Gets the constant element object, by it's index.
		//
		// Parameters:
		//	index - Index of element.
		//
		// Returns: Constant object containing the element.
		//
		inline const elementType &GetValue (const unsigned int index) const { return m_mapTable[index].element; }

		//
		// Function: GetElements
		//
		// Description: Gets the all elements of container.
		//
		// Returns: Array of elements, containing inside container.
		//
		// See also: Class 'DynamicArray'.
		//
		inline DynamicArray <MapItem_t> &GetElements (void) { return m_mapTable; }

		//
		// Function: Find
		//
		// Description:
		//	Finds element by his key name.
		//
		// Parameters:
		//	keyName - Key name to be searched.
		//	element - Holder for element object.
		//
		// Returns: True if element found, false otherwise.
		//
		const bool Find (const keyElementType &keyName, elementType &element) const
		{
			const unsigned int index = GetIndex (keyName);

			if (index == BadPosition)
				return false;

			element = m_mapTable[index].element;

			return true;
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
		const bool Find (const keyElementType &keyName, elementType *&elementPointer) const
		{
			const unsigned int index = GetIndex (keyName);

			if (index == BadPosition)
				return false;

			elementPointer = &m_mapTable[index].element;

			return true;
		}

		//
		// Function: Remove
		//
		// Description: Removes element from container.
		//
		// Parameters:
		//	keyName - Key name of element, that should be removed.
		//
		// Returns: True if key was removed successfully, false otherwise.
		//
		const bool Remove (const keyElementType &keyName)
		{
			const unsigned int hashID = Hash <keyElementType> (keyName) % m_hashSize;
			HashItem_t *hashItem = m_table[hashID], *nextHash = NULL;

			while (hashItem != NULL)
			{
				if (m_mapTable[hashItem->m_index].key == keyName)
				{
					if (nextHash == NULL)
						m_table[hashID] = hashItem->m_next;
					else
						nextHash->m_next = hashItem->m_next;

					m_mapTable.RemoveAt (hashItem->m_index);

					delete hashItem;

					return true;
				}

				nextHash = hashItem;
				hashItem = hashItem->m_next;
			}

			return false;
		}

		//
		// Function: RemoveAll
		//
		// Description: Removes all elements from container.
		//
		void RemoveAll (void)
		{
			HashItem_t *pointer, *next;

			for (unsigned int index (m_hashSize); index > 0u; /* Empty */)
			{
				pointer = m_table[--index];

				while (pointer != NULL)
				{
					next = pointer->m_next;

					delete pointer;

					pointer = next;
				}

				m_table[index] = NULL;
			}

			m_mapTable.RemoveAll ();
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
		const unsigned int GetIndex (const keyElementType &keyName, const bool create = false)
		{
			const unsigned int hashID = Hash <keyElementType> (keyName) % m_hashSize;

			for (const HashItem_t *pointer = m_table[hashID]; pointer != NULL; pointer = pointer->m_next)
				if (m_mapTable[pointer->m_index].key == keyName)
					return pointer->m_index;

			if (create)
			{
				const unsigned int item = m_mapTable.GetAllocatedSize ();

				if (m_mapTable.SetSize (item + 1u))
				{
					m_table[hashID] = new HashItem_t (item, m_table[hashID]);
					m_mapTable[item].key = keyName;

					return item;
				}
			}

			return BadPosition;
		}

	//
	// Group: Operators.
	//
	public:
		inline elementType       &operator [] (const keyElementType &keyName)       { return m_mapTable[GetIndex (keyName, true)].element; }
		inline const elementType &operator [] (const keyElementType &keyName) const { return m_mapTable[GetIndex (keyName, true)].element; }
};

#endif	// ifndef MAP_INCLUDED