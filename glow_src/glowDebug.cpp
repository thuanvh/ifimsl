/*
===============================================================================

	FILE:  glowDebug.cpp
	
	PROJECT:
	
		The GLOW Toolkit
	
	CONTENTS:
	
		Program logic error reporting utility class
	
	PROGRAMMERS:
	
		Daniel Azuma (DA)  <dazuma@kagi.com>
	
	COPYRIGHT:
	
		Copyright (C) 1997-2000  Daniel Azuma  (dazuma@kagi.com)
		
		This library is free software; you can redistribute it and/or
		modify it under the terms of the GNU Lesser General Public
		License as published by the Free Software Foundation; either
		version 2.1 of the License, or (at your option) any later version.
		
		This library is distributed in the hope that it will be useful,
		but WITHOUT ANY WARRANTY; without even the implied warranty of
		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
		GNU Lesser General Public License for more details.
		
		You should have received a copy of the GNU Lesser General Public
		License along with this library; if not, write to
			Free Software Foundation, Inc.
			59 Temple Place, Suite 330
			Boston, MA 02111-1307 USA
	
	VERSION:
	
		The GLOW Toolkit -- version 1.0.3  (10 September 2000)
	
	CHANGES:
	
		Original release (DA)

===============================================================================
*/


/*
===============================================================================
	Headers and declarations
===============================================================================
*/

#ifndef GLOW_HEADER__H
	#include "glowHeader.h"
#endif

#if !defined(GLOW_PLATFORM_MACOS) || !defined(GLOW_OPTION_REPORTDEBUGSTR)
	#include <cstdlib>
	#ifdef GLOW_OPTION_NOIOSTREAMS
		#include <cstdio>
	#else
		#include <iostream>
	#endif
#endif

#ifndef GLOW_DEBUG__H
	#include "glowDebug.h"
#endif


GLOW_INTERNAL_USINGSTD
GLOW_NAMESPACE_BEGIN


/*
===============================================================================
	Methods of VDebug
===============================================================================
*/

#ifdef GLOW_OPTION_DEBUG

#if defined(GLOW_PLATFORM_MACOS) && defined(GLOW_OPTION_REPORTDEBUGSTR)


void Glow_Debug_::Report(
	const char* message,
	const char* fileName,
	long line)
	throw()
{
	static Str255 str;
	short j=1;
	
	for (short i=0; message[i]!='\0' && j<256; i++)
	{
		str[j]=message[i];
		j++;
	}
	static const unsigned char* add1 = "\p, FILE: ";
	for (short i=1; i<=add1[0] && j<256; i++)
	{
		str[j] = add1[i];
		j++;
	}
	for (short i=0; fileName[i]!='\0' && j<256; i++)
	{
		str[j]=message[i];
		j++;
	}
	static const unsigned char* add2 = "\p, LINE: ";
	for (short i=1; i<=add2[0] && j<256; i++)
	{
		str[j] = add2[i];
		j++;
	}
	static unsigned char add3[10];
	::NumToString(line, add3);
	for (short i=1; i<=add3[0] && j<256; i++)
	{
		str[j] = add3[i];
		j++;
	}
	str[0] = j-1;
	
	::DebugStr(str);
}


void Glow_Debug_::Warning(
	const char* message,
	const char* fileName,
	long line)
	throw()
{
	static Str255 str;
	short j=1;
	
	static const unsigned char* add0 = "\pWARNING: ";
	for (short i=1; i<=add0[0] && j<256; i++)
	{
		str[j] = add0[i];
		j++;
	}
	for (short i=0; message[i]!='\0' && j<256; i++)
	{
		str[j]=message[i];
		j++;
	}
	static const unsigned char* add1 = "\p, FILE: ";
	for (short i=1; i<=add1[0] && j<256; i++)
	{
		str[j] = add1[i];
		j++;
	}
	for (short i=0; fileName[i]!='\0' && j<256; i++)
	{
		str[j]=message[i];
		j++;
	}
	static const unsigned char* add2 = "\p, LINE: ";
	for (short i=1; i<=add2[0] && j<256; i++)
	{
		str[j] = add2[i];
		j++;
	}
	static unsigned char add3[10];
	::NumToString(line, add3);
	for (short i=1; i<=add3[0] && j<256; i++)
	{
		str[j] = add3[i];
		j++;
	}
	str[0] = j-1;
	
	::DebugStr(str);
}


