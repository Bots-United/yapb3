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
// Log.hpp
//
// Class: Log
//
// Description: YaPB logger.
//
// Version: $ID:$
//

#if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000
#	pragma once
#endif	// if defined COMPILER_VISUALC && COMPILER_VISUALC > 1000

#ifndef CORE_TOOL_LIBRARY_LOG_INCLUDED
#define CORE_TOOL_LIBRARY_LOG_INCLUDED

class Log
{
	//
	// Group: Private members.
	//
	private:
		DynamicString m_directory;
		DynamicString m_filename;
		unsigned char m_currentDay;		// current day of the month - [1, 31]

		char          m_message[1024u];	// Just a constant buffer storage....

		#define WRITE_NEW_MESSAGE_LINE(prefix, level, message)																																			\
			if (!CheckCreateDirectory ())																																								\
			{																																															\
				/* Free everything that's freeable */																																					\
/*				FreeAllMemory ();	// AUTOMATICLY CALLED BY DllMain()*/																																	\
																																																		\
/*				DestroyWindow (GetForegroundWindow ());*/																																					\
																																																		\
				/* once everything is freed, just exit and print to message box */																														\
				MessageBox																																												\
				(																																														\
					NULL,																																												\
																																																		\
					FormatBuffer ("Unable to create the log directory \"%s\" (Error: \"%s\") for writing new message: \"%s: %s\"!", m_directory.GetData (), GetLastErrorMessage (), level, message),	\
																																																		\
					PRODUCT_SHORT_NAME " Critical Error!",																																				\
					MB_ICONERROR | MB_TOPMOST | MB_SYSTEMMODAL																																			\
				);																																														\
																																																		\
				ExitProcess (1u);																																										\
			}																																															\
																																																		\
			const tm *const localTime (GetLocalTime ());																																				\
																																																		\
			CheckFilename (localTime);																																									\
																																																		\
			const DynamicString fullFilename (m_directory + m_filename);																																\
																																																		\
			/* try to open log file in a standard stream */																																				\
			STDIOFile file (fullFilename, "at");																																						\
																																																		\
			/* check if we got a valid log file handle */																																				\
			if (file.IsValid ())																																										\
			{																																															\
				/* dump the new line string into the file */																																			\
				file.PrintFormat (prefix "[%02u:%02u:%02u] %s: %s\n", localTime->tm_hour, localTime->tm_min, localTime->tm_sec, level, message);														\
			}																																															\
			else																																														\
			{																																															\
				/* Free everything that's freeable */																																					\
/*				FreeAllMemory ();	// AUTOMATICLY CALLED BY DllMain() */																																\
																																																		\
/*				DestroyWindow (GetForegroundWindow ());*/																																					\
																																																		\
				/* once everything is freed, just exit and print to message box */																														\
				MessageBox																																												\
				(																																														\
					NULL,																																												\
																																																		\
					FormatBuffer ("Unable to open the log file \"%s\" (Error: \"%s\") for writing new message: \"%s: %s\"!", fullFilename.GetData (), GetLastErrorMessage (), level, message),			\
																																																		\
					PRODUCT_SHORT_NAME " Critical Error!",																																				\
					MB_ICONERROR | MB_TOPMOST | MB_SYSTEMMODAL																																			\
				);																																														\
																																																		\
				ExitProcess (1u);																																										\
			}																																															\

	//
	// Group: (Con/De)structors.
	//
	public:
		inline Log (void) : m_currentDay (static_cast <const unsigned char> (GetLocalTime ()->tm_mday))
		{
			SetDefaultDirectory ();
			SetDefaultFilename ();

//			for (unsigned short messageIndex (0u); messageIndex < sizeof (m_message); ++messageIndex)
//				m_message[messageIndex] = '\0';
		}
		inline ~Log (void)
		{
			WRITE_NEW_MESSAGE_LINE ("\n", "Log", "|--------------- " PRODUCT_SHORT_NAME " log file session closed! ---------------|\n");
		}

