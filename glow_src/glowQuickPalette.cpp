/*
===============================================================================

	FILE:  glowQuickPalette.cpp
	
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

#ifndef GLOW_HEADER__H
	#include "glowHeader.h"
#endif

#ifndef GLOW_QUICKPALETTE__H
	#include "glowQuickPalette.h"
#endif


GLOW_INTERNAL_USINGSTD
GLOW_NAMESPACE_BEGIN


/*
===============================================================================
	Methods of GlowQuickPaletteWindow
===============================================================================
*/

GlowQuickPaletteWindow::GlowQuickPaletteWindow(
	const char* name,
	int x,
	int y,
	Arrangement arrangement,
	Alignment alignment,
	int spacing,
	int hmargin,
	int vmargin)
{
	GLOW_DEBUGSCOPE("GlowQuickPaletteWindow::GlowQuickPaletteWindow");
	
	GLOW_ASSERT(arrangement!=parentArrangement);
	GLOW_ASSERT(alignment!=parentAlignment);
	GLOW_ASSERT(spacing!=parentSetting);
	GLOW_ASSERT(hmargin!=parentSetting);
	GLOW_ASSERT(vmargin!=parentSetting);
	GlowFixedSizeWidgetWindow::Init(name, x, y, 100, 100);
	_panel = new GlowQuickPanelWidget(this, 0,
		GlowQuickPanelWidget::plainStyle, 0, arrangement, alignment,
		spacing, hmargin, vmargin);
	Hide();
}


void GlowQuickPaletteWindow::Pack()
{
	GLOW_DEBUGSCOPE("GlowQuickPaletteWindow::Pack");
	
	_panel->AutoReshape();
	ForceReshape(_panel->Width(), _panel->Height());
	Show();
}


/*
===============================================================================
	Methods of GlowQuickPaletteSubwindow
===============================================================================
*/

GlowQuickPaletteSubwindow::GlowQuickPaletteSubwindow(
	GlowComponent* parent,
	int x,
	int y,
	int width,
	int height,
	Arrangement arrangement,
	Alignment alignment,
	int spacing,
	int hmargin,
	int vmargin)
{
	GLOW_DEBUGSCOPE("GlowQuickPaletteSubwindow::GlowQuickPaletteSubwindow");
	
	GLOW_ASSERT(arrangement!=parentArrangement);
	GLOW_ASSERT(alignment!=parentAlignment);
	GLOW_ASSERT(spacing!=parentSetting);
	GLOW_ASSERT(hmargin!=parentSetting);
	GLOW_ASSERT(vmargin!=parentSetting);
	GlowWidgetSubwindow::Init(parent, x, y, width, height);
	_panel = new GlowQuickPanelWidget(this, 0,
		GlowQuickPanelWidget::plainStyle, 0, arrangement, alignment,
		spacing, hmargin, vmargin);
	Hide();
}


void GlowQuickPaletteSubwindow::Pack(
	bool resizeOnPack)
{
	GLOW_DEBUGSCOPE("GlowQuickPaletteSubwindow::Pack");
	
	_panel->AutoReshape();
	if (resizeOnPack)
	{
		Reshape(_panel->Width(), _panel->Height());
	}
	Show();
}


/*
===============================================================================
	Methods of GlowQuickRadioGroupWidget
===============================================================================
*/

GlowQuickRadioGroupWidget::GlowQuickRadioGroupWidget(
	GlowWidget* parent,
	GlowRadioButtonReceiver* receiver,
	GlowQuickPalette::Arrangement arrangement,
	int spacing)
{
	GlowRadioGroupParams params;
	params.x = params.y = 0;
	params.receiver = receiver;
	GlowRadioGroupWidget::Init(parent, params);
	_arrangement = arrangement;
	_spacing = spacing;
}


GlowRadioButtonWidget* GlowQuickRadioGroupWidget::AddRadioButton(
	const char* label)
{
	GlowRadioButtonParams params;
	params.text = label;
	params.x = params.y = 0;
	return new GlowRadioButtonWidget(this, params);
}


