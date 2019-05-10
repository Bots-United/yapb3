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
// Grid2D.hpp
//
// Class: Grid2D
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef CORE_TOOL_LIBRARY_GRID_2D_INCLUDED
#define CORE_TOOL_LIBRARY_GRID_2D_INCLUDED

template <typename elementContainerType, const unsigned short CellSize = 300u> class Grid2D
{
	public:
		typedef typename elementContainerType::ElementType_t ElementType_t;
		typedef typename elementContainerType::IndexType_t   IndexType_t;
		typedef elementContainerType                         ElementContainer_t;

		struct Size2D_t
		{
			unsigned char x, y;

			inline Size2D_t (void) : x (0u), y (0u) { /*VOID*/ }
			inline Size2D_t (const unsigned char inputX, const unsigned char inputY) : x (inputX), y (inputY) { /*VOID*/ }
		};

	private:
		elementContainerType *m_grid;
		Size2D_t              m_size;
		Math::Vector2D        m_mins;
		IndexType_t           m_itemCount;

	public:
		inline  Grid2D (void) : m_grid (NULL) { /*VOID*/ }
		inline  Grid2D (const Math::Extent2D &extent) :
			m_size
			(
				static_cast <unsigned char> (extent.GetSizeX () / CellSize) + 1u,
				static_cast <unsigned char> (extent.GetSizeY () / CellSize) + 1u
			),
			m_mins (extent.mins),
			m_grid (new elementContainerType[x * y])
			m_itemCount (0u)
		{ /*VOID*/ }

		inline ~Grid2D (void) { delete [] m_grid; }

	private:
		inline const unsigned char WorldToGridX (const float worldX) const				///< given X component, return grid index
		{
			// Return bucket.
			return static_cast <const unsigned char> (Math::GetClampedValueInRange <short> (static_cast <short> ((worldX - m_mins.x) / CellSize), 0, m_size.x - 1));
		}
		inline const unsigned char WorldToGridY (const float worldY) const				///< given Y component, return grid index
		{
			// Return bucket.
			return static_cast <const unsigned char> (Math::GetClampedValueInRange <short> (static_cast <short> ((worldY - m_mins.y) / CellSize), 0, m_size.y - 1));
		}
		inline const Math::Vector2D GetWorldVectorFromGrid (const Size2D_t &gridIndexes) const
		{
			// Reliability checks.
			InternalAssert (gridIndexes.x < m_size.x);
			InternalAssert (gridIndexes.y < m_size.y);

			return Math::Vector2D (m_mins.x + gridIndexes.x * CellSize, m_mins.y + gridIndexes.y * CellSize);
		}

	public:
		/**
		* Allocate the grid and define its extents
		*/
		inline void Allocate (const Math::Extent2D &extent)
		{
			delete [] m_grid;	// destroy the old grid

			m_size.x    = static_cast <unsigned char> (extent.GetSizeX () / CellSize) + 1u;
			m_size.y    = static_cast <unsigned char> (extent.GetSizeY () / CellSize) + 1u;
			m_mins      = extent.mins;
			m_itemCount = 0u;

			m_grid = new elementContainerType[m_size.x * m_size.y];

			#if defined _DEBUG
				// Reliability check.
				if (m_grid == NULL)
					AddLogEntry (true, LogLevel_Error, false, "Couldn't allocate %u bytes for creating grid!", sizeof (elementContainerType) * m_size.x * m_size.y);
			#endif	// if defined _DEBUG
		}
		inline void RemoveAll (void)
		{
			delete [] m_grid;

			m_grid      = NULL;
			m_size.x    = m_size.y = 0u;
			m_mins      = Math::Vector2D::ZeroValue;
			m_itemCount = 0u;
		}

		inline const bool        IsEmpty          (void) const { return m_grid == NULL; }
		inline const IndexType_t GetElementNumber (void) const { return m_itemCount; }

		inline void AddElement (const ElementType_t &element, const Math::Vector2D &location)
		{
			const unsigned short gridIndex (WorldToGridX (location.x) + WorldToGridY (location.y) * m_size.x);

			// Reliability check.
			InternalAssert (gridIndex < m_size.x * m_size.y);

			// add to grid
			m_grid[gridIndex] += element;

			++m_itemCount;
		}
		inline void AddElement (const ElementType_t &element, const Math::Extent2D &extent)
		{
			const unsigned char loX (WorldToGridX (extent.mins.x));
			const unsigned char hiX (WorldToGridX (extent.maxs.x));
			const unsigned char hiY (WorldToGridY (extent.maxs.y));

			for (unsigned char loY (WorldToGridY (extent.mins.y)), x; loY <= hiY; ++loY)
				for (x = loX; x <= hiX; ++x)
				{
					const unsigned short gridIndex (x + loY * m_size.x);

					// Reliability check.
					InternalAssert (gridIndex < m_size.x * m_size.y);

					m_grid[gridIndex] += element;	// add to grid

					++m_itemCount;
				}
		}
		inline void RemoveElement (const ElementType_t &element, const Math::Vector2D &location)
		{
			const unsigned short gridIndex (WorldToGridX (location.x) + WorldToGridY (location.y) * m_size.x);

			// Reliability check.
			InternalAssert (gridIndex < m_size.x * m_size.y);

			// remove from grid
			m_grid[gridIndex] -= element;

			--m_itemCount;
		}
		inline void RemoveElement (const ElementType_t &element, const Math::Extent2D &extent)
		{
			const unsigned char loX (WorldToGridX (extent.mins.x));
			const unsigned char hiX (WorldToGridX (extent.maxs.x));
			const unsigned char hiY (WorldToGridY (extent.maxs.y));

			for (unsigned char loY (WorldToGridY (extent.mins.y)), x; loY <= hiY; ++loY)
				for (x = loX; x <= hiX; ++x)
				{
					const unsigned short gridIndex (x + loY * m_size.x);

					// Reliability check.
					InternalAssert (gridIndex < m_size.x * m_size.y);

					m_grid[gridIndex] -= element;	// remove from grid

					--m_itemCount;
				}
		}

		inline elementContainerType &GetElementsAtGridIndexes (const unsigned char x, const unsigned char y)
		{
			const unsigned short gridIndex (x + y * m_size.x);

			// Reliability check.
			InternalAssert (gridIndex < m_size.x * m_size.y);

			return m_grid[gridIndex];
		}
		inline elementContainerType &GetElementsAtGridIndexes (const Size2D_t &gridIndexes)
		{
			return GetElementsAtGridIndexes (gridIndexes.x, gridIndexes.y);
		}
		inline const elementContainerType &GetElementsAtGridIndexes (const unsigned char x, const unsigned char y) const
		{
			const unsigned short gridIndex (x + y * m_size.x);

			// Reliability check.
			InternalAssert (gridIndex < m_size.x * m_size.y);

			return m_grid[gridIndex];
		}
		inline const elementContainerType &GetElementsAtGridIndexes (const Size2D_t &gridIndexes) const
		{
			return GetElementsAtGridIndexes (gridIndexes.x, gridIndexes.y);
		}
		inline elementContainerType &GetElementsAtLocation (const Math::Vector2D &location)
		{
			const unsigned short gridIndex (WorldToGridX (location.x) + WorldToGridY (location.y) * m_size.x);

			// Reliability check.
			InternalAssert (gridIndex < m_size.x * m_size.y);

			return m_grid[gridIndex];
		}
		inline const elementContainerType &GetElementsAtLocation (const Math::Vector2D &location) const
		{
			const unsigned short gridIndex (WorldToGridX (location.x) + WorldToGridY (location.y) * m_size.x);

			// Reliability check.
			InternalAssert (gridIndex < m_size.x * m_size.y);

			return m_grid[gridIndex];
		}
		inline void RemoveElementsAtLocation (const Math::Vector2D &location)
		{
			const unsigned short gridIndex (WorldToGridX (location.x) + WorldToGridY (location.y) * m_size.x);

			// Reliability check.
			InternalAssert (gridIndex < m_size.x * m_size.y);

			m_grid[gridIndex].RemoveAll ();

			m_itemCount = 0u;
		}

		template <typename Functor> inline void ForAllElementsAtLocation (const Functor &function, const Math::Vector2D &location)
		{
			const unsigned short gridIndex (WorldToGridX (location.x) + WorldToGridY (location.y) * m_size.x);

			// Reliability check.
			InternalAssert (gridIndex < m_size.x * m_size.y);

			for (IndexType_t index (0u); index < m_grid[gridIndex].GetElementNumber (); ++index)
				function (m_grid[gridIndex][index]);
		}
		template <typename Functor> inline const bool ForAllElementsAtLocation (const Functor &function, const Math::Vector2D &location)
		{
			const unsigned short gridIndex (WorldToGridX (location.x) + WorldToGridY (location.y) * m_size.x);

			// Reliability check.
			InternalAssert (gridIndex < m_size.x * m_size.y);

			for (IndexType_t index (0u); index < m_grid[gridIndex].GetElementNumber (); ++index)
				if (function (m_grid[gridIndex][index]) == false)
					return false;

			return true;
		}
		template <typename Functor> inline void ForAllElementsAtExtent (const Functor &function, const Math::Extent2D &extent)
		{
			const unsigned char loX (WorldToGridX (extent.mins.x));
			const unsigned char hiX (WorldToGridX (extent.maxs.x));
			const unsigned char hiY (WorldToGridY (extent.maxs.y));

			for (unsigned char loY (WorldToGridY (extent.mins.y)), x; loY <= hiY; ++loY)
				for (x = loX; x <= hiX; ++x)
				{
					const unsigned short gridIndex (x + loY * m_size.x);

					// Reliability check.
					InternalAssert (gridIndex < m_size.x * m_size.y);

					for (IndexType_t index (0u); index < m_grid[gridIndex].GetElementNumber (); ++index)
						function (m_grid[gridIndex][index]);
				}
		}
		template <typename Functor> inline const bool ForAllElementsAtExtent (const Functor &function, const Math::Extent2D &extent)
		{
			const unsigned char loX (WorldToGridX (extent.mins.x));
			const unsigned char hiX (WorldToGridX (extent.maxs.x));
			const unsigned char hiY (WorldToGridY (extent.maxs.y));

			for (unsigned char loY (WorldToGridY (extent.mins.y)), x; loY <= hiY; ++loY)
				for (x = loX; x <= hiX; ++x)
				{
					const unsigned short gridIndex (x + loY * m_size.x);

					// Reliability check.
					InternalAssert (gridIndex < m_size.x * m_size.y);

					for (IndexType_t index (0u); index < m_grid[gridIndex].GetElementNumber (); ++index)
						if (function (m_grid[gridIndex][index]) == false)
							return false;
				}

			return true;
		}
};

#endif	// ifndef CORE_TOOL_LIBRARY_GRID_2D_INCLUDED