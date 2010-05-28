/*
===============================================================================

	FILE:  glowViewTransform.cpp
	
	PROJECT:
	
		The GLOW Toolkit
	
	CONTENTS:
	
		Arcball-based manipulator for GLOW
	
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

#include <cmath>

using namespace std;

#ifndef GLOW_DEBUG__H
	#include "glowDebug.h"
#endif
#ifndef GLOW_VIEWTRANSFORM__H
	#include "glowViewTransform.h"
#endif


GLOW_INTERNAL_USINGSTD
GLOW_NAMESPACE_BEGIN


/*
===============================================================================
	Param defaults
===============================================================================
*/

GlowViewTransformParams GlowViewTransformParams::defaults(true);

GlowViewTransformParams::GlowViewTransformParams()
{
	*this = defaults;
}

GlowViewTransformParams::GlowViewTransformParams(bool)
{
	connectTo = 0;
	initialScale = 1.0;
}


/*
===============================================================================
	Param defaults
===============================================================================
*/

GlowViewManipulatorParams GlowViewManipulatorParams::defaults(true);

GlowViewManipulatorParams::GlowViewManipulatorParams()
{
	*this = defaults;
}

GlowViewManipulatorParams::GlowViewManipulatorParams(bool) :
color(0.5, 1.0, 1.0)
{
	connectTo = 0;
	draw = false;
	spinnable = false;
	axisConstraintsActive = false;
	translationThrottle = 1.0;
	rotationThrottle = 1.0;
	scaleThrottle = 1.0;
	initialScale = 1.0;
	spinDataLength = 100;
}


/*
===============================================================================
	Methods of Glow_TransformData_IdleReceiver
===============================================================================
*/

void Glow_TransformData_IdleReceiver::OnMessage(
	const GlowIdleMessage& message)
{
	_transform->StepSpin();
}


/*
===============================================================================
	Methods of Glow_ViewManip_IdleReceiver
===============================================================================
*/

void Glow_ViewManip_IdleReceiver::OnMessage(
	const GlowIdleMessage& message)
{
	_manip->_SimDrag();
}


/*
===============================================================================
	Methods of GlowTransformData
===============================================================================
*/

void GlowTransformData::ApplyToGLMatrix() const
{
	GLOW_DEBUGSCOPE("GlowTransformData::ApplyToGLMatrix");
	
	// Base transform
	Vec3f axis;
	GLfloat angle;
	_rotation.GetRotation(axis, angle);
	glTranslatef(_translation.GetX(), _translation.GetY(), _translation.GetZ());
	glRotatef(angle*Math::radiansToDegrees, axis.GetX(), axis.GetY(), axis.GetZ());
	glScalef(_scale, _scale, _scale);
}


void GlowTransformData::ApplyInverseToGLMatrix() const
{
	GLOW_DEBUGSCOPE("GlowTransformData::ApplyInverseToGLMatrix");
	
	// Base transform
	Vec3f axis;
	GLfloat angle;
	_rotation.GetRotation(axis, angle);
	GLfloat scaleinv = 1.0f/_scale;
	glScalef(scaleinv, scaleinv, scaleinv);
	glRotatef(-angle*Math::radiansToDegrees, axis.GetX(), axis.GetY(), axis.GetZ());
	glTranslatef(-_translation.GetX(), -_translation.GetY(), -_translation.GetZ());
}


void GlowTransformData::GetMatrix(
	Mat4f& matrix) const
{
	GLOW_DEBUGSCOPE("GlowTransformData::GetMatrix");
	
	// Base transform
	Mat4f matrix2;
	Vec3f axis;
	GLfloat angle;
	_rotation.GetRotation(axis, angle);
	matrix2.SetTranslation(_translation);
	matrix *= matrix2;
	matrix2.SetRotation(axis, angle);
	matrix *= matrix2;
	matrix2.SetScale(_scale);
	matrix *= matrix2;
}


void GlowTransformData::GetGLMatrixf(
	GLfloat* matrix) const
{
	Mat4f matrix2;
	GetMatrix(matrix2);
	matrix2.GetGLMatrix(matrix);
}


void GlowTransformData::RefreshAll()
{
	for (GLOW_STD::vector<GlowViewTransform*>::iterator iter = _clients.begin();
		iter != _clients.end(); ++iter)
	{
		(*iter)->ParentWindow()->Refresh();
	}
}


