/*
===============================================================================

	FILE:  glowTextFieldWidget.inl.h
	
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


/*
===============================================================================
	Headers and declarations
===============================================================================
*/

GLOW_INTERNAL_USINGSTD
GLOW_NAMESPACE_BEGIN


/*
===============================================================================
	Inline methods of GlowTextFieldWidget
===============================================================================
*/

inline GlowTextFieldWidget::GlowTextFieldWidget()
{
}


inline GlowTextFieldWidget::GlowTextFieldWidget(
	GlowWidget* parent,
	const GlowTextFieldParams& params)
{
	Init(parent->Root(), parent, params);
}


inline GlowTextFieldWidget::GlowTextFieldWidget(
	GlowWidgetRoot* root,
	const GlowTextFieldParams& params)
{
	Init(root, 0, params);
}


inline void GlowTextFieldWidget::Init(
	GlowWidget* parent,
	const GlowTextFieldParams& params)
{
	Init(parent->Root(), parent, params);
}


inline void GlowTextFieldWidget::Init(
	GlowWidgetRoot* root,
	const GlowTextFieldParams& params)
{
	Init(root, 0, params);
}


inline GlowTextFieldWidget::Style GlowTextFieldWidget::GetStyle() const
{
	return _style;
}


inline void GlowTextFieldWidget::SetStyle(
	Style style)
{
	_style = style;
	Refresh();
}


inline GlowFont GlowTextFieldWidget::GetFont() const
{
	return _font;
}


inline void GlowTextFieldWidget::SetFont(
	GlowFont font)
{
	_font = font;
	Refresh();
}


inline int GlowTextFieldWidget::GetBlinkInterval() const
{
	return _blinkInterval;
}


inline void GlowTextFieldWidget::SetBlinkInterval(
	int interval)
{
	_blinkInterval = interval;
}


inline int GlowTextFieldWidget::GetAutoScrollInterval() const
{
	return _autoScrollInterval;
}


inline void GlowTextFieldWidget::SetAutoScrollInterval(
	int interval)
{
	_autoScrollInterval = interval;
}


inline const char* GlowTextFieldWidget::GetTextChars() const
{
	return _data.c_str();
}


inline int GlowTextFieldWidget::GetTextLength() const
{
	return _data.length();
}


inline const GLOW_STD::string& GlowTextFieldWidget::GetTextString() const
{
	return _data;
}


inline void GlowTextFieldWidget::SetText(
	const char* str)
{
	_data.assign(str);
	_data.RevalidateSelection();
	Refresh();
}


inline void GlowTextFieldWidget::SetText(
	const GLOW_STD::string& str)
{
	_data.assign(str);
	_data.RevalidateSelection();
	Refresh();
}


inline GLOW_STD::string GlowTextFieldWidget::GetSelectionString() const
{
	return _data.SelectedText();
}


inline int GlowTextFieldWidget::GetSelectionLength() const
{
	return _data.SelectionLength();
}


inline int GlowTextFieldWidget::GetSelectionStart() const
{
	return _data.SelectionStart();
}


inline int GlowTextFieldWidget::GetSelectionEnd() const
{
	return _data.SelectionEnd();
}


inline void GlowTextFieldWidget::SetSelection(
	int start,
	int end)
{
	_data.SetSelection(start, end);
	Refresh();
}


inline void GlowTextFieldWidget::SetSelection(
	int pos)
{
	_data.SetSelection(pos);
	Refresh();
}


inline void GlowTextFieldWidget::ReplaceSelectionWith(
	const char* str)
{
	_data.ReplaceSelectionWith(str);
	Refresh();
}


inline void GlowTextFieldWidget::ReplaceSelectionWith(
	const GLOW_STD::string& str)
{
	_data.ReplaceSelectionWith(str.c_str());
	Refresh();
}


inline GlowColor GlowTextFieldWidget::GetBackColor() const
{
	return _backColor;
}


inline GlowColor GlowTextFieldWidget::GetCaretColor() const
{
	return _caretColor;
}


inline GlowColor GlowTextFieldWidget::GetTextColor() const
{
	return _textColor;
}