void Glow_Debug_::Assert(
	const char* message,
	const char* fileName,
	long line)
	throw()
{
	static Str255 str;
	short j=1;
	
	static const unsigned char* add0 = "\pASSERTION FAILED: ";
	for (short i=1; i<=add0[0] && j<256; i++)
	{
		str[j] = add0[i];
		j++;
	}
	for (short i=0; message[i]!='\0' && j<256; i++)
	{
		str[j]=message[i];
		j++;
	}
	static const unsigned char* add1 = "\p, FILE: ";
	for (short i=1; i<=add1[0] && j<256; i++)
	{
		str[j] = add1[i];
		j++;
	}
	for (short i=0; fileName[i]!='\0' && j<256; i++)
	{
		str[j]=message[i];
		j++;
	}
	static const unsigned char* add2 = "\p, LINE: ";
	for (short i=1; i<=add2[0] && j<256; i++)
	{
		str[j] = add2[i];
		j++;
	}
	static unsigned char add3[10];
	::NumToString(line, add3);
	for (short i=1; i<=add3[0] && j<256; i++)
	{
		str[j] = add3[i];
		j++;
	}
	str[0] = j-1;
	
	::DebugStr(str);
}


void Glow_Debug_::Signal(
	int val)
	throw()
{
	static Str255 str;
	short j=1;
	
	static const unsigned char* add0 = "\pSIGNAL CAUGHT: ";
	for (short i=1; i<=add0[0] && j<256; i++)
	{
		str[j] = add0[i];
		j++;
	}
	static unsigned char add1[10];
	::NumToString(line, add1);
	for (short i=1; i<=add1[0] && j<256; i++)
	{
		str[j] = add1[i];
		j++;
	}
	str[0] = j-1;
	
	::DebugStr(str);
}


#else


#ifdef GLOW_COMPAT_NOSTDNAMESPACE
	#define GLOW_INTERNAL_STDERR stderr
#else
	#define GLOW_INTERNAL_STDERR std::stderr
#endif


void Glow_Debug_::Report(
	const char* message,
	const char* fileName,
	long line)
	throw()
{
#ifdef GLOW_OPTION_NOIOSTREAMS
	GLOW_CSTD::fprintf(GLOW_INTERNAL_STDERR,
		"\nGLOW DEBUG MESSAGE:\n%s\n... in file \"%s\", line %d\n\n",
		message, fileName, line);
	Glow_DebugScope_::PrintScopeTrace();
	GLOW_CSTD::fprintf(GLOW_INTERNAL_STDERR, "\n");
#else
	GLOW_STD::cerr << "\nGLOW DEBUG MESSAGE:\n" << message <<
		"\n... in file \"" << fileName << "\", line " << line <<
		GLOW_STD::endl << GLOW_STD::endl;
	Glow_DebugScope_::PrintScopeTrace();
	GLOW_STD::cerr << GLOW_STD::endl;
#endif
	GLOW_CSTD::abort();
}


void Glow_Debug_::Warning(
	const char* message,
	const char* fileName,
	long line)
	throw()
{
#ifdef GLOW_OPTION_NOIOSTREAMS
	GLOW_CSTD::fprintf(GLOW_INTERNAL_STDERR,
		"\nGLOW WARNING:\n%s\n... in file \"%s\", line %d\n\n",
		message, fileName, line);
	Glow_DebugScope_::PrintScopeTrace();
	GLOW_CSTD::fprintf(GLOW_INTERNAL_STDERR, "\n");
#else
	GLOW_STD::cerr << "\nGLOW WARNING:\n" << message <<
		"\n... in file \"" << fileName << "\", line " << line <<
		GLOW_STD::endl << GLOW_STD::endl;
	Glow_DebugScope_::PrintScopeTrace();
	GLOW_STD::cerr << GLOW_STD::endl;
#endif
}