GlowTransformData::~GlowTransformData()
{
	for (GLOW_STD::vector<GlowViewTransform*>::iterator iter = _clients.begin();
		iter != _clients.end(); ++iter)
	{
		(*iter)->_transform = new GlowTransformData(
			_translation, _rotation, _scale, false);
	}
}


/*
===============================================================================
	Methods of GlowViewTransform
===============================================================================
*/

void GlowViewTransform::Init(
	GlowComponent *parent,
	const GlowViewTransformParams& params)
{
	GLOW_DEBUGSCOPE("GlowViewTransform::Init");
	GlowComponent::Init(parent);
	
	_RawConnect((params.connectTo != 0) ? params.connectTo :
		new GlowTransformData(params.initialTranslation,
			params.initialRotation, params.initialScale, false));
}


GlowViewTransform::~GlowViewTransform()
{
	_RawDisconnect();
}


bool GlowViewTransform::OnBeginPaint()
{
	::glMatrixMode(GL_MODELVIEW);
	::glPushMatrix();
	ApplyToGLMatrix();
	return true;
}


void GlowViewTransform::OnEndPaint()
{
	::glMatrixMode(GL_MODELVIEW);
	::glPopMatrix();
}


void GlowViewTransform::ConnectTo(
	GlowTransformData* transform)
{
	GLOW_DEBUGSCOPE("GlowViewTransform::ConnectTo");
	GLOW_ASSERT(transform);
	if (transform != _transform)
	{
		_RawDisconnect();
		_RawConnect(transform);
	}
}


void GlowViewTransform::ConnectTo(
	GlowViewTransform* transform)
{
	GLOW_DEBUGSCOPE("GlowViewTransform::ConnectTo");
	GLOW_ASSERT(transform);
	if (transform->TransformData() != _transform)
	{
		_RawDisconnect();
		_RawConnect(transform->TransformData());
	}
}


void GlowViewTransform::Disconnect()
{
	GLOW_DEBUGSCOPE("GlowViewTransform::Disconnect");
	GlowTransformData* newTransform = new GlowTransformData(
		_transform->GetTranslation(), _transform->GetRotation(),
		_transform->GetScale(), false);
	_RawDisconnect();
	_RawConnect(newTransform);
}


/*
===============================================================================
	Methods of GlowViewManipulator
===============================================================================
*/

void GlowViewManipulator::Init(
	GlowComponent *parent,
	const GlowViewManipulatorParams& params)
{
	GLOW_DEBUGSCOPE("GlowViewManip::Init");
	GlowComponent::Init(parent);
	
	_color = params.color;
	_draw = params.draw;
	_axisConstraintsActive = params.axisConstraintsActive;
	_axisConstraints = params.axisConstraints;
	_transThrottle = params.translationThrottle;
	_rotThrottle = params.rotationThrottle * 0.5f;
	_scaleThrottle = params.scaleThrottle * 0.5f;
	_dragType = idleState;
	_spinnable = params.spinnable;
	_spinDataLength = params.spinDataLength;
	_spinStart = false;
	
	_RawConnect((params.connectTo != 0) ? params.connectTo :
		new GlowTransformData(params.initialTranslation,
			params.initialRotation, params.initialScale, false));
}


