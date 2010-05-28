/*
===============================================================================

	FILE:  glowScrollBarWidget.inl.h
	
	PROJECT:
	
		The GLOW Toolkit
	
	CONTENTS:
	
		Scroll bar widget for GLOW
	
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


#ifndef GLOW_DEBUG__H
	#include "glowDebug.h"
#endif


GLOW_INTERNAL_USINGSTD
GLOW_NAMESPACE_BEGIN


/*
===============================================================================
	Inline methods of GlowScrollBarWidget
===============================================================================
*/

inline GlowScrollBarWidget::GlowScrollBarWidget()
{
}


inline GlowScrollBarWidget::GlowScrollBarWidget(
	GlowWidget* parent,
	const GlowScrollBarParams& params)
{
	Init(parent->Root(), parent, params);
}


inline GlowScrollBarWidget::GlowScrollBarWidget(
	GlowWidgetRoot* root,
	const GlowScrollBarParams& params)
{
	Init(root, 0, params);
}


inline void GlowScrollBarWidget::Init(
	GlowWidget* parent,
	const GlowScrollBarParams& params)
{
	Init(parent->Root(), parent, params);
}


inline void GlowScrollBarWidget::Init(
	GlowWidgetRoot* root,
	const GlowScrollBarParams& params)
{
	Init(root, 0, params);
}


inline GlowColor GlowScrollBarWidget::GetStripColor() const
{
	return _stripColor;
}


inline GlowColor GlowScrollBarWidget::GetIndicatorColor() const
{
	return _indicatorColor;
}


inline GlowColor GlowScrollBarWidget::GetShadowColor() const
{
	return _shadowColor;
}


inline GlowColor GlowScrollBarWidget::GetButtonColor() const
{
	return _buttonColor;
}


inline GlowColor GlowScrollBarWidget::GetButtonIconColor() const
{
	return _buttonIconColor;
}


inline GlowColor GlowScrollBarWidget::GetHiliteStripColor() const
{
	return _hiliteStripColor;
}


inline GlowColor GlowScrollBarWidget::GetHiliteIndicatorColor() const
{
	return _hiliteIndicatorColor;
}


inline GlowColor GlowScrollBarWidget::GetHiliteButtonColor() const
{
	return _hiliteButtonColor;
}


inline GlowColor GlowScrollBarWidget::GetHiliteButtonIconColor() const
{
	return _hiliteButtonIconColor;
}


inline GlowColor GlowScrollBarWidget::GetDisableStripColor() const
{
	return _disableStripColor;
}


inline GlowColor GlowScrollBarWidget::GetDisableIndicatorColor() const
{
	return _disableIndicatorColor;
}


inline GlowColor GlowScrollBarWidget::GetDisableButtonColor() const
{
	return _disableButtonColor;
}


inline GlowColor GlowScrollBarWidget::GetDisableButtonIconColor() const
{
	return _disableButtonIconColor;
}


inline GlowColor GlowScrollBarWidget::GetDisableOutlineColor() const
{
	return _disableOutlineColor;
}


inline GlowColor GlowScrollBarWidget::GetLightBevelColor() const
{
	return _lightBevelColor;
}


inline GlowColor GlowScrollBarWidget::GetDarkBevelColor() const
{
	return _darkBevelColor;
}


inline void GlowScrollBarWidget::SetStripColor(
	GlowColor c)
{
	_stripColor = c;
	Refresh();
}


inline void GlowScrollBarWidget::SetIndicatorColor(
	GlowColor c)
{
	_indicatorColor = c;
	Refresh();
}


inline void GlowScrollBarWidget::SetShadowColor(
	GlowColor c)
{
	_shadowColor = c;
	Refresh();
}


inline void GlowScrollBarWidget::SetButtonColor(
	GlowColor c)
{
	_buttonColor = c;
	Refresh();
}


inline void GlowScrollBarWidget::SetButtonIconColor(
	GlowColor c)
{
	_buttonIconColor = c;
	Refresh();
}


inline void GlowScrollBarWidget::SetHiliteStripColor(
	GlowColor c)
{
	_hiliteStripColor = c;
	Refresh();
}


inline void GlowScrollBarWidget::SetHiliteIndicatorColor(
	GlowColor c)
{
	_hiliteIndicatorColor = c;
	Refresh();
}


