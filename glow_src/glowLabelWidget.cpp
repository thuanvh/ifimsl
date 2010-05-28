/*
===============================================================================

	FILE:  glowLabelWidget.cpp
	
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

#ifndef GLOW_HEADER__H
	#include "glowHeader.h"
#endif

#ifndef GLOW_LABELWIDGET__H
	#include "glowLabelWidget.h"
#endif


GLOW_INTERNAL_USINGSTD
GLOW_NAMESPACE_BEGIN


/*
===============================================================================
	Param defaults
===============================================================================
*/

GlowLabelParams GlowLabelParams::defaults(true);

GlowLabelParams::GlowLabelParams()
{
	*this = defaults;
}

GlowLabelParams::GlowLabelParams(bool) :
GlowWidgetParams(true),
backColor(0.8f, 0.8f, 0.8f),
textColor(0.0f, 0.0f, 0.0f),
disableTextColor(0.0f, 0.0f, 0.0f)
{
	clipping = true;
	width = 100;
	height = 100;
	text = "";
	font = GLUT_BITMAP_HELVETICA_12;
	opaque = false;
	hIndent = 1;
	vIndent = 0;
}


/*
===============================================================================
	Methods of GlowLabelWidget
===============================================================================
*/

void GlowLabelWidget::Init(
	GlowWidgetRoot* root,
	GlowWidget* parent,
	const GlowLabelParams& params)
{
	GLOW_DEBUGSCOPE("GlowLabelWidget::Init");
	
	GlowWidget::Init(root, parent, params);
	if (params.text == 0)
	{
		_text = 0;
	}
	else
	{
		_text = new char[GLOW_CSTD::strlen(params.text)+1];
		GLOW_CSTD::strcpy(_text, params.text);
	}
	_font = params.font;
	_RecalcText();
	_opaque = params.opaque;
	_hIndent = params.hIndent;
	_vIndent = params.vIndent;
	_backColor = params.backColor;
	_textColor = params.textColor;
	_disableTextColor = params.disableTextColor;
}


GlowLabelWidget::~GlowLabelWidget()
{
	GLOW_DEBUGSCOPE("GlowLabelWidget::~GlowLabelWidget");
	
	delete[] _text;
}


void GlowLabelWidget::SetText(
	const char* text)
{
	GLOW_DEBUGSCOPE("GlowLabelWidget::SetText");
	
	delete[] _text;
	if (text == 0)
	{
		_text = 0;
	}
	else
	{
		_text = new char[GLOW_CSTD::strlen(text)+1];
		GLOW_CSTD::strcpy(_text, text);
	}
	_RecalcText();
	Refresh();
}


void GlowLabelWidget::_RecalcText()
{
	_textHeight = 1;
	_maxLineWidth = 0;
	int thisLineWidth = 0;
	int strlength = (_text == 0) ? 0 : GLOW_CSTD::strlen(_text);
	for (int i=0; i<strlength; i++)
	{
		if (_text[i] == '\r')
		{
			_textHeight++;
			thisLineWidth = 0;
			if (i+1<strlength && _text[i+1] == '\n')
			{
				i++;
			}
		}
		else if (_text[i] == '\n')
		{
			_textHeight++;
			thisLineWidth = 0;
		}
		else
		{
			thisLineWidth += _font.CharWidth(_text[i]);
			if (thisLineWidth > _maxLineWidth)
			{
				_maxLineWidth = thisLineWidth;
			}
		}
	}
	_textHeight *= _font.Leading();
}