void Glow_Debug_::Assert(
	const char* message,
	const char* fileName,
	long line)
	throw()
{
#ifdef GLOW_OPTION_NOIOSTREAMS
	GLOW_CSTD::fprintf(GLOW_INTERNAL_STDERR,
		"\nGLOW ASSERTION FAILURE:\n%s\n... in file \"%s\", line %d\n\n",
		message, fileName, line);
	Glow_DebugScope_::PrintScopeTrace();
	GLOW_CSTD::fprintf(GLOW_INTERNAL_STDERR, "\n");
#else
	GLOW_STD::cerr << "\nGLOW ASSERTION FAILURE:\n" << message <<
		"\n... in file \"" << fileName << "\", line " << line <<
		GLOW_STD::endl << GLOW_STD::endl;
	Glow_DebugScope_::PrintScopeTrace();
	GLOW_STD::cerr << GLOW_STD::endl;
#endif
	GLOW_CSTD::abort();
}


void Glow_Debug_::Signal(
	int val)
	throw()
{
#ifdef GLOW_OPTION_NOIOSTREAMS
	GLOW_CSTD::fprintf(GLOW_INTERNAL_STDERR,
		"\nGLOW SIGNAL CAUGHT:\nvalue = %d ", val);
	if (val == SIGSEGV)
		GLOW_CSTD::fprintf(GLOW_INTERNAL_STDERR, "(SIGSEGV)");
	if (val == SIGINT)
		GLOW_CSTD::fprintf(GLOW_INTERNAL_STDERR, "(SIGINT)");
	if (val == SIGFPE)
		GLOW_CSTD::fprintf(GLOW_INTERNAL_STDERR, "(SIGFPE)");
	if (val == SIGILL)
		GLOW_CSTD::fprintf(GLOW_INTERNAL_STDERR, "(SIGILL)");
	GLOW_CSTD::fprintf(GLOW_INTERNAL_STDERR, "\n\n");
	Glow_DebugScope_::PrintScopeTrace();
	GLOW_CSTD::fprintf(GLOW_INTERNAL_STDERR, "\n");
#else
	GLOW_STD::cerr << "\nGLOW SIGNAL CAUGHT:\nvalue = " << val << ' ';
	if (val == SIGSEGV)
		GLOW_STD::cerr << "(SIGSEGV)";
	if (val == SIGINT)
		GLOW_STD::cerr << "(SIGINT)";
	if (val == SIGFPE)
		GLOW_STD::cerr << "(SIGFPE)";
	if (val == SIGILL)
		GLOW_STD::cerr << "(SIGILL)";
	GLOW_STD::cerr << GLOW_STD::endl << GLOW_STD::endl;
	Glow_DebugScope_::PrintScopeTrace();
	GLOW_STD::cerr << GLOW_STD::endl;
#endif
	GLOW_CSTD::abort();
}


#endif


Glow_DebugScope_* Glow_DebugScope_::_top = 0;


void Glow_DebugScope_::PrintScopeTrace()
	throw()
{
#if !defined(GLOW_PLATFORM_MACOS) || !defined(GLOW_OPTION_REPORTDEBUGSTR)
#ifdef GLOW_OPTION_NOIOSTREAMS
	GLOW_CSTD::fprintf(GLOW_INTERNAL_STDERR, "SCOPE TRACE:\n");
	for (Glow_DebugScope_* iter = _top; iter != 0; iter = iter->_next)
	{
		GLOW_CSTD::fprintf(GLOW_INTERNAL_STDERR, "%s\n", iter->_data);
	}
#else
	GLOW_STD::cerr << "SCOPE TRACE:";
	for (Glow_DebugScope_* iter = _top; iter != 0; iter = iter->_next)
	{
		GLOW_STD::cerr << "\n" << iter->_data;
	}
	GLOW_STD::cerr << GLOW_STD::endl;
#endif
#endif
}


#endif


/*
===============================================================================
*/

GLOW_NAMESPACE_END