GlowWidget::AutoPackError GlowQuickRadioGroupWidget::OnAutoPack(
	int hSize,
	int vSize,
	AutoPackOptions hOption,
	AutoPackOptions vOption,
	int& leftMargin,
	int& rightMargin,
	int& topMargin,
	int& bottomMargin)
{
	GLOW_DEBUGSCOPE("GlowQuickRadioGroupWidget::OnAutoPack");
	
	// Pack buttons
	int position = 0;
	int size = 0;
	for (GlowComponent* child = FirstChild(); child != 0; child = child->Next())
	{
		GlowRadioButtonWidget* button = dynamic_cast<GlowRadioButtonWidget*>(child);
		if (button != 0 && button->IsVisible() && !button->IsClosing())
		{
			if (_arrangement == GlowQuickPalette::horizontal)
			{
				button->AutoPack(position, unspecifiedPos, 0, unspecifiedPos,
					leftPos | preferredSize, topPos | preferredSize);
				if (size < button->Height())
				{
					size = button->Height();
				}
				position += button->Width()+_spacing;
			}
			else
			{
				button->AutoPack(0, unspecifiedPos, position, unspecifiedPos,
					leftPos | preferredSize, topPos | preferredSize);
				if (size < button->Width())
				{
					size = button->Width();
				}
				position += button->Height()+_spacing;
			}
		}
	}
	
	// Preferred width and height
	int pwidth = 0, pheight = 0;
	if (_arrangement == GlowQuickPalette::horizontal)
	{
		pwidth = position-_spacing;
		pheight = size;
	}
	else
	{
		pwidth = size;
		pheight = position-_spacing;
	}
	
	int hnew = Width();
	if (hSize != unspecifiedSize && hSize < pwidth)
	{
		return hAutoPackError;
	}
	if (hOption == forcedSize || hOption == expandPreferredSize)
	{
		hnew = hSize;
	}
	else if (hOption == preferredSize)
	{
		hnew = pwidth;
	}
	
	int vnew = Height();
	if (vSize != unspecifiedSize && vSize < pheight)
	{
		return vAutoPackError;
	}
	if (vOption == forcedSize || vOption == expandPreferredSize)
	{
		vnew = vSize;
	}
	else if (vOption == preferredSize)
	{
		vnew = pheight;
	}
	
	Reshape(hnew, vnew);
	
	return noAutoPackError;
}


/*
===============================================================================
	Methods of GlowQuickPanelWidget
===============================================================================
*/

GlowQuickPanelWidget* GlowQuickPalette::AddPanel(
	GlowPanelWidget::Style style,
	const char* label,
	Arrangement arrangement,
	Alignment alignment,
	int spacing,
	int hmargin,
	int vmargin)
{
	GLOW_DEBUGSCOPE("GlowQuickPanelWidget::AddPanel");
	
	return new GlowQuickPanelWidget(_panel->Root(), _panel,
		style, label, arrangement, alignment, spacing, hmargin, vmargin);
}


GlowPanelWidget* GlowQuickPalette::AddFixedPanel(
	int width,
	int height,
	GlowPanelWidget::Style style)
{
	GLOW_DEBUGSCOPE("GlowQuickPanelWidget::AddFixedPanel");
	
	GlowPanelParams params;
	params.width = width;
	params.height = height;
	params.style = style;
	return new GlowPanelWidget(_panel, params);
}


GlowSeparatorWidget* GlowQuickPalette::AddSeparator(
	GlowSeparatorWidget::Style style)
{
	GLOW_DEBUGSCOPE("GlowQuickPanelWidget::AddSeparator");
	
	GlowSeparatorParams params;
	params.x = params.y = 0;
	params.style = style;
	if (_panel->_arrangement == GlowQuickPalette::vertical)
	{
		params.width = 10;
		params.height = 4;
	}
	else
	{
		params.height = 10;
		params.width = 4;
	}
	return new GlowSeparatorWidget(_panel, params);
}


GlowPushButtonWidget* GlowQuickPalette::AddPushButton(
	const char* label,
	GlowPushButtonReceiver* receiver)
{
	GLOW_DEBUGSCOPE("GlowQuickPanelWidget::AddPushButton");
	
	GlowPushButtonParams params;
	params.x = params.y = 0;
	params.text = label;
	params.receiver = receiver;
	return new GlowPushButtonWidget(_panel, params);
}