GlowWidget::AutoPackError GlowLabelWidget::OnAutoPack(
	int hSize,
	int vSize,
	AutoPackOptions hOption,
	AutoPackOptions vOption,
	int& leftMargin,
	int& rightMargin,
	int& topMargin,
	int& bottomMargin)
{
	GLOW_DEBUGSCOPE("GlowLabelWidget::OnAutoPack");

	int hnew = Width();
	int preferred = _maxLineWidth+_hIndent+_hIndent;
	if (hSize != unspecifiedSize && hSize < preferred)
	{
		return hAutoPackError;
	}
	if (hOption == forcedSize || hOption == expandPreferredSize)
	{
		hnew = hSize;
	}
	else if (hOption == preferredSize)
	{
		hnew = preferred;
	}
	
	int vnew = Height();
	preferred = _textHeight+_vIndent+_vIndent;
	if (vSize != unspecifiedSize && vSize < preferred)
	{
		return vAutoPackError;
	}
	if (vOption == forcedSize || vOption == expandPreferredSize)
	{
		vnew = vSize;
	}
	else if (vOption == preferredSize)
	{
		vnew = preferred;
	}
	
	Reshape(hnew, vnew);
	
	return noAutoPackError;
}


void GlowLabelWidget::OnWidgetPaint()
{
	GLOW_DEBUGSCOPE("GlowLabelWidget::OnWidgetPaint");
	
	if (_opaque)
	{
		_backColor.Apply();
		::glRectf(-1.0f, -1.0f, 1.0f, 1.0f);
	}
	
	int vPos = _font.BaselinePos()+_vIndent;
	GLfloat x, y;
	
	if (IsActive())
	{
		_textColor.Apply();
	}
	else
	{
		_disableTextColor.Apply();
	}
	NormalizeCoordinates(_hIndent, vPos, x, y);
	::glRasterPos2f(x, y);
	int textlen = (_text == 0) ? 0 : GLOW_CSTD::strlen(_text);
	for (int i=0; i<textlen; i++)
	{
		if (_text[i] == '\n' ||
			(_text[i] == '\r' && i+1 < textlen && _text[i+1] != '\n'))
		{
			vPos += _font.Leading();
			NormalizeCoordinates(_hIndent, vPos, x, y);
			::glRasterPos2f(x, y);
		}
		else
		{
			::glutBitmapCharacter(_font, _text[i]);
		}
	}
}


/*
===============================================================================
	Methods of GlowWidgetLabelWidget
===============================================================================
*/

