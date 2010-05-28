/*
===============================================================================

	FILE:  glowQuickPalette.inl.h
	
	PROJECT:
	
		The GLOW Toolkit
	
	CONTENTS:
	
		Quick palette API for GLOW
	
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
	Inline methods of GlowQuickPalette
===============================================================================
*/

inline GlowQuickPalette::GlowQuickPalette()
{
	_panel = 0;
}


/*
===============================================================================
	Inline methods of GlowQuickPanelWidget
===============================================================================
*/

inline GlowQuickPanelWidget* GlowQuickPalette::AddArrangingPanel(
	Arrangement arrangement,
	Alignment alignment,
	int spacing,
	int inset)
{
	return AddPanel(GlowPanelWidget::plainStyle, 0, arrangement, alignment, spacing,
		(_panel->_arrangement == vertical) ? inset : 0,
		(_panel->_arrangement == horizontal) ? inset : 0);
}


inline GlowQuickPalette::Arrangement GlowQuickPalette::GetArrangement() const
{
	return _panel->_arrangement;
}


inline GlowQuickPalette::Alignment GlowQuickPalette::GetAlignment() const
{
	return _panel->_alignment;
}


inline int GlowQuickPalette::GetSpacing() const
{
	return _panel->_spacing;
}


inline int GlowQuickPalette::GetHMargin() const
{
	return _panel->_hmargin;
}


inline int GlowQuickPalette::GetVMargin() const
{
	return _panel->_vmargin;
}


inline void GlowQuickPalette::SetArrangement(
	Arrangement value)
{
	_panel->_arrangement = value;
}


inline void GlowQuickPalette::SetAlignment(
	Alignment value)
{
	_panel->_alignment = value;
}


inline void GlowQuickPalette::SetSpacing(
	int value)
{
	_panel->_spacing = value;
}


inline void GlowQuickPalette::SetHMargin(
	int value)
{
	_panel->_hmargin = value;
}


inline void GlowQuickPalette::SetVMargin(
	int value)
{
	_panel->_vmargin = value;
}


inline GlowQuickPanelWidget* GlowQuickPalette::QuickPaletteRoot() const
{
	return _panel;
}


/*
===============================================================================
*/

GLOW_NAMESPACE_END

