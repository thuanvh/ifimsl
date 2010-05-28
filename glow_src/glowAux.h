/*
===============================================================================

	FILE:  glowAux.h
	
	PROJECT:
	
		The GLOW Toolkit
	
	CONTENTS:
	
		Auxiliary classes for GLOW
	
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


#ifndef GLOW_AUX__H
#define GLOW_AUX__H


/*
===============================================================================
	Headers and declarations
===============================================================================
*/

#ifndef GLOW_HEADER__H
	#include "glowHeader.h"
#endif

// Microsoft cluelessness: the win32 gl.h depends on windows.h but
// doesn't include it.
#if defined(_WIN32) || defined(WIN32)
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
#endif

#ifdef GLOW_COMPAT_NOINCLUDESUBDIRECTORIES
	#include <glut.h>
#else
	#include <GL/glut.h>
#endif


GLOW_INTERNAL_USINGSTD
GLOW_NAMESPACE_BEGIN


/*
===============================================================================
	CLASS GlowFont
===============================================================================
*/

class GlowFont
{
	//-------------------------------------------------------------------------
	//	Public interface
	//-------------------------------------------------------------------------
	
	public:
	
		static GlowFont fixed8by13;
		static GlowFont fixed9by15;
		static GlowFont helvetica10;
		static GlowFont helvetica12;
		static GlowFont helvetica18;
		static GlowFont timesRoman10;
		static GlowFont timesRoman24;
	
	public:
	
		inline GlowFont();
		inline GlowFont(
			void* glutFont);
		
		inline operator void*();
		inline operator const void*() const;
		inline GlowFont& operator=(
			const GlowFont& font);
		inline GlowFont& operator=(
			void* glutFont);
		inline bool operator==(
			void* op2) const;
	
	public:
	
		int Leading() const;
		int BaselinePos() const;
		int StringWidth(
			const char* str) const;
		inline int CharWidth(
			int ch) const;
	
	public:
	
		static void SetLeadingMetric(
			GlowFont font,
			int value);
		static void SetBaselineMetric(
			GlowFont font,
			int value);
	
	
	//-------------------------------------------------------------------------
	//	Private implementation
	//-------------------------------------------------------------------------
	
	private:
	
		void* _glutFont;
	
	private:
	
		static int _leading8by13;
		static int _leading9by15;
		static int _leadingHelve10;
		static int _leadingHelve12;
		static int _leadingHelve18;
		static int _leadingTimes10;
		static int _leadingTimes24;
		
		static int _baseline8by13;
		static int _baseline9by15;
		static int _baselineHelve10;
		static int _baselineHelve12;
		static int _baselineHelve18;
		static int _baselineTimes10;
		static int _baselineTimes24;
};



/*
===============================================================================
	CLASS GlowColor

	Color value for Glow components
===============================================================================
*/

class GlowColor
{
	//-------------------------------------------------------------------------
	//	Public interface
	//-------------------------------------------------------------------------
	
	public:
	
		static const GlowColor black;
		static const GlowColor blue;
		static const GlowColor green;
		static const GlowColor cyan;
		static const GlowColor red;
		static const GlowColor magenta;
		static const GlowColor yellow;
		static const GlowColor white;
		static const GlowColor gray;
	
	public:
	
		inline explicit GlowColor(
			GLubyte rgbval = 0,
			GLubyte aval = 255);
		inline explicit GlowColor(
			GLubyte r,
			GLubyte g,
			GLubyte b,
			GLubyte a = 255);
		inline explicit GlowColor(
			GLfloat r,
			GLfloat g,
			GLfloat b,
			GLfloat a = 1.0f);
		inline explicit GlowColor(
			GLdouble r,
			GLdouble g,
			GLdouble b,
			GLdouble a = 1.0);
		
		inline void Set(
			GLubyte rgbval = 0,
			GLubyte aval = 255);
		inline void Set(
			GLubyte r,
			GLubyte g,
			GLubyte b,
			GLubyte a = 255);
		inline void Set(
			GLfloat r,
			GLfloat g,
			GLfloat b,
			GLfloat a = 1.0f);
		inline void Set(
			GLdouble r,
			GLdouble g,
			GLdouble b,
			GLdouble a = 1.0);
		
		inline operator GLubyte*();
		inline operator const GLubyte*() const;
		inline GLubyte& operator[](
			int i);
		inline const GLubyte& operator[](
			int i) const;
		
		inline bool operator==(
			const GlowColor& op2) const;
		inline bool operator!=(
			const GlowColor& op2) const;
		
		inline void Apply() const;
	
	
	//-------------------------------------------------------------------------
	//	Private implementation
	//-------------------------------------------------------------------------
	
	private:
	
		GLubyte _rgba[4];
};


/*
===============================================================================
*/

GLOW_NAMESPACE_END


#include "glowAux.inl.h"


#endif

