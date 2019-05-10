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
// TexturesManager.hpp
//
// Class: TexturesManager
//
// Description: Open materials.txt, get size, alloc space, save in array.
//				Only works first time called, ignored on subsequent calls.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef TEXTURES_MANAGER_INCLUDED
#define TEXTURES_MANAGER_INCLUDED

class TexturesManager
{
	//
	// Group: Private members.
	//
	private:
		struct Texture_t
		{
			Constants::TextureType_t type;	// texture type
			DynamicString            name;	// texture name
		};

		DynamicArray <Texture_t> m_textures;

	//
	// Group: Private functions.
	//
	private:
		inline void SwapTextures (const unsigned short i, const unsigned short j)
		{
			SwapElements (m_textures[i].type, m_textures[j].type);
			SwapElements (m_textures[i].name, m_textures[j].name);
		}

		inline void SortTextures (void)
		{
			// Bubble sort, yuck, but this only occurs at startup and it's only 512 elements...

			for (unsigned short i (0u), j; i < m_textures.GetElementNumber (); ++i)
				for (j = i + 1u; j < m_textures.GetElementNumber (); ++j)
					if (m_textures[i].name.CompareWithoutCaseCheck (m_textures[j].name) > 0)
					{
						// Swap
						SwapTextures (i, j);
					}
		}

		inline const Constants::TextureType_t FindTextureType (const char *const name) const
		{
			// given texture name, find texture type
			// if not found, return type 'concrete'

			unsigned short pivot, left (m_textures.GetFirstElementIndex ());
			int right (m_textures.GetLastElementIndex ()), value;

			do
			{
				// To start, find the subscript of the middle position.
				if ((value = m_textures[pivot = static_cast <unsigned short> ((left + right) / 2u)].name.CompareWithoutCaseCheck (name)) == 0)
					return m_textures[pivot].type;

				if (value > 0)	// If the number is > key, increase position by one.
					left = pivot + 1u;
				else	// Else (value < 0), decrease position by one.
					right = pivot - 1;
			} while (left <= right);

			return Constants::TextureType_Concrete;
		}

	//
	// Group: Functions.
	//
	public:
		void       LoadTextures   (void);
		const char GetTextureType (Classes::Edict *const entity, const Math::Vector3D &source, const Math::Vector3D &destination) const;
};

#endif	// ifndef TEXTURES_MANAGER_INCLUDED