/*
===============================================================================

	FILE:  glowVectorAlgebra.h
	
	PROJECT:
	
		The GLOW Toolkit
	
	CONTENTS:
	
		Vector, matrix and quaternion classes
	
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


#ifndef GLOW_VECTORALGEBRA__H
#define GLOW_VECTORALGEBRA__H


/*
===============================================================================
	Headers and declarations
===============================================================================
*/

#ifndef GLOW_HEADER__H
	#include "glowHeader.h"
#endif

// Microsoft cluelessness: the win32 gl.h depends on windows.h but
// doesn't include it.
#if defined(_WIN32) || defined(WIN32)
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
#endif

#ifdef GLOW_COMPAT_NOINCLUDESUBDIRECTORIES
	#include <gl.h>
#else
	#include <GL/gl.h>
#endif

#include <cstddef>
#ifndef GLOW_OPTION_NOIOSTREAMS
	#include <iosfwd>
#endif


GLOW_INTERNAL_USINGSTD
GLOW_NAMESPACE_BEGIN


/*
===============================================================================
	CLASS Math

	General math constants and functions
===============================================================================
*/

class Math
{
	//-------------------------------------------------------------------------
	//	Public interface
	//-------------------------------------------------------------------------
	
	public:
	
		// Constants
		static const double pi;
		static const double twopi;
		static const double halfpi;
		static const double radiansToDegrees;
		static const double degreesToRadians;
	
	
	//-------------------------------------------------------------------------
	//	Private implementation
	//-------------------------------------------------------------------------
	
	private:
	
		// Don't instantiate this class
		Math() {}
};


/*
===============================================================================
	CLASS Vec3f

	3-vector
===============================================================================
*/

class Vec3f
{
	//-------------------------------------------------------------------------
	//	Public interface
	//-------------------------------------------------------------------------
	
	public:
	
		// Constructors
		
		inline Vec3f(
			GLfloat val = GLfloat(0));
		inline Vec3f(
			GLfloat xval,
			GLfloat yval,
			GLfloat zval);
		inline Vec3f(
			const GLfloat* vals);
		inline Vec3f(
			const Vec3f& v);
		
		// Setters
		
		inline void Set(
			GLfloat xval,
			GLfloat yval,
			GLfloat zval);
		inline void Set(
			GLfloat val);
		inline void Set(
			const GLfloat* vals);
		inline void SetZero();
		inline bool IsZero() const;
		
		// Direct member access
		
		inline GLfloat GetX() const;
		inline GLfloat GetY() const;
		inline GLfloat GetZ() const;
		inline void SetX(
			GLfloat xval);
		inline void SetY(
			GLfloat yval);
		inline void SetZ(
			GLfloat zval);
		
		inline GLfloat& X();
		inline const GLfloat& X() const;
		inline GLfloat& Y();
		inline const GLfloat& Y() const;
		inline GLfloat& Z();
		inline const GLfloat& Z() const;
		
		// Indexed member access
		
		inline GLfloat GetVal(
			GLOW_CSTD::ptrdiff_t i) const;
		inline void SetVal(
			GLOW_CSTD::ptrdiff_t i,
			GLfloat val);
		
		inline GLfloat& operator[](
			GLOW_CSTD::ptrdiff_t i);
		inline GLfloat operator[](
			GLOW_CSTD::ptrdiff_t i) const;
		
		// Copy and cast
		
		inline Vec3f& operator=(
			const Vec3f& v);
		inline Vec3f& operator=(
			const GLfloat* ptr);
		
		inline operator const GLfloat*() const;
		
		// Comparison
		
		inline bool operator==(
			const Vec3f& v) const;
		inline bool operator!=(
			const Vec3f& v) const;
		
		// Negation
		
		inline const Vec3f operator-() const;
		inline void Negate();
		
		// Binary operations with scalars
		
		inline const Vec3f operator*(
			GLfloat op2) const;
		inline const Vec3f operator/(
			GLfloat op2) const;
		inline Vec3f& operator*=(
			GLfloat op2);
		inline Vec3f& operator/=(
			GLfloat op2);
		