GlowDismissPushButtonWidget* GlowQuickPalette::AddDismissPushButton(
	const char* label,
	GlowComponent* toDismiss,
	GlowPushButtonReceiver* receiver)
{
	GLOW_DEBUGSCOPE("GlowQuickPanelWidget::AddDismissPushButton");
	
	GlowPushButtonParams params;
	params.x = params.y = 0;
	params.text = label;
	params.receiver = receiver;
	return new GlowDismissPushButtonWidget(_panel, params, toDismiss);
}


GlowMenuButtonWidget* GlowQuickPalette::AddMenuButton(
	const char* label,
	GlowMenu* menu,
	GlowMenuButtonWidget::IconType iconType)
{
	GLOW_DEBUGSCOPE("GlowQuickPanelWidget::AddMenuButton");
	
	GlowMenuButtonParams params;
	params.x = params.y = 0;
	params.text = label;
	params.menu = menu;
	params.iconType = iconType;
	return new GlowMenuButtonWidget(_panel, params);
}


GlowLabeledPopupMenuWidget* GlowQuickPalette::AddPopupMenu(
	const char* label,
	GlowPopupMenuReceiver* receiver)
{
	GLOW_DEBUGSCOPE("GlowQuickPanelWidget::AddPopupMenu");
	
	GlowLabeledPopupMenuParams params;
	params.x = params.y = 0;
	params.receiver = receiver;
	params.labelText = label;
	return new GlowLabeledPopupMenuWidget(_panel, params);
}


GlowCheckBoxWidget* GlowQuickPalette::AddCheckBox(
	const char* label,
	GlowCheckBoxWidget::State initialValue,
	GlowCheckBoxReceiver* receiver)
{
	GLOW_DEBUGSCOPE("GlowQuickPanelWidget::AddCheckBox");
	
	GlowCheckBoxParams params;
	params.x = params.y = 0;
	params.text = label;
	params.receiver = receiver;
	params.state = initialValue;
	return new GlowCheckBoxWidget(_panel, params);
}


GlowQuickRadioGroupWidget* GlowQuickPalette::AddRadioGroup(
	Arrangement arrangement,
	int spacing,
	GlowRadioButtonReceiver* receiver)
{
	GLOW_DEBUGSCOPE("GlowQuickPanelWidget::AddRadioGroup");
	
	return new GlowQuickRadioGroupWidget(_panel, receiver, arrangement, spacing);
}


GlowLabeledSliderWidget* GlowQuickPalette::AddSlider(
	float min,
	float max,
	float initial,
	GlowSliderWidget::Options options,
	int numTicks,
	const char* valueLabel,
	const char* label,
	GlowSliderReceiver* receiver)
{
	GLOW_DEBUGSCOPE("GlowQuickPanelWidget::AddSlider");
	
	GlowLabeledSliderParams params;
	params.x = params.y = 0;
	if (_panel->_arrangement == horizontal)
	{
		params.width = 30;
		params.height = 130;
		params.labelPosition = GlowLabeledSliderWidget::topLabelPosition;
	}
	else
	{
		params.height = 30;
		params.width = 130;
		params.labelPosition = GlowLabeledSliderWidget::leftLabelPosition;
	}
	params.min = min;
	params.max = max;
	params.initial = initial;
	params.numTicks = numTicks;
	params.options = options;
	params.receiver = receiver;
	params.labelTemplate = label;
	params.minmaxTemplate = valueLabel;
	return new GlowLabeledSliderWidget(_panel, params);
}


GlowScrollBarWidget* GlowQuickPalette::AddScrollBar(
	long min,
	long max,
	long span,
	long initialTop,
	GlowScrollBarReceiver* receiver)
{
	GLOW_DEBUGSCOPE("GlowQuickPanelWidget::AddScrollBar");
	
	GlowScrollBarParams params;
	params.x = params.y = 0;
	if (_panel->_arrangement == horizontal)
	{
		params.width = 20;
		params.height = 120;
	}
	else
	{
		params.height = 20;
		params.width = 120;
	}
	params.min = min;
	params.max = max;
	params.span = span;
	params.initialTop = initialTop;
	params.receiver = receiver;
	return new GlowScrollBarWidget(_panel, params);
}


