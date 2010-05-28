/*
===============================================================================

	FILE:  glowViewTransform.h
	
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


#ifndef GLOW_VIEWTRANSFORM__H
#define GLOW_VIEWTRANSFORM__H


/*
===============================================================================
	Headers and declarations
===============================================================================
*/

#ifndef GLOW_HEADER__H
	#include "glowHeader.h"
#endif

#include <vector>
#include <deque>

#ifndef GLOW_VECTORALGEBRA__H
	#include "glowVectorAlgebra.h"
#endif
#ifndef GLOW__H
	#include "glow.h"
#endif
#ifndef GLOW_AUX__H
	#include "glowAux.h"
#endif


GLOW_INTERNAL_USINGSTD
GLOW_NAMESPACE_BEGIN


class GlowTransformData;

/*
===============================================================================
	CLASS GlowViewTransformParams

	View transform params
===============================================================================
*/

class GlowViewTransformParams
{
	public:
	
		GlowTransformData* connectTo;
		Vec3f initialTranslation;
		Quatf initialRotation;
		float initialScale;
		
		static GlowViewTransformParams defaults;
		
		GlowViewTransformParams();
	
	protected:
	
		GlowViewTransformParams(bool);
};


/*
===============================================================================
	CLASS GlowViewManipulatorParams

	View manip params
===============================================================================
*/

class GlowViewManipulatorParams
{
	public:
	
		GlowTransformData* connectTo;
		Vec3f initialTranslation;
		Quatf initialRotation;
		float initialScale;
		float translationThrottle;
		float rotationThrottle;
		float scaleThrottle;
		GLOW_STD::vector<Vec3f> axisConstraints;
		bool axisConstraintsActive;
		bool spinnable;
		bool draw;
		int spinDataLength;
		GlowColor color;
		
		static GlowViewManipulatorParams defaults;
		
		GlowViewManipulatorParams();
	
	protected:
	
		GlowViewManipulatorParams(bool);
};


/*
===============================================================================
	CLASS GlowTransformData_IdleReceiver

	Spin idle receiver
===============================================================================
*/

class Glow_TransformData_IdleReceiver :
	public GlowIdleReceiver
{
	friend class GlowTransformData;
	
	protected:
	
		inline Glow_TransformData_IdleReceiver(
			GlowTransformData* transform);
	
	protected:
	
		virtual void OnMessage(
			const GlowIdleMessage& message);
	
	private:
	
		GlowTransformData* _transform;
};


/*
===============================================================================
	CLASS GlowViewManip_IdleReceiver

	View manip idle receiver
===============================================================================
*/

class Glow_ViewManip_IdleReceiver :
	public GlowIdleReceiver
{
	friend class GlowViewManipulator;
	
	protected:
	
		inline Glow_ViewManip_IdleReceiver(
			GlowViewManipulator* manip);
	
	protected:
	
		virtual void OnMessage(
			const GlowIdleMessage& message);
	
	private:
	
		GlowViewManipulator* _manip;
};


/*
===============================================================================
	CLASS GlowTransformData

	Transform data
===============================================================================
*/

class GlowViewTransform;

class GlowTransformData
{
	friend class GlowViewTransform;
	
	//-------------------------------------------------------------------------
	//	Public interface
	//-------------------------------------------------------------------------
	
	public:
	
		inline GlowTransformData(
			bool persistent = true);
		inline GlowTransformData(
			const Vec3f& trans,
			const Quatf& rot,
			GLfloat scale,
			bool persistent = true);
		
		virtual ~GlowTransformData();
	
	public:
	
		inline void Set(
			const Vec3f& trans,
			const Quatf& rot,
			GLfloat scale);
		inline void SetTranslation(
			const Vec3f& trans);
		inline void SetRotation(
			const Quatf& rot);
		inline void SetScale(
			GLfloat scale);
		inline void SetIdentity();
		
		inline void AddTranslation(
			const Vec3f& trans);
		inline void AddRotation(
			const Quatf& rot);
		inline void AddScale(
			GLfloat scale);
		
		inline const Vec3f& GetTranslation() const;
		inline const Quatf& GetRotation() const;
		inline GLfloat GetScale() const;
		
		inline Vec3f Apply(
			const Vec3f& vec) const;
		inline Vec3f ApplyInverse(
			const Vec3f& vec) const;
		
		void GetGLMatrixf(
			GLfloat* matrix) const;
		void GetMatrix(
			Mat4f& matrix) const;
		void ApplyToGLMatrix() const;
		void ApplyInverseToGLMatrix() const;
		
		inline bool IsSpinning() const;
		inline void StartSpinning(
			const Quatf& spin);
		inline void StopSpinning();
		inline void StepSpin();
		
		inline bool IsPersistent() const;
		inline void SetPersistent(
			bool persist);
		
		void RefreshAll();
	
	
	//-------------------------------------------------------------------------
	//	Private implementation
	//-------------------------------------------------------------------------
	
	private:
	
		Vec3f _translation;
		Quatf _rotation;
		GLfloat _scale;
		Quatf _curSpin;
		bool _spinning;
		bool _persistent;
		Glow_TransformData_IdleReceiver _receiver;
		GLOW_STD::vector<GlowViewTransform*> _clients;
};


/*
===============================================================================
	CLASS GlowViewTransform

	Transform without arcball
===============================================================================
*/