		// Binary operations with vectors
		
		inline const Vec3f operator+(
			const Vec3f& op2) const;
		inline const Vec3f operator-(
			const Vec3f& op2) const;
		inline GLfloat operator*(
			const Vec3f& op2) const;
		inline const Vec3f operator%(
			const Vec3f& op2) const;
		inline Vec3f& operator+=(
			const Vec3f& op2);
		inline Vec3f& operator-=(
			const Vec3f& op2);
		inline Vec3f& operator%=(
			const Vec3f& op2);
		
		// Miscellaneous operations
		
		inline GLfloat Norm() const;
		inline GLfloat NormSquared() const;
		inline const Vec3f Normalized() const;
		inline void Normalize();
		
		// Binary I/O
		
#ifndef GLOW_OPTION_NOIOSTREAMS
		inline void WriteTo(
			GLOW_STD::ostream& stream) const;
		inline void ReadFrom(
			GLOW_STD::istream& stream);
#endif
	
	
	//-------------------------------------------------------------------------
	//	Private implementation
	//-------------------------------------------------------------------------
	
	private:
	
		GLfloat _vals[3];
};


/*
===============================================================================
	Non-method declarations
===============================================================================
*/

// Binary operator

inline const Vec3f operator*(
	GLfloat op1,
	const Vec3f& op2);


// vector input/output functions

#ifndef GLOW_OPTION_NOIOSTREAMS

inline GLOW_STD::ostream& operator<<(
	GLOW_STD::ostream& stream,
	const Vec3f& v);

GLOW_STD::istream& operator>>(
	GLOW_STD::istream& stream,
	Vec3f& v);

#endif



/*
===============================================================================
	CLASS Mat4f

	A generic 4-matrix
===============================================================================
*/

class Mat4f
{
	//-------------------------------------------------------------------------
	//	Public interface
	//-------------------------------------------------------------------------
	
	public:
	
		// Constructors
		
		inline Mat4f();
		inline Mat4f(
			GLfloat a, GLfloat b, GLfloat c, GLfloat d,
			GLfloat e, GLfloat f, GLfloat g, GLfloat h,
			GLfloat i, GLfloat j, GLfloat k, GLfloat l,
			GLfloat m, GLfloat n, GLfloat o, GLfloat p);
		inline Mat4f(
			const GLfloat* array);
		inline Mat4f(
			const Mat4f& m);
		
		// Copy and cast
		
		inline Mat4f& operator=(
			const Mat4f& m);
		inline Mat4f& operator=(
			const GLfloat* array);
		
		inline operator const GLfloat*() const;
		
		// Mutators/accessors
		
		inline void Set(
			GLfloat a, GLfloat b, GLfloat c, GLfloat d,
			GLfloat e, GLfloat f, GLfloat g, GLfloat h,
			GLfloat i, GLfloat j, GLfloat k, GLfloat l,
			GLfloat m, GLfloat n, GLfloat o, GLfloat p);
		inline void SetVal(
			short row,
			short col,
			GLfloat val);
		inline GLfloat GetVal(
			short row,
			short col) const;
		
		// Special values
		
		inline void SetIdentity();
		inline void SetZero();
		inline bool IsIdentity() const;
		inline bool IsZero() const;
		
		// Transforms
		
		void SetRotation(
			GLfloat x,
			GLfloat y,
			GLfloat z,
			GLfloat angle);
		void SetTranslation(
			GLfloat x,
			GLfloat y,
			GLfloat z);
		void SetScale(
			GLfloat x,
			GLfloat y,
			GLfloat z);
		inline void SetScale(
			GLfloat s);
		
		inline void SetRotation(
			const Vec3f& axis,
			GLfloat angle);
		inline void SetTranslation(
			const Vec3f& v);
		inline void SetScale(
			const Vec3f& v);
		
