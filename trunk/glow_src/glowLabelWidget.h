/*
===============================================================================

	FILE:  glowLabelWidget.h
	
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


#ifndef GLOW_LABELWIDGET__H
#define GLOW_LABELWIDGET__H


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


GLOW_INTERNAL_USINGSTD
GLOW_NAMESPACE_BEGIN


/*
===============================================================================
	CLASS GlowLabelParams
	
	Label params
===============================================================================
*/

class GlowLabelParams :
	public GlowWidgetParams
{
	public:
	
		const char* text;
		GlowFont font;
		bool opaque;
		int hIndent;
		int vIndent;
		GlowColor backColor;
		GlowColor textColor;
		GlowColor disableTextColor;
		
		static GlowLabelParams defaults;
		
		GlowLabelParams();
	
	protected:
	
		GlowLabelParams(bool);
};


/*
===============================================================================
	CLASS GlowLabelWidget
	
	Static label widget
===============================================================================
*/

class GlowLabelWidget :
	public GlowWidget
{
	//-------------------------------------------------------------------------
	//	Public interface
	//-------------------------------------------------------------------------
	
	public:
	
		inline GlowLabelWidget();
		inline GlowLabelWidget(
			GlowWidget* parent,
			const GlowLabelParams& params);
		inline GlowLabelWidget(
			GlowWidgetRoot* root,
			const GlowLabelParams& params);
		inline void Init(
			GlowWidget* parent,
			const GlowLabelParams& params);
		inline void Init(
			GlowWidgetRoot* root,
			const GlowLabelParams& params);
		
		virtual ~GlowLabelWidget();
	
	public:
	
		inline const char* GetText() const;
		void SetText(
			const char* text);
		
		inline GlowFont GetFont() const;
		inline void SetFont(
			GlowFont font);
		
		inline bool IsOpaque() const;
		inline void SetOpaque(
			bool opaque);
		
		inline int GetHIndent() const;
		inline int GetVIndent() const;
		inline void SetHIndent(
			int indent);
		inline void SetVIndent(
			int indent);
		
		inline GlowColor GetBackColor() const;
		inline GlowColor GetTextColor() const;
		inline GlowColor GetDisableTextColor() const;
		
		inline void SetBackColor(
			GlowColor c);
		inline void SetTextColor(
			GlowColor c);
		inline void SetDisableTextColor(
			GlowColor c);
		
		inline void FindPreferredSize(
			int& preferredWidth,
			int& preferredHeight);
	
	
	//-------------------------------------------------------------------------
	//	Overrideable implementation
	//-------------------------------------------------------------------------
	
	protected:
	
		virtual AutoPackError OnAutoPack(
			int hSize,
			int vSize,
			AutoPackOptions hOption,
			AutoPackOptions vOption,
			int& leftMargin,
			int& rightMargin,
			int& topMargin,
			int& bottomMargin);
	
	
	//-------------------------------------------------------------------------
	//	Subclass-visible implementation
	//-------------------------------------------------------------------------
	
	protected:
	
		void Init(
			GlowWidgetRoot* root,
			GlowWidget* parent,
			const GlowLabelParams& params);
	
	
	//-------------------------------------------------------------------------
	//	Private implementation
	//-------------------------------------------------------------------------
	
	private:
	
		char* _text;
		GlowFont _font;
		int _maxLineWidth;
		int _textHeight;
		int _hIndent;
		int _vIndent;
		bool _opaque;
		
		GlowColor _backColor;
		GlowColor _textColor;
		GlowColor _disableTextColor;
	
	private:
	
		void _RecalcText();
	
	protected:
	
		virtual void OnWidgetPaint();
};


/*
===============================================================================
	CLASS GlowWidgetLabelWidget
	
	Label widget meant to be attached to another widget
===============================================================================
*/

class GlowWidgetLabelWidget :
	public GlowLabelWidget
{
	//-------------------------------------------------------------------------
	//	Public interface
	//-------------------------------------------------------------------------
	
	public:
	
		inline GlowWidgetLabelWidget(
			GlowWidget* parent,
			const GlowLabelParams& params);
	
	
	//-------------------------------------------------------------------------
	//	Internal implementation
	//-------------------------------------------------------------------------
	
	protected:
	
		virtual AutoPackError OnAutoPack(
			int hSize,
			int vSize,
			AutoPackOptions hOption,
			AutoPackOptions vOption,
			int& leftMargin,
			int& rightMargin,
			int& topMargin,
			int& bottomMargin);
};


/*
===============================================================================
	CLASS GlowWidgetLabelHelper
	
	Class that helps attach a label to another widget
===============================================================================
*/

class GlowWidgetLabelHelper
{
	//-------------------------------------------------------------------------
	//	Public interface
	//-------------------------------------------------------------------------
	
	public:
	
		enum LabelPosition
		{
			defaultLabelPosition = 0,
			leftLabelPosition = 0,
			rightLabelPosition = 1,
			topLabelPosition = 2,
			bottomLabelPosition = 3
		};
	
	public:
	
		inline const char* GetLabelText() const;
		inline void SetLabelText(
			const char* text);
		inline GlowFont GetLabelFont() const;
		inline void SetLabelFont(
			GlowFont font);
		
		inline GlowColor GetLabelTextColor() const;
		inline void SetLabelTextColor(
			GlowColor c);
		inline GlowColor GetLabelDisableTextColor() const;
		inline void SetLabelDisableTextColor(
			GlowColor c);
		
		inline int GetLabelSpacing() const;
		inline void SetLabelSpacing(
			int spacing);
		inline LabelPosition GetLabelPosition() const;
		inline void SetLabelPosition(
			LabelPosition position);
		inline int GetLabelWidth() const;
		inline void SetLabelWidth(
			int width);
		inline int GetLabelHeight() const;
		inline void SetLabelHeight(
			int height);
		
		void RepositionLabel();
	
	
	//-------------------------------------------------------------------------
	//	Subclass helper interface
	//-------------------------------------------------------------------------
	
	protected:
	
		void InitLabel(
			GlowWidget* main,
			LabelPosition position,
			int width,
			int height,
			int spacing,
			const char* text,
			GlowFont font = GlowFont::helvetica12,
			GlowColor textColor = GlowLabelParams::defaults.textColor,
			GlowColor disableTextColor = GlowLabelParams::defaults.disableTextColor);
		
		GlowWidget::AutoPackError HelpAutoPack(
			int& hSize,
			int& vSize,
			int& leftMargin,
			int& rightMargin,
			int& topMargin,
			int& bottomMargin);
	
	protected:
	
		inline GlowWidgetLabelWidget* Label() const;
	
	
	//-------------------------------------------------------------------------
	//	Private implementation
	//-------------------------------------------------------------------------
	
	private:
	
		GlowWidget* _main;
		GlowWidgetLabelWidget* _label;
		LabelPosition _labelPosition;
		int _labelSpacing;
		int _labelWidth;
		int _labelHeight;
};


/*
===============================================================================
*/

GLOW_NAMESPACE_END


#include "glowLabelWidget.inl.h"


#endif
