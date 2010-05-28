/*
===============================================================================

	FILE:  glowSenderReceiver.inl.h
	
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


/*
===============================================================================
	Headers and declarations
===============================================================================
*/

#include <algorithm>

#ifndef GLOW_DEBUG__H
	#include "glowDebug.h"
#endif


GLOW_INTERNAL_USINGSTD
GLOW_NAMESPACE_BEGIN


/*
===============================================================================
	Inline methods of Receiver_Base
===============================================================================
*/

inline Receiver_Base::Receiver_Base()
{
	_numActualSenders = 0;
}


inline unsigned int Receiver_Base::NumSenders() const
{
	return _numActualSenders;
}


inline unsigned int Receiver_Base::NumTrackers() const
{
	return _senders.size() - _numActualSenders;
}


inline void Receiver_Base::_AddSender(
	Sender_Base* sender)
{
	++_numActualSenders;
	_senders.push_back(sender);
}


inline void Receiver_Base::_RemoveSender(
	Sender_Base* sender)
{
	GLOW_STD::list<Sender_Base*>::iterator iter =
		GLOW_STD::find(_senders.begin(), _senders.end(), sender);
	GLOW_DEBUG(iter == _senders.end(),
		"Sender not found in Receiver_Base::_RemoveSender");
	--_numActualSenders;
	_senders.erase(iter);
}


inline void Receiver_Base::_AddTracker(
	Sender_Base* sender)
{
	_senders.push_back(sender);
}


inline void Receiver_Base::_RemoveTracker(
	Sender_Base* sender)
{
	GLOW_STD::list<Sender_Base*>::iterator iter =
		GLOW_STD::find(_senders.begin(), _senders.end(), sender);
	GLOW_DEBUG(iter == _senders.end(),
		"Tracker not found in Receiver_Base::_RemoveTracker");
	_senders.erase(iter);
}


/*
===============================================================================
	Inline methods of Sender_Base
===============================================================================
*/

inline Sender_Base::Sender_Base()
{
}


inline unsigned int Sender_Base::NumReceivers() const
{
	return _receivers.size();
}


inline bool Sender_Base::IsBoundTo(
	Receiver_Base* receiver) const
{
	return GLOW_STD::find(_receivers.begin(), _receivers.end(), receiver)!=_receivers.end();
}


inline void Sender_Base::_RemoveReceiver(
	Receiver_Base* receiver)
{
	GLOW_STD::list<Receiver_Base*>::iterator iter =
		GLOW_STD::find(_receivers.begin(), _receivers.end(), receiver);
	GLOW_DEBUG(iter == _receivers.end(),
		"Receiver not found in Sender_Base::_RemoveReceiver");
	_receivers.erase(iter);
}


/*
===============================================================================
	Inline methods of ReceiverTracker
===============================================================================
*/

inline ReceiverTracker::ReceiverTracker()
{
	_options = neverDelete;
}


inline void ReceiverTracker::Bind(
	Receiver_Base* receiver)
{
	GLOW_DEBUG(GLOW_STD::find(_receivers.begin(), _receivers.end(), receiver)!=_receivers.end(),
		"ReceiverTracker already tracking TReceiver");
	receiver->_AddTracker(this);
	_receivers.push_back(receiver);
}


inline void ReceiverTracker::SetDeletingOptions(
	DeletingOptions options)
{
	_options = options;
}


inline ReceiverTracker::DeletingOptions ReceiverTracker::GetDeletingOptions() const
{
	return _options;
}


/*
===============================================================================
	Inline methods of TReceiver template
===============================================================================
*/

template <class T>
inline TReceiver<T>::TReceiver()
{
}


/*
===============================================================================
	Inline methods of TReceiver void specialization
===============================================================================
*/

#ifdef GLOW_COMPAT_NOTEMPLATESPECIALIZATIONS

inline TReceiver_Void::TReceiver_Void()
{
}

#else

inline TReceiver<void>::TReceiver()
{
}

#endif


/*
===============================================================================
	Inline methods of TSender template
===============================================================================
*/

