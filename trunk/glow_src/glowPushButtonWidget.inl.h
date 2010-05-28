/*
===============================================================================

	FILE:  glowPushButtonWidget.inl.h
	
	PROJECT:
	
		The GLOW Toolkit
	
	CONTENTS:
	
		Pushbutton widget for GLOW
	
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
	Inline methods of GlowPushButtonWidget
===============================================================================
*/

inline GlowPushButtonWidget::GlowPushButtonWidget()
{
}


inline GlowPushButtonWidget::GlowPushButtonWidget(
	GlowWidget* parent,
	const GlowPushButtonParams& params)
{
	Init(parent->Root(), parent, params);
}


inline GlowPushButtonWidget::GlowPushButtonWidget(
	GlowWidgetRoot* root,
	const GlowPushButtonParams& params)
{
	Init(root, 0, params);
}


inline void GlowPushButtonWidget::Init(
	GlowWidget* parent,
	const GlowPushButtonParams& params)
{
	Init(parent->Root(), parent, params);
}


inline void GlowPushButtonWidget::Init(
	GlowWidgetRoot* root,
	const GlowPushButtonParams& params)
{
	Init(root, 0, params);
}


inline const char* GlowPushButtonWidget::GetText() const
{
	return _label;
}


inline GlowFont GlowPushButtonWidget::GetFont() const
{
	return _font;
}


inline void GlowPushButtonWidget::SetFont(
	GlowFont font)
{
	_font = font;
	Refresh();
}


inline GlowPushButtonWidget::Behavior GlowPushButtonWidget::GetBehavior() const
{
	return _behavior;
}


inline void GlowPushButtonWidget::SetBehavior(
	Behavior behavior)
{
	_behavior = behavior;
	if (_behavior == normalBehavior && _state)
	{
		_state = false;
		Refresh();
	}
}


inline bool GlowPushButtonWidget::IsDown() const
{
	return _state;
}


inline void GlowPushButtonWidget::SetDown(
	bool down)
{
	_state = down;
	Refresh();
}


inline GlowColor GlowPushButtonWidget::GetUpBoxColor() const
{
	return _upBoxColor;
}


inline GlowColor GlowPushButtonWidget::GetUpTextColor() const
{
	return _upTextColor;
}


inline GlowColor GlowPushButtonWidget::GetDownBoxColor() const
{
	return _downBoxColor;
}


inline GlowColor GlowPushButtonWidget::GetDownTextColor() const
{
	return _downTextColor;
}


inline GlowColor GlowPushButtonWidget::GetHiliteBoxColor() const
{
	return _hiliteBoxColor;
}


inline GlowColor GlowPushButtonWidget::GetHiliteTextColor() const
{
	return _hiliteTextColor;
}


inline GlowColor GlowPushButtonWidget::GetDisableUpBoxColor() const
{
	return _disableUpBoxColor;
}


inline GlowColor GlowPushButtonWidget::GetDisableDownBoxColor() const
{
	return _disableDownBoxColor;
}


inline GlowColor GlowPushButtonWidget::GetDisableTextColor() const
{
	return _disableTextColor;
}


inline GlowColor GlowPushButtonWidget::GetDisableOutlineColor() const
{
	return _disableOutlineColor;
}


inline GlowColor GlowPushButtonWidget::GetLightBevelColor() const
{
	return _lightBevelColor;
}


inline GlowColor GlowPushButtonWidget::GetDarkBevelColor() const
{
	return _darkBevelColor;
}


inline void GlowPushButtonWidget::SetUpBoxColor(
	GlowColor c)
{
	_upBoxColor = c;
	Refresh();
}


inline void GlowPushButtonWidget::SetUpTextColor(
	GlowColor c)
{
	_upTextColor = c;
	Refresh();
}


inline void GlowPushButtonWidget::SetDownBoxColor(
	GlowColor c)
{
	_downBoxColor = c;
	Refresh();
}


inline void GlowPushButtonWidget::SetDownTextColor(
	GlowColor c)
{
	_downTextColor = c;
	Refresh();
}


inline void GlowPushButtonWidget::SetHiliteBoxColor(
	GlowColor c)
{
	_hiliteBoxColor = c;
	Refresh();
}


