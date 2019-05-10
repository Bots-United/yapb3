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
// Functions.hpp
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef FUNCTIONS_INCLUDED
#define FUNCTIONS_INCLUDED

inline const tm *const GetLocalTime (void)
{
	const time_t tickTime (time (NULL));

	return localtime (&tickTime);
}

inline const char *const GetLastErrorMessage (void)
{
	// Windows doesn't provide a functon analagous to dlerr() that returns a
	// string describing the error, so we include one here, as exampled at:
	// http://msdn.microsoft.com/library/en-us/debug/errors_0sdh.asp
	// except without FORMAT_MESSAGE_ALLOCATE_BUFFER,
	// since we use a local static 'errorMessage' buffer.

	static char errorMessage[GlobalConstant_MaximumBufferLength];

	if (FormatMessage (FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError (), MAKELANGID (LANG_NEUTRAL, SUBLANG_DEFAULT), errorMessage, sizeof (errorMessage), NULL) == 0)
		FormatInputBuffer (errorMessage, sizeof (errorMessage), "code %08x", GetLastError ());

	// Return message with default language.
	return errorMessage;
}

inline const bool GetCurrentWorkingDirectoryPath (char *const buffer, const unsigned short bufferSize/* Usually 'MAX_PATH' which is the maximum length allowed for a path */)
{
	// Function to return the current working directory.
	// This is generally the application path.

	return getcwd (buffer, bufferSize) != NULL;
}
inline const bool GetCurrentWorkingDirectoryPath (DynamicString &path)
{
	if (!path.SetSizeWithoutKeepingData (MAX_PATH))
		return false;

	return GetCurrentWorkingDirectoryPath (path, path.GetAllocatedSize ());
}

inline const bool IsDirectoryExists (const char *const folderName)
{
	const int directoryAttributes (GetFileAttributes (folderName));

	return directoryAttributes != INVALID_FILE_ATTRIBUTES && (directoryAttributes & FILE_ATTRIBUTE_DIRECTORY);
}

inline const bool CreateDirectory (const char *const directory)
{
	// This function creates a directory, by given 'directory' string.

	return mkdir (directory) == 0/* || errno == EEXIST*/;
}
inline const bool CreatePath (const char *const pathname)
{
	// This function creates a directory tree, by given 'pathname' string.

	// Reliability check.
	InternalAssert (!IsNullOrEmptyString (pathname));

	char directory[1024u] = {pathname[0u]/*, ... */};
	unsigned short index (1u);

	for (/* Empty */; /* Empty */; /* Empty */)
	{
		// Reliability check.
		InternalAssert (index < sizeof (directory));

		if (pathname[index] != '/' && pathname[index] != '\\')
			goto Label_End;

		directory[index] = '\0';

		// create the directory
		if (!IsDirectoryExists (directory) && !CreateDirectory (directory))
			return false;

		Label_End:
		{
			directory[index] = pathname[index];

			if (pathname[++index] != '\0')
				continue;

			directory[index] = '\0';

			// create the directory
			return IsDirectoryExists (directory) || CreateDirectory (directory);
		}
	}
/*/
	// We use the shell's "md" command here instead of the mkdir() function because
	// md will create all the subdirectories leading up to the bottom one and mkdir() won't.
	char command[1024u];

	_snprintf (command, sizeof (command), "md \"%s\"", pathname);

	system (command);

	return access (pathname, 0) == 0;*/
}

inline const bool IsEmptyOrCommentedFileBufferLine (const DynamicString &fileBufferLine)
{
	// This function returns true if the given 'fileBufferLine' is empty or commented, false otherwise.

	return fileBufferLine.IsEmpty () || fileBufferLine[0u] == '/' || fileBufferLine[0u] == ';' || fileBufferLine[0u] == '#' || fileBufferLine[0u] == '\n' || fileBufferLine[0u] == '\r';
/*
	static CharacterSet commentedCharacterSet ("/;#\n\r");

	return fileBufferLine.IsEmpty () || commentedCharacterSet[fileBufferLine[0u]];*/
}

template <typename elementType> inline const unsigned char GetDigitsNumber (elementType value)
{
	unsigned char digitsNumber (0u);

	// Count all digits in the value....
	do
	{
		++digitsNumber;
	} while ((value /= 10) != 0);

	return digitsNumber;
}

inline const char *const StringMakeLowerCase (char *const string)
{
	// lower a string to make it lower case.

	// Reliability check.
	InternalAssert (!IsNullOrEmptyString (string));

	char *tempString (string);

	do
	{
		*tempString = static_cast <char> (tolower (*tempString));
	} while (*++tempString != '\0');

	return string;
}
inline const char *const StringMakeUpperCase (char *const string)
{
	// higher a string to make it upper case.

	// Reliability check.
	InternalAssert (!IsNullOrEmptyString (string));

	char *tempString (string);

	do
	{
		*tempString = static_cast <char> (toupper (*tempString));
	} while (*++tempString != '\0');

	return string;
}