GlowLabeledTextFieldWidget* GlowQuickPalette::AddTextField(
	int width,
	const char* text,
	const char* label)
{
	GLOW_DEBUGSCOPE("GlowQuickPanelWidget::AddTextField");
	
	GlowLabeledTextFieldParams params;
	params.x = params.y = 0;
	params.width = width;
	params.initialText = text;
	params.labelText = label;
	GlowLabeledTextFieldWidget* ret =
		new GlowLabeledTextFieldWidget(_panel, params);
	if (_panel->Root()->GetKeyboardFocus() == 0)
	{
		ret->GrabKeyboardFocus();
	}
	return ret;
}


GlowLabeledHiddenTextFieldWidget* GlowQuickPalette::AddHiddenTextField(
	int width,
	const char* text,
	char hideCharacter,
	const char* label)
{
	GLOW_DEBUGSCOPE("GlowQuickPanelWidget::AddHiddenTextField");
	
	GlowLabeledTextFieldParams params;
	params.x = params.y = 0;
	params.width = width;
	params.initialText = text;
	params.labelText = label;
	GlowLabeledHiddenTextFieldWidget* ret =
		new GlowLabeledHiddenTextFieldWidget(_panel, params, hideCharacter);
	if (_panel->Root()->GetKeyboardFocus() == 0)
	{
		ret->GrabKeyboardFocus();
	}
	return ret;
}


GlowLabelWidget* GlowQuickPalette::AddLabel(
	const char* text)
{
	GLOW_DEBUGSCOPE("GlowQuickPanelWidget::AddLabel");
	
	GlowLabelParams params;
	params.x = params.y = 0;
	params.text = text;
	return new GlowLabelWidget(_panel, params);
}


GlowQuickPanelWidget::GlowQuickPanelWidget(
	GlowWidgetRoot* root,
	GlowQuickPanelWidget* parent,
	Style style,
	const char* label,
	Arrangement arrangement,
	Alignment alignment,
	int spacing,
	int hmargin,
	int vmargin)
{
	GLOW_DEBUGSCOPE("GlowQuickPanelWidget::GlowQuickPanelWidget");
	
	GlowPanelParams params;
	params.style = style;
	params.clipping = false;
	GlowPanelWidget::Init(root, parent, params);
	
	_panel = this;
	
	_arrangement = (arrangement == parentArrangement ?
		parent->GetArrangement() : arrangement);
	_alignment = (alignment == parentAlignment ?
		parent->GetAlignment() : alignment);
	_spacing = (spacing == parentSetting ?
		parent->GetSpacing() : spacing);
	_hmargin = (hmargin == parentSetting ?
		parent->GetHMargin() : hmargin);
	_vmargin = (vmargin == parentSetting ?
		parent->GetVMargin() : vmargin);
	_label = 0;
	
	if (label != 0 && *label != 0)
	{
		GlowLabelParams lparams;
		lparams.opaque = true;
		lparams.hIndent = 3;
		lparams.x = lparams.y = 0;
		lparams.text = label;
		_label = new GlowLabelWidget(this, lparams);
	}
}


