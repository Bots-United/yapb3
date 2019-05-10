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
// Stack.hpp
//
// Class: Stack
//
// Description: LIFO queue implemented with a container.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef STACK_INCLUDED
#define STACK_INCLUDED

template <typename elementType, typename indexType = unsigned short, typename nodeType = LinkedNode <elementType>> class Stack
{
	//
	// Group: Type definitions.
	//
	public:
		typedef elementType ElementType_t;
		typedef indexType   IndexType_t;
		typedef nodeType    NodeType_t;

	//
	// Group: Protected members.
	//
	protected:
		NodeType_t *m_first;	// access to the left end of the list

		indexType   m_itemCount;

	//
	// Group: (Con/De)structors.
	//
	public:
		inline Stack (void) : m_first (NULL), m_itemCount (0u) { /*VOID*/ }
		inline Stack (const Stack &right) : m_first (NULL), m_itemCount (0u) { Assign (right); }	// construct by copying 'right' container

		virtual inline ~Stack (void) { RemoveAll (); }

	//
	// Group: Functions.
	//
	public:
		inline void Assign (const Stack &inputStack)
		{
			// Reliability checks.
			InternalAssert (!inputStack.IsEmpty ());

			RemoveAll ();

			Push (inputStack);
		}
		inline void Assign (const Stack &inputStack, const indexType count)
		{
			// Reliability checks.
			InternalAssert (count <= inputStack.GetElementNumber ());
			InternalAssert (count > 0u);

			RemoveAll ();

			Push (inputStack, count);
		}

		virtual inline void Push (const elementType &element)
		{
			m_first = new NodeType_t (element, m_first);

			#if defined _DEBUG
				// Reliability check.
				if (m_first == NULL)
				{
					AddLogEntry (true, LogLevel_Error, false, "Couldn't allocate %u bytes for new stack node with element type '%s'!", sizeof (*m_first), typeid (elementType).name ());

					return;
				}
			#endif	// if defined _DEBUG

			++m_itemCount;
		}
		inline void Push (const Stack &other)
		{
			for (const NodeType_t *node (other.m_first); node != NULL; node = node->GetNext ())
				Push (node->GetData ());
		}
		inline void Push (const Stack &other, indexType count)
		{
			// Reliability checks.
			InternalAssert (count <= other.GetElementNumber ());

			for (const NodeType_t *node (other.m_first); count > 0u; node = node->GetNext (), --count)
			{
				// Reliability check.
				InternalAssert (node != NULL);

				Push (node->GetData ());
			}
		}

		inline elementType Pop (void)
		{
			// Reliability check.
			InternalAssert (!IsEmpty ());

			NodeType_t tempNode (*m_first);

			delete m_first;

			m_first = tempNode.GetNext ();

			--m_itemCount;

			return tempNode.GetData ();
		}
		inline const bool Pop (const elementType &element)
		{
			for (NodeType_t **node = &m_first; *node != NULL; node = &(*node)->GetNextReference ())
			{
				if ((*node)->GetData () != element)
					continue;

				Pop (*node);

				return true;
			}

			return false;
		}
		inline NodeType_t *const Pop (NodeType_t *&nodePointer)
		{
			// Reliability check.
			InternalAssert (!IsEmpty ());
			InternalAssert (nodePointer != NULL);

			NodeType_t *const tempNextNode (nodePointer->GetNext ());

			delete nodePointer;

			--m_itemCount;

			return nodePointer = tempNextNode;
		}
		inline void RemoveUpToNode (const NodeType_t *const node)
		{
			// Reliability check.
			InternalAssert (!IsEmpty ());
			InternalAssert (node != NULL);

			while (m_first != node)
				Pop (m_first);
		}

		//
		// Function: RemoveAll
		//
		// Description: Removes all elements from list, and destroys them.
		//
		void RemoveAll (void)
		{
			NodeType_t *nodeToDelete;

			while (m_first != NULL)
			{
				nodeToDelete = m_first;

				m_first = m_first->GetNext ();

				delete nodeToDelete;
			}

			m_itemCount = 0u;
		}
		// Delete all the element data. (@warning ONLY DATA!!!)
		inline void DeleteAll (void)
		{
			NodeType_t *node;

			while (m_first != NULL)
			{
				node = m_first;

				m_first = m_first->GetNext ();

				delete node->GetData ();
			}
		}
		// Delete all the elements and nodes.
		inline void DeleteAndPurgeAll (void)
		{
			NodeType_t *nodeToDelete;

			while (m_first != NULL)
			{
				nodeToDelete = m_first;

				m_first = m_first->GetNext ();

				delete nodeToDelete->GetData ();
				delete nodeToDelete;
			}

			m_itemCount = 0u;
		}

		inline const bool IsMember (const elementType &element) const
		{
			for (const NodeType_t *node (m_first); node != NULL; node = node->GetNext ())
				if (node->GetData () == element)
					return true;

			return false;
		}

		inline elementType &GetLastElement (void)
		{
			// Reliability check.
			InternalAssert (!IsEmpty ());

			NodeType_t *node (m_first);

			while (node->GetNext () != NULL)
				node = node->GetNext ();

			return node->GetData ();
		}
		inline const elementType &GetLastElement (void) const
		{
			// Reliability check.
			InternalAssert (!IsEmpty ());

			const NodeType_t *node (m_first);

			while (node->GetNext () != NULL)
				node = node->GetNext ();

			return node->GetData ();
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
		// Function: IsEmpty
		//
		// Description: Checks whether linked list is empty.
		//
		// Returns: True if linked list is empty, false otherwise.
		//
		inline const bool IsEmpty (void) const { return m_itemCount == 0u; }

		inline       NodeType_t *const   GetFirst          (void)                             { return m_first; }
		inline       NodeType_t        *&GetFirstReference (void)                             { return m_first; }
		inline const NodeType_t *const   GetFirst          (void)                       const { return m_first; }
		inline       void                SetFirst          (NodeType_t *const newFirst)       { m_first = newFirst; }

		//
		// Function: GetFirstElement
		//
		// Description: Gets the first (bottom) element from the stack.
		//
		// Returns: First (bottom) random element reference.
		//
		inline elementType &GetFirstElement (void)
		{	// return last element of mutable stack
			// Reliability check.
			InternalAssert (!IsEmpty ());

			return m_first->GetData ();
		}

		//
		// Function: GetFirstElement
		//
		// Description: Gets the first (bottom) constant element from the stack.
		//
		// Returns: First (bottom) random constant element reference.
		//
		inline const elementType &GetFirstElement (void) const
		{	// return last element of nonmutable stack
			// Reliability check.
			InternalAssert (!IsEmpty ());

			return m_first->GetData ();
		}

	//
	// Group: Operators.
	//
	public:
		inline Stack &operator =  (const Stack &other)
		{
			Assign (other);

			return *this;
		}

		inline Stack &operator -= (const elementType &object)
		{
			Pop (object);

			return *this;
		}

		inline Stack &operator += (const elementType &object)
		{
			Push (object);

			return *this;
		}
		inline Stack &operator += (const Stack &other)
		{
			Push (other);

			return *this;
		}
};
template <typename elementType, typename indexType/* = unsigned short*/> class Stack <elementType, indexType, Link>
{
	//
	// Group: Type definitions.
	//
	public:
		typedef elementType ElementType_t;
		typedef indexType   IndexType_t;
		typedef Link        NodeType_t;

	//
	// Group: Protected members.
	//
	protected:
		NodeType_t *m_first;	// access to the left end of the list

		indexType   m_itemCount;

	//
	// Group: (Con/De)structors.
	//
	public:
		inline Stack (void) : m_first (NULL), m_itemCount (0u) { /*VOID*/ }

		virtual inline ~Stack (void) { RemoveAll (); }

	public:
		inline       NodeType_t *const   GetFirst          (void)                             { return m_first; }
		inline       NodeType_t        *&GetFirstReference (void)                             { return m_first; }
		inline const NodeType_t *const   GetFirst          (void)                       const { return m_first; }
		inline       void                SetFirst          (NodeType_t *const newFirst)       { m_first = newFirst; }

		inline void Push (NodeType_t *const newObject)
		{
			newObject->SetNext (m_first);

			m_first = newObject;

			++m_itemCount;
		}
		inline const bool Pop (NodeType_t *&element)
		{
			NodeType_t *const tempNode (element->GetNext ());

			delete element;

			element = tempNode;

			--m_itemCount;

			return true;
		}

		void RemoveAll (void)
		{
			NodeType_t *nodeToDelete;

			while (m_first != NULL)
			{
				nodeToDelete = m_first;

				m_first = m_first->GetNext ();

				delete nodeToDelete;
			}

			m_itemCount = 0u;
		}
		inline void DeleteAll (void) { /*! @note there is nothing to delete.... */ }
		inline void DeleteAndPurgeAll (void) { RemoveAll (); }	// Same as RemoveAll().... (b/w compatibility)

		inline const bool IsEmpty (void) const { return m_itemCount == 0u; }

	public:
		inline Stack &operator += (NodeType_t *const newObject)
		{
			Push (newObject);

			return *this;
		}
};
template <typename elementType, typename indexType = unsigned short> class PriorityStack : public Stack <elementType, indexType>
{
	//
	// Group: Type definitions.
	//
	public:
		// Less function typedef
		// Returns true if the first parameter is "less priority" than the second
		// Items that are "less priority" sort toward the head of the stack
		typedef const bool (*LessFunction_t) (const elementType &, const elementType &);

	//
	// Group: Protected members.
	//
	protected:
		LessFunction_t m_lessFunction;	// The comparator functor. (Used for sorting)

	//
	// Group: (Con/De)structors.
	//
	public:
		inline PriorityStack (const LessFunction_t lessFunction) : Stack (), m_lessFunction (lessFunction) { /*VOID*/ }
		inline PriorityStack (const PriorityStack &right) : Stack (right), m_lessFunction (right.m_lessFunction) { /*VOID*/ }	// construct by copying 'right' container

		virtual inline ~PriorityStack (void) { /*VOID*/ }

	//
	// Group: Functions.
	//
	public:
		inline void Push (const elementType &element)
		{
			NodeType_t *const newNode (new NodeType_t (element));

			#if defined _DEBUG
				// Reliability check.
				if (newNode == NULL)
				{
					AddLogEntry (true, LogLevel_Error, false, "Couldn't allocate %u bytes for new stack node with element type '%s'!", sizeof (*newNode), typeid (elementType).name ());

					return;
				}
			#endif	// if defined _DEBUG

			if (m_first == NULL)
			{
				m_first = newNode;

				++m_itemCount;

				return;
			}

			if (m_lessFunction (element, m_first->GetData ()))
			{
				newNode->InsertBefore (m_first);
				m_first = newNode;
			}
			else
			{
				NodeType_t *p (m_first);
				NodeType_t *t (m_first->GetNext ());

				while (t != NULL)
				{
					if (m_lessFunction (element, t->GetData ()))
					{
						p->InsertBefore (newNode);
						newNode->InsertBefore (t);

						break;
					}

					p = t;
					t = t->GetNext ();
				}

				if (t == NULL)
					p->InsertBefore (newNode);
			}

			++m_itemCount;
		}

		// Sets the less function
		inline void SetLessFunction (const LessFunction_t lessFunction) { m_lessFunction = lessFunction; }
};
#if 0
template <typename elementType, typename indexType = unsigned short, class container = Deque <elementType, indexType>> class Stack
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
			ElementSize = sizeof (elementType),	// One stack element size.
			BadPosition = static_cast <indexType> (-1),	// Generic bad/missing length/position.
			MaximumPossibleSize = min/*Math::GetMinimumValueBetween <unsigned int>*/ (BadPosition, CoreToolLibrary::BadPosition / ElementSize / 2u)	// Maximum possible array size.
		};

	//
	// Group: (Con/De)structors.
	//
	public:
		inline Stack (void) : m_container () { /*VOID*/ }	// construct with empty container
		inline Stack (const Stack &right) : m_container (right.m_container) { /*VOID*/ }	// construct by copying 'right' container
		inline Stack (const container &other) : m_container (other) { /*VOID*/ }	// construct by copying specified container

	//
	// Group: Functions.
	//
	public:
		//
		// Function: Push
		//
		// Description: Appends new element to the end of stack.
		//
		// Parameters:
		//	object - Object to append.
		//	count - Number of element to insert.
		//
		// Returns: True if operation succeeded, false otherwise.
		//
		inline const bool Push (const elementType &object, const indexType count = 1u)
		{	// insert element at end
			return m_container.PushBack (object, count);
		}

		//
		// Function: Pop
		//
		// Description: Pops first element from stack.
		//
		// Returns: Object popped from the begin of stack.
		//
		inline const elementType /*&*/Pop (void)
		{	// erase last element
			return m_container.PopBack ();
		}

		inline void Swap (Stack &right)
		{	// exchange contents with 'right' container
			m_container.Swap (right.m_container);
		}

		//
		// Function: GetFirstElement
		//
		// Description: Gets the first (bottom) element from the stack.
		//
		// Returns: First (bottom) random element reference.
		//
		inline elementType &GetFirstElement (void)
		{	// return last element of mutable stack
			return m_container.GetLastElement ();
		}

		//
		// Function: GetFirstElement
		//
		// Description: Gets the first (bottom) constant element from the stack.
		//
		// Returns: First (bottom) random constant element reference.
		//
		inline const elementType &GetFirstElement (void) const
		{	// return last element of nonmutable stack
			return m_container.GetLastElement ();
		}

		//
		// Function: GetMemoryUsage
		//
		// Description: Gets the using by this dynamic stack memory.
		//
		// Returns: How much memory this dynamic stack is using.
		//
		inline const unsigned int GetMemoryUsage (void) const
		{
			return m_container.GetMemoryUsage ();
		}

		//
		// Function: GetAllocatedSize
		//
		// Description: Gets allocated size of stack.
		//
		// Returns: Number of allocated items.
		//
		inline const indexType GetAllocatedSize (void) const { return m_container.GetAllocatedSize (); }

		//
		// Function: GetElementNumber
		//
		// Description: Gets real number currently in stack.
		//
		// Returns: Number of elements.
		//
		inline const indexType GetElementNumber (void) const { return m_container.GetElementNumber (); }

		//
		// Function: IsEmpty
		//
		// Description: Checks whether stack is empty.
		//
		// Returns: True if stack is empty, false otherwise.
		//
		inline const bool IsEmpty (void) const { return m_container.IsEmpty (); }

	//
	// Group: Operators.
	//
	public:
/*		inline Stack &operator = (const container &right)
		{
			if (&m_container == &right)
				return *this;

			RemoveAll ();

			for (indexType index = 0u; index < right.m_sectors.GetElementNumber (); ++index)
			{
				m_sectors += new elementType[SectorSize];

				for (indexType i = 0u; i < SectorSize; ++i)
					m_sectors.GetLastElement ()[i] = m_sectors[index][i];
			}

			m_itemCount = right.m_itemCount;

			return *this;
		}
*/
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
#endif	// ifndef STACK_INCLUDED