/*
===============================================================================

	FILE:  glowMessageWindow.h
	
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


#ifndef GLOW_MESSAGEWINDOW__H
#define GLOW_MESSAGEWINDOW__H


/*
===============================================================================
	Headers and declarations
===============================================================================
*/

#ifndef GLOW_HEADER__H
	#include "glowHeader.h"
#endif

#ifndef GLOW_WIDGET__H
	#include "glowWidget.h"
#endif
#ifndef GLOW_UTILITIES__H
	#include "glowUtilities.h"
#endif


GLOW_INTERNAL_USINGSTD
GLOW_NAMESPACE_BEGIN


class GlowLabel;
class GlowPushButtonWidget;
class GlowMessageWindow;
class GlowWidgetMapToPushButtonFilter;


/*
===============================================================================
	CLASS GlowMessageWindowMessage
	
	Action for buttons window
===============================================================================
*/

class GlowMessageWindowMessage
{
	public:
	
		GlowMessageWindow* window;
		int response;
		Glow::MouseButton mouseButton;
		Glow::Modifiers modifiers;
};


typedef TReceiver<const GlowMessageWindowMessage&> GlowMessageWindowReceiver;


/*
===============================================================================
	CLASS GlowMessageWindowParams
	
	Message window params
===============================================================================
*/

class GlowMessageWindowParams
{
	public:
	
		const char* windowTitle;
		int x;
		int y;
		const char* text;
		GlowFont textFont;
		GlowFont buttonFont;
		const char* buttonLabels;
		int windowSpacing;
		int enterButton;
		int escapeButton;
		GlowMessageWindowReceiver* receiver;
		GlowColor backColor;
		GlowColor textColor;
	
		static GlowMessageWindowParams defaults;
	
		GlowMessageWindowParams();
	
	protected:
	
		GlowMessageWindowParams(bool);
};


/*
===============================================================================
	CLASS GlowMessageWindow
	
	Message window with buttons
===============================================================================
*/

class GlowMessageWindow :
	public GlowFixedSizeWidgetWindow
{
	friend class Glow_MessageWindowButton;
	
	//-------------------------------------------------------------------------
	//	Public interface
	//-------------------------------------------------------------------------
	
	public:
	
		inline GlowMessageWindow();
		inline GlowMessageWindow(
			const GlowMessageWindowParams& params);
		void Init(
			const GlowMessageWindowParams& params);
		inline GlowMessageWindow(
			const char* windowTitle,
			int x,
			int y,
			const char* text,
			const char* buttonLabels,
			GlowMessageWindowReceiver* receiver);
		void Init(
			const char* windowTitle,
			int x,
			int y,
			const char* text,
			const char* buttonLabels,
			GlowMessageWindowReceiver* receiver);
		
		virtual ~GlowMessageWindow();
		
		inline void SetEnterButton(
			int num);
		inline void SetEscapeButton(
			int num);
	
	public:
	
		inline TSender<const GlowMessageWindowMessage&>& Notifier();
	
	
	//-------------------------------------------------------------------------
	//	Overrideable implementation
	//-------------------------------------------------------------------------
	
	protected:
	
		virtual void OnButtonPressed(
			int response,
			Glow::MouseButton mouseButton,
			Glow::Modifiers modifiers);
	
	
	//-------------------------------------------------------------------------
	//	Private implementation
	//-------------------------------------------------------------------------
	
	private:
	
		TSender<const GlowMessageWindowMessage&> _sender;
		GLOW_STD::vector<GlowPushButtonWidget*> _buttons;
		GlowWidgetMapToPushButtonFilter* _enterFilter;
		GlowWidgetMapToPushButtonFilter* _escapeFilter;
};


/*
===============================================================================
*/

GLOW_NAMESPACE_END


#include "glowMessageWindow.inl.h"


#endif