		// Transforms axes to the given vectors.
		void SetAxisTransformation(
			const Vec3f& x,
			const Vec3f& y,
			const Vec3f& z);
		
		inline void RemoveTranslation();
		
		// Operations
		
		inline bool operator==(
			const Mat4f& op2) const;
		inline bool operator!=(
			const Mat4f& op2) const;
		
		const Mat4f operator*(
			GLfloat op2) const;
		const Mat4f operator/(
			GLfloat op2) const;
		inline Mat4f& operator*=(
			GLfloat op2);
		inline Mat4f& operator/=(
			GLfloat op2);
		
		const Mat4f operator-() const;
		
		void SetInverse(
			const Mat4f& original);
		const Mat4f Inverse() const;
		void SetTranspose(
			const Mat4f& original);
		const Mat4f Transpose() const;
		void SetCofactors(
			const Mat4f& original);
		const Mat4f Cofactors() const;
		GLfloat Determinant() const;
		
		inline void GetGLMatrix(
			GLfloat* mat) const;
		inline const GLfloat* GetGLMatrix() const;
		inline void SetGLMatrix(
			const GLfloat* mat);
		
		// Apply matrix to vector
		const Vec3f operator*(
			const Vec3f& v);
		
		// Matrix add/subtract/multiply to self operators
		const Mat4f operator+(
			const Mat4f& op2) const;
		const Mat4f operator-(
			const Mat4f& op2) const;
		const Mat4f operator*(
			const Mat4f& op2) const;
		inline Mat4f& operator+=(
			const Mat4f& op2);
		inline Mat4f& operator-=(
			const Mat4f& op2);
		Mat4f& operator*=(
			const Mat4f& op2);
		
#ifndef GLOW_OPTION_NOIOSTREAMS
		inline void WriteTo(
			GLOW_STD::ostream& stream) const;
		inline void ReadFrom(
			GLOW_STD::istream& stream);
#endif
	
	
	//-------------------------------------------------------------------------
	//	Private implementation
	//-------------------------------------------------------------------------
	
	protected:
	
		GLfloat _vals[4][4];
	
	private:
	
		inline void _GJScaleRow(
			short row,
			GLfloat scale);
		inline void _GJSwapRows(
			short row1,
			short row2);
		inline void _GJAddToRow(
			short rowToAdd,
			GLfloat scale,
			short rowToChange);
		inline GLfloat _CofactorElem(
			short i,
			short j) const;
};


/*
===============================================================================
	Non-method template declarations
===============================================================================
*/

// Binary operator

const Mat4f operator*(
	GLfloat op1,
	const Mat4f& op2);


// Matrix i/o operators

#ifndef GLOW_OPTION_NOIOSTREAMS

GLOW_STD::ostream& operator<<(
	GLOW_STD::ostream& stream,
	const Mat4f& m);

GLOW_STD::istream& operator>>(
	GLOW_STD::istream& stream,
	Mat4f& m);

#endif



/*
===============================================================================
	CLASS Quatf

	A generic quaternion
	Storage is in wxyz order
===============================================================================
*/

class Quatf
{
	//-------------------------------------------------------------------------
	//	Public interface
	//-------------------------------------------------------------------------
	
	public:
	
		// Constructors
		
		inline Quatf();
		inline Quatf(
			const Quatf& v);
		inline Quatf(
			const Vec3f& vec);
		inline Quatf(
			const Vec3f& axis,
			GLfloat angle);
		inline Quatf(
			GLfloat a,
			GLfloat b,
			GLfloat c,
			GLfloat d);
		inline Quatf(
			const GLfloat* vals);
		
		// Copy and cast
		
		inline Quatf& operator=(
			const Quatf& q);
		inline Quatf& operator=(
			const GLfloat* vals);
		inline Quatf& operator=(
			const Vec3f& vec);
		
		inline operator const GLfloat*() const;
		
		// Setters
		
		inline void Set(
			GLfloat a,
			GLfloat b,
			GLfloat c,
			GLfloat d);
		inline void Set(
			const GLfloat* vals);
		