GlowWidget::AutoPackError GlowWidgetLabelWidget::OnAutoPack(
	int hSize,
	int vSize,
	AutoPackOptions hOption,
	AutoPackOptions vOption,
	int& leftMargin,
	int& rightMargin,
	int& topMargin,
	int& bottomMargin)
{
	GLOW_DEBUGSCOPE("GlowWidgetLabelWidget::OnAutoPack");
	
	int hnew = Width();
	int pwidth = 0, pheight = 0;
	FindPreferredSize(pwidth, pheight);
	
	if (hSize != unspecifiedSize)
	{
		if (hSize < 2)
		{
			return hAutoPackError;
		}
		if (hSize < pwidth)
		{
			pwidth = hSize;
		}
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
	if (vSize != unspecifiedSize)
	{
		if (vSize < 2)
		{
			return vAutoPackError;
		}
		if (vSize < pheight)
		{
			pheight = vSize;
		}
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
	Methods of GlowWidgetLabelHelper
===============================================================================
*/

void GlowWidgetLabelHelper::InitLabel(
	GlowWidget* main,
	LabelPosition position,
	int width,
	int height,
	int spacing,
	const char* text,
	GlowFont font,
	GlowColor textColor,
	GlowColor disableTextColor)
{
	_main = main;
	_labelSpacing = spacing;
	_labelPosition = position;
	_labelWidth = width;
	_labelHeight = height;
	GlowLabelParams params;
	params.text = text;
	params.font = font;
	params.textColor = textColor;
	params.disableTextColor = disableTextColor;
	_label = new GlowWidgetLabelWidget(main, params);
	RepositionLabel();
}


GlowWidget::AutoPackError GlowWidgetLabelHelper::HelpAutoPack(
	int& hSize,
	int& vSize,
	int& leftMargin,
	int& rightMargin,
	int& topMargin,
	int& bottomMargin)
{
	if (_label->GetText() == 0)
	{
		return GlowWidget::noAutoPackError;
	}
	
	// Determine preferred size for label
	_label->FindPreferredSize(_labelWidth, _labelHeight);
	
	// Adjust size according to label
	if (hSize != GlowWidget::unspecifiedSize)
	{
		if (_labelPosition == leftLabelPosition ||
			_labelPosition == rightLabelPosition)
		{
			hSize -= _labelWidth + _labelSpacing;
		}
		else if (_labelWidth > _main->Width())
		{
			hSize -= _labelWidth - _main->Width();
		}
		if (hSize < 2)
		{
			return GlowWidget::hAutoPackError;
		}
	}
	if (vSize != GlowWidget::unspecifiedSize)
	{
		if (_labelPosition == topLabelPosition ||
			_labelPosition == bottomLabelPosition)
		{
			vSize -= _labelHeight + _labelSpacing;
		}
		else if (_labelHeight > _main->Height())
		{
			vSize -= _labelHeight - _main->Height();
		}
		if (vSize < 2)
		{
			return GlowWidget::vAutoPackError;
		}
	}
	
	// Adjust margins according to label
	switch (_labelPosition)
	{
		case leftLabelPosition:
			leftMargin = GLOW_STD::max(leftMargin, _labelWidth+_labelSpacing);
			break;
		case rightLabelPosition:
			rightMargin = GLOW_STD::max(rightMargin, _labelWidth+_labelSpacing);
			break;
		case topLabelPosition:
			topMargin = GLOW_STD::max(topMargin, _labelHeight+_labelSpacing);
			break;
		case bottomLabelPosition:
			bottomMargin = GLOW_STD::max(bottomMargin, _labelHeight+_labelSpacing);
			break;
	}
	if ((_labelPosition == leftLabelPosition || _labelPosition == rightLabelPosition) &&
		_labelHeight > _main->Height())
	{
		topMargin = GLOW_STD::max(topMargin, (_labelHeight-_main->Height())/2);
		bottomMargin = GLOW_STD::max(bottomMargin, (_labelHeight-_main->Height())/2);
	}
	if ((_labelPosition == topLabelPosition || _labelPosition == bottomLabelPosition) &&
		_labelWidth > _main->Width())
	{
		rightMargin = GLOW_STD::max(rightMargin, _labelWidth-_main->Width());
	}
	
	return GlowWidget::noAutoPackError;
}


void GlowWidgetLabelHelper::RepositionLabel()
{
	switch (_labelPosition)
	{
		case leftLabelPosition:
			_label->AutoPack(
				-_labelWidth-_labelSpacing,
				-_labelSpacing,
				(_main->Height()-_labelHeight-1)/2,
				(_main->Height()+_labelHeight+1)/2,
				GlowWidget::preferredSize | GlowWidget::rightPos,
				GlowWidget::preferredSize | GlowWidget::centerPos);
			break;
		case rightLabelPosition:
			_label->AutoPack(
				_labelSpacing,
				_labelSpacing+_labelWidth,
				(_main->Height()-_labelHeight-1)/2,
				(_main->Height()+_labelHeight+1)/2,
				GlowWidget::preferredSize | GlowWidget::leftPos,
				GlowWidget::preferredSize | GlowWidget::centerPos);
			break;
		case topLabelPosition:
			_label->AutoPack(
				0, _main->Width(),
				-_labelHeight-_labelSpacing,
				-_labelSpacing,
				GlowWidget::preferredSize | GlowWidget::leftPos,
				GlowWidget::preferredSize | GlowWidget::bottomPos);
			break;
		case bottomLabelPosition:
			_label->AutoPack(
				0, _main->Width(),
				_labelSpacing,
				_labelSpacing+_labelHeight,
				GlowWidget::preferredSize | GlowWidget::leftPos,
				GlowWidget::preferredSize | GlowWidget::topPos);
			break;
	}
}


/*
===============================================================================
*/

GLOW_NAMESPACE_END