inline void GlowScrollBarWidget::SetHiliteButtonColor(
	GlowColor c)
{
	_hiliteButtonColor = c;
	Refresh();
}


inline void GlowScrollBarWidget::SetHiliteButtonIconColor(
	GlowColor c)
{
	_hiliteButtonIconColor = c;
	Refresh();
}


inline void GlowScrollBarWidget::SetDisableStripColor(
	GlowColor c)
{
	_disableStripColor = c;
	Refresh();
}


inline void GlowScrollBarWidget::SetDisableIndicatorColor(
	GlowColor c)
{
	_disableIndicatorColor = c;
	Refresh();
}


inline void GlowScrollBarWidget::SetDisableButtonColor(
	GlowColor c)
{
	_disableButtonColor = c;
	Refresh();
}


inline void GlowScrollBarWidget::SetDisableButtonIconColor(
	GlowColor c)
{
	_disableButtonIconColor = c;
	Refresh();
}


inline void GlowScrollBarWidget::SetDisableOutlineColor(
	GlowColor c)
{
	_disableOutlineColor = c;
	Refresh();
}


inline void GlowScrollBarWidget::SetLightBevelColor(
	GlowColor c)
{
	_lightBevelColor = c;
	Refresh();
}


inline void GlowScrollBarWidget::SetDarkBevelColor(
	GlowColor c)
{
	_darkBevelColor = c;
	Refresh();
}


inline long GlowScrollBarWidget::GetTopValue() const
{
	return _topValue;
}


inline long GlowScrollBarWidget::GetBottomValue() const
{
	return _topValue+_span;
}


inline long GlowScrollBarWidget::GetMaximum() const
{
	return _max;
}


inline long GlowScrollBarWidget::GetMinimum() const
{
	return _min;
}


inline long GlowScrollBarWidget::GetSpan() const
{
	return _span;
}


inline void GlowScrollBarWidget::SetTopValue(
	long val)
{
	_topValue = val;
	if (_topValue < _min) _topValue = _min;
	if (_topValue+_span > _max) _topValue = _max-_span;
	Refresh();
}


inline void GlowScrollBarWidget::MoveTopValue(
	long delta)
{
	_topValue += delta;
	if (_topValue < _min) _topValue = _min;
	if (_topValue+_span > _max) _topValue = _max-_span;
	Refresh();
}


inline void GlowScrollBarWidget::SetMaximum(
	long val)
{
	GLOW_DEBUG(val < _min+_span, "max < min in GlowScrollBarWidget::SetMaximum");
	_max = val;
	if (_topValue+_span > _max) _topValue = _max-_span;
	Refresh();
}


inline void GlowScrollBarWidget::SetMinimum(
	long val)
{
	GLOW_DEBUG(_max-_span < val, "min > max in GlowScrollBarWidget::SetMinimum");
	_min = val;
	if (_topValue < _min) _topValue = _min;
	Refresh();
}


inline void GlowScrollBarWidget::SetSpan(
	long val)
{
	GLOW_DEBUG(val < 1, "span<1 in GlowScrollBarWidget::SetSpan");
	GLOW_DEBUG(val > _max-_min, "span > max-min in GlowScrollBarWidget::SetSpan");
	_span = val;
	if (_topValue+_span > _max) _topValue = _max-_span;
	Refresh();
}


inline int GlowScrollBarWidget::GetFirstDelay() const
{
	return _firstDelay;
}


inline void GlowScrollBarWidget::SetFirstDelay(
	int msecs)
{
	_firstDelay = msecs;
}


inline int GlowScrollBarWidget::GetSecondDelay() const
{
	return _secondDelay;
}


inline void GlowScrollBarWidget::SetSecondDelay(
	int msecs)
{
	_secondDelay = msecs;
}


inline long GlowScrollBarWidget::GetPageStep() const
{
	return _pageStep;
}


inline void GlowScrollBarWidget::SetPageStep(
	long pageStep)
{
	_pageStep = pageStep;
}


inline long GlowScrollBarWidget::GetArrowStep() const
{
	return _arrowStep;
}


inline void GlowScrollBarWidget::SetArrowStep(
	long arrowStep)
{
	_arrowStep = arrowStep;
}


inline TSender<const GlowScrollBarMessage&>& GlowScrollBarWidget::Notifier()
{
	return _sender;
}


/*
===============================================================================
*/

GLOW_NAMESPACE_END