void GlowViewManipulator::OnEndPaint()
{
	GLOW_DEBUGSCOPE("GlowViewManipulator::OnEndPaint");
	
	// Finish up
	if (_dragType != idleState && _draw)
	{
		::glMatrixMode(GL_MODELVIEW);
		::glLoadIdentity();
		
		::glMatrixMode(GL_PROJECTION);
		::glPushMatrix();
		::glLoadIdentity();
		
		GLboolean lightingSave;
		GLint depthFuncSave;
		::glGetBooleanv(GL_LIGHTING, &lightingSave);
		::glGetIntegerv(GL_DEPTH_FUNC, &depthFuncSave);
		
		::glDisable(GL_LIGHTING);
		::glDepthFunc(GL_ALWAYS);
		
		_color.Apply();
		::glBegin(GL_LINES);
		_DrawArc(Vec3f(1.0f, 0.0f, 0.0f), Vec3f(0.0f, 1.0f, 0.0f));
		_DrawArc(Vec3f(0.0f, 1.0f, 0.0f), Vec3f(-1.0f, 0.0f, 0.0f));
		_DrawArc(Vec3f(-1.0f, 0.0f, 0.0f), Vec3f(0.0f, -1.0f, 0.0f));
		_DrawArc(Vec3f(0.0f, -1.0f, 0.0f), Vec3f(1.0f, 0.0f, 0.0f));
		if (_dragType == scalingState)
		{
			::glVertex2f(_xStart, _yStart);
			::glVertex2f(_xStart, _yCur);
		}
		else if (_dragType == translatingState)
		{
			::glVertex2f(_xStart, _yStart);
			::glVertex2f(_xCur, _yCur);
		}
		else //if (_dragType == rotatingState)
		{
			_DrawArc(_ballDown, _ballCur);
		}
		::glEnd();
		
		::glDepthFunc((GLenum)depthFuncSave);
		if (lightingSave == GL_TRUE)
		{
			::glEnable(GL_LIGHTING);
		}
		
		::glPopMatrix();
		::glMatrixMode(GL_MODELVIEW);
	}
	
	// Compute spin data, and start spinning
	if (_spinnable)
	{
		if (_dragType == rotatingState &&
			(!_axisConstraintsActive || _axisConstraints.size() < 2))
		{
			// While rotating
			int curTime = glutGet(GLenum(GLUT_ELAPSED_TIME));
			_spinData.push_back(GLOW_STD::pair<Vec3f, int>(_ballCur, curTime));
			while (_spinData.front().second < curTime-_spinDataLength &&
				_spinData.size() > 3)
			{
				_spinData.pop_front();
			}
		}
		else if (_spinStart && _spinData.size() > 0)
		{
			_spinStart = false;
			int curTime = glutGet(GLenum(GLUT_ELAPSED_TIME));
			while (_spinData.front().second < curTime-_spinDataLength &&
				_spinData.size() > 2)
			{
				_spinData.pop_front();
			}
			if (_ballCur != _spinData.front().first)
			{
				// Ready to begin spinning
				Quatf curSpin;
				if (_spinData.size() < 2)
				{
					curSpin.SetImaginary(_spinData.front().first % _ballCur);
					curSpin.SetW(_spinData.front().first * _ballCur);
					curSpin.Normalize();
					curSpin.ScaleRotation(_rotThrottle/float(_spinData.size()));
					_transform->StartSpinning(curSpin);
				}
				else
				{
					curSpin.SetImaginary(_spinData.front().first % _spinData.back().first);
					curSpin.SetW(_spinData.front().first * _spinData.back().first);
					curSpin.Normalize();
					curSpin.ScaleRotation(_rotThrottle/float(_spinData.size()-1));
					_transform->StartSpinning(curSpin);
				}
			}
			_spinData.clear();
		}
	}
	::glPopMatrix();
}


void GlowViewManipulator::BeginScaleDrag(
	GLfloat xn,
	GLfloat yn)
{
	GLOW_DEBUGSCOPE("GlowViewManipulator::BeginScaleDrag");
	
	_transform->StopSpinning();
	_xStart = xn;
	_yStart = yn;
	_xCur = xn;
	_yCur = yn;
	_dragType = scalingState;
	_oldTranslation = _transform->GetTranslation();
	_oldRotation = _transform->GetRotation();
	_oldScale = _transform->GetScale();
}


void GlowViewManipulator::BeginTranslationDrag(
	GLfloat xn,
	GLfloat yn)
{
	GLOW_DEBUGSCOPE("GlowViewManipulator::BeginTranslationDrag");
	
	_transform->StopSpinning();
	_xStart = xn;
	_yStart = yn;
	_xCur = xn;
	_yCur = yn;
	_dragType = translatingState;
	_oldTranslation = _transform->GetTranslation();
	_oldRotation = _transform->GetRotation();
	_oldScale = _transform->GetScale();
}


void GlowViewManipulator::BeginRotationDrag(
	GLfloat xn,
	GLfloat yn)
{
	GLOW_DEBUGSCOPE("GlowViewManipulator::BeginRotationDrag");
	
	_transform->StopSpinning();
	_xStart = xn;
	_yStart = yn;
	_xCur = xn;
	_yCur = yn;
	_dragType = rotatingState;
	_ballDown = _ballCur = _MouseToBall(_xStart, _yStart);
	_oldTranslation = _transform->GetTranslation();
	_oldRotation = _transform->GetRotation();
	_oldScale = _transform->GetScale();
	if (_spinnable)
	{
		Glow::RegisterIdle(&_receiver);
	}
}


