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
// Queue.hpp
//
// Class: Queue
//
// Description: FIFO queue implemented with a container.
//				Simple data queue container.
// 				FIFO structure, can also be used as LIFO structure
//				can get head and tail, and add to front and back
//				also neat functions allowing you to remove nodes.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef QUEUE_INCLUDED
#define QUEUE_INCLUDED
//#if 0
template <typename elementType, typename indexType = unsigned short> class Queue
{
	//
	// Group: Type definitions.
	//
	public:
		typedef LinkedNode <elementType> LinkNodeType_t;

	//
	// Group: Protected members.
	//
	protected:
		// Typical single Node
		LinkNodeType_t *m_head, *m_tail;

		indexType       m_itemCount;

	//
	// Group: (Con/De)structors.
	//
	public:
		inline  Queue (void) : m_head (NULL), m_tail (NULL), m_itemCount (0u) { /*VOID*/ }	// constructor -- Must be public.
		inline ~Queue (void) { RemoveAll (); }	// destructor -- Must be public.

	//
	// Group: Functions.
	//
	public:
		// explicit delete function only now....
		inline void RemoveAll (void)
		{
			LinkNodeType_t *tempNode;

			while (m_head != NULL)
			{
				tempNode = m_head;

				m_head = m_head->GetNext ();

				delete tempNode;
			}

			m_head = m_tail = NULL;

			m_itemCount = 0u;
		}
		inline void DeleteAndPurgeAll (void)	// For basic 'LinkedNode'.
		{
			LinkNodeType_t *tempNode;

			while (m_head != NULL)
			{
				tempNode = m_head;

				m_head = m_head->GetNext ();

				delete tempNode->GetData ();
				delete tempNode;
			}

			m_head = m_tail = NULL;

			m_itemCount = 0u;
		}

		inline const bool IsEmpty (void) const
		{
			return m_head == NULL || m_tail == NULL;
		}

		inline       LinkNodeType_t *const  GetHead          (void)       { return m_head; }
		inline const LinkNodeType_t *const  GetHead          (void) const { return m_head; }
		inline       LinkNodeType_t       *&GetHeadReference (void)       { return m_head; }
		inline       LinkNodeType_t *const  GetTail          (void)       { return m_tail; }
		inline const LinkNodeType_t *const  GetTail          (void) const { return m_tail; }
		inline       LinkNodeType_t       *&GetTailReference (void)       { return m_tail; }

		inline void PushFront (const elementType &element)
		{
			LinkNodeType_t *const newNode (new LinkNodeType_t (element));

			if (m_head == NULL)
				m_tail = newNode;
			else
				newNode->InsertBefore (m_head);

			m_head = newNode;

			++m_itemCount;
		}

		inline void PushBack (const elementType &element)
		{
			LinkNodeType_t *const newNode (new LinkNodeType_t (element));

			if (IsEmpty ())
				m_head = newNode;
			else
				m_tail->InsertBefore (newNode);

			m_tail = newNode;

			++m_itemCount;
		}

		inline elementType &GetFirstElement (void)
		{
			return m_head->GetData ();
		}
		inline const elementType &GetFirstElement (void) const
		{
			return m_head->GetData ();
		}

		inline elementType PopFront (void)
		{
			InternalAssert (m_head != NULL);

			LinkNodeType_t tempNode (*m_head);

			delete m_head;

			m_head = tempNode.GetNext ();

			--m_itemCount;

			return tempNode.GetData ();
		}
		inline void DeleteFront (void)
		{
			InternalAssert (m_head != NULL);

			LinkNodeType_t *const headNext (m_head->GetNext ());

			delete m_head->GetData ();
			delete m_head;

			m_head = headNext;

			--m_itemCount;
		}

		inline void RemoveFront (void)
		{
			if (m_head == NULL)
			{
				// just set tail to null incase
				m_tail = NULL;

				// already empty
				return;
			}

			LinkNodeType_t *const tempNode (m_head);

			if (m_tail == m_head)
				m_tail = m_head = NULL;
			else
				m_head = m_head->GetNext ();

			delete tempNode;

			--m_itemCount;
		}
/*		inline void DeleteFront (void)
		{
			if (m_head == NULL)
			{
				// just set tail to null incase
				m_tail = NULL;

				// already empty
				return;
			}

			LinkNodeType_t *const tempNode (m_head);

			if (m_tail == m_head)
				m_tail = m_head = NULL;
			else
				m_head = m_head->GetNext ();

			delete tempNode->GetData ();
			delete tempNode;

			--m_itemCount;
		}
*/
		inline const bool IsMember (const elementType &element) const
		{
			for (LinkNodeType_t *tempNode (m_head); tempNode != NULL; tempNode = tempNode->GetNext ())
				if (tempNode->GetData () == element)
					return true;

			return false;
		}

		inline const bool FindAndRemoveElement (const elementType &element)
		{
			if (m_head == NULL)
				return false;

			LinkNodeType_t *tempNode (m_head);

			if (m_head->GetData () == element)
			{
				if (m_head == m_tail)
					m_tail = m_head = NULL;
				else
					m_head = m_head->GetNext ();

				delete tempNode;

				--m_itemCount;

				return true;
			}

			LinkNodeType_t *deleteNode;

			while (tempNode != NULL && tempNode->GetNext () != NULL)
			{
				if (tempNode->GetNext ()->GetData () == element)
				{
					deleteNode = tempNode->GetNext ();

					if (deleteNode == m_tail)
					{
						m_tail = tempNode;
						tempNode->SetNext (NULL);
					}
					else
						tempNode->SetNext (deleteNode->GetNext ());

					delete deleteNode;

					--m_itemCount;

					return true;
				}

				tempNode = tempNode->GetNext ();
			}

			return false;
		}
		inline const bool FindAndDeleteElement (const elementType &element)
		{
			if (m_head == NULL)
				return false;

			LinkNodeType_t *tempNode (m_head);

			if (m_head->GetData () == element)
			{
				if (m_head == m_tail)
					m_tail = m_head = NULL;
				else
					m_head = m_head->GetNext ();

				delete tempNode->GetData ();
				delete tempNode;

				--m_itemCount;

				return true;
			}

			LinkNodeType_t *deleteNode;

			while (tempNode != NULL && tempNode->GetNext () != NULL)
			{
				if (tempNode->GetNext ()->GetData () == element)
				{
					deleteNode = tempNode->GetNext ();

					if (deleteNode == m_tail)
					{
						m_tail = tempNode;
						tempNode->SetNext (NULL);
					}
					else
						tempNode->SetNext (deleteNode->GetNext ());

					delete deleteNode->GetData ();
					delete deleteNode;

					--m_itemCount;

					return true;
				}

				tempNode = tempNode->GetNext ();
			}

			return false;
		}

		//
		// Function: GetElementNumber
		//
		// Description: Gets the number of elements in linked list.
		//
		// Returns: Number of elements in list.
		//
		inline const indexType GetElementNumber (void) const { return m_itemCount; }

	//
	// Group: Operators.
	//
	public:
/*		inline Queue &operator =  (const Queue &other)
		{
			Assign (other);

			return *this;
		}

		inline Queue &operator -= (const elementType &object)
		{
			Pop (object);

			return *this;
		}
*/
		inline Queue &operator += (const elementType &object)
		{
			PushBack (object);

			return *this;
		}
/*		inline Queue &operator += (const Queue &other)
		{
			PushBack (other);

			return *this;
		}*/
};
//#endif	// if 0
#if 0
template <typename elementType, typename indexType = unsigned short, class container = Deque <elementType, indexType>> class Queue
{
	//
	// Group: Private members.
	//
	private:
		container m_container;	// the underlying container

	//
	// Group: Special points.
	//
	public:
		enum
		{
			ElementSize = sizeof (elementType),	// One queue element size.
			BadPosition = static_cast <indexType> (-1),	// Generic bad/missing length/position.
			MaximumPossibleSize = min/*Math::GetMinimumValueBetween <unsigned int>*/ (BadPosition, CoreToolLibrary::BadPosition / ElementSize / 2u)	// Maximum possible array size.
		};

	//
	// Group: (Con/De)structors.
	//
	public:
		inline Queue (void) : m_container () { /*VOID*/ }	// construct with empty container
		inline Queue (const Queue &right) : m_container (right.m_container) { /*VOID*/ }	// construct by copying 'right' container
		inline Queue (const container &other) : m_container (other) { /*VOID*/ }	// construct by copying specified container

	//
	// Group: Functions.
	//
	public:
		//
		// Function: Push
		//
		// Description: Appends new element to the end of queue.
		//
		// Parameters:
		//	object - Object to append.
		//	count - Number of element to insert.
		//
		// Returns: True if operation succeeded, false otherwise.
		//
		inline const bool Push (const elementType &object, const indexType count = 1u)
		{	// insert element at beginning
			return m_container.PushBack (object, count);
		}

		//
		// Function: Pop
		//
		// Description: Pops first element from queue.
		//
		// Returns: Object popped from the begin of queue.
		//
		inline const elementType /*&*/Pop (void)
		{	// erase element at end
			return m_container.PopBack ();
		}

		inline void Swap (Queue &right)
		{	// exchange contents with 'right' container
			m_container.Swap (right.m_container);
		}

		//
		// Function: GetFirstElement
		//
		// Description: Gets the first (bottom) element from the queue.
		//
		// Returns: First (bottom) random element reference.
		//
		inline elementType &GetFirstElement (void)
		{	// return last element of mutable queue
			return m_container.GetFirstElement ();
		}

		//
		// Function: GetFirstElement
		//
		// Description: Gets the first (bottom) constant element from the queue.
		//
		// Returns: First (bottom) random constant element reference.
		//
		inline const elementType &GetFirstElement (void) const
		{	// return last element of nonmutable queue
			return m_container.GetFirstElement ();
		}

		//
		// Function: GetLastElement
		//
		// Description: Gets the last (top) element from the queue.
		//
		// Returns: Last (top) element reference.
		//
		inline elementType &GetLastElement (void)
		{	// return last element of mutable queue
			return m_container.GetLastElement ();
		}

		//
		// Function: GetLastElement
		//
		// Description: Gets the last (top) constant element from the queue.
		//
		// Returns: Last (top) constant element reference.
		//
		inline const elementType &GetLastElement (void) const
		{	// return last element of nonmutable queue
			return m_container.GetLastElement ();
		}

		//
		// Function: GetMemoryUsage
		//
		// Description: Gets the using by this dynamic queue memory.
		//
		// Returns: How much memory this dynamic queue is using.
		//
		inline const unsigned int GetMemoryUsage (void) const
		{
			return m_container.GetMemoryUsage ();
		}

		//
		// Function: GetAllocatedSize
		//
		// Description: Gets allocated size of queue.
		//
		// Returns: Number of allocated items.
		//
		inline const indexType GetAllocatedSize (void) const { return m_container.GetAllocatedSize (); }

		//
		// Function: GetElementNumber
		//
		// Description: Gets real number currently in queue.
		//
		// Returns: Number of elements.
		//
		inline const indexType GetElementNumber (void) const { return m_container.GetElementNumber (); }

		//
		// Function: IsEmpty
		//
		// Description: Checks whether queue is empty.
		//
		// Returns: True if queue is empty, false otherwise.
		//
		inline const bool IsEmpty (void) const { return m_container.IsEmpty (); }

	//
	// Group: Operators.
	//
	public:
		inline Queue &operator = (const Queue &right)
		{	// assign by copying 'right' container
			m_container = other.m_container;

			return *this;
		}

		inline elementType       &operator [] (const indexType index)
		{
			return m_container[index];
		}

		inline const elementType &operator [] (const indexType index) const
		{
			return m_container[index];
		}
};
#endif	// if 0
#endif	// ifndef QUEUE_INCLUDED