class GlowViewTransform :
	public GlowComponent
{
	friend class GlowTransformData;
	
	//-------------------------------------------------------------------------
	//	Public interface
	//-------------------------------------------------------------------------
	
	public:
	
		inline GlowViewTransform();
		inline GlowViewTransform(
			GlowComponent *parent,
			const GlowViewTransformParams& params);
		void Init(
			GlowComponent *parent,
			const GlowViewTransformParams& params);
		
		virtual ~GlowViewTransform();
	
	public:
	
		inline void Set(
			const Vec3f& trans,
			const Quatf& rot,
			GLfloat scale);
		inline void SetTranslation(
			const Vec3f& trans);
		inline void SetRotation(
			const Quatf& rot);
		inline void SetScale(
			GLfloat scale);
		inline void SetIdentity();
		
		inline void AddTranslation(
			const Vec3f& trans);
		inline void AddRotation(
			const Quatf& rot);
		inline void AddScale(
			GLfloat scale);
		
		inline const Vec3f& GetTranslation() const;
		inline const Quatf& GetRotation() const;
		inline GLfloat GetScale() const;
		
		inline Vec3f Apply(
			const Vec3f& vec) const;
		inline Vec3f ApplyInverse(
			const Vec3f& vec) const;
		
		inline void GetGLMatrixf(
			GLfloat* matrix) const;
		inline void GetMatrix(
			Mat4f& matrix) const;
		inline void ApplyToGLMatrix() const;
		inline void ApplyInverseToGLMatrix() const;
		
		inline GlowTransformData* TransformData() const;
		void ConnectTo(
			GlowTransformData* transform);
		void ConnectTo(
			GlowViewTransform* transform);
		void Disconnect();
		
		inline bool IsSpinning() const;
		inline void StartSpinning(
			const Quatf& spin);
		inline void StopSpinning();
		inline void StepSpin();
		
		inline void RefreshAllConnected() const;
	
	
	//-------------------------------------------------------------------------
	//	Private implementation
	//-------------------------------------------------------------------------
	
	protected:
	
		GlowTransformData* _transform;
	
	protected:
	
		void _RawDisconnect();
		void _RawConnect(
			GlowTransformData* transform);
	
	protected:
	
		virtual bool OnBeginPaint();
		virtual void OnEndPaint();
};


/*
===============================================================================
	CLASS GlowViewManipulator

	Arcball manipulator
===============================================================================
*/

class GlowViewManipulator :
	public GlowViewTransform
{
	friend class Glow_ViewManip_IdleReceiver;
	
	//-------------------------------------------------------------------------
	//	Public interface
	//-------------------------------------------------------------------------
	
	public:
	
		enum State
		{
			idleState = 0,
			rotatingState = 1,
			translatingState = 2,
			scalingState = 3
		};
	
	public:
	
		inline GlowViewManipulator();
		inline GlowViewManipulator(
			GlowComponent *parent,
			const GlowViewManipulatorParams& params);
		void Init(
			GlowComponent *parent,
			const GlowViewManipulatorParams& params);
	
	public:
	
		inline State GetState() const;
		inline bool IsDragging() const;
		
		inline bool IsSpinnable() const;
		void SetSpinnable(
			bool spinnable);
		
		void BeginScaleDrag(
			GLfloat xn,
			GLfloat yn);
		void BeginTranslationDrag(
			GLfloat xn,
			GLfloat yn);
		void BeginRotationDrag(
			GLfloat xn,
			GLfloat yn);
		void InDrag(
			GLfloat xn,
			GLfloat yn);
		void EndDrag(
			GLfloat xn,
			GLfloat yn);
		
		inline GlowColor GetColor() const;
		inline void SetColor(
			GlowColor c);
		
		inline bool IsDrawing() const;
		inline void SetDrawing(
			bool draw);
		
		inline float GetTranslationThrottle() const;
		inline void SetTranslationThrottle(
			float throttle);
		inline float GetRotationThrottle() const;
		inline void SetRotationThrottle(
			float throttle);
		inline float GetScaleThrottle() const;
		inline void SetScaleThrottle(
			float throttle);
		
		inline GLOW_STD::vector<Vec3f>& AxisConstraints();
		inline const GLOW_STD::vector<Vec3f>& AxisConstraints() const;
		inline void SetAxisConstraintsActive(
			bool active);
		inline bool IsAxisConstraintsActive() const;
	
	
	//-------------------------------------------------------------------------
	//	Private implementation
	//-------------------------------------------------------------------------
	
	private:
	
		Glow_ViewManip_IdleReceiver _receiver;
		
		GLfloat _oldScale;
		Quatf _oldRotation;
		Vec3f _oldTranslation;
		
		GLfloat _scaleThrottle;
		GLfloat _transThrottle;
		GLfloat _rotThrottle;
		GLOW_STD::vector<Vec3f> _axisConstraints;
		bool _axisConstraintsActive;
		
		State _dragType;
		GLfloat _xStart;
		GLfloat _yStart;
		GLfloat _xCur;
		GLfloat _yCur;
		Vec3f _ballDown;
		Vec3f _ballCur;
		
		GLOW_STD::deque<GLOW_STD::pair<Vec3f, int> > _spinData;
		Quatf _curSpin;
		int _spinDataLength;
		bool _spinnable;
		bool _spinStart;
		
		GlowColor _color;
		bool _draw;
	
	private:
	
		static Vec3f _MouseToBall(
			GLfloat xn,
			GLfloat yn);
		static void _DrawArc(
			const Vec3f& start,
			const Vec3f& end,
			int level = 4);
		inline void _SimDrag();
	
	protected:
	
		virtual void OnEndPaint();
};


/*
===============================================================================
*/

GLOW_NAMESPACE_END


#include "glowViewTransform.inl.h"


#endif
