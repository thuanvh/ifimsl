/*
===============================================================================

	FILE:  glowTextFieldWidget.h
	
	PROJECT:
	
		The GLOW Toolkit
	
	CONTENTS:
	
		Editable text field for GLOW
	
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


#ifndef GLOW_TEXTFIELDWIDGET__H
#define GLOW_TEXTFIELDWIDGET__H


/*
===============================================================================
	Headers and declarations
===============================================================================
*/

#ifndef GLOW_HEADER__H
	#include "glowHeader.h"
#endif

#include <string>

#ifndef GLOW_WIDGET__H
	#include "glowWidget.h"
#endif
#ifndef GLOW_TEXTDATA__H
	#include "glowTextData.h"
#endif
#ifndef GLOW_LABELWIDGET__H
	#include "glowLabelWidget.h"
#endif


GLOW_INTERNAL_USINGSTD
GLOW_NAMESPACE_BEGIN


class GlowTextFieldParams;
class GlowLabeledTextFieldParams;
class GlowTextFieldWidget;


/*
===============================================================================
	CLASS Glow_TextFieldBlinkTask
===============================================================================
*/


class Glow_TextField_BlinkTask :
	public GlowDeferredTask
{
	public:
	
		inline void Init(
			GlowTextFieldWidget* field);
	
	protected:
	
		virtual void Task();
	
	private:
	
		GlowTextFieldWidget* _field;
};


/*
===============================================================================
	CLASS GlowTextFieldWidget
	
	Editable text field widget
===============================================================================
*/

class GlowTextFieldWidget :
	public GlowWidget
{
	friend class Glow_TextField_BlinkTask;
	friend class Glow_TextField_AutoScrollTimer;
	
	
	//-------------------------------------------------------------------------
	//	Public interface
	//-------------------------------------------------------------------------
	
	public:
	
		enum Style
		{
			plainStyle = 1,
			etchedStyle = 2,
			raisedStyle = 3,
			loweredStyle = 4
		};
	
	public:
	
		inline GlowTextFieldWidget();
		inline GlowTextFieldWidget(
			GlowWidgetRoot* root,
			const GlowTextFieldParams& params);
		inline GlowTextFieldWidget(
			GlowWidget* parent,
			const GlowTextFieldParams& params);
		
		inline void Init(
			GlowWidgetRoot* root,
			const GlowTextFieldParams& params);
		inline void Init(
			GlowWidget* parent,
			const GlowTextFieldParams& params);
		
		virtual ~GlowTextFieldWidget();
	
	public:
	
		inline void SetStyle(
			Style style);
		inline Style GetStyle() const;
		
		inline void SetFont(
			GlowFont font);
		inline GlowFont GetFont() const;
		
		inline int GetBlinkInterval() const;
		inline void SetBlinkInterval(
			int interval);
		
		inline int GetAutoScrollInterval() const;
		inline void SetAutoScrollInterval(
			int interval);
		
		inline const char* GetTextChars() const;
		inline int GetTextLength() const;
		inline const GLOW_STD::string& GetTextString() const;
		inline void SetText(
			const char* str);
		inline void SetText(
			const GLOW_STD::string& str);
		
		inline GLOW_STD::string GetSelectionString() const;
		inline int GetSelectionLength() const;
		inline int GetSelectionStart() const;
		inline int GetSelectionEnd() const;
		
		inline void SetSelection(
			int start,
			int end);
		inline void SetSelection(
			int pos);
		inline void ReplaceSelectionWith(
			const char* str);
		inline void ReplaceSelectionWith(
			const GLOW_STD::string& str);
		
		inline GlowColor GetBackColor() const;
		inline GlowColor GetCaretColor() const;
		inline GlowColor GetTextColor() const;
		inline GlowColor GetFocusBackColor() const;
		inline GlowColor GetFocusTextColor() const;
		inline GlowColor GetFocusCaretColor() const;
		inline GlowColor GetHiliteBackColor() const;
		inline GlowColor GetHiliteTextColor() const;
		inline GlowColor GetDisableBackColor() const;
		inline GlowColor GetDisableTextColor() const;
		inline GlowColor GetDisableCaretColor() const;
		inline GlowColor GetDisableOutlineColor() const;
		inline GlowColor GetLightBevelColor() const;
		inline GlowColor GetDarkBevelColor() const;
		
		inline void SetBackColor(
			GlowColor c);
		inline void SetCaretColor(
			GlowColor c);
		inline void SetTextColor(
			GlowColor c);
		inline void SetFocusBackColor(
			GlowColor c);
		inline void SetFocusTextColor(
			GlowColor c);
		inline void SetFocusCaretColor(
			GlowColor c);
		inline void SetHiliteBackColor(
			GlowColor c);
		inline void SetHiliteTextColor(
			GlowColor c);
		inline void SetDisableBackColor(
			GlowColor c);
		inline void SetDisableCaretColor(
			GlowColor c);
		inline void SetDisableTextColor(
			GlowColor c);
		inline void SetDisableOutlineColor(
			GlowColor c);
		inline void SetLightBevelColor(
			GlowColor c);
		inline void SetDarkBevelColor(
			GlowColor c);
	
	
	//-------------------------------------------------------------------------
	//	Subclass-visible implementation
	//-------------------------------------------------------------------------
	
	protected:
	
		void Init(
			GlowWidgetRoot* root,
			GlowWidget* parent,
			const GlowTextFieldParams& params);
		
		void _CheckAutoScroll();
	
	protected:
	
		GlowTextData _data;
	
	
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
		
		virtual void OnWidgetKeyboard(
			Glow::KeyCode key,
			int x,
			int y,
			Glow::Modifiers modifiers);
	
	
	//-------------------------------------------------------------------------
	//	Private implementation
	//-------------------------------------------------------------------------
	
	protected:
	
		virtual void OnWidgetMouseDown(
			Glow::MouseButton button,
			int x,
			int y,
			Glow::Modifiers modifiers);
		virtual void OnWidgetMouseUp(
			Glow::MouseButton button,
			int x,
			int y,
			Glow::Modifiers modifiers);
		virtual void OnWidgetMouseDrag(
			int x,
			int y);
		
		virtual void OnWidgetPaint();
		
		virtual void OnGotKeyboardFocus();
		virtual void OnLostKeyboardFocus();
	
	private:
	
		void _HandleAutoScrollTimer();
	
	private:
	
		GlowFont _font;
		int _hpos;
		Style _style;
		int _blinkInterval;
		int _autoScrollInterval;
		int _inset;
		int _caretInset;
		int _dragStart;
		int _dragEnd;
		int _dragX;
		bool _blink;
		bool _toggleAutoScroll;
		Glow_TextField_BlinkTask _blinkTask;
		
		GlowColor _backColor;
		GlowColor _caretColor;
		GlowColor _textColor;
		GlowColor _focusBackColor;
		GlowColor _focusTextColor;
		GlowColor _focusCaretColor;
		GlowColor _hiliteBackColor;
		GlowColor _hiliteTextColor;
		GlowColor _disableBackColor;
		GlowColor _disableCaretColor;
		GlowColor _disableTextColor;
		GlowColor _disableOutlineColor;
		GlowColor _lightBevelColor;
		GlowColor _darkBevelColor;
};


