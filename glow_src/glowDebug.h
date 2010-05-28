/*
===============================================================================

	FILE:  glowDebug.h
	
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


#ifndef GLOW_DEBUG__H
#define GLOW_DEBUG__H


/*
===============================================================================
	Headers and declarations
===============================================================================
*/

#ifndef GLOW_HEADER__H
	#include "glowHeader.h"
#endif

#include <csignal>


GLOW_INTERNAL_USINGSTD


#ifdef GLOW_OPTION_DEBUG
	#define GLOW_DEBUG(a,b) if (a) GLOW_NAMESPACE::Glow_Debug_::Report((b),__FILE__,__LINE__)
	#define GLOW_WARNING(a,b) if (a) GLOW_NAMESPACE::Glow_Debug_::Warning((b),__FILE__,__LINE__)
	#define GLOW_ASSERT(a) if (!(a)) GLOW_NAMESPACE::Glow_Debug_::Assert( # a ,__FILE__,__LINE__)
	#define GLOW_DEBUGSCOPE(a) GLOW_NAMESPACE::Glow_DebugScope_ glow_debugscope_(a)
	#define GLOW_DEBUGSIGNAL(a) GLOW_CSTD::signal(a, GLOW_NAMESPACE::Glow_Debug_::Signal)
	#define GLOW_TRACESCOPE() GLOW_NAMESPACE::Glow_DebugScope_::PrintScopeTrace()
#else
	#define GLOW_DEBUG(a,b)
	#define GLOW_WARNING(a,b)
	#define GLOW_ASSERT(a)
	#define GLOW_DEBUGSCOPE(a)
	#define GLOW_DEBUGSIGNAL(a)
	#define GLOW_TRACESCOPE()
#endif



#ifdef GLOW_OPTION_DEBUG

GLOW_NAMESPACE_BEGIN


/*
===============================================================================
	CLASS Glow_Debug_

	Error reporting class
===============================================================================
*/

class Glow_Debug_
{
	public:
	
		static void Report(
			const char* message,
			const char* fileName,
			long line)
			throw();
		static void Warning(
			const char* message,
			const char* fileName,
			long line)
			throw();
		static void Assert(
			const char* message,
			const char* fileName,
			long line)
			throw();
		static void Signal(
			int val)
			throw();
};


/*
===============================================================================
	CLASS Glow_DebugScope_

	Scope reporting class
===============================================================================
*/

class Glow_DebugScope_
{
	public:
	
		inline Glow_DebugScope_(
			const char* name)
			throw()
		{
			_data = name;
			_next = _top;
			_top = this;
		}
		inline ~Glow_DebugScope_()
			throw()
		{
			_top = _next;
		}
		
		static void PrintScopeTrace()
			throw();
	
	private:
	
		const char* _data;
		Glow_DebugScope_* _next;
	
	private:
	
		static Glow_DebugScope_* _top;
};


/*
===============================================================================
*/


GLOW_NAMESPACE_END

#endif



#endif