inline const unsigned short DecodeHexString (unsigned char *const buffer, const unsigned short maximumLength, const char *const hexString, const unsigned short hexStringLength)
{
	// Also known as StringToSignature().

	unsigned short written (0u);

	for (unsigned short hexStringIndex (0u); hexStringIndex < hexStringLength && written < maximumLength; ++hexStringIndex)
	{
		buffer[written] = hexString[hexStringIndex];

		++written;

		if (hexString[hexStringIndex] != '\\' || hexString[hexStringIndex + 1u] != 'x' || hexStringIndex + 3u >= hexStringLength)
			continue;

		// Get the hex part.
		const char s_byte[3u] = {hexString[hexStringIndex + 2u], hexString[hexStringIndex + 3u], '\0'};
		int r_byte;

		sscanf (s_byte, "%x", &r_byte);	// Read it as an integer

		buffer[written - 1u] = static_cast <unsigned char> (r_byte);	// Save the value

		hexStringIndex += 3u;	// Adjust index
	}

	return written;
}
inline const unsigned short DecodeHexString (unsigned char *const buffer, const unsigned short maximumLength, const char *const hexString)
{
	return DecodeHexString (buffer, maximumLength, hexString, GetStringLength <unsigned short> (hexString));
}
inline const unsigned short DecodeHexString (unsigned char *const buffer, const unsigned short maximumLength, const DynamicString &hexString)
{
	return DecodeHexString (buffer, maximumLength, hexString, hexString.GetElementNumber ());
}

inline const bool GetLineFromFileBuffer (const char *const fileBuffer, unsigned int fileSize, unsigned int &filePosition, DynamicString &buffer, const unsigned short bufferSize)
{
	// Bullet-proofing
	InternalAssert (filePosition < fileSize);

	unsigned int index (filePosition);

	// fgets always NULL terminates, so only read bufferSize - 1 characters
	if (fileSize - filePosition > bufferSize - 1u)
		fileSize = filePosition + bufferSize - 1u;

	// Stop at the next newline (inclusive) or end of buffer
	while (index < fileSize)
		if (fileBuffer[index++] == '\n')
			break;

	// If we actually advanced the pointer, copy it over
	if (index > filePosition)
	{
		// We read in size bytes, copy it out
		buffer.Assign (fileBuffer + filePosition, static_cast <unsigned short> (index - filePosition));

		// Update file pointer
		filePosition = index;

		return true;
	}

	// No data read, bail
	return false;
}

inline const bool IsPathSeparator (const char character) { return character == '\\' || character == '/'; }

inline const bool IsFilenameHasExtension (const DynamicString &filename)
{
	// Reliability check.
	InternalAssert (filename.GetElementNumber () >= 2u);

	unsigned short endPosition (filename.GetElementNumber () - 1u);

	while (filename[endPosition] != '.')
		if (IsPathSeparator (filename[endPosition]) || --endPosition == 0u)
			return false;

	return endPosition < filename.GetElementNumber () - 1u;
}
inline const bool IsFilenameHasExtension (const DynamicString &filename, const DynamicString &extension)
{
	// Reliability checks.
	InternalAssert (filename.GetElementNumber () >= 2u);
	InternalAssert (!extension.IsEmpty ());
	InternalAssert (extension[0u] != '.');

	unsigned short endPosition (filename.GetElementNumber () - 1u);

	while (filename[endPosition] != '.')
		if (IsPathSeparator (filename[endPosition]) || --endPosition == 0u)
			return false;

	if (++endPosition == filename.GetElementNumber ())
		return false;

	return filename.CompareWithoutCaseCheck (endPosition, extension) == 0;
}
inline const bool ReplaceFileExtension (DynamicString &filename, const DynamicString &newExtension)
{
//	const unsigned short oldExtensionStartPosition (filename.ReverseFindOneOfWithCaseCheck (".\\/"));

	// Reliability checks.
	InternalAssert (IsFilenameHasExtension (filename));
	InternalAssert (!IsFilenameHasExtension (filename, newExtension));

	unsigned short endPosition (filename.GetElementNumber () - 1u);

	while (filename[endPosition] != '.')
		if (IsPathSeparator (filename[endPosition]) || --endPosition == 0u)
			return false;

	if (++endPosition == filename.GetElementNumber ())
		return false;

	return filename.Replace (endPosition, newExtension);
}
inline const bool StripFilenameAndPathSeparator (DynamicString &filename)
{
	// Reliability checks.
	InternalAssert (IsFilenameHasExtension (filename));

	unsigned short endPosition (filename.GetElementNumber () - 1u);

	while (!IsPathSeparator (filename[endPosition]))
		if (--endPosition == 0u)
			return false;

	filename.ReleaseBuffer (endPosition);

	return true;
}

#endif	// ifndef FUNCTIONS_INCLUDED