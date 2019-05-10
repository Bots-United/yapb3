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

#include <Core.hpp>

namespace HalfLifeEngine
{
	namespace SDK
	{
		namespace Classes
		{
			void TexturesManager::LoadTextures (void)
			{
				unsigned int fileSize;
				unsigned char *const file (Globals::g_halfLifeEngine->LoadFile ("sound/materials.txt", &fileSize));

				// do some cleanup first
				m_textures.RemoveAll ();

				if (file == NULL)
					return;

				DynamicString buffer;
				unsigned int filePosition (0u);
				unsigned short startIndex, endIndex;
				Texture_t tempTexture;

				// for each line in the file...
				while (GetLineFromFileBuffer (static_cast <const char *const> (static_cast <const void *const> (file)), fileSize, filePosition, buffer, 511u))
				{
					startIndex = 0u;

					// skip whitespaces
					while (startIndex < buffer.GetElementNumber () && isspace (buffer[startIndex]))
						++startIndex;

					if (startIndex == buffer.GetElementNumber ())
						continue;

					// skip comment lines
					if (buffer[startIndex] == '/' || !isalpha (buffer[startIndex]))
						continue;

					// get texture type
					tempTexture.type = static_cast <char> (toupper (buffer[startIndex++]));

					// skip whitespace
					while (startIndex < buffer.GetElementNumber () && isspace (buffer[startIndex]))
						++startIndex;

					if (startIndex == buffer.GetElementNumber ())
						continue;

					// get sentence name
					endIndex = startIndex;

					while (endIndex < buffer.GetElementNumber () && !isspace (buffer[endIndex]))
						++endIndex;

					if (endIndex == buffer.GetElementNumber ())
						continue;

					// null-terminate name and save in sentences array
					tempTexture.name.Assign (buffer, startIndex, Math::GetMinimumValueBetween <unsigned short> (endIndex - startIndex, Constants::MaximumTextureNameLength - 1u));

					m_textures += tempTexture;

					if (m_textures.GetElementNumber () == Constants::MaximumTexturesNumber)
						break;
				}

				// Must use engine to free since we are in a .dll
				Globals::g_halfLifeEngine->FreeFile (file);

				SortTextures ();
			}

			const char TexturesManager::GetTextureType (Classes::Edict *const entity, const Math::Vector3D &source, const Math::Vector3D &destination) const
			{
				const unsigned short entityIndex (entity->GetIndex ());

				// Is world edict?
				if (entityIndex == 0u)
				{
					// get texture from entity or world (world is ent(0))
					const char *textureName (Globals::g_halfLifeEngine->TraceTexture (entity, source, destination));

					InternalAssert (textureName != NULL);
//					if (textureName != NULL)
//					{
						// strip leading '-0' or '+0~' or '{' or '!'
						if (*textureName == '-' || *textureName == '+')
							textureName += 2u;

						if (*textureName == '{' || *textureName == '!' || *textureName == '~' || *textureName == ' ')
							++textureName;

						// '}}'
						// get texture type
						return FindTextureType (textureName);
//					}
				}
				else if (entityIndex <= Globals::g_halfLifeEngine->GetMaximumClients ())	// Is player?
					return Constants::TextureType_Flesh;	// hit body

				return Constants::TextureType_None;
			}
		}
	}
}