/*
===============================================================================

	FILE:  glowSliderWidget.inl.h
	
	PROJECT:
	
		The GLOW Toolkit
	
	CONTENTS:
	
		Slider widget for GLOW
	
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
	Inline methods of GlowSliderWidget
===============================================================================
*/

inline GlowSliderWidget::GlowSliderWidget()
{
}


inline GlowSliderWidget::GlowSliderWidget(
	GlowWidget* parent,
	const GlowSliderParams& params)
{
	Init(parent->Root(), parent, params);
}


inline GlowSliderWidget::GlowSliderWidget(
	GlowWidgetRoot* root,
	const GlowSliderParams& params)
{
	Init(root, 0, params);
}


inline void GlowSliderWidget::Init(
	GlowWidget* parent,
	const GlowSliderParams& params)
{
	Init(parent->Root(), parent, params);
}


inline void GlowSliderWidget::Init(
	GlowWidgetRoot* root,
	const GlowSliderParams& params)
{
	Init(root, 0, params);
}


inline GlowColor GlowSliderWidget::GetStripColor() const
{
	return _stripColor;
}


inline GlowColor GlowSliderWidget::GetIndicatorColor() const
{
	return _indicatorColor;
}


inline GlowColor GlowSliderWidget::GetTickMarkColor() const
{
	return _tickMarkColor;
}


inline GlowColor GlowSliderWidget::GetHiliteIndicatorColor() const
{
	return _hiliteIndicatorColor;
}


inline GlowColor GlowSliderWidget::GetDisableStripColor() const
{
	return _disableStripColor;
}


inline GlowColor GlowSliderWidget::GetDisableIndicatorColor() const
{
	return _disableIndicatorColor;
}


inline GlowColor GlowSliderWidget::GetDisableTickMarkColor() const
{
	return _disableTickMarkColor;
}


inline GlowColor GlowSliderWidget::GetDisableOutlineColor() const
{
	return _disableOutlineColor;
}


inline GlowColor GlowSliderWidget::GetLightBevelColor() const
{
	return _lightBevelColor;
}


inline GlowColor GlowSliderWidget::GetDarkBevelColor() const
{
	return _darkBevelColor;
}


inline void GlowSliderWidget::SetStripColor(
	GlowColor c)
{
	_stripColor = c;
	Refresh();
}


inline void GlowSliderWidget::SetIndicatorColor(
	GlowColor c)
{
	_indicatorColor = c;
	Refresh();
}


inline void GlowSliderWidget::SetTickMarkColor(
	GlowColor c)
{
	_tickMarkColor = c;
	Refresh();
}


inline void GlowSliderWidget::SetHiliteIndicatorColor(
	GlowColor c)
{
	_hiliteIndicatorColor = c;
	Refresh();
}


inline void GlowSliderWidget::SetDisableStripColor(
	GlowColor c)
{
	_disableStripColor = c;
	Refresh();
}


inline void GlowSliderWidget::SetDisableIndicatorColor(
	GlowColor c)
{
	_disableIndicatorColor = c;
	Refresh();
}


inline void GlowSliderWidget::SetDisableTickMarkColor(
	GlowColor c)
{
	_disableTickMarkColor = c;
	Refresh();
}


inline void GlowSliderWidget::SetDisableOutlineColor(
	GlowColor c)
{
	_disableOutlineColor = c;
	Refresh();
}


inline void GlowSliderWidget::SetLightBevelColor(
	GlowColor c)
{
	_lightBevelColor = c;
	Refresh();
}


inline void GlowSliderWidget::SetDarkBevelColor(
	GlowColor c)
{
	_darkBevelColor = c;
	Refresh();
}


inline double GlowSliderWidget::GetValue() const
{
	return _value;
}


inline double GlowSliderWidget::GetMaximum() const
{
	return _max;
}


inline double GlowSliderWidget::GetMinimum() const
{
	return _min;
}


inline void GlowSliderWidget::SetValue(
	double val)
{
	_value = val;
	if (_value > _max) _value = _max;
	if (_value < _min) _value = _min;
	Refresh();
}


inline void GlowSliderWidget::SetMaximum(
	double val)
{
	GLOW_DEBUG(val < _min, "max < min in GlowSliderWidget::SetMaximum");
	_max = val;
	if (_value > _max) _value = _max;
	Refresh();
}


inline void GlowSliderWidget::SetMinimum(
	double val)
{
	GLOW_DEBUG(_max < val, "min > max in GlowSliderWidget::SetMinimum");
	_min = val;
	if (_value < _min) _value = _min;
	Refresh();
}


inline int GlowSliderWidget::GetNumTickMarks() const
{
	return _numTicks;
}


inline void GlowSliderWidget::SetNumTickMarks(
	int numTicks)
{
	_numTicks = numTicks;
	Refresh();
}


inline GlowSliderWidget::Options GlowSliderWidget::GetOptions() const
{
	return _type;
}


inline void GlowSliderWidget::SetOptions(
	Options options)
{
	_type = options;
	Refresh();
}


inline TSender<const GlowSliderMessage&>& GlowSliderWidget::Notifier()
{
	return _sender;
}


inline void GlowSliderWidget::_LeftBevelColor()
{
	if ((_type & decreasing) != 0)
	{
		_darkBevelColor.Apply();
	}
	else
	{
		_lightBevelColor.Apply();
	}
}


inline void GlowSliderWidget::_RightBevelColor()
{
	if ((_type & decreasing) != 0)
	{
		_lightBevelColor.Apply();
	}
	else
	{
		_darkBevelColor.Apply();
	}
}


