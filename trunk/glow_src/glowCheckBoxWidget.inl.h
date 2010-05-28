/*
===============================================================================

	FILE:  glowCheckBoxWidget.inl.h
	
	PROJECT:
	
		The GLOW Toolkit
	
	CONTENTS:
	
		Check box widget for GLOW
	
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
	Inline methods of GlowCheckBoxWidget
===============================================================================
*/

inline GlowCheckBoxWidget::GlowCheckBoxWidget()
{
}


inline GlowCheckBoxWidget::GlowCheckBoxWidget(
	GlowWidget* parent,
	const GlowCheckBoxParams& params)
{
	Init(parent->Root(), parent, params);
}


inline GlowCheckBoxWidget::GlowCheckBoxWidget(
	GlowWidgetRoot* root,
	const GlowCheckBoxParams& params)
{
	Init(root, 0, params);
}


inline void GlowCheckBoxWidget::Init(
	GlowWidget* parent,
	const GlowCheckBoxParams& params)
{
	Init(parent->Root(), parent, params);
}


inline void GlowCheckBoxWidget::Init(
	GlowWidgetRoot* root,
	const GlowCheckBoxParams& params)
{
	Init(root, 0, params);
}


inline const char* GlowCheckBoxWidget::GetText() const
{
	return _label;
}


inline GlowFont GlowCheckBoxWidget::GetFont() const
{
	return _font;
}


inline void GlowCheckBoxWidget::SetFont(
	GlowFont font)
{
	_font = font;
	Refresh();
}


inline int GlowCheckBoxWidget::GetSpacing() const
{
	return _spacing;
}


inline void GlowCheckBoxWidget::SetSpacing(
	int spacing)
{
	_spacing = spacing;
	Refresh();
}


inline GlowCheckBoxWidget::Behavior GlowCheckBoxWidget::GetBehavior() const
{
	return _behavior;
}


inline void GlowCheckBoxWidget::SetBehavior(
	Behavior behavior)
{
	_behavior = behavior;
}


inline GlowCheckBoxWidget::State GlowCheckBoxWidget::GetState() const
{
	return _state;
}


inline void GlowCheckBoxWidget::SetState(
	GlowCheckBoxWidget::State state)
{
	_state = state;
	Refresh();
}


inline void GlowCheckBoxWidget::Hit(
	Glow::MouseButton button,
	Glow::Modifiers modifiers)
{
	OnHit(button, modifiers);
}


inline GlowColor GlowCheckBoxWidget::GetBoxColor() const
{
	return _boxColor;
}


inline GlowColor GlowCheckBoxWidget::GetTextColor() const
{
	return _textColor;
}


inline GlowColor GlowCheckBoxWidget::GetCheckColor() const
{
	return _checkColor;
}


inline GlowColor GlowCheckBoxWidget::GetHiliteBoxColor() const
{
	return _hiliteBoxColor;
}


inline GlowColor GlowCheckBoxWidget::GetHiliteTextColor() const
{
	return _hiliteTextColor;
}


inline GlowColor GlowCheckBoxWidget::GetHiliteCheckColor() const
{
	return _hiliteCheckColor;
}


inline GlowColor GlowCheckBoxWidget::GetDisableBoxColor() const
{
	return _disableBoxColor;
}


inline GlowColor GlowCheckBoxWidget::GetDisableTextColor() const
{
	return _disableTextColor;
}


inline GlowColor GlowCheckBoxWidget::GetDisableCheckColor() const
{
	return _disableCheckColor;
}


inline GlowColor GlowCheckBoxWidget::GetDisableOutlineColor() const
{
	return _disableOutlineColor;
}


inline GlowColor GlowCheckBoxWidget::GetLightBevelColor() const
{
	return _lightBevelColor;
}


inline GlowColor GlowCheckBoxWidget::GetDarkBevelColor() const
{
	return _darkBevelColor;
}


inline void GlowCheckBoxWidget::SetBoxColor(
	GlowColor c)
{
	_boxColor = c;
	Refresh();
}


inline void GlowCheckBoxWidget::SetTextColor(
	GlowColor c)
{
	_textColor = c;
	Refresh();
}


inline void GlowCheckBoxWidget::SetCheckColor(
	GlowColor c)
{
	_checkColor = c;
	Refresh();
}


inline void GlowCheckBoxWidget::SetHiliteBoxColor(
	GlowColor c)
{
	_hiliteBoxColor = c;
	Refresh();
}


inline void GlowCheckBoxWidget::SetHiliteTextColor(
	GlowColor c)
{
	_hiliteTextColor = c;
	Refresh();
}


inline void GlowCheckBoxWidget::SetHiliteCheckColor(
	GlowColor c)
{
	_hiliteCheckColor = c;
	Refresh();
}


inline void GlowCheckBoxWidget::SetDisableBoxColor(
	GlowColor c)
{
	_disableBoxColor = c;
	Refresh();
}


inline void GlowCheckBoxWidget::SetDisableTextColor(
	GlowColor c)
{
	_disableTextColor = c;
	Refresh();
}


inline void GlowCheckBoxWidget::SetDisableCheckColor(
	GlowColor c)
{
	_disableCheckColor = c;
	Refresh();
}


inline void GlowCheckBoxWidget::SetDisableOutlineColor(
	GlowColor c)
{
	_disableOutlineColor = c;
	Refresh();
}


inline void GlowCheckBoxWidget::SetLightBevelColor(
	GlowColor c)
{
	_lightBevelColor = c;
	Refresh();
}


inline void GlowCheckBoxWidget::SetDarkBevelColor(
	GlowColor c)
{
	_darkBevelColor = c;
	Refresh();
}


inline TSender<const GlowCheckBoxMessage&>& GlowCheckBoxWidget::Notifier()
{
	return _sender;
}


/*
===============================================================================
*/

GLOW_NAMESPACE_END

