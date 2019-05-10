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
// Msec.hpp
//
// Class: Msec
//
// Description: General msec value computing class definitions for pfnRunPlayerMove() function.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef MSEC_INCLUDED
#define MSEC_INCLUDED

class Msec
{
	//
	// Group: Private members.
	//
	private:
		unsigned char m_value;	// calculated msec value

		#if defined MULTIPLE_MSEC_METHODS
			float     m_del;	// used for msec calculation
			float     m_num;	// also used for msec calculation
		#endif	// if defined MULTIPLE_MSEC_METHODS

	//
	// Group: (Con/De)structors.
	//
	public:
		inline Msec (void) :
			m_value (0u)

			#if defined MULTIPLE_MSEC_METHODS
				, m_del (0.0f/*0.1f*/),
				m_num (0.0f)
			#endif	// if defined MULTIPLE_MSEC_METHODS
		{ /*VOID*/ }

	//
	// Group: Functions.
	//
	public:
		void Think (const float botFrameIntervalTime);

	//
	// Group: Operators.
	//
	public:
		inline operator const unsigned char (void) const { return m_value; }	// computed msec value will now automatically convert to 'unsigned char' when needed
};

#endif	// ifndef MSEC_INCLUDED