GlowWidget::AutoPackError GlowQuickPanelWidget::OnAutoPack(
	int hSize,
	int vSize,
	AutoPackOptions hOption,
	AutoPackOptions vOption,
	int& leftMargin,
	int& rightMargin,
	int& topMargin,
	int& bottomMargin)
{
	GLOW_DEBUGSCOPE("GlowQuickPanelWidget::OnAutoPack");
	
	int position = (_arrangement == vertical) ? _vmargin : _hmargin;
	int size = 0;
	int minWidth = position;
	
	// Arrange widgets with default alignment
	for (GlowComponent* child = FirstChild(); child != 0; child = child->Next())
	{
		// Get next widget
		GlowWidget* widget = dynamic_cast<GlowWidget*>(child);
		if (widget == 0 || !widget->IsVisible() || widget->IsClosing())
		{
			continue;
		}
		
		// Handle the label of the panel (if any) differently.
		if (widget == _label)
		{
			if (GetStyle() == GlowPanelWidget::etchedStyle)
			{
				// Etched style: label is half in and half out of the panel
				widget->AutoPack(_hmargin, unspecifiedPos, 0, unspecifiedPos,
					leftPos | preferredSize, centerPos | preferredSize);
				topMargin = widget->Height()/2;
				if (_arrangement == vertical)
				{
					position += widget->Height()/2;
				}
			}
			else
			{
				// Other style: label is completely out of the panel
				widget->AutoPack(_hmargin, unspecifiedPos, unspecifiedPos, 0,
					leftPos | preferredSize, bottomPos | preferredSize);
				topMargin = widget->Height();
			}
			minWidth = widget->Width()+_hmargin+_hmargin;
			continue;
		}
		
		int leftMargin = 0, rightMargin = 0, topMargin = 0, bottomMargin = 0;
		if (_arrangement == horizontal)
		{
			widget->AutoPack(position, unspecifiedPos, _vmargin, unspecifiedPos,
				leftPos | preferredSize, topPos | preferredSize,
				leftMargin, rightMargin, topMargin, bottomMargin);
			size = GLOW_STD::max(size, widget->Height()+topMargin+bottomMargin);
			position += widget->Width()+leftMargin+rightMargin+_spacing;
		}
		else
		{
			widget->AutoPack(_hmargin, unspecifiedPos, position, unspecifiedPos,
				leftPos | preferredSize, topPos | preferredSize,
				leftMargin, rightMargin, topMargin, bottomMargin);
			size = GLOW_STD::max(size, widget->Width()+leftMargin+rightMargin);
			position += widget->Height()+topMargin+bottomMargin+_spacing;
		}
	}
	
	// Preferred width and height
	int pwidth = 0, pheight = 0;
	if (_arrangement == horizontal)
	{
		pwidth = GLOW_STD::max(minWidth, position-_spacing+_hmargin);
		pheight = size+_vmargin+_vmargin;
	}
	else
	{
		pwidth = GLOW_STD::max(minWidth, size+_hmargin+_hmargin);
		pheight = position-_spacing+_vmargin;
	}
	
	// Panel width
	int hnew = Width();
	if (hOption == forcedSize || hOption == expandPreferredSize)
	{
		hnew = hSize;
	}
	else if (hOption == preferredSize)
	{
		hnew = pwidth;
	}
	if (hnew < pwidth)
	{
		return hAutoPackError;
	}
	
	// Panel height
	int vnew = Height();
	if (vOption == forcedSize || vOption == expandPreferredSize)
	{
		vnew = vSize-topMargin;  // because of label
	}
	else if (vOption == preferredSize)
	{
		vnew = pheight;
	}
	if (vnew < pheight)
	{
		return vAutoPackError;
	}
	
	// Update size
	if (_arrangement == horizontal)
	{
		size = vnew-_vmargin-_vmargin;
	}
	else
	{
		size = hnew-_hmargin-_hmargin;
	}
	
	// Now align contents
	int i=0;
	for (GlowComponent* child = FirstChild(); child != 0; child = child->Next(), ++i)
	{
		GlowWidget* widget = dynamic_cast<GlowWidget*>(child);
		if (widget == 0 || widget == _label || !widget->IsVisible())
		{
			continue;
		}
		
		if (_arrangement == horizontal)
		{
			// Horizontal arrangement
			AutoPackOptions vOption = (_alignment & alignExpand) ?
				expandPreferredSize : preferredSize;
			
			if ((_alignment & 3) == alignBottom)
			{
				vOption |= bottomPos;
			}
			else if ((_alignment & 3) == alignCenter)
			{
				vOption |= centerPos;
			}
			else
			{
				vOption |= topPos;
			}
			widget->AutoPack(unspecifiedPos, unspecifiedPos,
				_vmargin, _vmargin+size, noMove | noReshape, vOption);
		}
		else
		{
			// Vertical arrangement
			AutoPackOptions hOption = (_alignment & alignExpand) ?
				expandPreferredSize : preferredSize;
			
			if ((_alignment & 3) == alignLeft)
			{
				hOption |= leftPos;
			}
			else if ((_alignment & 3) == alignCenter)
			{
				hOption |= centerPos;
			}
			else
			{
				hOption |= rightPos;
			}
			widget->AutoPack(_hmargin, _hmargin+size,
				unspecifiedPos, unspecifiedPos, hOption, noMove | noReshape);
		}
	}
	
	Reshape(hnew, vnew);
	
	return noAutoPackError;
}


/*
===============================================================================
*/

GLOW_NAMESPACE_END

