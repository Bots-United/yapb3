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
// PriorityQueue.hpp
//
// Class: PriorityQueue
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef PRIORITY_QUEUE_INCLUDED
#define PRIORITY_QUEUE_INCLUDED

// elementType is the type stored in the queue, it must include the priority
// The head of the list contains the element with GREATEST priority
// configure the LessFunction_t to get the desired queue order
template <typename elementType, typename indexType = unsigned short> class PriorityQueue
{
	//
	// Group: Types definitions.
	//
	public:
		// Less function typedef
		// Returns true if the first parameter is "less priority" than the second
		// Items that are "less priority" sort toward the tail of the queue
		typedef const bool (*LessFunction_t) (const elementType &, const elementType &);

		typedef elementType ElementType_t;
		typedef indexType   IndexType_t;

	//
	// Group: Protected members.
	//
	protected:
		DynamicArray <elementType, indexType> m_heap;
		LessFunction_t                        m_lessFunction;	// The comparator functor. (Used for sorting)

	//
	// Group: (Con/De)structors.
	//
	public:
		// constructor: lessFunction is required, but may be set after the constructor with
		// SetLessFunction
		inline PriorityQueue (const LessFunction_t lessFunction) :
			m_lessFunction (lessFunction)
		{ /*VOID*/ }

		inline PriorityQueue (const LessFunction_t lessFunction, const indexType size) :
			m_heap (size), m_lessFunction (lessFunction)
		{ /*VOID*/ }

		inline PriorityQueue (const LessFunction_t lessFunction, const elementType &input, const indexType count = 1u) :
			m_heap (input, count), m_lessFunction (lessFunction)
		{ /*VOID*/ }

		inline PriorityQueue (const LessFunction_t lessFunction, const elementType *const bufferPointer, const indexType count) :
			m_heap (bufferPointer, count), m_lessFunction (lessFunction)
		{ /*VOID*/ }

		virtual inline ~PriorityQueue (void) { /*VOID*/ }
/*
	//
	// Group: Protected functions.
	//
	protected:
		inline void Swap (const indexType index1, const indexType index2)
		{
			SwapElements (m_heap[index1], m_heap[index2]);
		}
*/
	//
	// Group: Functions.
	//
	public:
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
			return m_heap.IsInside (bufferPointer);
		}

		inline void Swap (PriorityQueue &right)
		{	// exchange contents with 'right'
			InternalAssert (this != &right);	// Same object? - assert!

			// Same allocator, swap control information....
			m_heap.Swap (right.m_heap);
			SwapElements (m_lessFunction, right.m_lessFunction);
		}

		//
		// Function: FreeExtra
		//
		// Description: Frees unused space.
		//
		// Returns: True if element is empty, false otherwise.
		//
		inline const bool FreeExtra (void)
		{
			return m_heap.FreeExtra ();
		}

		// gets particular elements
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

			return m_heap.GetRandomElement ();
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

			return m_heap.GetRandomElement ();
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

			return m_heap.GetRandomElementIndex ();
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

			return m_heap.GetFirstElement ();
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

			return m_heap.GetFirstElement ();
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

			return m_heap.GetFirstElementIndex ();
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

			return m_heap.GetLastElement ();
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

			return m_heap.GetLastElement ();
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

			return m_heap.GetLastElementIndex ();
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
			return Find (element, startIndex, GetElementNumber () - startIndex);
		}
		inline const indexType Find (const elementType &element, const indexType startIndex, const indexType count) const
		{
			// Reliability check.
			InternalAssert (startIndex <= GetElementNumber () && count <= GetElementNumber () - startIndex);

			return m_heap.Find (element, startIndex, count);
		}

		inline const indexType BinaryFind (const elementType &element, const indexType startIndex = 0u) const
		{
			// Reliability check.
			InternalAssert (!IsEmpty ());

			return BinaryFind (element, startIndex, GetElementNumber () - startIndex);
		}

		inline const indexType BinaryFind (const elementType &element, const indexType startIndex, const int count) const
		{
			// Reliability check.
			InternalAssert (startIndex < GetElementNumber () && count > 0 && count <= GetElementNumber () - startIndex);

			return m_heap.BinaryFind (element, startIndex, count);
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
			return IsMember (element);
		}

		// O(lgn) to rebalance heap
		inline void Push (const elementType &element)
		{	// insert value in priority order
/*			m_heap += element;

			for (indexType child (m_heap.GetLastElementIndex ()), parent; child > 0u; child = parent)
			{
				parent = (child - 1u) / 2u;

				if (m_lessFunction (m_heap[parent], m_heap[child]))
					break;

				// swap with parent and repeat
				Swap (parent, child);
			}/*/
			for (indexType index (0u); index < GetElementNumber (); ++index)
				if (!m_lessFunction (m_heap[index], element))
				{
					m_heap.Insert (index, element);

					return;
				}

			// Priority queue is empty or hasn't items with priority lower, that a new element, yet....
			m_heap += element;
		}
		// O(lgn) to rebalance heap
		inline void Push (const PriorityQueue &other)
		{	// insert value in priority order
/*			m_heap += other;

			for (indexType child (m_heap.GetLastElementIndex ()), parent; child > 0u; child = parent)
			{
				parent = (child - 1u) / 2u;

				if (m_lessFunction (m_heap[parent], m_heap[child]))
					break;

				// swap with parent and repeat
				Swap (parent, child);
			}*/
			for (indexType index (0u); index < other.GetElementNumber (); ++index)
				Push (other[index]);
		}

		//
		// Function: PopFront
		//
		// Description: Pops first element from array.
		//
		// Returns: Object popped from the begin of array.
		//
		// O(lgn) to rebalance heap
		inline const elementType /*&*/PopFront (void)
		{	// removes the smallest item from the priority queue
			// Reliability check.
			InternalAssert (!IsEmpty ());

			return m_heap.PopFront ();
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

			m_heap.PopFront (count);
		}

		inline void DeleteFront (void)
		{	// deletes the smallest item from the priority queue
			// Reliability check.
			InternalAssert (!IsEmpty ());

			return m_heap.DeleteFront ();
		}

		//
		// Function: PopBack
		//
		// Description: Pops last element from array.
		//
		// Returns: Object popped from the end of array.
		//
		// O(lgn) to rebalance heap
		inline const elementType /*&*/PopBack (void)
		{	// removes the highest item from the priority queue
			// Reliability check.
			InternalAssert (!IsEmpty ());

			return m_heap.PopBack ();
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

			m_heap.PopBack (count);
		}

		inline void DeleteBack (void)
		{
			// Reliability check.
			InternalAssert (!IsEmpty ());

			m_heap.DeleteBack ();
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
		// O(lgn) to rebalance heap
		inline void Pop (const elementType &element)
		{
			// Reliability check.
			InternalAssert (!IsEmpty ());

			m_heap -= element;
		}

		// Sets the less function
		inline void SetLessFunction (const LessFunction_t lessFunction) { m_lessFunction = lessFunction; }

		//
		// Function: GetElementNumber
		//
		// Description: Gets real number currently in array.
		//
		// Returns: Number of elements.
		//
		// Returns the count of elements in the queue
		inline const indexType GetElementNumber (void) const { return m_heap.GetElementNumber (); }

		//
		// Function: IsEmpty
		//
		// Description: Checks whether array is empty.
		//
		// Returns: True if array is empty, false otherwise.
		//
		inline const bool      IsEmpty          (void) const { return m_heap.IsEmpty (); }

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
			InternalAssert (count < m_heap.MaximumPossibleSize);

			return m_heap.Assign (input, count);
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
			if (!SetSize (count, false))
			{
				AddLogEntry (true, LogLevel_Error, false, "Couldn't allocate %.2f kilobytes in array for assigning %s!", count * sizeof (elementType) / 1024.0f, count == 1u ? "new element" : "%u elements", count);

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
		inline const bool Assign (const DynamicArray <elementType, indexType> &inputArray, const indexType startIndex, const indexType count)
		{
			// Reliability check ('startIndex' off end).
			InternalAssert (startIndex <= inputArray.GetElementNumber () && count <= inputArray.GetElementNumber () - startIndex);

			if (this == &inputArray)
			{
				// Subarray.

				Remove (0u, startIndex);
				Remove (startIndex + count);

				return true;
			}
			else if (SetSize (count, false))	// Make room and assign new stuff.
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
		// Description: Assigns the array to existing buffer. (startIndex, inputArray.GetElementNumber () - startIndex)
		//
		// Parameters:
		//	inputArray - Array buffer to assign.
		//	startIndex - Starting position of the subarray of the input array that forms the new content.
		//
		// Returns: True if input array is assigned, false otherwise.
		//
		inline const bool Assign (const DynamicArray <elementType, indexType> &inputArray, const indexType startIndex = 0u)
		{
			return Assign (inputArray, startIndex, inputArray.GetElementNumber () - startIndex);
		}

		inline const bool Assign (const PriorityQueue &inputPriorityQueue, const indexType startIndex, const indexType count)
		{
			// Reliability check ('startIndex' off end).
			InternalAssert (startIndex <= inputPriorityQueue.GetElementNumber () && count <= inputPriorityQueue.GetElementNumber () - startIndex);

			return m_heap.Assign (inputPriorityQueue.m_heap, startIndex, count);
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
		// O(lgn) to rebalance the heap
		inline void Remove (const indexType sourceIndex, const indexType count)
		{
			// Checks for bounds....
			InternalAssert (sourceIndex < GetElementNumber () && count <= GetElementNumber () - sourceIndex);

			m_heap.Remove (sourceIndex, count);
/*! DO WE NEED THIS?????????????????!!!!!!!!!!!!!!!!!!
			if (IsEmpty ())
				return;

			const indexType half (count / 2u);
			indexType larger (index);

			while (index < half)
			{
				indexType child ((index + 1u) * 2u - 1u);	// if we wasted an element, this math would be more compact (1 based array)
				InternalAssert (child == (index * 2u) + 1u);

					// Item has been filtered down to its proper place, terminate.
				if (child < count && m_lessFunction (m_heap[child], m_heap[index]))
				{
					// mark the potential swap and check the other child
					larger = child;
				}

				// go to sibling
				// If this child is larger, swap it instead
				if (++child < count && m_lessFunction (m_heap[child], m_heap[larger]))
					larger = child;

				if (larger == index)
					break;

				// swap with the larger child
				Swap (index, larger);

				index = larger;
			}*/
		}

		//
		// Function: Remove
		//
		// Description: Deletes elements from array. (sourceIndex, GetElementNumber () - sourceIndex)
		//
		// Parameters:
		//	sourceIndex - Starting position of elements to remove.
		//
		// Returns: True if the deleting passed successfully, false otherwise.
		//
		inline void Remove (const indexType sourceIndex = 0u)
		{
			Remove (sourceIndex, GetElementNumber () - sourceIndex);
		}

		// Delete the element.
		inline void Delete (const indexType sourceIndex, const indexType count)
		{
			// Checks for bounds....
			InternalAssert (sourceIndex < GetElementNumber () && count <= GetElementNumber () - sourceIndex);

			m_heap.Delete (sourceIndex, count);
		}
		inline void Delete (const indexType sourceIndex = 0u)
		{
			Delete (sourceIndex, GetElementNumber () - sourceIndex);
		}

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
		inline const bool SetSize (const indexType newSize, const bool keepData = true)
		{
			m_heap.SetSize (newSize, keepData);
		}

		//
		// Function: MakeEmpty
		//
		// Description: Empties the array without memory deallocation.
		//
		// doesn't deallocate memory
		inline void MakeEmpty (void) { m_heap.MakeEmpty (); }

		//
		// Function: RemoveAll
		//
		// Description: Destroys array object, and all elements.
		//
		// Memory deallocation
		inline void RemoveAll (void) { m_heap.RemoveAll (); }

		// Destruct all the elements.
		inline void DestructAll (void)
		{
			m_heap.DestructAll ();
		}

		// Delete all the elements.
		inline void DeleteAll (void)
		{
			// Free all used memory....
			m_heap.DeleteAll ();
		}

		// Delete all the elements then call Purge.
		inline void DeleteAndPurgeAll (void)
		{
			// Free all used memory....
			m_heap.DeleteAndPurgeAll ();
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
			return m_heap.GetMemoryUsage ();
		}

		//
		// Function: GetAllocatedSize
		//
		// Description: Gets allocated size of array.
		//
		// Returns: Number of allocated items.
		//
		inline const indexType GetAllocatedSize (void) const { return m_heap.GetAllocatedSize (); }

		//
		// Function: GetData
		//
		// Description: Gets the pointer to all elements in array.
		//
		// Returns: Pointer to object list.
		//
		inline elementType *const GetData (void) { return m_heap.GetData (); }

		//
		// Function: GetData
		//
		// Description: Gets the constant pointer to all element in array.
		//
		// Returns: Constant pointer to object list.
		//
		inline const elementType *const GetData (void) const { return m_heap.GetData (); }

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
		inline PriorityQueue &operator =  (const elementType &input)
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
		inline PriorityQueue &operator =  (const PriorityQueue &other)
		{
			Assign (other);

			return *this;
		}

		inline PriorityQueue &operator -= (const elementType &object)
		{
			Pop (object);

			return *this;
		}

		inline PriorityQueue &operator += (const elementType &object)
		{
			Push (object);

			return *this;
		}

		inline PriorityQueue &operator += (const PriorityQueue &other)
		{
			Push (other);

			return *this;
		}

		// Following five operators are equal's PriorityQueue::GetData() function.
		inline operator       elementType *const    (void)       { return GetData (); }
		inline operator       elementType *const    (void) const { return GetData (); }	// Needed too :{O (Half-Life engine so stupid....)
		inline operator const elementType *const    (void) const { return GetData (); }

		inline       elementType *const operator () (void)       { return GetData (); }
		inline const elementType *const operator () (void) const { return GetData (); }

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
			InternalAssert (index < GetElementNumber ());

			return m_heap[index];
		}
		inline const elementType &operator [] (const indexType index) const
		{
			// Reliability check.
			InternalAssert (index < GetElementNumber ());

			return m_heap[index];
		}

		// Equality operation.
		inline const bool operator == (const PriorityQueue &right) const
		{
			return m_heap == right.m_heap;
		}

		// Inequality operation.
		inline const bool operator != (const PriorityQueue &right) const
		{
			return !(*this == right);
		}

		inline const bool operator < (const PriorityQueue &right) const
		{
			return m_heap < right.m_heap;
		}
		inline const bool operator > (const PriorityQueue &right) const
		{
			return right < *this;
		}
		inline const bool operator >= (const PriorityQueue &right) const
		{
			return !(*this < right);
		}
		inline const bool operator <= (const PriorityQueue &right) const
		{
			return !(right < *this);
		}
};

#endif	// ifndef PRIORITY_QUEUE_INCLUDED