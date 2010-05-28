/*
===============================================================================

	FILE:  glowTextFieldWindow.cpp
	
	PROJECT:
	
		The GLOW Toolkit
	
	CONTENTS:
	
		Text field window for GLOW
	
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

#ifndef GLOW_TEXTFIELDWINDOW__H
	#include "glowTextFieldWindow.h"
#endif
#ifndef GLOW_LABELWIDGET__H
	#include "glowLabelWidget.h"
#endif
#ifndef GLOW_TEXTFIELDWIDGET__H
	#include "glowTextFieldWidget.h"
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

GlowTextFieldWindowParams GlowTextFieldWindowParams::defaults(true);

GlowTextFieldWindowParams::GlowTextFieldWindowParams()
{
	*this = defaults;
}

GlowTextFieldWindowParams::GlowTextFieldWindowParams(bool)
{
	windowTitle = "Enter text";
	x = -1;
	y = -1;
	labelText = "";
	labelFont = GLUT_BITMAP_HELVETICA_12;
	fieldText = "";
	fieldFont = GLUT_BITMAP_HELVETICA_12;
	fieldWidth = 100;
	fieldHideCharacter = 0;
	buttonFont = GLUT_BITMAP_HELVETICA_12;
	buttonLabels = "OK";
	windowSpacing = 10;
	enterButton = 0;
	escapeButton = 1;
	receiver = 0;
	backColor.Set(0.8f, 0.8f, 0.8f);
	labelColor.Set(0.0f, 0.0f, 0.0f);
	fieldBackColor.Set(1.0f, 1.0f, 1.0f);
	fieldHiliteBackColor.Set(0.9f, 0.9f, 0.7f);
	fieldTextColor.Set(0.0f, 0.0f, 0.0f);
	fieldHiliteTextColor.Set(0.0f, 0.0f, 0.0f);
	fieldCaretColor.Set(0.4f, 0.4f, 0.5f);
}


/*
===============================================================================
	Glow_TextFieldWindowButton
===============================================================================
*/

class Glow_TextFieldWindowButton :
	public GlowPushButtonWidget
{
	friend class GlowTextFieldWindow;
	
	protected:
	
		inline Glow_TextFieldWindowButton(
			GlowTextFieldWindow* wind,
			const GlowPushButtonParams& params,
			int number);
	
	protected:
	
		virtual void OnHit(
			Glow::MouseButton mouseButton,
			Glow::Modifiers modifiers);
	
	private:
	
		GlowTextFieldWindow* _wind;
		int _number;
};


inline Glow_TextFieldWindowButton::Glow_TextFieldWindowButton(
	GlowTextFieldWindow* wind,
	const GlowPushButtonParams& params,
	int number) :
GlowPushButtonWidget(wind, params)
{
	_wind = wind;
	_number = number;
}


void Glow_TextFieldWindowButton::OnHit(
	Glow::MouseButton mouseButton,
	Glow::Modifiers modifiers)
{
	_wind->OnButtonPressed(_number, _wind->_field->GetTextChars(),
		mouseButton, modifiers);
}


/*
===============================================================================
	Methods of GlowTextFieldWindow
===============================================================================
*/

void GlowTextFieldWindow::Init(
	const char* windowTitle,
	int x,
	int y,
	const char* labelText,
	int fieldWidth,
	const char* fieldText,
	const char* buttonLabels,
	GlowTextFieldWindowReceiver* receiver)
{
	GlowTextFieldWindowParams params;
	params.windowTitle = windowTitle;
	params.x = x;
	params.y = y;
	params.labelText = labelText;
	params.fieldWidth = fieldWidth;
	params.fieldText = fieldText;
	params.buttonLabels = buttonLabels;
	params.receiver = receiver;
	params.enterButton = 0;
	params.escapeButton = 1;
	Init(params);
}


