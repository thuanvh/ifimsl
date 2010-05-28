/*
===============================================================================

	FILE:  glowTextData.h
	
	PROJECT:
	
		The GLOW Toolkit
	
	CONTENTS:
	
		Text data for GLOW
	
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


#ifndef GLOW_TEXTDATA__H
#define GLOW_TEXTDATA__H


/*
===============================================================================
	Headers and declarations
===============================================================================
*/

#ifndef GLOW_HEADER__H
	#include "glowHeader.h"
#endif

#include <string>
#include <vector>

#ifndef GLOW_AUX__H
	#include "glowAux.h"
#endif


GLOW_INTERNAL_USINGSTD
GLOW_NAMESPACE_BEGIN


/*
===============================================================================
	CLASS GlowTextData
	
	Data for text field
===============================================================================
*/

class GlowTextData :
	public GLOW_STD::string
{
	//-------------------------------------------------------------------------
	//	Public interface
	//-------------------------------------------------------------------------
	
	public:
	
		inline GlowTextData();
		inline GlowTextData(
			const GLOW_STD::string& str);
		inline GlowTextData(
			const char* str);
	
	public:
	
		inline void ClearLineBreaks();
		void RecalcLineBreaks(
			GlowFont font,
			int pixelWidth,
			int startingLine = 0);
		inline int NumLines() const;
		inline GLOW_STD::string Line(
			int num) const;
		int LineNumOf(
			int pos) const;
		int LinePixelWidth(
			GlowFont font,
			int line) const;
		int MaxPixelWidth(
			GlowFont font) const;
		int AtPixelPos(
			GlowFont font,
			int line,
			int pixpos) const;
		void CalcLineDrawInfo(
			GlowFont font,
			int line,
			int pixoffset,
			int pixwidth,
			int& start,
			int& end,
			int& leftoffset) const;
		inline GLOW_STD::string ToEndOfLine(
			int pos) const;
		int PixelPosOf(
			GlowFont font,
			int pos,
			int line = -1) const;
	
	public:
	
		inline int SelectionStart() const;
		inline int SelectionEnd() const;
		inline int SelectionLength() const;
		inline void SetSelection(
			int start,
			int end);
		inline void SetSelection(
			int pos);
		inline GLOW_STD::string SelectedText() const;
		void RevalidateSelection();
		
		inline void ReplaceSelectionWith(
			const char* str);
		inline void ReplaceSelectionWith(
			char ch);
		inline void DeleteSelection();
	
	
	//-------------------------------------------------------------------------
	//	Private implementation
	//-------------------------------------------------------------------------
	
	private:
	
		GLOW_STD::vector<int> _lineBreaks;
		int _selStart;
		int _selEnd;
};


/*
===============================================================================
*/

GLOW_NAMESPACE_END


#include "glowTextData.inl.h"


#endif