inline GlowColor GlowTextFieldWidget::GetFocusBackColor() const
{
	return _focusBackColor;
}


inline GlowColor GlowTextFieldWidget::GetFocusTextColor() const
{
	return _focusTextColor;
}


inline GlowColor GlowTextFieldWidget::GetFocusCaretColor() const
{
	return _focusCaretColor;
}


inline GlowColor GlowTextFieldWidget::GetHiliteBackColor() const
{
	return _hiliteBackColor;
}


inline GlowColor GlowTextFieldWidget::GetHiliteTextColor() const
{
	return _hiliteTextColor;
}


inline GlowColor GlowTextFieldWidget::GetDisableBackColor() const
{
	return _disableBackColor;
}


inline GlowColor GlowTextFieldWidget::GetDisableTextColor() const
{
	return _disableTextColor;
}


inline GlowColor GlowTextFieldWidget::GetDisableCaretColor() const
{
	return _disableCaretColor;
}


inline GlowColor GlowTextFieldWidget::GetDisableOutlineColor() const
{
	return _disableOutlineColor;
}


inline GlowColor GlowTextFieldWidget::GetLightBevelColor() const
{
	return _lightBevelColor;
}


inline GlowColor GlowTextFieldWidget::GetDarkBevelColor() const
{
	return _darkBevelColor;
}


inline void GlowTextFieldWidget::SetBackColor(
	GlowColor c)
{
	_backColor = c;
	Refresh();
}


inline void GlowTextFieldWidget::SetCaretColor(
	GlowColor c)
{
	_caretColor = c;
	Refresh();
}


inline void GlowTextFieldWidget::SetTextColor(
	GlowColor c)
{
	_textColor = c;
	Refresh();
}


inline void GlowTextFieldWidget::SetFocusBackColor(
	GlowColor c)
{
	_focusBackColor = c;
	Refresh();
}


inline void GlowTextFieldWidget::SetFocusTextColor(
	GlowColor c)
{
	_focusTextColor = c;
	Refresh();
}


inline void GlowTextFieldWidget::SetFocusCaretColor(
	GlowColor c)
{
	_focusCaretColor = c;
	Refresh();
}


inline void GlowTextFieldWidget::SetHiliteBackColor(
	GlowColor c)
{
	_hiliteBackColor = c;
	Refresh();
}


inline void GlowTextFieldWidget::SetHiliteTextColor(
	GlowColor c)
{
	_hiliteTextColor = c;
	Refresh();
}


inline void GlowTextFieldWidget::SetDisableBackColor(
	GlowColor c)
{
	_disableBackColor = c;
	Refresh();
}


inline void GlowTextFieldWidget::SetDisableTextColor(
	GlowColor c)
{
	_disableTextColor = c;
	Refresh();
}


inline void GlowTextFieldWidget::SetDisableCaretColor(
	GlowColor c)
{
	_disableCaretColor = c;
	Refresh();
}


inline void GlowTextFieldWidget::SetDisableOutlineColor(
	GlowColor c)
{
	_disableOutlineColor = c;
	Refresh();
}


inline void GlowTextFieldWidget::SetLightBevelColor(
	GlowColor c)
{
	_lightBevelColor = c;
	Refresh();
}


inline void GlowTextFieldWidget::SetDarkBevelColor(
	GlowColor c)
{
	_darkBevelColor = c;
	Refresh();
}


/*
===============================================================================
	Timer function
===============================================================================
*/

inline void Glow_TextField_BlinkTask::Init(
	GlowTextFieldWidget* field)
{
	_field = field;
}


/*
===============================================================================
	Inline methods of GlowHiddenTextFieldWidget
===============================================================================
*/

inline GlowHiddenTextFieldWidget::GlowHiddenTextFieldWidget()
{
}


inline GlowHiddenTextFieldWidget::GlowHiddenTextFieldWidget(
	GlowWidget* parent,
	const GlowTextFieldParams& params,
	char hideCharacter)
{
	Init(parent->Root(), parent, params, hideCharacter);
}