/*
===============================================================================
	CLASS GlowHiddenTextFieldWidget
	
	Editable text field widget (hidden data)
===============================================================================
*/

class GlowHiddenTextFieldWidget :
	public GlowTextFieldWidget
{
	//-------------------------------------------------------------------------
	//	Public interface
	//-------------------------------------------------------------------------
	
	public:
	
		inline GlowHiddenTextFieldWidget();
		inline GlowHiddenTextFieldWidget(
			GlowWidgetRoot* root,
			const GlowTextFieldParams& params,
			char hideCharacter = '#');
		inline GlowHiddenTextFieldWidget(
			GlowWidget* parent,
			const GlowTextFieldParams& params,
			char hideCharacter = '#');
		
		inline void Init(
			GlowWidgetRoot* root,
			const GlowTextFieldParams& params,
			char hideCharacter = '#');
		inline void Init(
			GlowWidget* parent,
			const GlowTextFieldParams& params,
			char hideCharacter = '#');
	
	public:
	
		inline const char* GetTextChars() const;
		inline const GLOW_STD::string& GetTextString() const;
		inline void SetText(
			const char* str);
		inline void SetText(
			const GLOW_STD::string& str);
		
		inline GLOW_STD::string GetSelectionString() const;
		
		inline char GetHideCharacter() const;
		inline void SetHideCharacter(
			char hc);
	
	
	//-------------------------------------------------------------------------
	//	Subclass-visible implementation
	//-------------------------------------------------------------------------
	
	protected:
	
		void Init(
			GlowWidgetRoot* root,
			GlowWidget* parent,
			const GlowTextFieldParams& params,
			char hideCharacter);
	
	
	//-------------------------------------------------------------------------
	//	Overrideable implementation
	//-------------------------------------------------------------------------
	
	protected:
	
		virtual void OnWidgetKeyboard(
			Glow::KeyCode key,
			int x,
			int y,
			Glow::Modifiers modifiers);
	
	
	//-------------------------------------------------------------------------
	//	Private implementation
	//-------------------------------------------------------------------------
	
	private:
	
		GLOW_STD::string _hiddenData;
		char _hideCharacter;
};