inline void GlowSliderWidget::_TopBevelColor()
{
	if ((_type & ticksOnTop) != 0)
	{
		_darkBevelColor.Apply();
	}
	else
	{
		_lightBevelColor.Apply();
	}
}


inline void GlowSliderWidget::_BottomBevelColor()
{
	if ((_type & ticksOnTop) != 0)
	{
		_lightBevelColor.Apply();
	}
	else
	{
		_darkBevelColor.Apply();
	}
}


/*
===============================================================================
	Inline methods of GlowLabeledSliderWidget
===============================================================================
*/

inline GlowLabeledSliderWidget::GlowLabeledSliderWidget()
{
}


inline GlowLabeledSliderWidget::GlowLabeledSliderWidget(
	GlowWidget* parent,
	const GlowLabeledSliderParams& params)
{
	Init(parent->Root(), parent, params);
}


inline GlowLabeledSliderWidget::GlowLabeledSliderWidget(
	GlowWidgetRoot* root,
	const GlowLabeledSliderParams& params)
{
	Init(root, 0, params);
}


inline void GlowLabeledSliderWidget::Init(
	GlowWidget* parent,
	const GlowLabeledSliderParams& params)
{
	Init(parent->Root(), parent, params);
}


inline void GlowLabeledSliderWidget::Init(
	GlowWidgetRoot* root,
	const GlowLabeledSliderParams& params)
{
	Init(root, 0, params);
}


inline bool GlowLabeledSliderWidget::HasLabel() const
{
	return _label->GetText() != 0;
}


inline GlowColor GlowLabeledSliderWidget::GetLabelTextColor() const
{
	return _label->GetTextColor();
}


inline void GlowLabeledSliderWidget::SetLabelTextColor(
	GlowColor c)
{
	_label->SetTextColor(c);
}


inline GlowColor GlowLabeledSliderWidget::GetLabelDisableTextColor() const
{
	return _label->GetDisableTextColor();
}


inline void GlowLabeledSliderWidget::SetLabelDisableTextColor(
	GlowColor c)
{
	_label->SetDisableTextColor(c);
}


inline const char* GlowLabeledSliderWidget::GetLabelTemplate() const
{
	return _labelTemplate;
}


inline GlowFont GlowLabeledSliderWidget::GetLabelFont() const
{
	return _label->GetFont();
}


inline void GlowLabeledSliderWidget::SetLabelFont(
	GlowFont font)
{
	_label->SetFont(font);
	RepositionLabel();
}


inline int GlowLabeledSliderWidget::GetLabelSpacing() const
{
	return _labelSpacing;
}


inline void GlowLabeledSliderWidget::SetLabelSpacing(
	int spacing)
{
	if (spacing != _labelSpacing)
	{
		_labelSpacing = spacing;
		RepositionLabel();
	}
}


inline GlowLabeledSliderWidget::LabelPosition GlowLabeledSliderWidget::GetLabelPosition() const
{
	return _labelPosition;
}


inline void GlowLabeledSliderWidget::SetLabelPosition(
	LabelPosition position)
{
	if (position != _labelPosition)
	{
		_labelPosition = position;
		RepositionLabel();
	}
}


inline int GlowLabeledSliderWidget::GetLabelWidth() const
{
	return _labelWidth;
}


inline void GlowLabeledSliderWidget::SetLabelWidth(
	int width)
{
	if (width != _labelWidth)
	{
		_labelWidth = width;
		RepositionLabel();
	}
}


inline int GlowLabeledSliderWidget::GetLabelHeight() const
{
	return _labelHeight;
}


inline void GlowLabeledSliderWidget::SetLabelHeight(
	int height)
{
	if (height != _labelHeight)
	{
		_labelHeight = height;
		RepositionLabel();
	}
}


inline bool GlowLabeledSliderWidget::HasMinmax() const
{
	return _minmaxTemplate != 0;
}


inline const char* GlowLabeledSliderWidget::GetMinmaxTemplate() const
{
	return _minmaxTemplate;
}


inline GlowFont GlowLabeledSliderWidget::GetMinmaxFont() const
{
	return _minLabel->GetFont();
}


inline void GlowLabeledSliderWidget::SetMinmaxFont(
	GlowFont font)
{
	_minLabel->SetFont(font);
	_maxLabel->SetFont(font);
	if (_minmaxTemplate != 0)
	{
		RepositionMinmax();
	}
}


inline GlowColor GlowLabeledSliderWidget::GetMinmaxTextColor() const
{
	return _minLabel->GetTextColor();
}


inline void GlowLabeledSliderWidget::SetMinmaxTextColor(
	GlowColor c)
{
	_minLabel->SetTextColor(c);
	_maxLabel->SetTextColor(c);
}


inline GlowColor GlowLabeledSliderWidget::GetMinmaxDisableTextColor() const
{
	return _minLabel->GetDisableTextColor();
}


inline void GlowLabeledSliderWidget::SetMinmaxDisableTextColor(
	GlowColor c)
{
	_minLabel->SetDisableTextColor(c);
	_maxLabel->SetDisableTextColor(c);
}


inline int GlowLabeledSliderWidget::GetMinmaxSize() const
{
	return _minmaxSize;
}


inline void GlowLabeledSliderWidget::SetMinmaxSize(
	int size)
{
	_minmaxSize = size;
	if (_minmaxTemplate != 0)
	{
		RepositionMinmax();
	}
}


/*
===============================================================================
*/

GLOW_NAMESPACE_END