inline GlowHiddenTextFieldWidget::GlowHiddenTextFieldWidget(
	GlowWidgetRoot* root,
	const GlowTextFieldParams& params,
	char hideCharacter)
{
	Init(root, 0, params, hideCharacter);
}


inline void GlowHiddenTextFieldWidget::Init(
	GlowWidget* parent,
	const GlowTextFieldParams& params,
	char hideCharacter)
{
	Init(parent->Root(), parent, params, hideCharacter);
}


inline void GlowHiddenTextFieldWidget::Init(
	GlowWidgetRoot* root,
	const GlowTextFieldParams& params,
	char hideCharacter)
{
	Init(root, 0, params, hideCharacter);
}


inline const char* GlowHiddenTextFieldWidget::GetTextChars() const
{
	return _hiddenData.c_str();
}


inline const GLOW_STD::string& GlowHiddenTextFieldWidget::GetTextString() const
{
	return _hiddenData;
}


inline void GlowHiddenTextFieldWidget::SetText(
	const char* str)
{
	_hiddenData.assign(str);
	_data.assign(_hiddenData.size(), _hideCharacter);
	_data.RevalidateSelection();
	Refresh();
}


inline void GlowHiddenTextFieldWidget::SetText(
	const GLOW_STD::string& str)
{
	_hiddenData.assign(str);
	_data.assign(_hiddenData.size(), _hideCharacter);
	_data.RevalidateSelection();
	Refresh();
}


inline GLOW_STD::string GlowHiddenTextFieldWidget::GetSelectionString() const
{
	return _hiddenData.substr(_data.SelectionStart(),
		_data.SelectionEnd()-_data.SelectionStart());
}


inline char GlowHiddenTextFieldWidget::GetHideCharacter() const
{
	return _hideCharacter;
}


/*
===============================================================================
	Inline methods of GlowLabeledTextFieldWidget
===============================================================================
*/

inline GlowLabeledTextFieldWidget::GlowLabeledTextFieldWidget()
{
}


inline GlowLabeledTextFieldWidget::GlowLabeledTextFieldWidget(
	GlowWidget* parent,
	const GlowLabeledTextFieldParams& params)
{
	Init(parent->Root(), parent, params);
}


inline GlowLabeledTextFieldWidget::GlowLabeledTextFieldWidget(
	GlowWidgetRoot* root,
	const GlowLabeledTextFieldParams& params)
{
	Init(root, 0, params);
}


inline void GlowLabeledTextFieldWidget::Init(
	GlowWidget* parent,
	const GlowLabeledTextFieldParams& params)
{
	Init(parent->Root(), parent, params);
}


inline void GlowLabeledTextFieldWidget::Init(
	GlowWidgetRoot* root,
	const GlowLabeledTextFieldParams& params)
{
	Init(root, 0, params);
}


/*
===============================================================================
	Inline methods of GlowLabeledHiddenTextFieldWidget
===============================================================================
*/

inline GlowLabeledHiddenTextFieldWidget::GlowLabeledHiddenTextFieldWidget()
{
}


inline GlowLabeledHiddenTextFieldWidget::GlowLabeledHiddenTextFieldWidget(
	GlowWidget* parent,
	const GlowLabeledTextFieldParams& params,
	char hideCharacter)
{
	Init(parent->Root(), parent, params, hideCharacter);
}


inline GlowLabeledHiddenTextFieldWidget::GlowLabeledHiddenTextFieldWidget(
	GlowWidgetRoot* root,
	const GlowLabeledTextFieldParams& params,
	char hideCharacter)
{
	Init(root, 0, params, hideCharacter);
}


inline void GlowLabeledHiddenTextFieldWidget::Init(
	GlowWidget* parent,
	const GlowLabeledTextFieldParams& params,
	char hideCharacter)
{
	Init(parent->Root(), parent, params, hideCharacter);
}


inline void GlowLabeledHiddenTextFieldWidget::Init(
	GlowWidgetRoot* root,
	const GlowLabeledTextFieldParams& params,
	char hideCharacter)
{
	Init(root, 0, params, hideCharacter);
}


/*
===============================================================================
*/

GLOW_NAMESPACE_END