	//
	// Group: Private functions.
	//
	private:
		inline void SetDefaultDirectory (void);	// Note: This function declared in Trampolines.hpp.
		inline void SetDefaultFilename (const tm *const localTime)
		{
			m_filename.AssignFormat ("/%02u.%02u.%04u.LOG", localTime->tm_mday, localTime->tm_mon + 1u, localTime->tm_year + 1900u);
		}
		inline void SetDefaultFilename (void)
		{
			// get current local time and set filename
			SetDefaultFilename (GetLocalTime ());
		}

		inline const bool CheckCreateDirectory (void) const { return IsDirectoryExists (m_directory) || CreatePath (m_directory); }
		inline const bool CheckFilename (const tm *const localTime)
		{
			if (m_currentDay == localTime->tm_mday)
				return false;

			m_currentDay = static_cast <const unsigned char> (localTime->tm_mday);

			SetDefaultFilename (localTime);

			return true;
		}
		inline const bool CheckFilename (void)
		{
			// get current local time and check filename
			return CheckFilename (GetLocalTime ());
		}

		inline void WriteNewMessageLine                    (const char *const level, const char *const message)
		{
			WRITE_NEW_MESSAGE_LINE ("", level, message);
		}
		inline void WriteNewMessageLineFormat              (const char *const level, const char *const format, ...)
		{
			// Concatenate all the arguments in one string....
			COLLECT_AND_STORE_ARGUMENTS_INTO_ONE_STRING (format, m_message, sizeof (m_message));

			WriteNewMessageLine (level, m_message);
		}

		// default log message
		inline void JustWriteDefaultMessage (const char *const message)
		{
			WriteNewMessageLine ("Log", message);
		}
		// note log message
		inline void JustWriteNoteMessage    (const char *const message)
		{
			WriteNewMessageLine ("Note", message);
		}
		// warning log message
		inline void JustWriteWarningMessage (const char *const message)
		{
			WriteNewMessageLine ("Warning", message);
		}
		// error log message
		inline void JustWriteErrorMessage   (const char *const message)
		{
			WriteNewMessageLine ("Error", message);
		}

	//
	// Group: Functions.
	//
	public:
		inline void Initialize (void)
		{
			JustWriteDefaultMessage ("|--------------- " PRODUCT_SHORT_NAME " log file session started! ---------------|\n");
		}

		inline void SetDirectory (const DynamicString &directory)
		{
			m_directory = directory;

			Initialize ();
		}

		/// @note The below 8 function are declared in the Core.hpp.

		// default log message
		inline void WriteDefaultMessage             (const char *const message);
		inline void WriteDefaultMessageFormat       (const char *const format, ...);

		// note log message
		inline void WriteNoteMessage                (const char *const message);
		inline void WriteNoteMessageFormat          (const char *const format, ...);

		// warning log message
		inline void WriteWarningMessage             (const char *const message);
		inline void WriteWarningMessageFormat       (const char *const format, ...);

		// error log message
		inline void WriteErrorMessage               (const char *const message);
		inline void WriteErrorMessageFormat         (const char *const format, ...);

		// fatal error log message (terminate the program!)
		inline void WriteCriticalErrorMessage       (const char *const message)
		{
			WriteNewMessageLine ("Critical", message);

			// Free everything that's freeable
//			FreeAllMemory ();	// AUTOMATICLY CALLED BY DllMain()

//			DestroyWindow (GetForegroundWindow ());

			// once everything is freed, just exit and print to message box
			MessageBox (NULL, message, PRODUCT_SHORT_NAME " Critical Error", MB_ICONERROR | MB_TOPMOST | MB_SYSTEMMODAL);

			ExitProcess (1u);
		}
		inline void WriteCriticalErrorMessageFormat (const char *const format, ...)
		{
			// Concatenate all the arguments in one string....
			COLLECT_AND_STORE_ARGUMENTS_INTO_ONE_STRING (format, m_message, sizeof (m_message));

			WriteCriticalErrorMessage (m_message);
		}
};

extern Log g_log;

#endif	// ifndef CORE_TOOL_LIBRARY_LOG_INCLUDED