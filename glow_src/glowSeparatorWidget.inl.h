/*
===============================================================================

	FILE:  glowSeparatorWidget.inl.h
	
	PROJECT:
	
		The GLOW Toolkit
	
	CONTENTS:
	
		Separator rule for GLOW
	
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

GLOW_INTERNAL_USINGSTD
GLOW_NAMESPACE_BEGIN


/*
===============================================================================
	Inline methods of GlowSeparatorWidget
===============================================================================
*/

inline GlowSeparatorWidget::GlowSeparatorWidget()
{
}


inline GlowSeparatorWidget::GlowSeparatorWidget(
	GlowWidget* parent,
	const GlowSeparatorParams& params)
{
	Init(parent->Root(), parent, params);
}


inline GlowSeparatorWidget::GlowSeparatorWidget(
	GlowWidgetRoot* root,
	const GlowSeparatorParams& params)
{
	Init(root, 0, params);
}


inline void GlowSeparatorWidget::Init(
	GlowWidget* parent,
	const GlowSeparatorParams& params)
{
	Init(parent->Root(), parent, params);
}


inline void GlowSeparatorWidget::Init(
	GlowWidgetRoot* root,
	const GlowSeparatorParams& params)
{
	Init(root, 0, params);
}


inline GlowSeparatorWidget::Style GlowSeparatorWidget::GetStyle() const
{
	return _style;
}


inline void GlowSeparatorWidget::SetStyle(
	Style style)
{
	_style = style;
	Refresh();
}


inline GlowColor GlowSeparatorWidget::GetLightBevelColor() const
{
	return _lightBevelColor;
}


inline GlowColor GlowSeparatorWidget::GetDarkBevelColor() const
{
	return _darkBevelColor;
}


inline void GlowSeparatorWidget::SetLightBevelColor(
	GlowColor c)
{
	_lightBevelColor = c;
	Refresh();
}


inline void GlowSeparatorWidget::SetDarkBevelColor(
	GlowColor c)
{
	_darkBevelColor = c;
	Refresh();
}


/*
===============================================================================
*/

GLOW_NAMESPACE_END

