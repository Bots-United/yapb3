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
// GCFFile.hpp
//
// Description: Structure definitions for Half-Life Game Cache File.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef HALF_LIFE_ENGINE_SDK_STRUCTURES_GCF_FILE_INCLUDED
#define HALF_LIFE_ENGINE_SDK_STRUCTURES_GCF_FILE_INCLUDED

struct Header_t
{
	unsigned int uiDummy0;				// Always 0x00000001
	unsigned int uiMajorVersion;		// Always 0x00000001
	unsigned int uiMinorVersion;		// GCF version number.
	unsigned int uiCacheID;
	unsigned int uiLastVersionPlayed;
	unsigned int uiDummy1;
	unsigned int uiDummy2;
	unsigned int uiFileSize;			// Total size of GCF file in bytes.
	unsigned int uiBlockSize;			// Size of each data block in bytes.
	unsigned int uiBlockCount;			// Number of data blocks.
	unsigned int uiDummy3;
};

struct BlockEntryHeader_t
{
	unsigned int uiBlockCount;	// Number of data blocks.
	unsigned int uiBlocksUsed;	// Number of data blocks that point to data.
	unsigned int uiDummy0;
	unsigned int uiDummy1;
	unsigned int uiDummy2;
	unsigned int uiDummy3;
	unsigned int uiDummy4;
	unsigned int uiChecksum;	// Header checksum.
};

struct BlockEntry_t
{
	unsigned int uiEntryFlags;				// Flags for the block entry. 0x200F0000 == Not used.
	unsigned int uiFileDataOffset;			// The offset for the data contained in this block entry in the file.
	unsigned int uiFileDataSize;			// The length of the data in this block entry.
	unsigned int uiFirstDataBlockIndex;		// The index to the first data block of this block entry's data.
	unsigned int uiNextBlockEntryIndex;		// The next block entry in the series. (N/A if == BlockCount.)
	unsigned int uiPreviousBlockEntryIndex;	// The previous block entry in the series. (N/A if == BlockCount.)
	unsigned int uiDirectoryIndex;			// The index of the block entry in the directory.
};

struct FragmentationMapHeader_t
{
	unsigned int uiBlockCount;			// Number of data blocks.
	unsigned int uiFirstUnusedEntry;	// The index of the first unused fragmentation map entry.
	unsigned int uiTerminator;			// The block entry terminator; 0 = 0x0000ffff or 1 = 0xffffffff.
	unsigned int uiChecksum;			// Header checksum.
};

struct FragmentationMap_t
{
	unsigned int uiNextDataBlockIndex;	// The index of the next data block.
};

// The below section is part of version 5 but not version 6.

struct BlockEntryMapHeader_t
{
	unsigned int uiBlockCount;				// Number of data blocks.
	unsigned int uiFirstBlockEntryIndex;	// Index of the first block entry.
	unsigned int uiLastBlockEntryIndex;		// Index of the last block entry.
	unsigned int uiDummy0;
	unsigned int uiChecksum;				// Header checksum.
};

struct BlockEntryMap_t
{
	unsigned int uiPreviousBlockEntryIndex;	// The previous block entry. (N/A if == BlockCount.)
	unsigned int uiNextBlockEntryIndex;		// The next block entry. (N/A if == BlockCount.)
};

// End section.

struct DirectoryHeader_t
{
	unsigned int uiDummy0;				// Always 0x00000004
	unsigned int uiCacheID;				// Cache ID.
	unsigned int uiLastVersionPlayed;	// GCF file version.
	unsigned int uiItemCount;			// Number of items in the directory.
	unsigned int uiFileCount;			// Number of files in the directory.
	unsigned int uiDummy1;				// Always 0x00008000. Data per checksum?
	unsigned int uiDirectorySize;		// Size of lpGCFDirectoryEntries & lpGCFDirectoryNames & lpGCFDirectoryInfo1Entries & lpGCFDirectoryInfo2Entries & lpGCFDirectoryCopyEntries & lpGCFDirectoryLocalEntries in bytes.
	unsigned int uiNameSize;			// Size of the directory names in bytes.
	unsigned int uiInfo1Count;			// Number of Info1 entires.
	unsigned int uiCopyCount;			// Number of files to copy.
	unsigned int uiLocalCount;			// Number of files to keep local.
	unsigned int uiDummy2;
	unsigned int uiDummy3;
	unsigned int uiChecksum;			// Header checksum.
};

struct DirectoryEntry_t
{
	unsigned int uiNameOffset;		// Offset to the directory item name from the end of the directory items.
	unsigned int uiItemSize;		// Size of the item. (If file, file size. If folder, num items.)
	unsigned int uiChecksumIndex;	// Checksome index. (0xFFFFFFFF == None).
	unsigned int uiDirectoryFlags;	// Flags for the directory item. (0x00000000 == Folder).
	unsigned int uiParentIndex;		// Index of the parent directory item. (0xFFFFFFFF == None).
	unsigned int uiNextIndex;		// Index of the next directory item. (0x00000000 == None).
	unsigned int uiFirstIndex;		// Index of the first directory item. (0x00000000 == None).
};

struct DirectoryInfo1Entry_t
{
	unsigned int uiDummy0;
};

struct DirectoryInfo2Entry_t
{
	unsigned int uiDummy0;
};

struct DirectoryCopyEntry_t
{
	unsigned int uiDirectoryIndex;	// Index of the directory item.
};

struct DirectoryLocalEntry_t
{
	unsigned int uiDirectoryIndex;	// Index of the directory item.
};

// The below header was added in version 4 or version 5.

struct DirectoryMapHeader_t
{
	unsigned int uiDummy0;	// Always 0x00000001
	unsigned int uiDummy1;	// Always 0x00000000
};

struct DirectoryMapEntry_t
{
	unsigned int uiFirstBlockIndex;	// Index of the first data block. (N/A if == BlockCount.)
};

struct ChecksumHeader_t
{
	unsigned int uiDummy0;			// Always 0x00000001
	unsigned int uiChecksumSize;	// Size of LPGCFCHECKSUMHEADER & LPGCFCHECKSUMMAPHEADER & in bytes.
};

struct ChecksumMapHeader_t
{
	unsigned int uiDummy0;			// Always 0x14893721
	unsigned int uiDummy1;			// Always 0x00000001
	unsigned int uiItemCount;		// Number of items.
	unsigned int uiChecksumCount;	// Number of checksums.
};

struct ChecksumMapEntry_t
{
	unsigned int uiChecksumCount;		// Number of checksums.
	unsigned int uiFirstChecksumIndex;	// Index of first checksum.
};

struct ChecksumEntry_t
{
	unsigned long uiChecksum;	// Checksum.
};

struct DataBlockHeader_t
{
	unsigned int uiLastVersionPlayed;	// GCF file version. This field is not part of all file versions.
	unsigned int uiBlockCount;			// Number of data blocks.
	unsigned int uiBlockSize;			// Size of each data block in bytes.
	unsigned int uiFirstBlockOffset;	// Offset to first data block.
	unsigned int uiBlocksUsed;			// Number of data blocks that contain data.
	unsigned int uiChecksum;			// Header checksum.
};

#endif	// ifndef HALF_LIFE_ENGINE_SDK_STRUCTURES_GCF_FILE_INCLUDED