void GlowTextFieldWindow::Init(
	const GlowTextFieldWindowParams& params)
{
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
		_buttons.push_back(new Glow_TextFieldWindowButton(this, pbparams, _buttons.size()));
		_buttons.back()->AutoReshape();
		buttonsWidth += _buttons.back()->Width() + params.windowSpacing;
	}
	int buttonHeight = _buttons.front()->Height();
	delete[] tempbuf;
	GLOW_DEBUG(_buttons.size()==0, "No buttons specified in GlowTextFieldWindow");
	GLOW_DEBUG(buttonsWidth>800, "Buttons too wide in GlowTextFieldWindow");
	
	// Set up label
	GlowLabelParams lparams;
	lparams.text = params.labelText;
	lparams.font = params.labelFont;
	lparams.textColor = lparams.disableTextColor = params.labelColor;
	lparams.x = lparams.y = params.windowSpacing;
	GlowLabelWidget* label = new GlowLabelWidget(this, lparams);
	label->AutoReshape();
	GLOW_DEBUG(label->Width()>800, "Label too wide in GlowTextFieldWindow");
	GLOW_DEBUG(label->Height()>700, "Label too tall in GlowTextFieldWindow");
	
	// Set up field
	GlowTextFieldParams tparams;
	tparams.x = params.windowSpacing;
	tparams.y = params.windowSpacing*2+label->Height();
	tparams.width = params.fieldWidth;
	tparams.initialText = params.fieldText;
	tparams.font = params.fieldFont;
	tparams.focusCaretColor = params.fieldCaretColor;
	tparams.focusBackColor = params.fieldBackColor;
	tparams.focusTextColor = params.fieldTextColor;
	tparams.hiliteBackColor = params.fieldHiliteBackColor;
	tparams.hiliteTextColor = params.fieldHiliteTextColor;
	_field = (params.fieldHideCharacter == '\0') ?
		new GlowTextFieldWidget(this, tparams) :
		new GlowHiddenTextFieldWidget(this, tparams, params.fieldHideCharacter);
	_field->AutoReshape();
	GLOW_DEBUG(_field->Width()>800, "Field too wide in GlowTextFieldWindow");
	GLOW_DEBUG(_field->Height()>100, "Field too tall in GlowTextFieldWindow");
	_field->GrabKeyboardFocus();
	
	// Determine window width
	int windowWidth = buttonsWidth + params.windowSpacing*2;
	if (label->Width()+params.windowSpacing*2 > windowWidth)
	{
		windowWidth = label->Width()+params.windowSpacing*2;
	}
	if (_field->Width()+params.windowSpacing*2 > windowWidth)
	{
		windowWidth = _field->Width()+params.windowSpacing*2;
	}
	
	// Arrange buttons
	int xbutton = (windowWidth - buttonsWidth)/2;
	for (GLOW_STD::vector<GlowPushButtonWidget*>::iterator iter = _buttons.begin();
		iter != _buttons.end(); ++iter)
	{
		(*iter)->Move(xbutton,
			label->Height()+_field->Height()+params.windowSpacing*3+10);
		xbutton += (*iter)->Width()+params.windowSpacing;
	}
	
	// Size window
	ForceReshape(windowWidth, label->Height()+buttonHeight+_field->Height()+
		params.windowSpacing*4+10);
	
	if (params.receiver != 0)
	{
		_sender.Bind(params.receiver);
	}
	
	// Enter and escape filters
	_enterFilter = new GlowWidgetMapToPushButtonFilter(
		(params.enterButton<0 || params.enterButton>=int(_buttons.size())) ? 0 : _buttons[params.enterButton],
		Glow::enterKey, Glow::noModifier);
	_escapeFilter = new GlowWidgetMapToPushButtonFilter(
		(params.escapeButton<0 || params.escapeButton>=int(_buttons.size())) ? 0 : _buttons[params.escapeButton],
		Glow::escapeKey, Glow::noModifier);
	RegisterFilter(_enterFilter);
	RegisterFilter(_escapeFilter);
}


GlowTextFieldWindow::~GlowTextFieldWindow()
{
	delete _enterFilter;
	delete _escapeFilter;
}


void GlowTextFieldWindow::OnButtonPressed(
	int response,
	const char* text,
	Glow::MouseButton mouseButton,
	Glow::Modifiers modifiers)
{
	GlowTextFieldWindowMessage message;
	message.window = this;
	message.response = response;
	message.text = text;
	message.mouseButton = mouseButton;
	message.modifiers = modifiers;
	_sender.Send(message);
	Close();
}


/*
===============================================================================
*/

GLOW_NAMESPACE_END

