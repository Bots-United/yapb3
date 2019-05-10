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
// LinkedList.hpp
//
// Class: LinkedList
//
// Description: Simple linked list container.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef LINKED_LIST_INCLUDED
#define LINKED_LIST_INCLUDED
#if 0
template <typename elementType> class LinkedList
{
	//
	// Group: Members.
	//
	public:
		// typical node (one list element)
		class Node
		{
			//
			// Group: Members.
			//
			public:
				// one cell of data to store
				elementType *m_data;

				// previous and next elements
				Node        *m_prev;
				Node        *m_next;

			//
			// Group: (Con/De)structors.
			//
			public:
				inline Node (elementType *const data = NULL) : m_data (data), m_prev (NULL), m_next (NULL) { /*VOID*/ }
		};

	//
	// Group: Private members.
	//
	private:
		// access to the left and right ends of the list
		Node         *m_first;
		Node         *m_last;

		unsigned int  m_itemCount;

	//
	// Group: (Con/De)structors.
	//
	public:
		inline LinkedList (void) : m_first (NULL), m_last (NULL), m_itemCount (0u) { /*VOID*/ }
		virtual inline ~LinkedList (void) { /*VOID*/ }

	//
	// Group: Private functions.
	//
	private:
		//
		// Function: GetIterator
		//
		// Description: Utility node iterator.
		//
		// Parameters:
		//	entry - Previous entry.
		//
		// Returns: Node pointer.
		//
		inline Node *GetIterator (const Node *const entry = NULL) const
		{
			if (entry == NULL)
				return m_first;

			return entry->m_next;
		}

		//
		// Function: FindNode
		//
		// Description: Find node by it's entry.
		//
		// Parameters:
		//	entry - Entry to search.
		//
		// Returns: Node pointer.
		//
		inline Node *FindNode (const elementType *const entry) const
		{
			Node *iterator = NULL;

			while ((iterator = GetIterator (iterator)) != NULL && iterator->m_data == entry)
				return iterator;

			return NULL;
		}

	//
	// Group: Functions.
	//
	public:
		//
		// Function: Destory
		//
		// Description: Resets list to empty state by abandoning contents.
		//
		inline void Destory (void)
		{
			m_first = NULL;
			m_last = NULL;
			m_itemCount = 0u;
		}

		//
		// Function: GetElementNumber
		//
		// Description: Gets the number of elements in linked list.
		//
		// Returns: Number of elements in list.
		//
		inline const unsigned int GetElementNumber (void) const { return m_itemCount; }

		//
		// Function: GetFirst
		//
		// Description: Gets the first list entry. NULL in case list is empty.
		//
		// Returns: First list entry.
		//
		inline elementType *GetFirst (void) const
		{
			if (m_first == NULL)
				return NULL;

			return m_first->m_data;
		}

		//
		// Function: GetLast
		//
		// Description: Gets the last list entry. NULL in case list is empty.
		//
		// Returns: Last list entry.
		//
		inline elementType *GetLast (void) const
		{
			if (m_last == NULL)
				return NULL;

			return m_last->m_data;
		}

		//
		// Function: GetNext
		//
		// Description: Gets the next element from linked list.
		//
		// Parameters:
		//	current - Current node.
		//
		// Returns: Node data.
		//
		inline elementType *GetNext (const elementType *const current) const
		{
			if (current == NULL)
				return GetFirst ();

			Node *next = FindNode (current)->m_next;

			if (next == NULL)
				return NULL;

			return next->m_data;
		}

		//
		// Function: GetPrev
		//
		// Description: Gets the previous element from linked list.
		//
		// Parameters:
		//	current - Current node.
		//
		// Returns: Node data.
		//
		inline elementType *GetPrev (const elementType *const current) const
		{
			if (current == NULL)
				return GetLast ();

			Node *prev = FindNode (current)->m_prev;

			if (prev != NULL)	// I THINK IT MUST BE '==', OR I NUB?
				return NULL;

			return prev->m_prev;
		}

		//
		// Function: Push
		//
		// Description: Adds item to linked list.
		//
		// Parameters:
		//	entry - Node that should be inserted in linked list.
		//	next - Next node to be inserted into linked list.
		//
		// Returns: Item if operation success, NULL otherwise.
		//
		void Push (elementType *entry, const elementType *const next = NULL)
		{
			Node *newNode = new Node (entry), *nextNode = FindNode (next), *prevNode = NULL;

			if (nextNode == NULL)
			{
				prevNode = m_last;
				m_last = newNode;
			}
			else
			{
				prevNode = nextNode->m_prev;
				nextNode->m_prev = newNode;
			}

			if (prevNode == NULL)
				m_first = newNode;
			else
				prevNode->m_next = newNode;

			newNode->m_next = nextNode;
			newNode->m_prev = prevNode;
		}

		//
		// Function: IsEmpty
		//
		// Description: Checks whether linked list is empty.
		//
		// Returns: True if linked list is empty, false otherwise.
		//
		inline const bool IsEmpty (void) const { return m_first == NULL; }

		void PushFront (const elementType &element)
		{
			// create separate node
			Node *newNode = new Node (entry);

			if (newNode == NULL)
				TerminateOnMalloc ();

			// If the list is empty...:
			if (m_first == NULL)
				m_last = newNode;	// new first node is also first;
			else
				m_first->m_prev = newNode;	// else we connect first node with new.

			newNode->m_next = m_first;	// Connect new node with first,

			m_first = newNode;	// and make new - first.
		}

		void PushBack (const elementType &element)
		{
			// create separate node
			Node *newNode = new Node (entry);

			if (newNode == NULL)
				TerminateOnMalloc ();

			// If the list is empty...:
			if (m_first == NULL)
				m_first = newNode;	// new last node is also first;
			else
				m_last->m_next = newNode;	// else we connect last node with new.

			newNode->m_prev = m_last;	// Connect new node with last,

			m_last = newNode;	// and make new - last.
		}

		//
		// Function: Push
		//
		// Description: Adds item to linked list (as reference).
		//
		// Parameters:
		//	entry - Node that should be inserted in linked list.
		//	next - Next node to be inserted into linked list.
		//
		// Returns: Item if operation success, NULL otherwise.
		//
		inline void Push (elementType &entry, const elementType *const next = NULL)
		{
			elementType *newEntry = new elementType ();

			*newEntry = entry;

			Push (newEntry, next);
		}

		//
		// Function: Pop
		//
		// Description: Removes element from linked list.
		//
		// Parameters:
		//	entry - Element that should be moved out of list.
		//
		void Pop (const elementType *const entry)
		{
			Node *entryNode = FindNode (entry);

			// Reliability check.
			InternalAssert (entryNode != NULL);

			if (entryNode->m_prev == NULL)
				m_first = entryNode->m_next;
			else
				entryNode->m_prev->m_next = entryNode->m_next;

			if (entryNode->m_next == NULL)
				m_last = entryNode->m_prev;
			else
				entryNode->m_next->m_prev = entryNode->m_prev;

			delete entryNode;
		}

		//
		// Function: Allocate
		//
		// Description: Inserts item into linked list, and allocating it automatically.
		//
		// Parameters:
		//	next - Optional next element.
		//
		// Returns: Item that was inserted.
		//
		inline void Allocate (const elementType *const next = NULL) const
		{
			elementType *entry = new elementType ();

			if (entry == NULL)
				TerminateOnMalloc ();

			Push (entry, next);
		}

		//
		// Function: Remove
		//
		// Description: Removes element from list, and destroys it.
		//
		// Parameters:
		//	entry - Entry to perform operation on.
		//
		inline void Remove (const elementType *const entry) const
		{
			Pop (entry);

			delete entry;
		}

		//
		// Function: RemoveAll
		//
		// Description: Removes all elements from list, and destroys them.
		//
		void RemoveAll (void)
		{
			Node *node = NULL, *nodeToKill;

			while ((node = GetIterator (node)) != NULL)
			{
				nodeToKill = node;

				node = node->m_prev;

				delete nodeToKill->m_data;
			}
		}

		//
		// Function: IsMember
		//
		// Description: Checks whether entry is in list.
		//
		// Parameters:
		//	entry - Entry to check.
		//
		// Returns:  True if entry is in list, false otherwise.
		//
		inline const bool IsMember (const elementType *const entry) const
		{
			return FindNode (entry) != NULL;
		}
};
//#endif	// if 0
namespace Another
{
template <typename elementType, typename indexType = unsigned short> class LinkedList
{
	//
	// Group: Members.
	//
	public:
		// typical node (one list element)
		class Node
		{
			//
			// Group: Members.
			//
			public:
				// one cell of data to store
				elementType  m_element;

				// previous and next elements
				Node        *m_prev;
				Node        *m_next;

			//
			// Group: (Con/De)structors.
			//
			public:
				inline Node (const elementType &element) : m_element (element), m_prev (NULL), m_next (NULL) { /*VOID*/ }
				inline Node (const elementType &element, Node *const prev, Node *const next) : m_element (element), m_prev (prev), m_next (next) { /*VOID*/ }
		};

	//
	// Group: Private members.
	//
	private:
		// access to the left and right ends of the list
		Node      *m_first;
		Node      *m_last;

		indexType  m_itemCount;

	//
	// Group: (Con/De)structors.
	//
	public:
		inline LinkedList (void) : m_first (NULL), m_last (NULL), m_itemCount (0u) { /*VOID*/ }
		virtual inline ~LinkedList (void) { /*VOID*/ }

	//
	// Group: Private functions.
	//
	private:
		//
		// Function: GetIterator
		//
		// Description: Utility node iterator.
		//
		// Parameters:
		//	entry - Previous entry.
		//
		// Returns: Node pointer.
		//
		inline Node *GetIterator (const Node *const entry = NULL) const
		{
			if (entry == NULL)
				return m_first;

			return entry->m_next;
		}

		//
		// Function: Find
		//
		// Description: Find node by it's entry.
		//
		// Parameters:
		//	entry - Entry to search.
		//
		// Returns: Node pointer.
		//
		inline Node *Find (const elementType &entry) const
		{
			Node *iterator = NULL;

			while ((iterator = GetIterator (iterator)) != NULL && iterator->m_element == entry)
				return iterator;

			return NULL;
		}

	//
	// Group: Functions.
	//
	public:
		// List iterator.
		class Iterator
		{
			public:
				Node *m_node;	// Pointer on the node of the list containing value of an element in which pointed by iterator.

			public:
				// Creates iterator, pointed at the list element stored in node to 'node' address.
				inline Iterator (Node *const node = NULL) : m_node (node) { /*VOID*/ }

				inline Iterator (const LinkedList &source) : m_node (source.m_first) { /*VOID*/ }

			public:
				// Операция разыменовывания для не константного итератора.
				// Возвращает не константного ссылку на значение эл-та, по которой
				// оно может быть изменено.
				inline elementType &operator * (void)
				{
					return m_node->m_data;
				}

				// Операция разыменовывания для константного итератора.
				// Возвращает константую ссылку на значение эл-та.
				inline const elementType &operator * (void) const
				{
					return m_node->m_data;
				}

				inline elementType *const operator -> (void)
				{
					return &m_node->m_data;
				}

				inline const elementType *const operator -> (void) const
				{
					return &m_node->m_data;
				}

				// Префиксная операция инкремента. Перемещает итератор к следующему эл-ту списка.
				inline Iterator &operator ++ (void)
				{
					m_node = m_node->m_next;	// переходим к следующему эл-ту

					return *this;	// возвращаем "увеличенный" итератор
				}

				// Постфиксная операция инкремента. Перемещает итератор к следующему эл-ту списка.
				// Аргумент типа int указывает компилятору, что данная функция-операция operator ++
				// является постфиксной.
				inline Iterator operator ++ (int)
				{
					Iterator old (*this);	// сохраняем "не увеличенное" значение итератора

					m_node = m_node->m_next;	// переходим к следующему эл-ту

					return old;	// возвращаем "не увеличенное" значение итератора
				}

				// Префиксная операция декремента. Перемещает итератор к предыдущему эл-ту списка.
				inline Iterator &operator -- (void)
				{
					m_node = m_node->m_prev;	// переходим к предыдущему эл-ту

					return *this;	// возвращаем "уменьшенный" итератор
				}

				// Постфиксная операция декремента. Перемещает итератор к предыдущему эл-ту списка.
				// Аргумент типа int указывает компилятору, что данная функция-операция operator --
				// является постфиксной.
				inline Iterator operator -- (int)
				{
					Iterator old (*this);	// сохраняем "не уменьшенное" значение итератора

					m_node = m_node->m_prev;	// переходим к предыдущему эл-ту

					return old;	// возвращаем "не уменьшенное" значение итератора
				}

				// Операции равенства/неравенства.
				inline const bool operator != (const Iterator &right) const
				{
					return !(*this == right);
				}

				inline const bool operator == (const Iterator &right) const
				{
					return m_node == right.m_node;
				}
		};

		// Возвращает константный итератор на первый элемент (для константного списка).
		inline const Iterator GetFirst (void) const
		{
			return Iterator (m_first);
		}

		// Возвращает итератор на первый элемент (для не константного списка).
		inline Iterator GetFirst (void)
		{
			return Iterator (m_first);
		}

		// Возвращает константный итератор на последний элемент (для константного списка).
		inline const Iterator GetLast (void) const
		{
			return Iterator (m_last);
		}

		// Возвращает итератор на последний элемент (для не константного списка).
		inline Iterator GetLast (void)
		{
			return Iterator (m_last);
		}

		// Возвращает константный итератор на "конец" (для константного списка).
		inline const Iterator GetEnd (void) const
		{
			return Iterator ();
		}

		// Возвращает итератор на "конец" (для не константного списка).
		inline Iterator GetEnd (void)
		{
			return Iterator ();
		}

		//
		// Function: Destory
		//
		// Description: Resets list to empty state by abandoning contents.
		//
		inline void Destory (void)
		{
			m_first = NULL;
			m_last = NULL;
			m_itemCount = 0u;
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

		//
		// Function: GetFirst
		//
		// Description: Gets the first list entry. NULL in case list is empty.
		//
		// Returns: First list entry.
		//
		inline elementType &GetFirst (void) const
		{
			// Reliability check.
			InternalAssert (m_first != NULL);

			return m_first->m_element;
		}

		//
		// Function: GetLast
		//
		// Description: Gets the last list entry. NULL in case list is empty.
		//
		// Returns: Last list entry.
		//
		inline elementType &GetLast (void) const
		{
			// Reliability check.
			InternalAssert (m_last != NULL);

			return m_last->m_element;
		}

		//
		// Function: GetNext
		//
		// Description: Gets the next element from linked list.
		//
		// Parameters:
		//	current - Current node.
		//
		// Returns: Node data.
		//
		inline elementType &GetNext (const elementType *const current) const
		{
			if (current == NULL)
				return GetFirst ();

			Node *const next = Find (current)->m_next;

			if (next == NULL)
				return NULL;

			return next->m_element;
		}

		//
		// Function: GetPrev
		//
		// Description: Gets the previous element from linked list.
		//
		// Parameters:
		//	current - Current node.
		//
		// Returns: Node data.
		//
		inline elementType *GetPrev (const elementType *const current) const
		{
			if (current == NULL)
				return GetLast ();

			Node *prev = Find (current)->m_prev;

			if (prev == NULL)
				return NULL;

			return prev->m_prev;
		}

		void PushFront (const elementType &element)
		{
			// create separate node
			Node *const newNode = new Node (entry);

			if (newNode == NULL)
				TerminateOnMalloc ();

			// If the list is empty...:
			if (m_first == NULL)
				m_last = newNode;	// new first node is also first;
			else
				m_first->m_prev = newNode;	// else we connect first node with new.

			newNode->m_next = m_first;	// Connect new node with first,

			m_first = newNode;	// and make new - first.
		}

		void PushBack (const elementType &element)
		{
			// create separate node
			Node *const newNode = new Node (entry);

			if (newNode == NULL)
				TerminateOnMalloc ();

			// If the list is empty...:
			if (m_first == NULL)
				m_first = newNode;	// new last node is also first;
			else
				m_last->m_next = newNode;	// else we connect last node with new.

			newNode->m_prev = m_last;	// Connect new node with last,

			m_last = newNode;	// and make new - last.
		}

		//
		// Function: Pop
		//
		// Description: Removes element from linked list.
		//
		// Parameters:
		//	entry - Element that should be moved out of list.
		//
		void Pop (const elementType *const entry)
		{
			Node *const entryNode = Find (entry);

			// Reliability check.
			InternalAssert (entryNode != NULL);

			if (entryNode->m_prev == NULL)
				m_first = entryNode->m_next;
			else
				entryNode->m_prev->m_next = entryNode->m_next;

			if (entryNode->m_next == NULL)
				m_last = entryNode->m_prev;
			else
				entryNode->m_next->m_prev = entryNode->m_prev;

			delete entryNode;
		}

		//
		// Function: Remove
		//
		// Description: Removes element from list, and destroys it.
		//
		// Parameters:
		//	entry - Entry to perform operation on.
		//
		inline void Remove (const elementType &entry) const
		{
			Pop (entry);

			delete entry;
		}

		//
		// Function: RemoveAll
		//
		// Description: Removes all elements from list, and destroys them.
		//
		void RemoveAll (void)
		{
			Node *node = NULL, *nodeToKill;

			while ((node = GetIterator (node)) != NULL)
			{
				nodeToKill = node;

				node = node->m_prev;

				delete nodeToKill->m_element;
			}
		}

		//
		// Function: IsMember
		//
		// Description: Checks whether entry is in list.
		//
		// Parameters:
		//	entry - Entry to check.
		//
		// Returns:  True if entry is in list, false otherwise.
		//
		inline const bool IsMember (const elementType &entry) const
		{
			return Find (entry) != NULL;
		}

	//
	// Group: Operators.
	//
	public:
		inline LinkedList &operator -= (const elementType &object)
		{
			Pop (object);

			return *this;
		}

		inline LinkedList &operator += (const elementType &object)
		{
			PushBack (object);

			return *this;
		}

		inline LinkedList &operator = (const LinkedList &src)
		{
			RemoveAll ();

			for (const Iterator iterator = src.GetFrontNode (); iterator != src.GetBackNode (); ++iterator)
				PushBack (*iterator);

			return *this;
		}
};
}
#if 0
//-----------------------------------------------------------------------------
// Class to drop in to replace a CUtlLinkedList that needs to be more memory agressive
//-----------------------------------------------------------------------------

#define DECLARE_POINTER_HANDLE(name) struct name##__ { int unused; }; typedef struct name##__ *name

DECLARE_POINTER_HANDLE( UtlPtrLinkedListIndex_t );	// to enforce correct usage

template < typename T >
class CUtlPtrLinkedList
{
private:
	struct Node_t
	{
		Node_t() {}
		Node_t( const T &elem ) : elem( elem ) {}

