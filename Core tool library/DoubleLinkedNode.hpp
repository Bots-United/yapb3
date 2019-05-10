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
// DoubleLinkedNode.hpp
//
// Class: DoubleLinkedNode
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef CORE_TOOL_LIBRARY_DOUBLE_LINKED_NODE_INCLUDED
#define CORE_TOOL_LIBRARY_DOUBLE_LINKED_NODE_INCLUDED

class DoubleLink
{
	public:
		class Iterator
		{
			public:
				typedef DoubleLink NodeType_t;

			public:
				NodeType_t *m_node;	// Pointer on the node of the list containing value of an element in which pointed by iterator.

			public:
				// Creates iterator, pointed at the list element stored in node to 'node' address.
				inline Iterator (NodeType_t *const node = NULL) : m_node (node) { /*VOID*/ }

			public:
				// validity check operator
				inline operator const bool (void) const { return m_node != NULL; }

				// casting operators
				inline operator       NodeType_t *&       (void)       { return m_node; }
				inline operator const NodeType_t *const & (void) const { return m_node; }

				// Prefix increment operator. Перемещает итератор к следующему эл-ту списка.
				inline Iterator &operator ++ (void)
				{
					m_node = m_node->m_next;	// переходим к следующему эл-ту

					return *this;	// возвращаем "увеличенный" итератор
				}

				// Postfix increment operator. Перемещает итератор к следующему эл-ту списка.
				// Аргумент типа int указывает компилятору, что данная функция-операция operator ++
				// является постфиксной.
				inline Iterator operator ++ (int)
				{
					Iterator old (*this);	// сохраняем "не увеличенное" значение итератора

					m_node = m_node->m_next;	// переходим к следующему эл-ту

					return old;	// возвращаем "не увеличенное" значение итератора
				}

				// Prefix decrement operator. Перемещает итератор к предыдущему эл-ту списка.
				inline Iterator &operator -- (void)
				{
					m_node = m_node->m_prev;	// переходим к предыдущему эл-ту

					return *this;	// возвращаем "уменьшенный" итератор
				}

				// Postfix decrement operator. Перемещает итератор к предыдущему эл-ту списка.
				// Аргумент типа int указывает компилятору, что данная функция-операция operator --
				// является постфиксной.
				inline Iterator operator -- (int)
				{
					Iterator old (*this);	// сохраняем "не уменьшенное" значение итератора

					m_node = m_node->m_prev;	// переходим к предыдущему эл-ту

					return old;	// возвращаем "не уменьшенное" значение итератора
				}

				// Операции равенства/неравенства.
				inline const bool operator == (const Iterator &right) const { return m_node == right.m_node; }
				inline const bool operator != (const Iterator &right) const { return !operator == (right); }

			public:
				inline       NodeType_t *const &GetBase (void)       { return m_node; }
				inline const NodeType_t *const &GetBase (void) const { return m_node; }
		};

	protected:
		// previous and next elements
		DoubleLink *m_prev;	// previuous link in the double-linked list
		DoubleLink *m_next;	// next link in the double-linked list

	public:
		inline DoubleLink (DoubleLink *const prev = NULL, DoubleLink *const next = NULL) : m_prev (prev), m_next (next) { /*VOID*/ }

		virtual inline ~DoubleLink (void) { /* VOID */ }

	public:
//		inline       DoubleLink        &GetData          (void)                         { return this; }
//		inline const DoubleLink        &GetData          (void)                   const { return this; }
		inline       DoubleLink *const  GetNext          (void)                         { return m_next; }
		inline const DoubleLink *const  GetNext          (void)                   const { return m_next; }
		inline       DoubleLink       *&GetNextReference (void)                         { return m_next; }
		inline void                     SetNext          (DoubleLink *const node)       { m_next = node; }
		inline       DoubleLink *const  GetPrev          (void)                         { return m_prev; }
		inline const DoubleLink *const  GetPrev          (void)                   const { return m_prev; }
		inline       DoubleLink       *&GetPrevReference (void)                         { return m_prev; }
		inline void                     SetPrev          (DoubleLink *const node)       { m_prev = node; }

