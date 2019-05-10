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
// LinkedNode.hpp
//
// Class: LinkedNode
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef CORE_TOOL_LIBRARY_LINKED_NODE_INCLUDED
#define CORE_TOOL_LIBRARY_LINKED_NODE_INCLUDED

class Link
{
	protected:
		Link *m_next;	// pointer to the next element

	public:
		inline Link (Link *const next = NULL) : m_next (next) { /* VOID */ }

		virtual inline ~Link (void) { /* VOID */ }

	public:
//		inline       Link        &GetData          (void)                   { return this; }
//		inline const Link        &GetData          (void)             const { return this; }
		inline       Link *const  GetNext          (void)                   { return m_next; }
		inline const Link *const  GetNext          (void)             const { return m_next; }
		inline       Link       *&GetNextReference (void)                   { return m_next; }
		inline void               SetNext          (Link *const node)       { m_next = node; }

		inline const bool IsTail (void) const { return m_next == NULL; }

		inline void Clear (void)
		{
			// Is used for new headnodes

			m_next = this;
		}

		inline void InsertBefore (Link *const before)
		{
			m_next = before;
		}
		inline void InsertAfter (Link *const after)
		{
			m_next = after->m_next;

			after->m_next = this;
		}
};

template <typename elementType> class LinkedNode/// @todo !!! : public Link
{
	//
	// Group: Private members.
	//
	private:
		elementType  m_data;	// one cell of data to store

		LinkedNode  *m_next;	// pointer to the next element

	//
	// Group: (Con/De)structors.
	//
	public:
		inline LinkedNode (const elementType &data, LinkedNode *const next = NULL) : m_data (data), m_next (next) { /* VOID */ }

	//
	// Group: Functions.
	//
	public:
		inline void                     SetData          (const elementType &newData)       { m_data = newData; }
		inline       elementType       &GetData          (void)                             { return m_data; }
		inline const elementType       &GetData          (void)                       const { return m_data; }
		inline       LinkedNode *const  GetNext          (void)                             { return m_next; }
		inline const LinkedNode *const  GetNext          (void)                       const { return m_next; }
		inline       LinkedNode       *&GetNextReference (void)                             { return m_next; }
		inline void                     SetNext          (LinkedNode *const node)           { m_next = node; }

		inline const bool IsTail (void) const { return m_next == NULL; }

		inline void InsertBefore (LinkedNode *const node)
		{
			m_next = node;
		}

		inline void InsertAfter (LinkedNode *const node)
		{
			m_next = node->m_next;

			node->m_next = this;
		}
};

#endif	// ifndef CORE_TOOL_LIBRARY_LINKED_NODE_INCLUDED