		T elem;
		Node_t *pPrev, *pNext;
	};

	Node_t *m_pFirst;
	unsigned int m_nElems;

public:
	typedef UtlPtrLinkedListIndex_t IndexType_t;

public:
	CUtlPtrLinkedList()
		: m_pFirst( NULL ),
		m_nElems( 0 )
	{
		CompileTimeAssert( sizeof(IndexType_t) == sizeof(Node_t *) );
	}

	~CUtlPtrLinkedList()
	{
		RemoveAll();
	}

public:
	T &operator[]( IndexType_t i )
	{
		return (( Node_t * )i)->elem;
	}

	const T &operator[]( IndexType_t i ) const
	{
		return (( Node_t * )i)->elem;
	}

public:
	IndexType_t	AddToTail()
	{
		return DoInsertBefore( (IndexType_t)m_pFirst, NULL );
	}

	IndexType_t	AddToTail( T const& src )
	{
		return DoInsertBefore( (IndexType_t)m_pFirst, &src );
	}

	IndexType_t	AddToHead()
	{
		IndexType_t result = DoInsertBefore( (IndexType_t)m_pFirst, NULL );
		m_pFirst = ((Node_t *)result);
		return result;
	}

	IndexType_t	AddToHead( T const& src )
	{
		IndexType_t result = DoInsertBefore( (IndexType_t)m_pFirst, &src );
		m_pFirst = ((Node_t *)result);
		return result;
	}