template <class T>
inline TSender<T>::TSender()
{
}


template <class T>
inline void TSender<T>::Bind(
	TReceiver<T>* receiver)
{
	GLOW_DEBUG(GLOW_STD::find(_receivers.begin(), _receivers.end(), receiver)!=_receivers.end(),
		"TSender already bound to TReceiver");
	
	receiver->_AddSender(this);
	_receivers.push_back(receiver);
}


template <class T>
inline void TSender<T>::Unbind(
	TReceiver<T>* receiver)
{
	GLOW_STD::list<Receiver_Base*>::iterator iter =
		GLOW_STD::find(_receivers.begin(), _receivers.end(), receiver);
	GLOW_DEBUG(iter == _receivers.end(),
		"TSender not bound to TReceiver");
	receiver->_RemoveSender(this);
	_receivers.erase(iter);
}


template <class T>
inline void TSender<T>::Send(
	T message) const
{
	GLOW_STD::list<Receiver_Base*>::const_iterator iter = _receivers.begin();
	while (iter != _receivers.end())
	{
		TReceiver<T>* cur = static_cast<TReceiver<T>*>(*iter);
		iter++;
		cur->OnMessage(message);
	}
}


template <class T>
inline void TSender<T>::Send(
	TReceiver<T>* receiver,
	T message)
{
	receiver->OnMessage(message);
}


/*
===============================================================================
	Inline methods of TSender void specialization
===============================================================================
*/

#ifdef GLOW_COMPAT_NOTEMPLATESPECIALIZATIONS

inline TSender_Void::TSender_Void()
{
}


inline void TSender_Void::Bind(
	TReceiver_Void* receiver)
{
	GLOW_DEBUG(GLOW_STD::find(_receivers.begin(), _receivers.end(), receiver)!=_receivers.end(),
		"TSender already bound to TReceiver");
	
	receiver->_AddSender(this);
	_receivers.push_back(receiver);
}


inline void TSender_Void::Unbind(
	TReceiver_Void* receiver)
{
	receiver->_RemoveSender(this);
	GLOW_STD::list<Receiver_Base*>::iterator iter =
		GLOW_STD::find(_receivers.begin(), _receivers.end(), receiver);
	GLOW_DEBUG(iter == _receivers.end(),
		"TSender not bound to TReceiver");
	_receivers.erase(iter);
}


inline void TSender_Void::Send() const
{
	GLOW_STD::list<Receiver_Base*>::const_iterator iter = _receivers.begin();
	while (iter != _receivers.end())
	{
		TReceiver_Void* cur = static_cast<TReceiver_Void*>(*iter);
		iter++;
		cur->OnMessage();
	}
}


inline void TSender_Void::Send(
	TReceiver_Void* receiver)
{
	receiver->OnMessage();
}

#else

inline TSender<void>::TSender()
{
}


inline void TSender<void>::Bind(
	TReceiver<void>* receiver)
{
	GLOW_DEBUG(GLOW_STD::find(_receivers.begin(), _receivers.end(), receiver)!=_receivers.end(),
		"TSender already bound to TReceiver");
	
	receiver->_AddSender(this);
	_receivers.push_back(receiver);
}


inline void TSender<void>::Unbind(
	TReceiver<void>* receiver)
{
	receiver->_RemoveSender(this);
	GLOW_STD::list<Receiver_Base*>::iterator iter =
		GLOW_STD::find(_receivers.begin(), _receivers.end(), receiver);
	GLOW_DEBUG(iter == _receivers.end(),
		"TSender not bound to TReceiver");
	_receivers.erase(iter);
}


inline void TSender<void>::Send() const
{
	GLOW_STD::list<Receiver_Base*>::const_iterator iter = _receivers.begin();
	while (iter != _receivers.end())
	{
		TReceiver<void>* cur = static_cast<TReceiver<void>*>(*iter);
		iter++;
		cur->OnMessage();
	}
}


inline void TSender<void>::Send(
	TReceiver<void>* receiver)
{
	receiver->OnMessage();
}

#endif


/*
===============================================================================
*/

GLOW_NAMESPACE_END