void GlowViewManipulator::InDrag(
	GLfloat xn,
	GLfloat yn)
{
	GLOW_DEBUGSCOPE("GlowViewManipulator::InDrag");
	
	_xCur = xn;
	_yCur = yn;
	if (_dragType == scalingState)
	{
		float curScale = GLOW_CSTD::pow(2.0f, (_yCur-_yStart)*_scaleThrottle);
		_transform->Set(_oldTranslation * curScale, _oldRotation,
			_oldScale * curScale);
	}
	else if (_dragType == translatingState)
	{
		Vec3f curTranslation((_xCur - _xStart) * _transThrottle,
			(_yCur - _yStart) * _transThrottle, 0);
		_transform->Set(_oldTranslation + curTranslation, _oldRotation, _oldScale);
	}
	else if (_dragType == rotatingState)
	{
		_ballDown = _MouseToBall(_xStart, _yStart);
		_ballCur = _MouseToBall(_xCur, _yCur);
		Quatf curRotation;
		if (_xCur != _xStart || _yCur != _yStart)
		{
			if (_axisConstraintsActive && _axisConstraints.size() != 0)
			{
				GLOW_STD::vector<Vec3f>::iterator best = _axisConstraints.begin();
				float bestValue = 2;
				for (GLOW_STD::vector<Vec3f>::iterator iter = best;
					iter != _axisConstraints.end(); ++iter)
				{
					float curValue = GLOW_CSTD::fabs(_ballCur*(*iter));
					if (curValue < bestValue)
					{
						bestValue = curValue;
						best = iter;
					}
				}
				_ballDown = (_ballDown-(*best)*(_ballDown*(*best))).Normalized();
				_ballCur = (_ballCur-(*best)*(_ballCur*(*best))).Normalized();
			}
			curRotation.SetImaginary(_ballDown%_ballCur);
			curRotation.SetW(_ballDown*_ballCur);
			curRotation.Normalize();
			curRotation.ScaleRotation(_rotThrottle);
		}
		_transform->Set(curRotation * _oldTranslation,
			curRotation % _oldRotation, _oldScale);
	}
}


void GlowViewManipulator::EndDrag(
	GLfloat xn,
	GLfloat yn)
{
	GLOW_DEBUGSCOPE("GlowViewManipulator::EndDrag");
	
	if (IsDragging())
	{
		InDrag(xn, yn);
		if (_dragType == rotatingState && _spinnable)
		{
			Glow::UnregisterIdle(&_receiver);
			_spinStart = true;
		}
		_dragType = idleState;
	}
}


void GlowViewManipulator::SetSpinnable(
	bool spinnable)
{
	GLOW_DEBUGSCOPE("GlowViewManipulator::SetSpinnable");
	if (!spinnable && _spinnable)
	{
		_transform->StopSpinning();
	}
	if (_dragType == rotatingState)
	{
		if (spinnable && !_spinnable)
		{
			Glow::RegisterIdle(&_receiver);
		}
		else if (!spinnable && _spinnable)
		{
			Glow::UnregisterIdle(&_receiver);
			_spinData.clear();
		}
	}
	_spinnable = spinnable;
}


Vec3f GlowViewManipulator::_MouseToBall(
	GLfloat xn,
	GLfloat yn)
{
	GLfloat mag = xn*xn+yn*yn;
	if (mag >= 1.0f)
	{
		return Vec3f(xn, yn, 0.0f);
	}
	else
	{
		return Vec3f(xn, yn, GLOW_CSTD::sqrt(1.0f-mag));
	}
}


void GlowViewManipulator::_DrawArc(
	const Vec3f& start,
	const Vec3f& end,
	int level)
{
	if (level <= 0)
	{
		glVertex2fv(start);
		glVertex2fv(end);
	}
	else
	{
		Vec3f mid = (start+end).Normalized();
		_DrawArc(start, mid, level-1);
		_DrawArc(mid, end, level-1);
	}
}


/*
===============================================================================
*/

GLOW_NAMESPACE_END