		// Direct member access
		
		inline GLfloat GetW() const;
		inline GLfloat GetX() const;
		inline GLfloat GetY() const;
		inline GLfloat GetZ() const;
		inline void SetW(
			GLfloat val);
		inline void SetX(
			GLfloat val);
		inline void SetY(
			GLfloat val);
		inline void SetZ(
			GLfloat val);
		
		inline GLfloat& W();
		inline const GLfloat& W() const;
		inline GLfloat& X();
		inline const GLfloat& X() const;
		inline GLfloat& Y();
		inline const GLfloat& Y() const;
		inline GLfloat& Z();
		inline const GLfloat& Z() const;
		
		// Indexed member access
		
		inline GLfloat GetVal(
			GLOW_CSTD::ptrdiff_t a) const;
		inline void SetVal(
			GLOW_CSTD::ptrdiff_t a,
			GLfloat val);
		
		inline GLfloat& operator[](
			GLOW_CSTD::ptrdiff_t i);
		inline GLfloat operator[](
			GLOW_CSTD::ptrdiff_t i) const;
		
		// Special values
		
		inline void SetIdentity();
		inline bool IsIdentity();
		inline void SetZero();
		inline bool IsZero() const;
		
		// Imaginary part
		
		inline void SetImaginary(
			GLfloat vecX,
			GLfloat vecY,
			GLfloat vecZ);
		inline void SetImaginary(
			const Vec3f& vec);
		inline const Vec3f GetImaginary() const;
		
		// Rotations
		
		inline void SetRotation(
			GLfloat axisX,
			GLfloat axisY,
			GLfloat axisZ,
			GLfloat angle);
		inline void SetRotation(
			const Vec3f& axis,
			GLfloat angle);
		void GetRotation(
			Vec3f& axis,
			GLfloat& angle) const;
		inline Vec3f GetAxis() const;
		inline GLfloat GetAngle() const;
		void ScaleRotation(
			GLfloat factor);
		
		// Operations
		
		inline const Quatf Conjugate() const;
		
		inline GLfloat Norm() const;
		inline void Normalize();
		inline const Quatf Normalized() const;
		
		// Operators
		
		inline bool operator==(
			const Quatf& q) const;
		inline bool operator!=(
			const Quatf& q) const;
		
		inline const Quatf operator-() const;
		
		inline const Quatf operator*(
			GLfloat op2) const;
		inline const Quatf operator/(
			GLfloat op2) const;
		inline Quatf& operator*=(
			GLfloat op2);
		inline Quatf& operator/=(
			GLfloat op2);
		
		inline const Quatf operator+(
			const Quatf& op2) const;
		inline const Quatf operator-(
			const Quatf& op2) const;
		inline GLfloat operator*(
			const Quatf& op2) const;
		const Quatf operator%(
			const Quatf& op2) const;
		inline Quatf& operator+=(
			const Quatf& op2);
		inline Quatf& operator-=(
			const Quatf& op2);
		Quatf& operator%=(
			const Quatf& op2);
		
		// Apply quaternion to a vector
		const Vec3f operator*(
			const Vec3f& v) const;
	
	
	//-------------------------------------------------------------------------
	//	Private implementation
	//-------------------------------------------------------------------------
	
	private:
	
		GLfloat _vals[4];
};


/*
===============================================================================
	Non-method template declarations
===============================================================================
*/

// Binary operator

inline const Quatf operator*(
	GLfloat op1,
	const Quatf& op2);


// Quaternion i/o

#ifndef GLOW_OPTION_NOIOSTREAMS

GLOW_STD::ostream& operator<<(
	GLOW_STD::ostream& stream,
	const Quatf& q);

GLOW_STD::istream& operator>>(
	GLOW_STD::istream& stream,
	Quatf& q);

#endif



/*
===============================================================================
*/


GLOW_NAMESPACE_END


#include "glowVectorAlgebra.inl.h"


#endif

