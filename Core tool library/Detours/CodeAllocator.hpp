//
// Copyright © 2003-2012, by YaPB Development Team. All rights reserved.
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
// CodeAllocator.hpp
//
// Class: CodeAllocator
//
// Note: Taken from SourceMod "KeCodeAllocator.h".
//
// Version: $ID:$
//

#if defined _MSC_VER && _MSC_VER > 1000
#	pragma once
#endif	// if defined _MSC_VER && _MSC_VER > 1000

#ifndef CORE_TOOL_LIBRARY_DETOURS_CODE_ALLOCATOR_INCLUDED
#define CORE_TOOL_LIBRARY_DETOURS_CODE_ALLOCATOR_INCLUDED

class BaseAllocator
{/*
	public:
		// Release versions
		virtual void *const Allocate   (const size_t size) = 0;
		virtual void *const Reallocate (void *const memory, const size_t size) = 0;
		virtual void *const Delete     (void *const memory) = 0;*/
};

class CodeAllocator : public BaseAllocator
{
	private:
		struct FreedCode;

		/**
		* Defines a region of memory that is made of pages. 
		*/
		class CodeRegion
		{
			public:
				CodeRegion *next;
				unsigned char *block_start;
				unsigned char *block_pos;
				FreedCode *free_list;
				size_t total_size;
				size_t end_free;
				size_t total_free;

			public:
				inline CodeRegion *const Destroy (void)
				{
					CodeRegion *const tempNext (next);

					VirtualFree (block_start, 0, MEM_RELEASE);

					delete this;

					return tempNext;
				}
				inline void DestroyChain (void)
				{
					for (CodeRegion *first (this); first != NULL; first = first->Destroy ());
				}
		};

		/**
		* Defines freed code. We keep the size here because 
		* when we touch the linked list we don't want to dirty pages.
		*/
		struct FreedCode
		{
			CodeRegion *region;
			unsigned char *block_start;
			size_t size;
			FreedCode *next;
		};

		struct Secret
		{
			CodeRegion *region;
			size_t size;
		};

		class CodeCache
		{
			public:
			/**
			* First region that is live for use.
			*/
			CodeRegion *first_live;

			/**
			* First region that is full but has free entries.
			*/
			CodeRegion *first_partial;

			/**
			* First region that is full.
			*/
			CodeRegion *first_full;

			/**
			* Page granularity and size.
			*/
			unsigned int page_size;
			unsigned int page_granularity;

			/**
			* This isn't actually for code, this is the node cache.
			*/
			CodeRegion *node_cache;
			FreedCode *free_node_list;

			public:
				/**
				* @brief Creates a new code cache/allocator.
				*
				* @return				New code cache allocator.
				*/
				inline CodeCache (void) :
					first_live (NULL),
					first_partial (NULL),
					first_full (NULL),
					node_cache (NULL),
					free_node_list (NULL)
				{
					SYSTEM_INFO info;

					GetSystemInfo (&info);

					page_size = info.dwPageSize;
					page_granularity = info.dwAllocationGranularity;
				}

				inline ~CodeCache (void) { Destroy (); }

			private:
				void *const AllocInRegion (CodeRegion **const prev, CodeRegion *const region, unsigned char *ptr, const size_t alloc_size, const bool is_live);
				void *const AllocFromLive (const size_t size);
				void *const AllocFromPartial (const size_t size);
				CodeRegion *const AddRegionForSize (size_t size);
				FreedCode *const GetFreeNode (void);

				static inline const size_t MinAllocSize (void) { return GetAllocSize (0u); }
				static inline const size_t GetAllocSize (size_t size)
				{
					#define ALIGNMENT 16u

					size += sizeof (Secret);
					size += ALIGNMENT;
					size -= size % ALIGNMENT;

					return size;
				}

			public:
				/**
				* @brief Destroys a code cache allocator.
				*
				* @param cache			Code cache object.
				*/
				void Destroy (void);

				/**
				* @brief Allocates code memory that is readable, writable, 
				* and executable.
				*
				* The address returned wlil be aligned, minimally, on a 16-byte 
				* boundary.
				*
				* @param cache			Code cache object.
				* @param size			Amount of memory needed.
				* @return				Address pointing to the memory.
				*/
				void *const AllocCode (const size_t size);

				/**
				* @brief Frees code memory.
				*
				* @param cache			Code cache object.
				* @param code			Address of code memory.
				*/
				void FreeCode (void *const code);
		};

	private:
		CodeCache m_codeCache;

	public:
		template <typename elementType> inline elementType *const Allocate (const size_t size) { return static_cast <elementType *const> (m_codeCache.AllocCode (size)); }
		inline void Free (void *const code) { m_codeCache.FreeCode (code); }
};

extern CodeAllocator g_codeAllocator;

#endif	// ifndef CORE_TOOL_LIBRARY_DETOURS_CODE_ALLOCATOR_INCLUDED