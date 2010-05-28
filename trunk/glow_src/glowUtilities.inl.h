/*
===============================================================================

	FILE:  glowUtilities.inl.h
	
	PROJECT:
	
		The GLOW Toolkit
	
	CONTENTS:
	
		Extensions and utilities for GLOW
	
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
	Inline methods for GlowWindowSetting
===============================================================================
*/

inline GlowWindowSetting::GlowWindowSetting()
{
	_saveWindowNum = 0;
}


inline GlowWindowSetting::GlowWindowSetting(
	int windowNum)
{
	_saveWindowNum = ::glutGetWindow();
	::glutSetWindow(windowNum);
}


inline GlowWindowSetting::~GlowWindowSetting()
{
	if (_saveWindowNum != 0)
	{
		::glutSetWindow(_saveWindowNum);
	}
}


inline void GlowWindowSetting::Set(
	int windowNum)
{
	GLOW_DEBUG(_saveWindowNum != 0, "Attempt to re-enter GlowWindowSetting");
	_saveWindowNum = ::glutGetWindow();
	::glutSetWindow(windowNum);
}


inline void GlowWindowSetting::Reset()
{
	GLOW_DEBUG(_saveWindowNum == 0, "Attempt to re-exit GlowWindowSetting");
	::glutSetWindow(_saveWindowNum);
	_saveWindowNum = 0;
}


/*
===============================================================================
	Inline methods for GlowDeferredTask
===============================================================================
*/

inline GlowDeferredTask::GlowDeferredTask()
{
	_curTimerID = 0;
}


inline void GlowDeferredTask::Schedule(
	unsigned int msecs)
{
	if (_curTimerID != 0)
	{
		Glow::UnregisterTimer(_curTimerID);
	}
	_curTimerID = Glow::RegisterTimer(msecs, this);
}


inline void GlowDeferredTask::Unschedule()
{
	if (_curTimerID != 0)
	{
		Glow::UnregisterTimer(_curTimerID);
		_curTimerID = 0;
	}
}


inline bool GlowDeferredTask::IsScheduled()
{
	return _curTimerID != 0;
}


/*
===============================================================================
	Inline methods of GlowFixedSizeWindow
===============================================================================
*/

inline GlowFixedSizeWindow::GlowFixedSizeWindow()
{
}


inline GlowFixedSizeWindow::GlowFixedSizeWindow(
	const GlowWindowParams& params)
{
	_canonicalWidth = params.width;
	_canonicalHeight = params.height;
	GlowWindow::Init(params);
}


inline GlowFixedSizeWindow::GlowFixedSizeWindow(
	const char* title,
	int x,
	int y,
	int width,
	int height,
	Glow::BufferType mode,
	Glow::EventMask eventMask)
{
	_canonicalWidth = width;
	_canonicalHeight = height;
	GlowWindow::Init(title, x, y, width, height, mode, eventMask);
}


inline void GlowFixedSizeWindow::Init(
	const GlowWindowParams& params)
{
	_canonicalWidth = params.width;
	_canonicalHeight = params.height;
	GlowWindow::Init(params);
}


inline void GlowFixedSizeWindow::Init(
	const char* title,
	int x,
	int y,
	int width,
	int height,
	Glow::BufferType mode,
	Glow::EventMask eventMask)
{
	_canonicalWidth = width;
	_canonicalHeight = height;
	GlowWindow::Init(title, x, y, width, height, mode, eventMask);
}


inline void GlowFixedSizeWindow::ForceReshape(
	int width,
	int height)
{
	_canonicalWidth = width;
	_canonicalHeight = height;
	Reshape(width, height);
}


inline int GlowFixedSizeWindow::FixedWidth() const
{
	return _canonicalWidth;
}


inline int GlowFixedSizeWindow::FixedHeight() const
{
	return _canonicalHeight;
}


/*
===============================================================================
	Inline methods of GlowReceiverTrackerComponent
===============================================================================
*/

inline GlowReceiverTrackerComponent::GlowReceiverTrackerComponent(
	GlowComponent* parent) :
GlowComponent(parent)
{
	SetDeletingOptions(referenceCountDelete);
}


inline GlowReceiverTrackerComponent::GlowReceiverTrackerComponent()
{
	SetDeletingOptions(referenceCountDelete);
}


inline void GlowReceiverTrackerComponent::Init(
	GlowComponent* parent)
{
	GlowComponent::Init(parent);
}


/*
===============================================================================
*/

GLOW_NAMESPACE_END