/*
===============================================================================
	CLASS GlowLabeledTextFieldWidget
	
	Labeled text field widget
===============================================================================
*/

class GlowLabeledTextFieldWidget :
	public GlowTextFieldWidget,
	public GlowWidgetLabelHelper
{
	//-------------------------------------------------------------------------
	//	Public interface
	//-------------------------------------------------------------------------
	
	public:
	
		inline GlowLabeledTextFieldWidget();
		inline GlowLabeledTextFieldWidget(
			GlowWidget* parent,
			const GlowLabeledTextFieldParams& params);
		inline GlowLabeledTextFieldWidget(
			GlowWidgetRoot* root,
			const GlowLabeledTextFieldParams& params);
		inline void Init(
			GlowWidget* parent,
			const GlowLabeledTextFieldParams& params);
		inline void Init(
			GlowWidgetRoot* root,
			const GlowLabeledTextFieldParams& params);
	
	
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
			const GlowLabeledTextFieldParams& params);
};


/*
===============================================================================
	CLASS GlowLabeledHiddenTextFieldWidget
	
	Labeled text field widget
===============================================================================
*/

class GlowLabeledHiddenTextFieldWidget :
	public GlowHiddenTextFieldWidget,
	public GlowWidgetLabelHelper
{
	//-------------------------------------------------------------------------
	//	Public interface
	//-------------------------------------------------------------------------
	
	public:
	
		inline GlowLabeledHiddenTextFieldWidget();
		inline GlowLabeledHiddenTextFieldWidget(
			GlowWidget* parent,
			const GlowLabeledTextFieldParams& params,
			char hideCharacter = '#');
		inline GlowLabeledHiddenTextFieldWidget(
			GlowWidgetRoot* root,
			const GlowLabeledTextFieldParams& params,
			char hideCharacter = '#');
		inline void Init(
			GlowWidget* parent,
			const GlowLabeledTextFieldParams& params,
			char hideCharacter = '#');
		inline void Init(
			GlowWidgetRoot* root,
			const GlowLabeledTextFieldParams& params,
			char hideCharacter = '#');
	
	
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
			const GlowLabeledTextFieldParams& params,
			char hideCharacter);
};


/*
===============================================================================
	CLASS GlowTextFieldParams
	
	Editable text field params
===============================================================================
*/

class GlowTextFieldParams :
	public GlowWidgetParams
{
	public:
	
		GlowTextFieldWidget::Style style;
		const char* initialText;
		GlowFont font;
		int selectionStart;
		int selectionEnd;
		int blinkInterval;
		int autoScrollInterval;
		int inset;
		int caretInset;
		GlowColor backColor;
		GlowColor textColor;
		GlowColor caretColor;
		GlowColor focusBackColor;
		GlowColor focusTextColor;
		GlowColor focusCaretColor;
		GlowColor hiliteBackColor;
		GlowColor hiliteTextColor;
		GlowColor disableBackColor;
		GlowColor disableTextColor;
		GlowColor disableCaretColor;
		GlowColor disableOutlineColor;
		GlowColor lightBevelColor;
		GlowColor darkBevelColor;
		
		static GlowTextFieldParams defaults;
		
		GlowTextFieldParams();
	
	protected:
	
		GlowTextFieldParams(bool);
};


/*
===============================================================================
	CLASS GlowLabeledTextFieldParams
	
	Labeled text field params
===============================================================================
*/

class GlowLabeledTextFieldParams :
	public GlowTextFieldParams
{
	public:
	
		const char* labelText;
		GlowFont labelFont;
		GlowLabeledTextFieldWidget::LabelPosition labelPosition;
		int labelWidth;
		int labelHeight;
		int labelSpacing;
		GlowColor labelColor;
		GlowColor disableLabelColor;
		
		static GlowLabeledTextFieldParams defaults;
		
		GlowLabeledTextFieldParams();
	
	protected:
	
		GlowLabeledTextFieldParams(bool);
};


/*
===============================================================================
*/

GLOW_NAMESPACE_END


#include "glowTextFieldWidget.inl.h"


#endif