		inline const bool IsTail (void) const { return m_next == NULL; }
		inline const bool IsHead (void) const { return m_prev == NULL; }

		inline void Clear (void)
		{
			// Is used for new headnodes

			m_prev = m_next = this;
		}

		inline void Remove (void)
		{
			m_next->m_prev = m_prev;
			m_prev->m_next = m_next;
		}

		inline void InsertBefore (DoubleLink *const before)
		{
			m_next = before;
			m_prev = before->m_prev;

			m_next->m_prev = this;
			m_prev->m_next = this;
		}
		inline void InsertAfter (DoubleLink *const after)
		{
			m_next = after->m_next;
			m_prev = after;

			m_next->m_prev = this;
			m_prev->m_next = this;
		}
};

template <typename elementType> class DoubleLinkedNode/// @todo !!! : public DoubleLink
{
	public:
		class Iterator : public DoubleLink::Iterator
		{
			public:
				typedef DoubleLinkedNode NodeType_t;

			public:
				// Creates iterator, pointed at the list element stored in node to 'node' address.
				inline Iterator (NodeType_t *const node = NULL) : DoubleLink::Iterator (node) { /*VOID*/ }

			public:
				// Операция разыменовывания для не константного итератора.
				// Возвращает не константного ссылку на значение эл-та, по которой
				// оно может быть изменено.
				inline elementType &operator * (void) { return m_node->m_data; }

				// Операция разыменовывания для константного итератора.
				// Возвращает константую ссылку на значение эл-та.
				inline const elementType &operator * (void) const { return m_node->m_data; }

				inline elementType *const operator -> (void) { return &m_node->m_data; }

				inline const elementType *const operator -> (void) const { return &m_node->m_data; }
		};

	//
	// Group: Private members.
	//
	private:
		elementType       m_data;	// one cell of data to store

		// previous and next elements
		DoubleLinkedNode *m_prev;
		DoubleLinkedNode *m_next;

	//
	// Group: (Con/De)structors.
	//
	public:
		inline DoubleLinkedNode (const elementType &data, DoubleLinkedNode *const prev = NULL, DoubleLinkedNode *const next = NULL) :
			m_data (data),
			m_prev (prev),
			m_next (next)
		{ /*VOID*/ }

	//
	// Group: Functions.
	//
	public:
		inline void                           SetData          (const elementType &newData)         { m_data = newData; }
		inline       elementType             &GetData          (void)                               { return m_data; }
		inline const elementType             &GetData          (void)                         const { return m_data; }
		inline       DoubleLinkedNode *const  GetNext          (void)                               { return m_next; }
		inline const DoubleLinkedNode *const  GetNext          (void)                         const { return m_next; }
		inline       DoubleLinkedNode       *&GetNextReference (void)                               { return m_next; }
		inline void                           SetNext          (DoubleLinkedNode *const node)       { m_next = node; }
		inline       DoubleLinkedNode *const  GetPrev          (void)                               { return m_prev; }
		inline const DoubleLinkedNode *const  GetPrev          (void)                         const { return m_prev; }
		inline       DoubleLinkedNode       *&GetPrevReference (void)                               { return m_prev; }
		inline void                           SetPrev          (DoubleLinkedNode *const node)       { m_prev = node; }

		inline const bool IsHead (void) const { return m_prev == NULL; }
		inline const bool IsTail (void) const { return m_next == NULL; }

		inline void Clear (void)
		{
			// Is used for new headnodes

			m_prev = m_next = this;
		}

		inline void Remove (void)
		{
			m_next->m_prev = m_prev;
			m_prev->m_next = m_next;
		}

		inline void InsertBefore (DoubleLinkedNode *const node)
		{
			m_next         = node;
			m_prev         = node->m_prev;

			m_next->m_prev = this;
			m_prev->m_next = this;
		}
		inline void InsertAfter (DoubleLinkedNode *const node)
		{
			m_next         = node->m_next;
			m_prev         = node;

			m_next->m_prev = this;
			m_prev->m_next = this;
		}
};

#endif	// ifndef CORE_TOOL_LIBRARY_DOUBLE_LINKED_NODE_INCLUDED