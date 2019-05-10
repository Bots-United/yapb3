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
// CodeAllocator.cpp
//
// Class: CodeAllocator
//
// Note: Taken from SourceMod "KeCodeAllocator.cpp".
//
// Version: $ID:$
//

#include <Core.hpp>

namespace CoreToolLibrary
{
	namespace Detours
	{
		CodeAllocator g_codeAllocator;

		void *const CodeAllocator::CodeCache::AllocInRegion (CodeRegion **const prev, CodeRegion *const region, unsigned char *ptr, const size_t alloc_size, const bool is_live)
		{
			const size_t minimumAllocateSize (MinAllocSize ());

			// Squirrel some info in the alloc.
			Secret *const secret (reinterpret_cast <Secret *> (ptr));
			secret->region = region;
			secret->size = alloc_size;
			ptr += sizeof (Secret);

			region->total_free -= alloc_size;

			// Check if we can't use the fast-path anymore.
			if ((is_live && region->end_free < minimumAllocateSize) || (!is_live && region->total_free < minimumAllocateSize))
			{
				CodeRegion **start;

				*prev = region->next;

				// Select the appropriate arena.
				if (is_live)
				{
					if (region->total_free < minimumAllocateSize)
						start = &first_full;
					else
						start = &first_partial;
				}
				else
					start = &first_full;
				
				region->next = *start;
				*start = region;
			}

			return ptr;
		}

		void *const CodeAllocator::CodeCache::AllocFromLive (const size_t size)
		{
			void *ptr;
			const size_t alloc_size (GetAllocSize (size));
			CodeRegion *region (first_live), **prev = &first_live;

			while (region != NULL)
			{
				if (region->end_free >= alloc_size)
				{
					// Yay! We can do a simple alloc here.
					ptr = AllocInRegion (prev, region, region->block_pos, alloc_size, true);

					// Update our counters.
					region->block_pos += alloc_size;
					region->end_free -= alloc_size;

					return ptr;
				}

				prev = &region->next;
				region = region->next;
			}

			return NULL;
		}

		void *const CodeAllocator::CodeCache::AllocFromPartial (const size_t size)
		{
			void *ptr;
			const size_t alloc_size (GetAllocSize (size));
			CodeRegion *region (first_partial), **prev = &first_partial;

			while (region != NULL)
			{
				if (region->total_free >= alloc_size)
				{
					InternalAssert (region->free_list != NULL);

					FreedCode *node (region->free_list), **last = &region->free_list;

					while (node != NULL)
					{
						if (node->size >= alloc_size)
						{
							// Use this node
							ptr = AllocInRegion (prev, region, node->block_start, alloc_size, false);

							region->total_free -= node->size;
							*last = node->next;

							// Make sure bookkeepping is correct.
							InternalAssert ((region->free_list == NULL && region->total_free == 0) || (region->free_list != NULL && region->total_free != 0));

							// Link us back into the free node list.
							node->next = free_node_list;
							free_node_list = node->next;

							return ptr;
						}

						last = &node->next;
						node = node->next;
					}
				}

				prev = &region->next;
				region = region->next;
			}

			return NULL;
		}

		CodeAllocator::CodeRegion *const CodeAllocator::CodeCache::AddRegionForSize (size_t size)
		{
			CodeRegion *const region (new CodeRegion ());

			size = GetAllocSize (size);
			size += page_granularity * 2;
			size -= size % page_granularity;

			region->block_start = static_cast <unsigned char *> (VirtualAlloc (NULL, size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE));

			if (region->block_start == NULL)
			{
				delete region;

				return NULL;
			}

			region->block_pos = region->block_start;
			region->end_free = region->total_free = region->total_size = size;
			region->next = first_live;
			first_live = region;
			region->free_list = NULL;

			return region;
		}

		void *const CodeAllocator::CodeCache::AllocCode (const size_t size)
		{
			void *ptr;

			// Check live easy-adds
			if (first_live != NULL && (ptr = AllocFromLive (size)) != NULL)
				return ptr;
			
			// Try looking in the free lists
			if (first_partial != NULL && (ptr = AllocFromPartial (size)) != NULL)
				return ptr;

			// Create a new region
			if (AddRegionForSize (size) == NULL)
				return NULL;

			return AllocFromLive (size);
		}

		CodeAllocator::FreedCode *const CodeAllocator::CodeCache::GetFreeNode (void)
		{
			FreedCode *ret;

			if (free_node_list != NULL)
			{
				ret = free_node_list;
				free_node_list = ret->next;

				return ret;
			}

			// See if the current free node region has space.
			if (node_cache != NULL && node_cache->end_free >= sizeof (FreedCode))
			{
				ret = reinterpret_cast <FreedCode *> (node_cache->block_pos);
				node_cache->block_pos += sizeof (FreedCode);
				node_cache->total_free -= sizeof (FreedCode);
				node_cache->end_free -= sizeof (FreedCode);

				return ret;
			}

			// Otherwise, we need to alloc a new region.
			CodeRegion *const region (new CodeRegion ());

			region->total_size = page_size / sizeof (FreedCode);
			region->block_start = new unsigned char[region->total_size];
			region->block_pos = region->block_start + sizeof (FreedCode);
			region->total_free = region->end_free = region->total_size - sizeof (FreedCode);
			region->free_list = NULL;
			region->next = node_cache;
			node_cache = region;

			return reinterpret_cast <FreedCode *> (region->block_start);
		}

		void CodeAllocator::CodeCache::FreeCode (void *const code)
		{
			unsigned char *const ptr (static_cast <unsigned char *> (code));
			Secret *const secret (reinterpret_cast <Secret *> (ptr - sizeof (Secret)));
			CodeRegion *const region (secret->region);
			FreedCode *const node (GetFreeNode ());

			node->block_start = ptr;
			node->next = region->free_list;
			region->free_list = node;
			node->region = region;
			node->size = secret->size;
		}

		void CodeAllocator::CodeCache::Destroy (void)
		{
			// Destroy every region and call it a day.
			first_full->DestroyChain ();
			first_full = NULL;
			first_live->DestroyChain ();
			first_live = NULL;
			first_partial->DestroyChain ();
			first_partial = NULL;

			// We use normal malloc for node cache regions
			CodeRegion *region (node_cache), *next;

			while (region != NULL)
			{
				next = region->next;
				delete [] region->block_start;
				delete region;
				region = next;
			}
		}
	}
}