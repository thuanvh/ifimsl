/*
===============================================================================

	FILE:  glowSenderReceiver.h
	
	PROJECT:
	
		The GLOW Toolkit
	
	CONTENTS:
	
		Messaging system
	
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


#ifndef GLOW_SENDERRECEIVER__H
#define GLOW_SENDERRECEIVER__H


/*
===============================================================================
	Headers and declarations
===============================================================================
*/

#ifndef GLOW_HEADER__H
	#include "glowHeader.h"
#endif

#include <list>


GLOW_INTERNAL_USINGSTD
GLOW_NAMESPACE_BEGIN


class Sender_Base;
class Receiver_Base;
template <class T> class TSender;
template <class T> class TReceiver;


/*
===============================================================================
	CLASS Receiver_Base

	Message receiver base class (auto disconnect)
===============================================================================
*/

class Receiver_Base
{
	friend class Sender_Base;
	friend class ReceiverTracker;
	
	
	//-------------------------------------------------------------------------
	//	Public interface
	//-------------------------------------------------------------------------
	
	public:
	
		virtual ~Receiver_Base();
		
		inline unsigned int NumSenders() const;
		inline unsigned int NumTrackers() const;
	
	
	//-------------------------------------------------------------------------
	//	Private implementation
	//-------------------------------------------------------------------------
	
	protected:
	
		inline Receiver_Base();
	
	protected:
	
		inline void _AddSender(
			Sender_Base* sender);
		inline void _RemoveSender(
			Sender_Base* sender);
		inline void _AddTracker(
			Sender_Base* sender);
		inline void _RemoveTracker(
			Sender_Base* sender);
	
	protected:
	
		GLOW_STD::list<Sender_Base*> _senders;
		unsigned int _numActualSenders;
};


/*
===============================================================================
	CLASS Sender_Base

	Message sender base class (auto disconnect)
===============================================================================
*/

class Sender_Base
{
	friend class Receiver_Base;
	
	
	//-------------------------------------------------------------------------
	//	Public interface
	//-------------------------------------------------------------------------
	
	public:
	
		virtual ~Sender_Base();
		
		inline unsigned int NumReceivers() const;
		
		void UnbindAll();
		inline bool IsBoundTo(
			Receiver_Base* receiver) const;
		
	
	//-------------------------------------------------------------------------
	//	Private implementation
	//-------------------------------------------------------------------------
	
	protected:
	
		inline Sender_Base();
	
	protected:
	
		inline void _RemoveReceiver(
			Receiver_Base* receiver);
	
	protected:
	
		GLOW_STD::list<Receiver_Base*> _receivers;
};


/*
===============================================================================
	CLASS ReceiverTracker

	Class that can track receivers
===============================================================================
*/

class ReceiverTracker :
	public Sender_Base
{
	//-------------------------------------------------------------------------
	//	Public interface
	//-------------------------------------------------------------------------
	
	public:
	
		enum DeletingOptions
		{
			neverDelete = 0,
			alwaysDelete = 1,
			referenceCountDelete = 2
		};
	
	public:
	
		inline ReceiverTracker();
		
		virtual ~ReceiverTracker();
		
		inline void Bind(
			Receiver_Base* receiver);
		inline void Unbind(
			Receiver_Base* receiver);
		void UnbindAll();
		
		void DeleteAllReceivers();
		
		inline void SetDeletingOptions(
			DeletingOptions options);
		inline DeletingOptions GetDeletingOptions() const;
	
	
	//-------------------------------------------------------------------------
	//	Private implementation
	//-------------------------------------------------------------------------
	
	private:
	
		DeletingOptions _options;
};


/*
===============================================================================
	CLASS TReceiver

	Message receiver (auto disconnect)
===============================================================================
*/

template <class T>
class TReceiver :
	public Receiver_Base
{
	friend class TSender<T>;
	
	
	//-------------------------------------------------------------------------
	//	Public interface
	//-------------------------------------------------------------------------
	
	public:
	
		typedef T MessageType;
	
	public:
	
		inline TReceiver();
	
	
	//-------------------------------------------------------------------------
	//	Overrideable implementation
	//-------------------------------------------------------------------------
	
	protected:
	
		virtual void OnMessage(
			T message) = 0;
};


/*
===============================================================================
	SPECIALIZATION for void
===============================================================================
*/

#ifdef GLOW_COMPAT_NOTEMPLATESPECIALIZATIONS

class TReceiver_Void :
	public Receiver_Base
{
	friend class TSender_Void;
	
	
	//-------------------------------------------------------------------------
	//	Public interface
	//-------------------------------------------------------------------------
	
	public:
	
		typedef void MessageType;
	
	public:
	
		inline TReceiver_Void();
	
	
	//-------------------------------------------------------------------------
	//	Overrideable implementation
	//-------------------------------------------------------------------------
	
	protected:
	
		virtual void OnMessage() = 0;
};

#else

template<>
class TReceiver<void> :
	public Receiver_Base
{
	friend class TSender<void>;
	
	
	//-------------------------------------------------------------------------
	//	Public interface
	//-------------------------------------------------------------------------
	
	public:
	
		typedef void MessageType;
	
	public:
	
		inline TReceiver();
	
	
	//-------------------------------------------------------------------------
	//	Overrideable implementation
	//-------------------------------------------------------------------------
	
	protected:
	
		virtual void OnMessage() = 0;
};

#endif


/*
===============================================================================
	CLASS TSender

	Bound message sender (auto disconnect)
===============================================================================
*/

template <class T>
class TSender :
	public Sender_Base
{
	friend class TReceiver<T>;
	
	
	//-------------------------------------------------------------------------
	//	Public interface
	//-------------------------------------------------------------------------
	
	public:
	
		typedef T MessageType;
	
	public:
	
		inline static void Send(
			TReceiver<T>* receiver,
			T message);
	
	public:
	
		inline TSender();
		
		inline void Bind(
			TReceiver<T>* receiver);
		inline void Unbind(
			TReceiver<T>* receiver);
		
		inline void Send(
			T message) const;
};


/*
===============================================================================
	SPECIALIZATION for void
===============================================================================
*/

#ifdef GLOW_COMPAT_NOTEMPLATESPECIALIZATIONS

class TSender_Void :
	public Sender_Base
{
	friend class TReceiver_Void;
	
	
	//-------------------------------------------------------------------------
	//	Public interface
	//-------------------------------------------------------------------------
	
	public:
	
		typedef void MessageType;
	
	public:
	
		inline static void Send(
			TReceiver_Void* receiver);
	
	public:
	
		inline TSender_Void();
		
		inline void Bind(
			TReceiver_Void* receiver);
		inline void Unbind(
			TReceiver_Void* receiver);
		
		inline void Send() const;
};

#else

template<>
class TSender<void> :
	public Sender_Base
{
	friend class TReceiver<void>;
	
	
	//-------------------------------------------------------------------------
	//	Public interface
	//-------------------------------------------------------------------------
	
	public:
	
		typedef void MessageType;
	
	public:
	
		inline static void Send(
			TReceiver<void>* receiver);
	
	public:
	
		inline TSender();
		
		inline void Bind(
			TReceiver<void>* receiver);
		inline void Unbind(
			TReceiver<void>* receiver);
		
		inline void Send() const;
};

#endif


/*
===============================================================================
*/

GLOW_NAMESPACE_END


#include "glowSenderReceiver.inl.h"


#endif
