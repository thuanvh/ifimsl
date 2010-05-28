/*
===============================================================================

	FILE:  glowUtilities.h
	
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


#ifndef GLOW_UTILITIES__H
#define GLOW_UTILITIES__H


/*
===============================================================================
	Headers and declarations
===============================================================================
*/

#ifndef GLOW_HEADER__H
	#include "glowHeader.h"
#endif

#ifndef GLOW__H
	#include "glow.h"
#endif


GLOW_INTERNAL_USINGSTD
GLOW_NAMESPACE_BEGIN


/*
===============================================================================
	CLASS GlowWindowSetting

	Tool for setting/resetting the current window
===============================================================================
*/

class GlowWindowSetting
{
	//-------------------------------------------------------------------------
	//	Public interface
	//-------------------------------------------------------------------------
	
	public:
	
		inline GlowWindowSetting();
		inline GlowWindowSetting(
			int windowNum);
		inline ~GlowWindowSetting();
		
		inline void Set(
			int windowNum);
		inline void Reset();
	
	
	//-------------------------------------------------------------------------
	//	Private implementation
	//-------------------------------------------------------------------------
	
	private:
	
		int _saveWindowNum;
};


/*
===============================================================================
	CLASS GlowDeferredTask

	Deferred task (convenience API for timer)
===============================================================================
*/

class GlowDeferredTask :
	public GlowTimerReceiver
{
	//-------------------------------------------------------------------------
	//	Public interface
	//-------------------------------------------------------------------------
	
	public:
	
		inline GlowDeferredTask();
		virtual ~GlowDeferredTask();
		
		inline void Schedule(
			unsigned int msecs);
		inline void Unschedule();
		inline bool IsScheduled();
	
	
	//-------------------------------------------------------------------------
	//	Overrideable implementation
	//-------------------------------------------------------------------------
	
	protected:
	
		virtual void Task() = 0;
	
	
	//-------------------------------------------------------------------------
	//	Private implementation
	//-------------------------------------------------------------------------
	
	protected:
	
		virtual void OnMessage(
			const GlowTimerMessage& message);
	
	private:
	
		int _curTimerID;
};


/*
===============================================================================
	CLASS GlowFixedSizeWindow
	
	Window that can't be resized (useful for widget palettes)
===============================================================================
*/

class GlowFixedSizeWindow :
	public GlowWindow
{
	//-------------------------------------------------------------------------
	//	Public interface
	//-------------------------------------------------------------------------
	
	public:
	
		inline GlowFixedSizeWindow();
		inline GlowFixedSizeWindow(
			const GlowWindowParams& params);
		inline GlowFixedSizeWindow(
			const char* title,
			int x,
			int y,
			int width,
			int height,
			Glow::BufferType mode,
			Glow::EventMask eventMask);
		inline void Init(
			const GlowWindowParams& params);
		inline void Init(
			const char* title,
			int x,
			int y,
			int width,
			int height,
			Glow::BufferType mode,
			Glow::EventMask eventMask);
	
	public:
	
		inline void ForceReshape(
			int width,
			int height);
		inline int FixedWidth() const;
		inline int FixedHeight() const;
	
	
	//-------------------------------------------------------------------------
	//	Private implementation
	//-------------------------------------------------------------------------
	
	private:
	
		int _canonicalWidth;
		int _canonicalHeight;
	
	protected:
	
		virtual void OnReshape(
			int width,
			int height);
};


/*
===============================================================================
	CLASS GlowReceiverTrackerComponent
	
	Component that tracks receivers and automatically deletes them when the
	component goes away
===============================================================================
*/

class GlowReceiverTrackerComponent :
	public GlowComponent,
	public ReceiverTracker
{
	//-------------------------------------------------------------------------
	//	Public interface
	//-------------------------------------------------------------------------
	
	public:
	
		inline GlowReceiverTrackerComponent(
			GlowComponent* parent);
		inline GlowReceiverTrackerComponent();
		inline void Init(
			GlowComponent* parent);
};


/*
===============================================================================
*/

GLOW_NAMESPACE_END


#include "glowUtilities.inl.h"


#endif