	IndexType_t InsertBefore( IndexType_t before )
	{
		return DoInsertBefore( before, NULL );
	}

	IndexType_t InsertAfter( IndexType_t after )
	{
		Node_t *pBefore = ((Node_t *)after)->next;
		return DoInsertBefore( pBefore, NULL );
	}

	IndexType_t InsertBefore( IndexType_t before, T const& src  )
	{
		return DoInsertBefore( before, &src );
	}

	IndexType_t InsertAfter( IndexType_t after, T const& src  )
	{
		Node_t *pBefore = ((Node_t *)after)->next;
		return DoInsertBefore( pBefore, &src );
	}

	void Remove( IndexType_t elem )
	{
		Node_t *p = (Node_t *)elem;

		if ( p->pNext == p )
		{
			m_pFirst = NULL;
		}
		else
		{
			if ( m_pFirst == p )
			{
				m_pFirst = p->pNext;
			}
			p->pNext->pPrev = p->pPrev;
			p->pPrev->pNext = p->pNext;
		}

		delete p;
		m_nElems--;
	}

	void RemoveAll()
	{
		Node_t *p = m_pFirst;
		if ( p )
		{
			do
			{
				Node_t *pNext = p->pNext;
				delete p;
				p = pNext;
			} while( p != m_pFirst );
		}

		m_pFirst = NULL;
		m_nElems = 0;
	}

