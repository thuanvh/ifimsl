/*
===============================================================================

	FILE:  glowLabelWidget.inl.h
	
	PROJECT:
	
		The GLOW Toolkit
	
	CONTENTS:
	
		Static label widget for GLOW
	
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


GLOW_INTERNAL_USINGSTD
GLOW_NAMESPACE_BEGIN


/*
===============================================================================
	Inline methods of GlowLabel
===============================================================================
*/

inline GlowLabelWidget::GlowLabelWidget()
{
}


inline GlowLabelWidget::GlowLabelWidget(
	GlowWidget* parent,
	const GlowLabelParams& params)
{
	Init(parent->Root(), parent, params);
}


inline GlowLabelWidget::GlowLabelWidget(
	GlowWidgetRoot* root,
	const GlowLabelParams& params)
{
	Init(root, 0, params);
}


inline void GlowLabelWidget::Init(
	GlowWidget* parent,
	const GlowLabelParams& params)
{
	Init(parent->Root(), parent, params);
}


inline void GlowLabelWidget::Init(
	GlowWidgetRoot* root,
	const GlowLabelParams& params)
{
	Init(root, 0, params);
}


inline const char* GlowLabelWidget::GetText() const
{
	return _text;
}


inline GlowFont GlowLabelWidget::GetFont() const
{
	return _font;
}


inline void GlowLabelWidget::SetFont(
	GlowFont font)
{
	_font = font;
	_RecalcText();
	Refresh();
}


inline bool GlowLabelWidget::IsOpaque() const
{
	return _opaque;
}


inline void GlowLabelWidget::SetOpaque(
	bool opaque)
{
	_opaque = opaque;
	Refresh();
}


inline int GlowLabelWidget::GetHIndent() const
{
	return _hIndent;
}


inline int GlowLabelWidget::GetVIndent() const
{
	return _vIndent;
}


inline void GlowLabelWidget::SetHIndent(
	int indent)
{
	_hIndent = indent;
	Refresh();
}


inline void GlowLabelWidget::SetVIndent(
	int indent)
{
	_vIndent = indent;
	Refresh();
}


inline GlowColor GlowLabelWidget::GetBackColor() const
{
	return _backColor;
}


inline GlowColor GlowLabelWidget::GetTextColor() const
{
	return _textColor;
}


inline GlowColor GlowLabelWidget::GetDisableTextColor() const
{
	return _disableTextColor;
}


inline void GlowLabelWidget::SetBackColor(
	GlowColor c)
{
	_backColor = c;
	Refresh();
}


inline void GlowLabelWidget::SetTextColor(
	GlowColor c)
{
	_textColor = c;
	Refresh();
}


inline void GlowLabelWidget::SetDisableTextColor(
	GlowColor c)
{
	_disableTextColor = c;
	Refresh();
}


inline void GlowLabelWidget::FindPreferredSize(
	int& preferredWidth,
	int& preferredHeight)
{
	preferredWidth = _maxLineWidth+GetHIndent()*2;
	preferredHeight = _textHeight+GetVIndent()*2;
}


/*
===============================================================================
	Inline methods of GlowWidgetLabelWidget
===============================================================================
*/

inline GlowWidgetLabelWidget::GlowWidgetLabelWidget(
	GlowWidget* parent,
	const GlowLabelParams& params) :
GlowLabelWidget(parent, params)
{
}


/*
===============================================================================
	Inline methods of GlowWidgetLabelHelper
===============================================================================
*/

inline GlowColor GlowWidgetLabelHelper::GetLabelTextColor() const
{
	return _label->GetTextColor();
}


inline void GlowWidgetLabelHelper::SetLabelTextColor(
	GlowColor c)
{
	_label->SetTextColor(c);
}


inline GlowColor GlowWidgetLabelHelper::GetLabelDisableTextColor() const
{
	return _label->GetDisableTextColor();
}


inline void GlowWidgetLabelHelper::SetLabelDisableTextColor(
	GlowColor c)
{
	_label->SetDisableTextColor(c);
}


inline const char* GlowWidgetLabelHelper::GetLabelText() const
{
	return _label->GetText();
}


inline void GlowWidgetLabelHelper::SetLabelText(
	const char* text)
{
	_label->SetText(text);
	RepositionLabel();
}


inline GlowFont GlowWidgetLabelHelper::GetLabelFont() const
{
	return _label->GetFont();
}


inline void GlowWidgetLabelHelper::SetLabelFont(
	GlowFont font)
{
	_label->SetFont(font);
	RepositionLabel();
}


inline int GlowWidgetLabelHelper::GetLabelSpacing() const
{
	return _labelSpacing;
}


inline void GlowWidgetLabelHelper::SetLabelSpacing(
	int spacing)
{
	if (spacing != _labelSpacing)
	{
		_labelSpacing = spacing;
		RepositionLabel();
	}
}


inline GlowWidgetLabelHelper::LabelPosition GlowWidgetLabelHelper::GetLabelPosition() const
{
	return _labelPosition;
}


inline void GlowWidgetLabelHelper::SetLabelPosition(
	LabelPosition position)
{
	if (position != _labelPosition)
	{
		_labelPosition = position;
		RepositionLabel();
	}
}


inline int GlowWidgetLabelHelper::GetLabelWidth() const
{
	return _labelWidth;
}


inline void GlowWidgetLabelHelper::SetLabelWidth(
	int width)
{
	if (width != _labelWidth)
	{
		_labelWidth = width;
		RepositionLabel();
	}
}


inline int GlowWidgetLabelHelper::GetLabelHeight() const
{
	return _labelHeight;
}


inline void GlowWidgetLabelHelper::SetLabelHeight(
	int height)
{
	if (height != _labelHeight)
	{
		_labelHeight = height;
		RepositionLabel();
	}
}


inline GlowWidgetLabelWidget* GlowWidgetLabelHelper::Label() const
{
	return _label;
}


/*
===============================================================================
*/

GLOW_NAMESPACE_END

