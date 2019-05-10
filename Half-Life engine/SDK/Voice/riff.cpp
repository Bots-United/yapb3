
#include <Core.hpp>

namespace HalfLifeEngine
{
	namespace SDK
	{
		namespace Voice
		{
#if 0
//-----------------------------------------------------------------------------
// Purpose: Test code that implements the interface on stdio
//-----------------------------------------------------------------------------
class StdIOReadBinary : public IFileReadBinary
{
public:
	int open( const char *pFileName )
	{
		return (int)fopen( pFileName, "rb" );
	}

	int read( void *pOutput, int size, int file )
	{
		FILE *fp = (FILE *)file;

		return fread( pOutput, size, 1, fp );
	}

	void seek( int file, int pos )
	{
		fseek( (FILE *)file, pos, SEEK_SET );
	}

	unsigned int tell( int file )
	{
		return ftell( (FILE *)file );
	}

	unsigned int size( int file )
	{
		FILE *fp = (FILE *)file;
		if ( !fp )
			return 0;

		unsigned int pos = ftell( fp );
		fseek( fp, 0, SEEK_END );
		unsigned int size = ftell( fp );

		fseek( fp, pos, SEEK_SET );
		return size;
	}

	void close( int file )
	{
		FILE *fp = (FILE *)file;

		fclose( fp );
	}
};
#endif


//-----------------------------------------------------------------------------
// Purpose: Opens a RIFF file using the given I/O mechanism
// Input  : *pFileName 
//			&io - I/O interface
//-----------------------------------------------------------------------------
InFileRIFF::InFileRIFF( const char *pFileName, IFileReadBinary &io ) : m_io(io)
{
	m_file = m_io.open( pFileName );
	
	int riff = 0;
	if ( !m_file )
	{
		m_riffSize = 0;
		m_riffName = 0;
		return;
	}
	m_io.read( &riff, 4, m_file );
	if ( riff != RIFF_ID )
	{
		printf("Not a RIFF File\n" );
		m_riffSize = 0;
	}
	else
	{
		// we store size as size of all chunks
		// subtract off the RIFF form type (e.g. 'WAVE', 4 bytes)
		m_riffSize = ReadInt() - 4;
		m_riffName = ReadInt();

		// HACKHACK: LWV files don't obey the RIFF format!!!
		// Do this or miss the linguistic chunks at the end. Lame!
		// subtract off 12 bytes for (RIFF, size, WAVE)
		m_riffSize = m_io.size( m_file ) - 12;
	}
}


//-----------------------------------------------------------------------------
// Purpose: Close the file
//-----------------------------------------------------------------------------
InFileRIFF::~InFileRIFF( void )
{
	m_io.close( m_file );
}


//-----------------------------------------------------------------------------
// Purpose: read a 4-byte int out of the stream
// Output : int = read value, default is zero
//-----------------------------------------------------------------------------
int InFileRIFF::ReadInt( void )
{
	int tmp = 0;
	m_io.read( &tmp, sizeof(int), m_file );

	return tmp;
}

//-----------------------------------------------------------------------------
// Purpose: Read a block of binary data
// Input  : *pOutput - pointer to destination memory
//			dataSize - size of block to read
// Output : int - number of bytes read
//-----------------------------------------------------------------------------
int InFileRIFF::ReadData( void *pOutput, int dataSize )
{
	int count = m_io.read( pOutput, dataSize, m_file );

	return count;
}


//-----------------------------------------------------------------------------
// Purpose: Gets the file position
// Output : int (bytes from start of file)
//-----------------------------------------------------------------------------
int InFileRIFF::PositionGet( void )
{
	return m_io.tell( m_file );
}


//-----------------------------------------------------------------------------
// Purpose: Seek to file position
// Input  : position - bytes from start of file
//-----------------------------------------------------------------------------
void InFileRIFF::PositionSet( int position )
{
	m_io.seek( m_file, position );
}



//-----------------------------------------------------------------------------
// Purpose: Create an iterator for the given file
// Input  : &riff - riff file
//			size - size of file or sub-chunk
//-----------------------------------------------------------------------------
IterateRIFF::IterateRIFF( InFileRIFF &riff, int size )
	: m_riff(riff), m_size(size)
{
	if ( !m_riff.RIFFSize() )
	{
		// bad file, just be an empty iterator
		ChunkClear();
		return;
	}

	// get the position and parse a chunk
	m_start = riff.PositionGet();
	ChunkSetup();
}


//-----------------------------------------------------------------------------
// Purpose: Set up a sub-chunk iterator
// Input  : &parent - parent iterator
//-----------------------------------------------------------------------------
IterateRIFF::IterateRIFF( IterateRIFF &parent )
	: m_riff(parent.m_riff), m_size(parent.ChunkSize())
{
	m_start = parent.ChunkFilePosition();
	ChunkSetup();
}

//-----------------------------------------------------------------------------
// Purpose: Parse the chunk at the current file position 
//			This object will iterate over the sub-chunks of this chunk.
//			This makes for easy hierarchical parsing
//-----------------------------------------------------------------------------
void IterateRIFF::ChunkSetup( void )
{
	m_chunkPosition = m_riff.PositionGet();

	m_chunkName = m_riff.ReadInt();
	m_chunkSize = m_riff.ReadInt();
}

//-----------------------------------------------------------------------------
// Purpose: clear chunk setup, ChunkAvailable will return false
//-----------------------------------------------------------------------------
void IterateRIFF::ChunkClear( void )
{
	m_chunkSize = -1;
}

//-----------------------------------------------------------------------------
// Purpose: If there are chunks left to read beyond this one, return true
//-----------------------------------------------------------------------------
bool IterateRIFF::ChunkAvailable( void )
{
	if ( m_chunkSize != -1 )
		return true;

	return false;
}


//-----------------------------------------------------------------------------
// Purpose: Go to the next chunk in the file, return true if there is one.
//-----------------------------------------------------------------------------
bool IterateRIFF::ChunkNext( void )
{
	if ( !ChunkAvailable() )
		return false;

	int nextPos = m_chunkPosition + 8 + m_chunkSize;
	
	// chunks are aligned
	nextPos += m_chunkSize & 1;

	if ( nextPos >= (m_start + m_size) )
	{
		ChunkClear();
		return false;
	}

	m_riff.PositionSet( nextPos );

	ChunkSetup();
	return ChunkAvailable();

}


//-----------------------------------------------------------------------------
// Purpose: get the chunk FOURCC as an int
// Output : unsigned int
//-----------------------------------------------------------------------------
unsigned int IterateRIFF::ChunkName( void )
{
	return m_chunkName;
}


//-----------------------------------------------------------------------------
// Purpose: get the size of this chunk
// Output : unsigned int
//-----------------------------------------------------------------------------
unsigned int IterateRIFF::ChunkSize( void )
{
	return m_chunkSize;
}

//-----------------------------------------------------------------------------
// Purpose: Read the entire chunk into a buffer
// Input  : *pOutput - dest buffer
// Output : int bytes read
//-----------------------------------------------------------------------------
int IterateRIFF::ChunkRead( void *pOutput )
{
	return m_riff.ReadData( pOutput, ChunkSize() );
}


//-----------------------------------------------------------------------------
// Purpose: Read a partial chunk (updates file position for subsequent partial reads).
// Input  : *pOutput - dest buffer
//			dataSize - partial size
// Output : int - bytes read
//-----------------------------------------------------------------------------
int IterateRIFF::ChunkReadPartial( void *pOutput, int dataSize )
{
	return m_riff.ReadData( pOutput, dataSize );
}


//-----------------------------------------------------------------------------
// Purpose: Read a 4-byte int
// Output : int - read int
//-----------------------------------------------------------------------------
int IterateRIFF::ChunkReadInt( void )
{
	return m_riff.ReadInt();
}
		}
	}
}