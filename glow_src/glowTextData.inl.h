/*
===============================================================================

	FILE:  glowTextData.inl.h
	
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


/*
===============================================================================
	Headers and declarations
===============================================================================
*/

#include <cstring>

#ifndef GLOW_DEBUG__H
	#include "glowDebug.h"
#endif


GLOW_INTERNAL_USINGSTD
GLOW_NAMESPACE_BEGIN


/*
===============================================================================
	Inline methods of GlowTextData
===============================================================================
*/

inline GlowTextData::GlowTextData()
{
	_lineBreaks.push_back(0);
	_selStart = _selEnd = 0;
}


// MSVC doesn't like the base class initializers here. I have no idea why.
// Luckily there's a workaround, but probably with worse performance.

#if 0

inline GlowTextData::GlowTextData(
	const GLOW_STD::string& str) :
GLOW_STD::string(str)
{
	_lineBreaks.push_back(0);
	_selStart = _selEnd = 0;
}


inline GlowTextData::GlowTextData(
	const char* str) :
GLOW_STD::string(str)
{
	_lineBreaks.push_back(0);
	_selStart = _selEnd = 0;
}

#else

inline GlowTextData::GlowTextData(
	const GLOW_STD::string& str)
{
	assign(str);
	_lineBreaks.push_back(0);
	_selStart = _selEnd = 0;
}


inline GlowTextData::GlowTextData(
	const char* str)
{
	assign(str);
	_lineBreaks.push_back(0);
	_selStart = _selEnd = 0;
}

#endif


inline void GlowTextData::ClearLineBreaks()
{
	_lineBreaks.erase(_lineBreaks.begin(), _lineBreaks.end());
	_lineBreaks.push_back(0);
}


inline int GlowTextData::NumLines() const
{
	return _lineBreaks.size();
}


inline GLOW_STD::string GlowTextData::Line(
	int num) const
{
	GLOW_ASSERT(num > 0);
	GLOW_ASSERT(num < int(_lineBreaks.size()));
	return substr(_lineBreaks[num],
		(num == int(_lineBreaks.size())-1) ? GLOW_STD::string::npos :
		_lineBreaks[num+1]-_lineBreaks[num]);
}


inline GLOW_STD::string GlowTextData::ToEndOfLine(
	int pos) const
{
	int num = LineNumOf(pos);
	return substr(_lineBreaks[num],
		(num == int(_lineBreaks.size())-1) ? GLOW_STD::string::npos :
		_lineBreaks[num+1]-_lineBreaks[num]);
}


inline int GlowTextData::SelectionStart() const
{
	return _selStart;
}


inline int GlowTextData::SelectionEnd() const
{
	return _selEnd;
}


inline int GlowTextData::SelectionLength() const
{
	return _selEnd - _selStart;
}


inline void GlowTextData::SetSelection(
	int start,
	int end)
{
	if (start > end)
	{
		GLOW_ASSERT(end >= 0);
		GLOW_ASSERT(start <= int(size()));
		_selStart = end;
		_selEnd = start;
	}
	else
	{
		GLOW_ASSERT(start >= 0);
		GLOW_ASSERT(end <= int(size()));
		_selStart = start;
		_selEnd = end;
	}
}


inline void GlowTextData::SetSelection(
	int pos)
{
	GLOW_ASSERT(pos >= 0);
	GLOW_ASSERT(pos <= int(size()));
	_selStart = pos;
	_selEnd = pos;
}


inline GLOW_STD::string GlowTextData::SelectedText() const
{
	return substr(_selStart, _selEnd-_selStart);
}


inline void GlowTextData::ReplaceSelectionWith(
	const char* str)
{
	int len = GLOW_CSTD::strlen(str);
	replace(_selStart, _selEnd-_selStart, str, len);
	_selEnd = _selStart+len;
}


inline void GlowTextData::ReplaceSelectionWith(
	char ch)
{
	replace(_selStart, _selEnd-_selStart, 1, ch);
	_selEnd = _selStart+1;
}


inline void GlowTextData::DeleteSelection()
{
	erase(_selStart, _selEnd-_selStart);
	_selEnd = _selStart;
}


/*
===============================================================================
*/

GLOW_NAMESPACE_END