inline void GlowPushButtonWidget::SetHiliteTextColor(
	GlowColor c)
{
	_hiliteTextColor = c;
	Refresh();
}


inline void GlowPushButtonWidget::SetDisableUpBoxColor(
	GlowColor c)
{
	_disableUpBoxColor = c;
	Refresh();
}


inline void GlowPushButtonWidget::SetDisableDownBoxColor(
	GlowColor c)
{
	_disableDownBoxColor = c;
	Refresh();
}


inline void GlowPushButtonWidget::SetDisableTextColor(
	GlowColor c)
{
	_disableTextColor = c;
	Refresh();
}


inline void GlowPushButtonWidget::SetDisableOutlineColor(
	GlowColor c)
{
	_disableOutlineColor = c;
	Refresh();
}


inline void GlowPushButtonWidget::SetLightBevelColor(
	GlowColor c)
{
	_lightBevelColor = c;
	Refresh();
}


inline void GlowPushButtonWidget::SetDarkBevelColor(
	GlowColor c)
{
	_darkBevelColor = c;
	Refresh();
}


inline TSender<const GlowPushButtonMessage&>& GlowPushButtonWidget::Notifier()
{
	return _sender;
}


inline void GlowPushButtonWidget::Hit(
	Glow::MouseButton button,
	Glow::Modifiers modifiers)
{
	OnHit(button, modifiers);
}


/*
===============================================================================
	Inline methods of GlowDismissPushButtonWidget
===============================================================================
*/

inline GlowDismissPushButtonWidget::GlowDismissPushButtonWidget()
{
}


inline GlowDismissPushButtonWidget::GlowDismissPushButtonWidget(
	GlowWidget* parent,
	const GlowPushButtonParams& params,
	GlowComponent* todismiss)
{
	GlowPushButtonWidget::Init(parent->Root(), parent, params);
	_todismiss = todismiss;
}


inline GlowDismissPushButtonWidget::GlowDismissPushButtonWidget(
	GlowWidgetRoot* root,
	const GlowPushButtonParams& params,
	GlowComponent* todismiss)
{
	GlowPushButtonWidget::Init(root, 0, params);
	_todismiss = todismiss;
}


inline void GlowDismissPushButtonWidget::Init(
	GlowWidget* parent,
	const GlowPushButtonParams& params,
	GlowComponent* todismiss)
{
	GlowPushButtonWidget::Init(parent->Root(), parent, params);
	_todismiss = todismiss;
}


inline void GlowDismissPushButtonWidget::Init(
	GlowWidgetRoot* root,
	const GlowPushButtonParams& params,
	GlowComponent* todismiss)
{
	GlowPushButtonWidget::Init(root, 0, params);
	_todismiss = todismiss;
}


inline void GlowDismissPushButtonWidget::SetToDismiss(
	GlowComponent* todismiss)
{
	_todismiss = todismiss;
}


inline GlowComponent* GlowDismissPushButtonWidget::GetToDismiss() const
{
	return _todismiss;
}


/*
===============================================================================
	Inline methods of GlowWidgetMapToPushButtonFilter
===============================================================================
*/

inline GlowWidgetMapToPushButtonFilter::GlowWidgetMapToPushButtonFilter(
	GlowPushButtonWidget* widget,
	Glow::KeyCode key,
	Glow::Modifiers modifiers)
{
	_widget = widget;
	_keyCode = key;
	_modifiers = modifiers;
}


inline void GlowWidgetMapToPushButtonFilter::SetPushButton(
	GlowPushButtonWidget* widget)
{
	_widget = widget;
}


inline GlowPushButtonWidget* GlowWidgetMapToPushButtonFilter::GetPushButton() const
{
	return _widget;
}


inline void GlowWidgetMapToPushButtonFilter::SetKeyCode(
	Glow::KeyCode key)
{
	_keyCode = key;
}


inline Glow::KeyCode GlowWidgetMapToPushButtonFilter::GetKeyCode() const
{
	return _keyCode;
}


inline void GlowWidgetMapToPushButtonFilter::SetModifiers(
	Glow::Modifiers modifiers)
{
	_modifiers = modifiers;
}


inline Glow::Modifiers GlowWidgetMapToPushButtonFilter::GetModifiers() const
{
	return _modifiers;
}


/*
===============================================================================
*/

GLOW_NAMESPACE_END

