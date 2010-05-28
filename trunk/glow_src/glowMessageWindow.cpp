/*
===============================================================================

	FILE:  glowMessageWindow.cpp
	
	PROJECT:
	
		The GLOW Toolkit
	
	CONTENTS:
	
		Message window for GLOW
	
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

#include <cstring>
#include <vector>

#ifndef GLOW_MESSAGEWINDOW__H
	#include "glowMessageWindow.h"
#endif
#ifndef GLOW_LABELWIDGET__H
	#include "glowLabelWidget.h"
#endif
#ifndef GLOW_PUSHBUTTONWIDGET__H
	#include "glowPushButtonWidget.h"
#endif


GLOW_INTERNAL_USINGSTD
GLOW_NAMESPACE_BEGIN


/*
===============================================================================
	Param defaults
===============================================================================
*/

GlowMessageWindowParams GlowMessageWindowParams::defaults(true);

GlowMessageWindowParams::GlowMessageWindowParams()
{
	*this = defaults;
}

GlowMessageWindowParams::GlowMessageWindowParams(bool)
{
	windowTitle = "Note";
	x = -1;
	y = -1;
	text = "";
	textFont = GLUT_BITMAP_HELVETICA_12;
	buttonFont = GLUT_BITMAP_HELVETICA_12;
	buttonLabels = "OK";
	windowSpacing = 10;
	enterButton = 0;
	escapeButton = 1;
	receiver = 0;
	backColor.Set(0.8f, 0.8f, 0.8f);
	textColor.Set(0.0f, 0.0f, 0.0f);
}


/*
===============================================================================
	Glow_MessageWindowButton
===============================================================================
*/

class Glow_MessageWindowButton :
	public GlowPushButtonWidget
{
	friend class GlowMessageWindow;
	
	protected:
	
		inline Glow_MessageWindowButton(
			GlowMessageWindow* wind,
			const GlowPushButtonParams& params,
			int number);
	
	protected:
	
		virtual void OnHit(
			Glow::MouseButton mouseButton,
			Glow::Modifiers modifiers);
	
	private:
	
		GlowMessageWindow* _wind;
		int _number;
};


inline Glow_MessageWindowButton::Glow_MessageWindowButton(
	GlowMessageWindow* wind,
	const GlowPushButtonParams& params,
	int number) :
GlowPushButtonWidget(wind, params)
{
	_wind = wind;
	_number = number;
}


void Glow_MessageWindowButton::OnHit(
	Glow::MouseButton mouseButton,
	Glow::Modifiers modifiers)
{
	_wind->OnButtonPressed(_number, mouseButton, modifiers);
}


/*
===============================================================================
	Methods of GlowMessageWindow
===============================================================================
*/

void GlowMessageWindow::Init(
	const char* windowTitle,
	int x,
	int y,
	const char* text,
	const char* buttonLabels,
	GlowMessageWindowReceiver* receiver)
{
	GlowMessageWindowParams params;
	params.windowTitle = windowTitle;
	params.x = x;
	params.y = y;
	params.text = text;
	params.buttonLabels = buttonLabels;
	params.receiver = receiver;
	params.enterButton = 0;
	params.escapeButton = 1;
	Init(params);
}


void GlowMessageWindow::Init(
	const GlowMessageWindowParams& params)
{
	GLOW_DEBUGSCOPE("GlowMessageWindow::Init");
	
	GlowWidgetWindowParams wparams;
	wparams.title = params.windowTitle;
	wparams.x = params.x;
	wparams.y = params.y;
	wparams.backColor = params.backColor;
	GlowFixedSizeWidgetWindow::Init(wparams);
	
	// Make buttons
	char* tempbuf = new char[GLOW_CSTD::strlen(params.buttonLabels)+1];
	GLOW_CSTD::strcpy(tempbuf, params.buttonLabels);
	GlowPushButtonParams pbparams;
	pbparams.font = params.buttonFont;
	int buttonsWidth = -params.windowSpacing;
	while (true)
	{
		pbparams.text = GLOW_CSTD::strtok(_buttons.size()==0 ? tempbuf : 0, "\t");
		if (pbparams.text == 0) break;
		_buttons.push_back(new Glow_MessageWindowButton(this, pbparams, _buttons.size()));
		_buttons.back()->AutoReshape();
		buttonsWidth += _buttons.back()->Width() + params.windowSpacing;
	}
	int buttonHeight = _buttons.front()->Height();
	GLOW_DEBUG(_buttons.size()==0, "No buttons specified in GlowMessageWindow");
	GLOW_DEBUG(buttonsWidth>800, "Buttons too wide in GlowMessageWindow");
	delete[] tempbuf;
	
	// Label
	GlowLabelParams lparams;
	lparams.text = params.text;
	lparams.font = params.textFont;
	lparams.textColor = lparams.disableTextColor = params.textColor;
	lparams.x = lparams.y = params.windowSpacing;
	GlowLabelWidget* label = new GlowLabelWidget(this, lparams);
	label->AutoReshape();
	GLOW_DEBUG(label->Width()>800, "Text too wide");
	GLOW_DEBUG(label->Height()>800, "Text too tall");
	
	// Compute window width
	int windowWidth = buttonsWidth + params.windowSpacing*2;
	if (label->Width()+params.windowSpacing*2 > windowWidth)
	{
		windowWidth = label->Width()+params.windowSpacing*2;
	}
	
	// Arrange buttons
	int xbutton = (windowWidth - buttonsWidth)/2;
	for (GLOW_STD::vector<GlowPushButtonWidget*>::iterator iter = _buttons.begin();
		iter != _buttons.end(); ++iter)
	{
		(*iter)->Move(xbutton, label->Height()+params.windowSpacing*2+10);
		xbutton += (*iter)->Width()+params.windowSpacing;
	}
	
	// Size window
	ForceReshape(windowWidth, label->Height()+buttonHeight+
		params.windowSpacing*3+10);
	
	if (params.receiver != 0)
	{
		_sender.Bind(params.receiver);
	}
	
	// Enter and escape filters
	_enterFilter = new GlowWidgetMapToPushButtonFilter(
		(params.enterButton<0 || params.enterButton>=int(_buttons.size())) ?
			0 : _buttons[params.enterButton],
		Glow::enterKey, Glow::noModifier);
	_escapeFilter = new GlowWidgetMapToPushButtonFilter(
		(params.escapeButton<0 || params.escapeButton>=int(_buttons.size())) ?
			0 : _buttons[params.escapeButton],
		Glow::escapeKey, Glow::noModifier);
	RegisterFilter(_enterFilter);
	RegisterFilter(_escapeFilter);
}


GlowMessageWindow::~GlowMessageWindow()
{
	delete _enterFilter;
	delete _escapeFilter;
}


void GlowMessageWindow::OnButtonPressed(
	int response,
	Glow::MouseButton mouseButton,
	Glow::Modifiers modifiers)
{
	GLOW_DEBUGSCOPE("GlowMessageWindow::OnButtonPressed");
	
	GlowMessageWindowMessage message;
	message.window = this;
	message.response = response;
	message.mouseButton = mouseButton;
	message.modifiers = modifiers;
	_sender.Send(message);
	Close();
}


/*
===============================================================================
*/

GLOW_NAMESPACE_END

