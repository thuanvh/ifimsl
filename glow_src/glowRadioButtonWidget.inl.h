/*
===============================================================================

	FILE:  glowRadioButtonWidget.inl.h
	
	PROJECT:
	
		The GLOW Toolkit
	
	CONTENTS:
	
		Radio button widget for GLOW
	
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
	Inline methods of GlowRadioButtonWidget
===============================================================================
*/

inline GlowRadioButtonWidget::GlowRadioButtonWidget()
{
}


inline GlowRadioButtonWidget::GlowRadioButtonWidget(
	GlowRadioGroupWidget* parent,
	const GlowRadioButtonParams& params)
{
	Init(parent, params);
}


inline const char* GlowRadioButtonWidget::GetText() const
{
	return _label;
}


inline GlowFont GlowRadioButtonWidget::GetFont() const
{
	return _font;
}


inline void GlowRadioButtonWidget::SetFont(
	GlowFont font)
{
	_font = font;
	Refresh();
}


inline int GlowRadioButtonWidget::GetSpacing() const
{
	return _spacing;
}


inline void GlowRadioButtonWidget::SetSpacing(
	int spacing)
{
	_spacing = spacing;
	Refresh();
}


inline bool GlowRadioButtonWidget::GetState() const
{
	return _state;
}


inline GlowColor GlowRadioButtonWidget::GetBoxColor() const
{
	return _boxColor;
}


inline GlowColor GlowRadioButtonWidget::GetTextColor() const
{
	return _textColor;
}


inline GlowColor GlowRadioButtonWidget::GetSpotColor() const
{
	return _spotColor;
}


inline GlowColor GlowRadioButtonWidget::GetHiliteBoxColor() const
{
	return _hiliteBoxColor;
}


inline GlowColor GlowRadioButtonWidget::GetHiliteTextColor() const
{
	return _hiliteTextColor;
}


inline GlowColor GlowRadioButtonWidget::GetHiliteSpotColor() const
{
	return _hiliteSpotColor;
}


inline GlowColor GlowRadioButtonWidget::GetDisableBoxColor() const
{
	return _disableBoxColor;
}


inline GlowColor GlowRadioButtonWidget::GetDisableTextColor() const
{
	return _disableTextColor;
}


inline GlowColor GlowRadioButtonWidget::GetDisableSpotColor() const
{
	return _disableSpotColor;
}


inline GlowColor GlowRadioButtonWidget::GetDisableOutlineColor() const
{
	return _disableOutlineColor;
}


inline GlowColor GlowRadioButtonWidget::GetLightBevelColor() const
{
	return _lightBevelColor;
}


inline GlowColor GlowRadioButtonWidget::GetDarkBevelColor() const
{
	return _darkBevelColor;
}


inline void GlowRadioButtonWidget::SetBoxColor(
	GlowColor c)
{
	_boxColor = c;
	Refresh();
}


inline void GlowRadioButtonWidget::SetTextColor(
	GlowColor c)
{
	_textColor = c;
	Refresh();
}


inline void GlowRadioButtonWidget::SetSpotColor(
	GlowColor c)
{
	_spotColor = c;
	Refresh();
}


inline void GlowRadioButtonWidget::SetHiliteBoxColor(
	GlowColor c)
{
	_hiliteBoxColor = c;
	Refresh();
}


inline void GlowRadioButtonWidget::SetHiliteTextColor(
	GlowColor c)
{
	_hiliteTextColor = c;
	Refresh();
}


inline void GlowRadioButtonWidget::SetHiliteSpotColor(
	GlowColor c)
{
	_hiliteSpotColor = c;
	Refresh();
}


inline void GlowRadioButtonWidget::SetDisableBoxColor(
	GlowColor c)
{
	_disableBoxColor = c;
	Refresh();
}


inline void GlowRadioButtonWidget::SetDisableTextColor(
	GlowColor c)
{
	_disableTextColor = c;
	Refresh();
}


inline void GlowRadioButtonWidget::SetDisableSpotColor(
	GlowColor c)
{
	_disableSpotColor = c;
	Refresh();
}


inline void GlowRadioButtonWidget::SetDisableOutlineColor(
	GlowColor c)
{
	_disableOutlineColor = c;
	Refresh();
}


inline void GlowRadioButtonWidget::SetLightBevelColor(
	GlowColor c)
{
	_lightBevelColor = c;
	Refresh();
}


inline void GlowRadioButtonWidget::SetDarkBevelColor(
	GlowColor c)
{
	_darkBevelColor = c;
	Refresh();
}


inline void GlowRadioButtonWidget::SetState(
	bool state)
{
	if (state != _state)
	{
		_state = state;
		Refresh();
	}
}


/*
===============================================================================
	Inline methods of GlowRadioGroupWidget
===============================================================================
*/

inline GlowRadioGroupWidget::GlowRadioGroupWidget()
{
}


inline GlowRadioGroupWidget::GlowRadioGroupWidget(
	GlowWidgetRoot* root,
	const GlowRadioGroupParams& params)
{
	Init(root, 0, params);
}


inline GlowRadioGroupWidget::GlowRadioGroupWidget(
	GlowWidget* parent,
	const GlowRadioGroupParams& params)
{
	Init(parent->Root(), parent, params);
}


inline void GlowRadioGroupWidget::Init(
	GlowWidgetRoot* root,
	const GlowRadioGroupParams& params)
{
	Init(root, 0, params);
}


inline void GlowRadioGroupWidget::Init(
	GlowWidget* parent,
	const GlowRadioGroupParams& params)
{
	Init(parent->Root(), parent, params);
}


inline void GlowRadioGroupWidget::SetState(
	GlowRadioButtonWidget* button)
{
	GLOW_DEBUGSCOPE("GlowRadioGroupWidget::SetState");
	
	if (button != _state)
	{
		button->SetState(true);
		_state->SetState(false);
		_state = button;
	}
}


inline GlowRadioButtonWidget* GlowRadioGroupWidget::GetState() const
{
	return _state;
}


inline TSender<const GlowRadioButtonMessage&>& GlowRadioGroupWidget::Notifier()
{
	return _sender;
}


inline bool GlowRadioGroupWidget::_Register(
	GlowRadioButtonWidget* button)
{
	if (_state == 0)
	{
		_state = button;
		return true;
	}
	return false;
}


/*
===============================================================================
*/

GLOW_NAMESPACE_END

