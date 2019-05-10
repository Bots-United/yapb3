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
// Singleton.hpp
//
// Class: Singleton
//
// Description: Implements no-copying singleton.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef SINGLETON_INCLUDED
#define SINGLETON_INCLUDED

template <typename elementType> class Singleton
{
	//
	// Group: (Con/De)structors.
	//
	protected:
		//
		// Function: Singleton
		//
		// Description: Default singleton constructor.
		//
		inline Singleton (void) { /*VOID*/ }

		//
		// Function: ~Singleton
		//
		// Description: Default singleton destructor.
		//
		virtual inline ~Singleton (void) { /*VOID*/ }

	//
	// Group: Functions.
	//
	public:
		//
		// Function: GetReference
		//
		// Description: Gets the object of singleton as reference.
		//
		// Returns: Object reference.
		//
		static inline elementType &GetReference (void)
		{
			static elementType reference;

			return reference;
		}

		//
		// Function: GetPointer
		//
		// Description: Gets the object of singleton.
		//
		// Returns: Object pointer.
		//
		static inline elementType *const &GetPointer (void) { return &GetReference (); }
};

#endif	// ifndef SINGLETON_INCLUDED