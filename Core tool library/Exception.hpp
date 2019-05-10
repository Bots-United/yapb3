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
// Exception.hpp
//
// Class: Exception
//
// Description: Simple exception raiser.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef EXCEPTION_INCLUDED
#define EXCEPTION_INCLUDED

class Exception
{
	//
	// Group: Private members.
	//
	private:
		const DynamicString m_exceptionText;
		const DynamicString m_filename;
		const unsigned int  m_line;

	//
	// Group: (Con/De)structors.
	//
	public:
		//
		// Function: Exception
		//
		// Description: Default exception constructor.
		//
		// Parameters:
		//	exceptionText - Text to throw.
		//	filename - Debug filename.
		//	line - Debug line number.
		//
		inline Exception (const DynamicString &exceptionText, const DynamicString &filename = "(no)", const unsigned int line = BadPosition) : m_exceptionText (exceptionText), m_filename (filename), m_line (line) { /*VOID*/ }

		//
		// Function: ~Exception
		//
		// Description: Default exception destructor.
		//
		virtual inline ~Exception (void) { /*VOID*/ }

	//
	// Group: Private operators.
	//
	private:
		inline Exception &operator = (const Exception &/*right*/);	// Avoid "warning C4512: 'Exception' : assignment operator could not be generated".

	//
	// Group: Functions.
	//
	public:
		//
		// Function: GetDescription
		//
		// Description: Gets the description from throw object.
		//
		// Returns: Exception text.
		//
		inline const DynamicString &GetDescription (void) const
		{
			static DynamicString result;

			result = m_filename != "(no)" && m_line != BadPosition ? FormatBuffer ("Exception: %s at %s:%u", m_exceptionText.GetData (), m_filename.GetData (), m_line) : m_exceptionText;

			return result;
		}
};

#endif	// ifndef EXCEPTION_INCLUDED