	int	Count() const
	{
		return m_nElems;
	}

	IndexType_t Head() const
	{
		return (IndexType_t)m_pFirst;
	}

	IndexType_t Next( IndexType_t i ) const
	{
		Node_t *p = ((Node_t *)i)->pNext;
		if ( p != m_pFirst )
		{
			return (IndexType_t)p;
		}
		return NULL;
	}

	bool IsValidIndex( IndexType_t i ) const
	{
		Node_t *p = ((Node_t *)i);
		return ( p && p->pNext && p->pPrev );
	}

	inline static IndexType_t InvalidIndex()
	{
		return NULL;
	}

private:
	Node_t *AllocNode( const T *pCopyFrom )
	{
		MEM_ALLOC_CREDIT_CLASS();
		Node_t *p;

		if ( !pCopyFrom )
		{
			p = new Node_t;
		}
		else
		{
			p = new Node_t( *pCopyFrom );
		}

		return p;
	}

	IndexType_t DoInsertBefore( IndexType_t before, const T *pCopyFrom )
	{
		Node_t *p = AllocNode( pCopyFrom );
		Node_t *pBefore = (Node_t *)before;
		if ( pBefore )
		{
			p->pNext = pBefore;
			p->pPrev = pBefore->pPrev;
			pBefore->pPrev = p;
			p->pPrev->pNext = p;
		}
		else
		{
			InternalAssert( !m_pFirst );
			m_pFirst = p->pNext = p->pPrev = p;
		}

		m_nElems++;
		return (IndexType_t)p;
	}
};
#endif
#endif	// if 0

//-----------------------------------------------------------------------------
// class LinkedList:
// description:
//		A lovely index-based linked list! 'elementType' is the class type, 'indexType' is the index
//		type, which usually should be an short integer or smaller, but only unsigned!!!!
//-----------------------------------------------------------------------------
template <typename elementType, typename indexType = unsigned short> class LinkedList
{
	//
	// Group: Protected members.
	//
	protected:
		// What the linked list element looks like
		struct Element_t
		{
			// one cell of data to store
			elementType m_element;

			// previous and next elements
			indexType   m_prev;
			indexType   m_next;

			inline Element_t (void) { /*VOID*/ }

			//
			// Group: Private members.
			//
			private:
				// No copy constructor for these...
				inline Element_t (const Element_t &);	// not supported
		};

		DynamicArray <Element_t, indexType> m_memory;
		indexType                           m_first;
		indexType                           m_last;
		indexType                           m_firstFree;
		indexType                           m_allocatedSize;	// The number allocated
		indexType                           m_itemCount;		// The number actually in the list

	//
	// Group: Special points.
	//
	public:
		enum
		{
			ElementSize = sizeof (Element_t),	// One list element size.
			BadPosition = static_cast <indexType> (-1),//m_memory.BadPosition,	// Generic bad/missing length/position.
			MaximumPossibleSize = min/*Math::GetMinimumValueBetween <unsigned int>*/ (BadPosition, CoreToolLibrary::BadPosition / sizeof (elementType) / 2u)//m_memory.MaximumPossibleSize	// Maximum possible array size.
		};

	//
	// Group: Types definitions.
	//
	public:
		typedef elementType ElementType_t;
		typedef indexType   IndexType_t;

	//
	// Group: (Con/De)structors.
	//
	public:
		// constructor, destructor
		inline LinkedList (void) : m_first (BadPosition), m_last (BadPosition), m_firstFree (BadPosition), m_allocatedSize (0u), m_itemCount (0u)
		{ /*VOID*/ }
		inline LinkedList (const indexType size) : m_memory (size), m_first (BadPosition), m_last (BadPosition), m_firstFree (BadPosition), m_allocatedSize (0u), m_itemCount (0u)
		{ /*VOID*/ }
		inline LinkedList (const Element_t *const memory, const indexType memorySize) : m_memory (memory, memorySize / ElementSize), m_first (BadPosition), m_last (BadPosition), m_firstFree (BadPosition), m_allocatedSize (0u), m_itemCount (0u)
		{ /*VOID*/ }
		inline ~LinkedList (void)
		{
			RemoveAll ();
		}

	//
	// Group: Private (con/de)structors.
	//
	private:
		// copy constructors not allowed
		inline LinkedList (const LinkedList &) { InternalAssert (0u); }	// not supported

	//
	// Group: Protected functions.
	//
	protected:
		// constructs the class
		const indexType AllocInternal (const bool multiList = false)
		{
			indexType elem;

			if (m_firstFree == BadPosition)
			{
				// Reliability check.
				InternalAssert (m_allocatedSize != MaximumPossibleSize);

				// Nothing in the free list; add.
				// Since nothing is in the free list, m_allocatedSize == total # of elements
				// the list knows about.
				if (m_allocatedSize == m_memory.GetAllocatedSize ())
//					m_memory.SetSize (m_memory.GetElementNumber () + 1u);
					m_memory.PushBackDefaultElement ();	// Add new element.

				elem = m_allocatedSize++;

				if (elem == MaximumPossibleSize)
					AddLogEntry (false, LogLevel_Critical, false, "LinkedList overflow!");
			}
			else
			{
				elem = m_firstFree;
				m_firstFree = m_memory[m_firstFree].m_next;
			}

			m_memory[elem].m_next = m_memory[elem].m_prev = multiList == false ? elem : BadPosition;

			return elem;
		}

	//
	// Group: Functions.
	//
	public:
		//
		// Function: GetMemoryUsage
		//
		// Description: Gets the using by this linked list memory.
		//
		// Returns: How much memory this linked list is using.
		//
		inline const unsigned int GetMemoryUsage (void) const
		{
			return sizeof (LinkedList) + sizeof (elementType) * m_allocatedSize;
		}

		//
		// Function: GetAllocatedSize
		//
		// Description: Gets allocated size of linked list.
		//
		// Returns: Number of allocated items.
		//
		inline const indexType GetAllocatedSize (void) const { return m_allocatedSize; }
		inline const indexType GetTotalAllocatedSize (void) const { return m_memory.GetAllocatedSize (); }

		//
		// Function: GetElementNumber
		//
		// Description: Gets real number currently in linked list.
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

		// Delete all the elements.
		inline void DeleteAll (void)
		{
			// Free all used memory....
			for (indexType iterator (m_first); iterator != BadPosition; iterator = GetNext (iterator))
				delete m_memory[iterator].m_element;
		}

		// Memory deallocation
		inline void Purge (void)
		{
			// Prevent reentrant calls to Purge()
			if (!IsEmpty ())
				RemoveAll ();

			m_memory.RemoveAll ();
			m_firstFree = BadPosition;
			m_allocatedSize = 0u;
		}

		// Delete all the elements then call Purge.
		inline void DeleteAndPurgeAll (void)
		{
			// Free all used memory....
			DeleteAll ();
			Purge ();
		}

		// Insertion methods....
		inline const indexType InsertBefore (const indexType before)
		{
			// Make a new node
			const indexType newNode (AllocInternal ());

			// Link it in
			LinkBefore (before, newNode);

			// Construct the data
			Construct (&m_memory[newNode].m_element);

			return newNode;
		}
		inline const indexType InsertAfter (const indexType after)
		{
			// Make a new node
			const indexType newNode (AllocInternal ());

			// Link it in
			LinkAfter (after, newNode);

			// Construct the data
			Construct (&m_memory[newNode].m_element);

			return newNode;
		}
		inline const indexType PushFront (void) { return InsertAfter (BadPosition); }
		inline const indexType PushBack  (void) { return InsertBefore (BadPosition); }

		inline const indexType InsertBefore (const indexType before, const elementType &src)
		{
			// Make a new node
			const indexType newNode (AllocInternal ());

			// Link it in
			LinkBefore (before, newNode);

			// Construct the data
			CopyConstruct (&m_memory[newNode].m_element, src);

			return newNode;
		}
		inline const indexType InsertAfter (const indexType after, const elementType &src)
		{
			// Make a new node
			const indexType newNode (AllocInternal ());

			// Link it in
			LinkAfter (after, newNode);

			// Construct the data
			CopyConstruct (&m_memory[newNode].m_element, src);

			return newNode;
		}
		inline const indexType PushFront (const elementType &src) { return InsertAfter (BadPosition, src); }
		inline const indexType PushBack (const elementType &src) { return InsertBefore (BadPosition, src); }

		// Find an element and return its index or BadPosition if it couldn't be found.
		inline const indexType Find (const elementType &src) const
		{
			for (indexType iterator (m_first); iterator != BadPosition; iterator = GetNext (iterator))
				if (m_memory[iterator].m_element == src)
					return iterator;

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

		// Look for the element. If it exists, remove it and return true. Otherwise, return false.
		inline const bool Pop (const elementType &element)
		{
			const indexType elementPosition (Find (element));

			if (elementPosition == BadPosition)
				return false;

			// Else....

			Delete (elementPosition);

			return true;
		}

		// Removal methods
		inline void Delete (const indexType elem)
		{
			// Reliability check.
			InternalAssert (IsValidIndex (elem));

			Unlink (elem);

			Element_t &internalElem = m_memory[elem];

			Destruct (&internalElem.m_element);

			internalElem.m_next = m_firstFree;
			m_firstFree = elem;
		}
		inline void PopFirst (void) { Delete (m_first); }
		inline void PopBack  (void) { Delete (m_last); }
		void RemoveAll (void)
		{
			if (m_allocatedSize == 0u)
				return;

			// Put everything into the free list
			for (indexType index (m_allocatedSize - 1u), prev = BadPosition; index < m_allocatedSize; --index)
			{
				// Invoke the destructor
				if (IsValidIndex (index))
					Destruct (&m_memory[index].m_element);

				// next points to the next free list item
				m_memory[index].m_next = prev;

				// Indicates it's in the free list
				m_memory[index].m_prev = index;
				prev = index;
			}

			// First free points to the first element
			m_firstFree = 0u;

			// Clear everything else out
			m_first = m_last = BadPosition;
			m_itemCount = 0u;
		}

		// Allocation/deallocation methods
		// If multiList == true, then list list may contain many
		// non-connected lists, and IsInList and Head + Tail are meaningless...
		inline const indexType Alloc (const bool multiList = false)
		{
			const indexType elem (AllocInternal (multiList));

			Construct (&m_memory[elem].m_element);

			return elem;
		}

		// list modification
		void LinkBefore (const indexType before, const indexType elem)
		{
			// Reliability check.
			InternalAssert (IsValidIndex (elem));

			// Unlink it if it's in the list at the moment
			Unlink (elem);

			Element_t &newElem (m_memory[elem]);

			// The element *after* our newly linked one is the one we linked before.
			newElem.m_next = before;

			if (before == BadPosition)
			{
				// In this case, we're linking to the end of the list, so reset the tail
				newElem.m_prev = m_last;
				m_last = elem;
			}
			else
			{
				// Reliability check.
				InternalAssert (IsInList (before));

				Element_t &beforeElem (m_memory[before]);

				// Here, we're not linking to the end. Set the prev pointer to point to the element we're linking.
				newElem.m_prev = beforeElem.m_prev;
				beforeElem.m_prev = elem;
			}

			// Reset the head if we linked to the head of the list
			if (newElem.m_prev == BadPosition)
				m_first = elem;
			else
				m_memory[newElem.m_prev].m_next = elem;

			// one more element baby
			++m_itemCount;
		}
		void LinkAfter (const indexType after, const indexType elem)
		{
			// Reliability check.
			InternalAssert (IsValidIndex (elem));

			// Unlink it if it's in the list at the moment
			if (IsInList (elem))
				Unlink (elem);

			Element_t &newElem (m_memory[elem]);

			// The element *before* our newly linked one is the one we linked after
			newElem.m_prev = after;

			if (after == BadPosition)
			{
				// In this case, we're linking to the head of the list, reset the head
				newElem.m_next = m_first;
				m_first = elem;
			}
			else
			{
				// Reliability check.
				InternalAssert (IsInList (after));

				Element_t &afterElem (m_memory[after]);

				// Here, we're not linking to the end. Set the next pointer to point to the element we're linking.
				newElem.m_next = afterElem.m_next;
				afterElem.m_next = elem;
			}

			// Reset the tail if we linked to the tail of the list
			if (newElem.m_next == BadPosition)
				m_last = elem;
			else
				m_memory[newElem.m_next].m_prev = elem;

			// one more element baby
			++m_itemCount;
		}
		void Unlink (const indexType elem)
		{
			// Reliability check.
			InternalAssert (IsValidIndex (elem));

			if (!IsInList (elem))
				return;

			Element_t *const base (m_memory);
			Element_t *const oldElement (base + elem);

			// If we're the first guy, reset the head otherwise, make our previous node's next pointer = our next
			if (oldElement->m_prev != BadPosition)
				base[oldElement->m_prev].m_next = oldElement->m_next;
			else
				m_first = oldElement->m_next;

			// If we're the last guy, reset the tail otherwise, make our next node's prev pointer = our prev
			if (oldElement->m_next != BadPosition)
				base[oldElement->m_next].m_prev = oldElement->m_prev;
			else
				m_last = oldElement->m_prev;

			// This marks this node as not in the list, but not in the free list either
			oldElement->m_prev = oldElement->m_next = elem;

			// One less puppy
			--m_itemCount;
		}
		inline void LinkToHead (const indexType elem) { LinkAfter (BadPosition, elem); }
		inline void LinkToTail (const indexType elem) { LinkBefore (BadPosition, elem); }

		// Traversing the list
		inline const indexType    GetFirst        (void) const { return m_first; }
		inline elementType       &GetFirstElement (void)       { return m_memory[m_first].m_element; }
		inline const elementType &GetFirstElement (void) const { return m_memory[m_first].m_element; }

		inline const indexType    GetLast         (void) const { return m_last; }
		inline elementType       &GetLastElement  (void)       { return m_memory[m_last].m_element; }
		inline const elementType &GetLastElement  (void) const { return m_memory[m_last].m_element; }

		inline const indexType    GetPrev         (const indexType index) const
		{
			// Reliability check.
			InternalAssert (IsValidIndex (index));

			return m_memory[index].m_prev;
		}
		inline const indexType    GetNext         (const indexType index) const
		{
			// Reliability check.
			InternalAssert (IsValidIndex (index));

			return m_memory[index].m_next;
		}

		// Are nodes in the list or valid?
		inline const bool IsValidIndex (const indexType index) const { return index < m_allocatedSize && (m_memory[index].m_prev != index || m_memory[index].m_next == index); }
		inline const bool IsInList (const indexType index) const { return index < m_allocatedSize && GetPrev (index) != index; }

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

			return m_memory[g_randomNumberGenerator.GetValueBetween <indexType> (0u, m_itemCount - 1u)].m_element;
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

			return m_memory[g_randomNumberGenerator.GetValueBetween <indexType> (0u, m_itemCount - 1u)].m_element;
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
	// Group: Operators.
	//
	public:
		// Element access...

		// gets particular elements
		// Gets at the list element....
		inline elementType       &operator [] (const indexType index)
		{
			// Reliability check.
			InternalAssert (IsValidIndex (index));

			return m_memory[index].m_element;
		}
		inline const elementType &operator [] (const indexType index) const
		{
			// Reliability check.
			InternalAssert (IsValidIndex (index));

			return m_memory[index].m_element;
		}

		inline LinkedList &operator -= (const elementType &object)
		{
			Pop (object);

			return *this;
		}

		inline LinkedList &operator += (const elementType &object)
		{
			PushBack (object);

			return *this;
		}

		// Equality operation.
		inline const bool operator == (const LinkedList &right) const
		{
			// If lists are various objects we carry out comparison.
			if (this != &right)
			{
				// If list is empty or number of elements not equally,
				if (m_itemCount != right.m_itemCount)
					return false;	// lists are not equal.

				// Else we carry out step by step comparison.
				for (indexType iterator (m_first); iterator != BadPosition; iterator = GetNext (iterator))
					if (m_memory[index].m_element != right.m_memory[index].m_element)
						return false;	// lists are not equal.
			}

			// Otherwise we return true (since any list is equal to itself).
			return true;
		}

		// Inequality operation.
		inline const bool operator != (const LinkedList &right) const
		{
			return !(*this == right);
		}

		inline const bool operator < (const LinkedList &right) const
		{
			if (this == &right)
				return false;	// lists are not equal.

			const indexType length (Math::GetMinimumValueBetween (m_itemCount, right.m_itemCount));

			for (indexType iterator (m_first); iterator != BadPosition; iterator = GetNext (iterator))
			{
				if (m_elements[iterator] < right.m_elements[iterator])
					return true;	// lists are equal.

				if (m_elements[iterator] > right.m_elements[iterator])
					return false;	// lists are not equal.
			}

			// They are the same, judge by length.
			return m_itemCount < right.m_itemCount;
		}
		inline const bool operator > (const LinkedList &right) const
		{
			return right < *this;
		}
		inline const bool operator >= (const LinkedList &right) const
		{
			return !(*this < right);
		}
		inline const bool operator <= (const LinkedList &right) const
		{
			return !(right < *this);
		}
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif	// ifndef LINKED_LIST_INCLUDED