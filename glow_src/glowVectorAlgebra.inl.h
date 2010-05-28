/*
===============================================================================

	FILE:  glowVectorAlgebra.inl.h
	
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


/*
===============================================================================
	Headers and declarations
===============================================================================
*/

#include <cmath>
#ifndef GLOW_OPTION_NOIOSTREAMS
	#include <iostream>
#endif

#ifndef GLOW_DEBUG__H
	#include "glowDebug.h"
#endif

GLOW_INTERNAL_USINGSTD
GLOW_NAMESPACE_BEGIN


/*
===============================================================================
	Inline methods of Vec3f and friends
===============================================================================
*/

//-----------------------------------------------------------------------------
// Vector constructors
//-----------------------------------------------------------------------------

inline Vec3f::Vec3f(
	GLfloat val)
{
	Set(val);
}


inline Vec3f::Vec3f(
	GLfloat xval,
	GLfloat yval,
	GLfloat zval)
{
	Set(xval, yval, zval);
}


inline Vec3f::Vec3f(
	const GLfloat* vals)
{
	Set(vals);
}


inline Vec3f::Vec3f(
	const Vec3f& v)
{
	Set(v._vals[0], v._vals[1], v._vals[2]);
}


//-----------------------------------------------------------------------------
// Vector global mutator
//-----------------------------------------------------------------------------

inline void Vec3f::Set(
	GLfloat xval,
	GLfloat yval,
	GLfloat zval)
{
	_vals[0]=xval;
	_vals[1]=yval;
	_vals[2]=zval;
}


inline void Vec3f::Set(
	GLfloat val)
{
	_vals[0]=val;
	_vals[1]=val;
	_vals[2]=val;
}


inline void Vec3f::Set(
	const GLfloat* vals)
{
	_vals[0]=vals[0];
	_vals[1]=vals[1];
	_vals[2]=vals[2];
}


//-----------------------------------------------------------------------------
// Vector set zero
//-----------------------------------------------------------------------------

inline void Vec3f::SetZero()
{
	_vals[0] = GLfloat(0);
	_vals[1] = GLfloat(0);
	_vals[2] = GLfloat(0);
}


inline bool Vec3f::IsZero() const
{
	return (_vals[0] == GLfloat(0) && _vals[1] == GLfloat(0) &&
		_vals[2] == GLfloat(0));
}


//-----------------------------------------------------------------------------
// Vector individual accessors
//-----------------------------------------------------------------------------

inline GLfloat Vec3f::GetX() const
{
	return _vals[0];
}

inline GLfloat Vec3f::GetY() const
{
	return _vals[1];
}

inline GLfloat Vec3f::GetZ() const
{
	return _vals[2];
}


//-----------------------------------------------------------------------------
// Vector individual mutators
//-----------------------------------------------------------------------------

inline void Vec3f::SetX(
	GLfloat xval)
{
	_vals[0]=xval;
}

inline void Vec3f::SetY(
	GLfloat yval)
{
	_vals[1]=yval;
}

inline void Vec3f::SetZ(
	GLfloat zval)
{
	_vals[2]=zval;
}


//-----------------------------------------------------------------------------
// Vector direct member access
//-----------------------------------------------------------------------------

inline GLfloat& Vec3f::X()
{
	return _vals[0];
}

inline GLfloat& Vec3f::Y()
{
	return _vals[1];
}

inline GLfloat& Vec3f::Z()
{
	return _vals[2];
}

inline const GLfloat& Vec3f::X() const
{
	return _vals[0];
}

inline const GLfloat& Vec3f::Y() const
{
	return _vals[1];
}

inline const GLfloat& Vec3f::Z() const
{
	return _vals[2];
}


//-----------------------------------------------------------------------------
// Vector indexed accessors
//-----------------------------------------------------------------------------

inline GLfloat Vec3f::GetVal(
	GLOW_CSTD::ptrdiff_t i) const
{
	GLOW_DEBUG(i < 0 || i >= 3, "index out of bounds in Vec3f::GetVal");
	return _vals[i];
}

inline void Vec3f::SetVal(
	GLOW_CSTD::ptrdiff_t i,
	GLfloat val)
{
	GLOW_DEBUG(i < 0 || i >= 3, "index out of bounds in Vec3f::SetVal");
	_vals[i] = val;
}


//-----------------------------------------------------------------------------
// Vector operator []
//-----------------------------------------------------------------------------

inline GLfloat& Vec3f::operator[](
	GLOW_CSTD::ptrdiff_t i)
{
	GLOW_DEBUG(i < 0 || i >= 3, "index out of bounds in Vec3f::operator[]");
	return _vals[i];
}

inline GLfloat Vec3f::operator[](
	GLOW_CSTD::ptrdiff_t i) const
{
	GLOW_DEBUG(i < 0 || i >= 3, "index out of bounds in Vec3f::operator[]");
	return _vals[i];
}


//-----------------------------------------------------------------------------
// Vector cast
//-----------------------------------------------------------------------------

inline Vec3f::operator const GLfloat*() const
{
	return _vals;
}


//-----------------------------------------------------------------------------
// Vector operator =
//-----------------------------------------------------------------------------

inline Vec3f& Vec3f::operator=(
	const Vec3f& v)
{
	_vals[0] = v._vals[0];
	_vals[1] = v._vals[1];
	_vals[2] = v._vals[2];
	return *this;
}


inline Vec3f& Vec3f::operator=(
	const GLfloat* array)
{
	_vals[0] = array[0];
	_vals[1] = array[1];
	_vals[2] = array[2];
	return *this;
}


//-----------------------------------------------------------------------------
// Vector binary operator == and !=
//-----------------------------------------------------------------------------

inline bool Vec3f::operator==(
	const Vec3f& v) const
{
	return _vals[0] == v._vals[0] && _vals[1] == v._vals[1] &&
		_vals[2] == v._vals[2];
}


inline bool Vec3f::operator!=(
	const Vec3f& v) const
{
	return _vals[0] != v._vals[0] || _vals[1] != v._vals[1] ||
		_vals[2] != v._vals[2];
}


//-----------------------------------------------------------------------------
// Vector unary operator -
//-----------------------------------------------------------------------------

inline const Vec3f Vec3f::operator-() const
{
	return Vec3f(-_vals[0], -_vals[1], -_vals[2]);
}


inline void Vec3f::Negate()
{
	_vals[0] = -_vals[0];
	_vals[1] = -_vals[1];
	_vals[2] = -_vals[2];
}


//-----------------------------------------------------------------------------
// Vector binary operations with scalars
//-----------------------------------------------------------------------------

inline const Vec3f Vec3f::operator*(
	GLfloat op2) const
{
	return Vec3f(_vals[0]*op2, _vals[1]*op2, _vals[2]*op2);
}


inline const Vec3f operator*(
	GLfloat op1,
	const Vec3f& op2)
{
	return Vec3f(op1*op2.GetX(), op1*op2.GetY(), op1*op2.GetZ());
}


inline const Vec3f Vec3f::operator/(
	GLfloat op2) const
{
	return Vec3f(_vals[0]/op2, _vals[1]/op2, _vals[2]/op2);
}


inline Vec3f& Vec3f::operator*=(
	GLfloat op2)
{
	_vals[0] *= op2;
	_vals[1] *= op2;
	_vals[2] *= op2;
	
	return *this;
}


inline Vec3f& Vec3f::operator/=(
	GLfloat op2)
{
	_vals[0] /= op2;
	_vals[1] /= op2;
	_vals[2] /= op2;
	
	return *this;
}


//-----------------------------------------------------------------------------
// Vector binary operators with vector
//-----------------------------------------------------------------------------

inline const Vec3f Vec3f::operator+(
	const Vec3f& op2) const
{
	return Vec3f(
		_vals[0]+op2._vals[0],
		_vals[1]+op2._vals[1],
		_vals[2]+op2._vals[2]);
}


inline const Vec3f Vec3f::operator-(
	const Vec3f& op2) const
{
	return Vec3f(
		_vals[0]-op2._vals[0],
		_vals[1]-op2._vals[1],
		_vals[2]-op2._vals[2]);
}


inline GLfloat Vec3f::operator*(
	const Vec3f& op2) const
{
	return _vals[0]*op2._vals[0] +
		_vals[1]*op2._vals[1] +
		_vals[2]*op2._vals[2];
}


inline const Vec3f Vec3f::operator%(
	const Vec3f& op2) const
{
	return Vec3f(
		_vals[1]*op2._vals[2]-_vals[2]*op2._vals[1],
		_vals[2]*op2._vals[0]-_vals[0]*op2._vals[2],
		_vals[0]*op2._vals[1]-_vals[1]*op2._vals[0]);
}


inline Vec3f& Vec3f::operator+=(
	const Vec3f& op2)
{
	_vals[0] += op2._vals[0];
	_vals[1] += op2._vals[1];
	_vals[2] += op2._vals[2];
	return *this;
}


inline Vec3f& Vec3f::operator-=(
	const Vec3f& op2)
{
	_vals[0] -= op2._vals[0];
	_vals[1] -= op2._vals[1];
	_vals[2] -= op2._vals[2];
	return *this;
}


inline Vec3f& Vec3f::operator%=(
	const Vec3f& op2)
{
	Set(_vals[1]*op2._vals[2]-_vals[2]*op2._vals[1],
		_vals[2]*op2._vals[0]-_vals[0]*op2._vals[2],
		_vals[0]*op2._vals[1]-_vals[1]*op2._vals[0]);
	return *this;
}


//-----------------------------------------------------------------------------
// Vector Norm operations
//-----------------------------------------------------------------------------

inline GLfloat Vec3f::Norm() const
{
	return GLOW_CSTD::sqrt(_vals[0]*_vals[0]+_vals[1]*_vals[1]+_vals[2]*_vals[2]);
}


inline GLfloat Vec3f::NormSquared() const
{
	return GLfloat(_vals[0]*_vals[0]+_vals[1]*_vals[1]+_vals[2]*_vals[2]);
}


inline const Vec3f Vec3f::Normalized() const
{
	GLfloat norm = GLOW_CSTD::sqrt(_vals[0]*_vals[0]+_vals[1]*_vals[1]+_vals[2]*_vals[2]);
	return (norm == 0.0) ?
		Vec3f(0, 0, 0) :
		Vec3f(_vals[0]/norm, _vals[1]/norm, _vals[2]/norm);
}


inline void Vec3f::Normalize()
{
	GLfloat norm = GLOW_CSTD::sqrt(_vals[0]*_vals[0]+_vals[1]*_vals[1]+_vals[2]*_vals[2]);
	if (norm != 0)
	{
		_vals[0]/=norm;
		_vals[1]/=norm;
		_vals[2]/=norm;
	}
}


//-----------------------------------------------------------------------------
// Binary i/o
//-----------------------------------------------------------------------------

#ifndef GLOW_OPTION_NOIOSTREAMS

inline void Vec3f::WriteTo(
	GLOW_STD::ostream& stream) const
{
	stream.write(reinterpret_cast<const char*>(&_vals[0]), sizeof(GLfloat));
	stream.write(reinterpret_cast<const char*>(&_vals[1]), sizeof(GLfloat));
	stream.write(reinterpret_cast<const char*>(&_vals[2]), sizeof(GLfloat));
}


inline void Vec3f::ReadFrom(
	GLOW_STD::istream& stream)
{
	stream.read(reinterpret_cast<char*>(&_vals[0]), sizeof(GLfloat));
	stream.read(reinterpret_cast<char*>(&_vals[1]), sizeof(GLfloat));
	stream.read(reinterpret_cast<char*>(&_vals[2]), sizeof(GLfloat));
}

#endif


//********************************************************* I/O FUNCTIONS *****

#ifndef GLOW_OPTION_NOIOSTREAMS

//-----------------------------------------------------------------------------
// Templated vector output function
//-----------------------------------------------------------------------------
inline GLOW_STD::ostream& operator<<(
	GLOW_STD::ostream& stream,
	const Vec3f& v)
{
	stream << "[ " << v.GetX() << " " << v.GetY() << " " << v.GetZ() << " ]";
	return stream;
}

#endif



/*
===============================================================================
	Inline methods of Mat4f
===============================================================================
*/

//-----------------------------------------------------------------------------
// Constructors
//-----------------------------------------------------------------------------

inline Mat4f::Mat4f()
{
	SetIdentity();
}


inline Mat4f::Mat4f(
	GLfloat a, GLfloat b, GLfloat c, GLfloat d,
	GLfloat e, GLfloat f, GLfloat g, GLfloat h,
	GLfloat i, GLfloat j, GLfloat k, GLfloat l,
	GLfloat m, GLfloat n, GLfloat o, GLfloat p)
{
	_vals[0][0] = a;
	_vals[1][0] = b;
	_vals[2][0] = c;
	_vals[3][0] = d;
	_vals[0][1] = e;
	_vals[1][1] = f;
	_vals[2][1] = g;
	_vals[3][1] = h;
	_vals[0][2] = i;
	_vals[1][2] = j;
	_vals[2][2] = k;
	_vals[3][2] = l;
	_vals[0][3] = m;
	_vals[1][3] = n;
	_vals[2][3] = o;
	_vals[3][3] = p;
}


inline Mat4f::Mat4f(
	const GLfloat* array)
{
	GLfloat* ptr = (GLfloat*)_vals;
	for (short i=0; i<16; ++i)
	{
		ptr[i] = array[i];
	}
}


inline Mat4f::Mat4f(
	const Mat4f& m)
{
	GLfloat* ptr = (GLfloat*)_vals;
	const GLfloat* ptr2 = (GLfloat*)(m._vals);
	for (short i=0; i<16; ++i)
	{
		ptr[i] = ptr2[i];
	}
}


//-----------------------------------------------------------------------------
// Matrix operator =
//-----------------------------------------------------------------------------

inline Mat4f& Mat4f::operator=(
	const Mat4f& m)
{
	GLfloat* ptr = (GLfloat*)_vals;
	const GLfloat* ptr2 = (GLfloat*)(m._vals);
	for (short i=0; i<16; ++i)
	{
		ptr[i] = ptr2[i];
	}
	return *this;
}


inline Mat4f& Mat4f::operator=(
	const GLfloat* array)
{
	GLfloat* ptr = (GLfloat*)_vals;
	for (short i=0; i<16; ++i)
	{
		ptr[i] = array[i];
	}
	return *this;
}


//-----------------------------------------------------------------------------
// Matrix cast
//-----------------------------------------------------------------------------

inline Mat4f::operator const GLfloat*() const
{
	return (const GLfloat*)_vals;
}


//-----------------------------------------------------------------------------
// Set matrix
//-----------------------------------------------------------------------------

inline void Mat4f::Set(
	GLfloat a, GLfloat b, GLfloat c, GLfloat d,
	GLfloat e, GLfloat f, GLfloat g, GLfloat h,
	GLfloat i, GLfloat j, GLfloat k, GLfloat l,
	GLfloat m, GLfloat n, GLfloat o, GLfloat p)
{
	_vals[0][0] = a;
	_vals[1][0] = b;
	_vals[2][0] = c;
	_vals[3][0] = d;
	_vals[0][1] = e;
	_vals[1][1] = f;
	_vals[2][1] = g;
	_vals[3][1] = h;
	_vals[0][2] = i;
	_vals[1][2] = j;
	_vals[2][2] = k;
	_vals[3][2] = l;
	_vals[0][3] = m;
	_vals[1][3] = n;
	_vals[2][3] = o;
	_vals[3][3] = p;
}


//-----------------------------------------------------------------------------
// Indexed access
//-----------------------------------------------------------------------------

inline void Mat4f::SetVal(
	short row,
	short col,
	GLfloat val)
{
	_vals[col][row] = val;
}


inline GLfloat Mat4f::GetVal(
	short row,
	short col) const
{
	return _vals[col][row];
}


//-----------------------------------------------------------------------------
// Identity and zero matrices
//-----------------------------------------------------------------------------

inline void Mat4f::SetIdentity()
{
	for (short j=0; j<4; j++)
	for (short i=0; i<4; i++)
	{
		_vals[i][j] = (i==j) ? 1 : 0;
	}
}


inline void Mat4f::SetZero()
{
	for (short j=0; j<4; j++)
	for (short i=0; i<4; i++)
	{
		_vals[i][j] = 0;
	}
}


inline bool Mat4f::IsIdentity() const
{
	for (short j=0; j<4; j++)
	for (short i=0; i<4; i++)
	{
		if ((i != j && _vals[i][j] != 0) ||
			(i == j && _vals[i][j] != GLfloat(1)))
				return false;
	}
	return true;
}


inline bool Mat4f::IsZero() const
{
	for (short j=0; j<4; j++)
	for (short i=0; i<4; i++)
	{
		if (_vals[i][j] != 0)
			return false;
	}
	return true;
}


//-----------------------------------------------------------------------------
// Extract an OpenGL matrix
//-----------------------------------------------------------------------------

inline void Mat4f::GetGLMatrix(
	GLfloat* mat) const
{
	const GLfloat* ptr = (GLfloat*)_vals;
	for (short i=0; i<16; ++i)
	{
		mat[i] = ptr[i];
	}
}


inline const GLfloat* Mat4f::GetGLMatrix() const
{
	return (const GLfloat*)_vals;
}


inline void Mat4f::SetGLMatrix(
	const GLfloat* mat)
{
	GLfloat* ptr = (GLfloat*)_vals;
	for (short i=0; i<16; ++i)
	{
		ptr[i] = mat[i];
	}
}


//-----------------------------------------------------------------------------
// Create transforms
//-----------------------------------------------------------------------------

inline void Mat4f::SetScale(
	GLfloat s)
{
	SetScale(s, s, s);
}


inline void Mat4f::SetRotation(
	const Vec3f& axis,
	GLfloat angle)
{
	SetRotation(axis.GetX(), axis.GetY(), axis.GetZ(), angle);
}


inline void Mat4f::SetTranslation(
	const Vec3f &v)
{
	SetTranslation(v.GetX(), v.GetY(), v.GetZ());
}


inline void Mat4f::SetScale(
	const Vec3f &v)
{
	SetScale(v.GetX(), v.GetY(), v.GetZ());
}


//-----------------------------------------------------------------------------
// Remove translation component
//-----------------------------------------------------------------------------

inline void Mat4f::RemoveTranslation()
{
	_vals[3][0] = 0;
	_vals[3][1] = 0;
	_vals[3][2] = 0;
}


//-----------------------------------------------------------------------------
// Gauss-Jordan helper: Scale a row
//-----------------------------------------------------------------------------

inline void Mat4f::_GJScaleRow(
	short row,
	GLfloat scale)
{
	for (short i=0; i<4; i++)
	{
		_vals[i][row] *= scale;
	}
}


//-----------------------------------------------------------------------------
// Gauss-Jordan helper: Swap two rows
//-----------------------------------------------------------------------------

inline void Mat4f::_GJSwapRows(
	short row1,
	short row2)
{
	for (short i=0; i<4; i++)
	{
		GLfloat temp = _vals[i][row1];
		_vals[i][row1] = _vals[i][row2];
		_vals[i][row2] = temp;
	}
}


//-----------------------------------------------------------------------------
// Gauss-Jordan helper: Add a row to another
//-----------------------------------------------------------------------------

inline void Mat4f::_GJAddToRow(
	short rowToAdd,
	GLfloat scale,
	short rowToChange)
{
	for (short i=0; i<4; i++)
	{
		_vals[i][rowToChange] += _vals[i][rowToAdd]*scale;
	}
}


//-----------------------------------------------------------------------------
// Find cofactor element at given location
//-----------------------------------------------------------------------------

inline GLfloat Mat4f::_CofactorElem(
	short a,
	short b) const
{
	short p[3], q[3];
	
	for (short i=0; i<3; i++)
	{
		p[i] = (i<a) ? i : i+1;
		q[i] = (i<b) ? i : i+1;
	}
	
	GLfloat ret = _vals[p[0]][q[0]] * _vals[p[1]][q[1]] * _vals[p[2]][q[2]] +
	      _vals[p[1]][q[0]] * _vals[p[2]][q[1]] * _vals[p[0]][q[2]] +
	      _vals[p[2]][q[0]] * _vals[p[0]][q[1]] * _vals[p[1]][q[2]] -
	      _vals[p[2]][q[0]] * _vals[p[1]][q[1]] * _vals[p[0]][q[2]] -
	      _vals[p[1]][q[0]] * _vals[p[0]][q[1]] * _vals[p[2]][q[2]] -
	      _vals[p[0]][q[0]] * _vals[p[2]][q[1]] * _vals[p[1]][q[2]];
	
	return ((((a+b)%2) != 0) ? -ret : ret);
}


//-----------------------------------------------------------------------------
// Comparisons
//-----------------------------------------------------------------------------

inline bool Mat4f::operator==(
	const Mat4f& op2) const
{
	for (short j=0; j<4; j++)
	for (short i=0; i<4; i++)
	{
		if (_vals[i][j] != op2._vals[i][j])
			return false;
	}
	
	return true;
}


inline bool Mat4f::operator!=(
	const Mat4f& op2) const
{
	for (short j=0; j<4; j++)
	for (short i=0; i<4; i++)
	{
		if (_vals[i][j] != op2._vals[i][j])
			return true;
	}
	
	return false;
}


//-----------------------------------------------------------------------------
// Self-multiplication and division by a scalar
//-----------------------------------------------------------------------------

inline Mat4f& Mat4f::operator*=(
	GLfloat op2)
{
	for (short j=0; j<4; j++)
	for (short i=0; i<4; i++)
	{
		_vals[i][j] *= op2;
	}
	
	return *this;
}


inline Mat4f& Mat4f::operator/=(
	GLfloat op2)
{
	for (short j=0; j<4; j++)
	for (short i=0; i<4; i++)
	{
		_vals[i][j] /= op2;
	}
	
	return *this;
}


//-----------------------------------------------------------------------------
// Matrix self-add and self-subtract
//-----------------------------------------------------------------------------

inline Mat4f& Mat4f::operator+=(
	const Mat4f& op2)
{
	for (short j=0; j<4; j++)
	for (short i=0; i<4; i++)
	{
		_vals[j][i] += op2._vals[j][i];
	}
	
	return *this;
}


inline Mat4f& Mat4f::operator-=(
	const Mat4f& op2)
{
	for (short j=0; j<4; j++)
	for (short i=0; i<4; i++)
	{
		_vals[j][i] -= op2._vals[j][i];
	}
	
	return *this;
}


//-----------------------------------------------------------------------------
// Binary i/o
//-----------------------------------------------------------------------------

#ifndef GLOW_OPTION_NOIOSTREAMS

inline void Mat4f::WriteTo(
	GLOW_STD::ostream& stream) const
{
	for (short j=0; j<4; j++)
	for (short i=0; i<4; i++)
	{
		stream.write(reinterpret_cast<const char*>(&_vals[i][j]), sizeof(GLfloat));
	}
}


inline void Mat4f::ReadFrom(
	GLOW_STD::istream& stream)
{
	for (short j=0; j<4; j++)
	for (short i=0; i<4; i++)
	{
		stream.read(reinterpret_cast<char*>(&_vals[i][j]), sizeof(GLfloat));
	}
}

#endif



/*
===============================================================================
	Inline methods of Quatf
===============================================================================
*/

//-----------------------------------------------------------------------------
// Constructors
//-----------------------------------------------------------------------------

inline Quatf::Quatf()
{
	SetIdentity();
}


inline Quatf::Quatf(
	const Quatf& q)
{
	_vals[0] = q._vals[0];
	_vals[1] = q._vals[1];
	_vals[2] = q._vals[2];
	_vals[3] = q._vals[3];
}


inline Quatf::Quatf(
	GLfloat a,
	GLfloat b,
	GLfloat c,
	GLfloat d)
{
	_vals[0] = a;
	_vals[1] = b;
	_vals[2] = c;
	_vals[3] = d;
}


inline Quatf::Quatf(
	const GLfloat* vals)
{
	_vals[0] = vals[0];
	_vals[1] = vals[1];
	_vals[2] = vals[2];
	_vals[3] = vals[3];
}


//-----------------------------------------------------------------------------
// Vector (pure imaginary and rotation) constructors
//-----------------------------------------------------------------------------

inline Quatf::Quatf(
	const Vec3f& vec)
{
	_vals[0] = 0.0f;
	_vals[1] = vec.GetX();
	_vals[2] = vec.GetY();
	_vals[3] = vec.GetZ();
}


inline Quatf::Quatf(
	const Vec3f& axis,
	GLfloat angle)
{
	SetRotation(axis, angle);
}


//-----------------------------------------------------------------------------
// Quaternion operator =
//-----------------------------------------------------------------------------

inline Quatf& Quatf::operator=(
	const Quatf& q)
{
	_vals[0] = q._vals[0];
	_vals[1] = q._vals[1];
	_vals[2] = q._vals[2];
	_vals[3] = q._vals[3];
	return *this;
}


inline Quatf& Quatf::operator=(
	const GLfloat* vals)
{
	_vals[0] = vals[0];
	_vals[1] = vals[1];
	_vals[2] = vals[2];
	_vals[3] = vals[3];
	return *this;
}


inline Quatf& Quatf::operator=(
	const Vec3f& vec)
{
	_vals[0] = 0.0f;
	_vals[1] = vec.GetX();
	_vals[2] = vec.GetY();
	_vals[3] = vec.GetZ();
	return *this;
}


//-----------------------------------------------------------------------------
// Quaternion cast
//-----------------------------------------------------------------------------

inline Quatf::operator const GLfloat*() const
{
	return _vals;
}


//-----------------------------------------------------------------------------
// Set quaternion
//-----------------------------------------------------------------------------

inline void Quatf::Set(
	GLfloat a,
	GLfloat b,
	GLfloat c,
	GLfloat d)
{
	_vals[0] = a;
	_vals[1] = b;
	_vals[2] = c;
	_vals[3] = d;
}


inline void Quatf::Set(
	const GLfloat* vals)
{
	_vals[0] = vals[0];
	_vals[1] = vals[1];
	_vals[2] = vals[2];
	_vals[3] = vals[3];
}


//-----------------------------------------------------------------------------
// Direct set value in quaternion
//-----------------------------------------------------------------------------

inline void Quatf::SetW(
	GLfloat val)
{
	_vals[0] = val;
}


inline void Quatf::SetX(
	GLfloat val)
{
	_vals[1] = val;
}


inline void Quatf::SetY(
	GLfloat val)
{
	_vals[2] = val;
}


inline void Quatf::SetZ(
	GLfloat val)
{
	_vals[3] = val;
}


//-----------------------------------------------------------------------------
// Direct get value in quaternion
//-----------------------------------------------------------------------------

inline GLfloat Quatf::GetW() const
{
	return _vals[0];
}


inline GLfloat Quatf::GetX() const
{
	return _vals[1];
}


inline GLfloat Quatf::GetY() const
{
	return _vals[2];
}


inline GLfloat Quatf::GetZ() const
{
	return _vals[3];
}


//-----------------------------------------------------------------------------
// Direct get reference to member
//-----------------------------------------------------------------------------

inline GLfloat& Quatf::W()
{
	return _vals[0];
}


inline const GLfloat& Quatf::W() const
{
	return _vals[0];
}


inline GLfloat& Quatf::X()
{
	return _vals[1];
}


inline const GLfloat& Quatf::X() const
{
	return _vals[1];
}


inline GLfloat& Quatf::Y()
{
	return _vals[2];
}


inline const GLfloat& Quatf::Y() const
{
	return _vals[2];
}


inline GLfloat& Quatf::Z()
{
	return _vals[3];
}


inline const GLfloat& Quatf::Z() const
{
	return _vals[3];
}


//-----------------------------------------------------------------------------
// Indexed set and get
//-----------------------------------------------------------------------------

inline void Quatf::SetVal(
	GLOW_CSTD::ptrdiff_t a,
	GLfloat val)
{
	GLOW_DEBUG(a < 0 || a >= 4, "index out of bounds in Quatf::SetVal");
	_vals[a] = val;
}


inline GLfloat Quatf::GetVal(
	GLOW_CSTD::ptrdiff_t a) const
{
	GLOW_DEBUG(a < 0 || a >= 4, "index out of bounds in Quatf::GetVal");
	return _vals[a];
}


//-----------------------------------------------------------------------------
// Quaternion operator []
//-----------------------------------------------------------------------------

inline GLfloat& Quatf::operator[](
	GLOW_CSTD::ptrdiff_t i)
{
	GLOW_DEBUG(i < 0 || i >= 4, "index out of bounds in Quatf::operator[]");
	return _vals[i];
}

inline GLfloat Quatf::operator[](
	GLOW_CSTD::ptrdiff_t i) const
{
	GLOW_DEBUG(i < 0 || i >= 4, "index out of bounds in Quatf::operator[]");
	return _vals[i];
}


//-----------------------------------------------------------------------------
// Create a rotation quaternion
//-----------------------------------------------------------------------------

inline void Quatf::SetRotation(
	GLfloat axisX,
	GLfloat axisY,
	GLfloat axisZ,
	GLfloat angle)
{
	GLfloat fac = GLOW_CSTD::sin(angle*0.5f)/
		GLOW_CSTD::sqrt(axisX*axisX+axisY*axisY+axisZ*axisZ);
	_vals[0] = GLOW_CSTD::cos(angle*0.5f);
	_vals[1] = fac*axisX;
	_vals[2] = fac*axisY;
	_vals[3] = fac*axisZ;
}


inline void Quatf::SetRotation(
	const Vec3f& axis,
	GLfloat angle)
{
	GLfloat fac = GLOW_CSTD::sin(angle*0.5f)/axis.Norm();
	_vals[0] = GLOW_CSTD::cos(angle*0.5f);
	_vals[1] = fac*axis.GetX();
	_vals[2] = fac*axis.GetY();
	_vals[3] = fac*axis.GetZ();
}


//-----------------------------------------------------------------------------
// Get axis or angle from a rotation quaternion
//-----------------------------------------------------------------------------

inline GLfloat Quatf::GetAngle() const
{
	GLfloat val = _vals[0];
	if (val > 1.0)
	{
		val = 1.0;
	}
	else if (val < -1.0)
	{
		val = -1.0;
	}
	return GLfloat(2.0)*GLOW_CSTD::acos(val);
}


//-----------------------------------------------------------------------------
// Get axis or angle from a rotation quaternion
//-----------------------------------------------------------------------------

inline Vec3f Quatf::GetAxis() const
{
	Vec3f result(_vals[1], _vals[2], _vals[3]);
	if (!result.IsZero())
	{
		result.Normalize();
	}
	return result;
}


//-----------------------------------------------------------------------------
// Pure imaginary quaternion
//-----------------------------------------------------------------------------

inline void Quatf::SetImaginary(
	GLfloat vecX,
	GLfloat vecY,
	GLfloat vecZ)
{
	_vals[0] = 0.0f;
	_vals[1] = vecX;
	_vals[2] = vecY;
	_vals[3] = vecZ;
}


inline void Quatf::SetImaginary(
	const Vec3f& vec)
{
	_vals[0] = 0.0f;
	_vals[1] = vec.GetX();
	_vals[2] = vec.GetY();
	_vals[3] = vec.GetZ();
}


inline const Vec3f Quatf::GetImaginary() const
{
	return Vec3f(_vals[1], _vals[2], _vals[3]);
}


//-----------------------------------------------------------------------------
// Identity and zero quaternions
//-----------------------------------------------------------------------------

inline void Quatf::SetIdentity()
{
	_vals[0] = GLfloat(1);
	_vals[1] = GLfloat(0);
	_vals[2] = GLfloat(0);
	_vals[3] = GLfloat(0);
}


inline void Quatf::SetZero()
{
	_vals[0] = GLfloat(0);
	_vals[1] = GLfloat(0);
	_vals[2] = GLfloat(0);
	_vals[3] = GLfloat(0);
}


inline bool Quatf::IsZero() const
{
	return (_vals[0]==GLfloat(0) && _vals[1]==GLfloat(0) &&
		_vals[2]==GLfloat(0) && _vals[3]==GLfloat(0));
}


//-----------------------------------------------------------------------------
// Norms
//-----------------------------------------------------------------------------

inline GLfloat Quatf::Norm() const
{
	return GLOW_CSTD::sqrt(_vals[0]*_vals[0]+_vals[1]*_vals[1]+_vals[2]*_vals[2]+_vals[3]*_vals[3]);
}


inline void Quatf::Normalize()
{
	GLfloat n = Norm();
	_vals[0] /= n;
	_vals[1] /= n;
	_vals[2] /= n;
	_vals[3] /= n;
}


inline const Quatf Quatf::Normalized() const
{
	GLfloat n = Norm();
	return Quatf(_vals[0]/n, _vals[1]/n, _vals[2]/n, _vals[3]/n);
}


//-----------------------------------------------------------------------------
// Conjugate and negation
//-----------------------------------------------------------------------------

inline const Quatf Quatf::Conjugate() const
{
	return Quatf(_vals[0], -_vals[1], -_vals[2], -_vals[3]);
}


inline const Quatf Quatf::operator-() const
{
	return Quatf(-_vals[0], -_vals[1], -_vals[2], -_vals[3]);
}


//-----------------------------------------------------------------------------
// Quatf binary operators with scalar
//-----------------------------------------------------------------------------

inline const Quatf Quatf::operator*(
	GLfloat op2) const
{
	return Quatf(_vals[0]*op2, _vals[1]*op2, _vals[2]*op2, _vals[3]*op2);
}


inline const Quatf operator*(
	GLfloat op1,
	const Quatf& op2)
{
	return Quatf(op1*op2.GetW(), op1*op2.GetX(), op1*op2.GetY(), op1*op2.GetZ());
}


inline const Quatf Quatf::operator/(
	GLfloat op2) const
{
	return Quatf(_vals[0]/op2, _vals[1]/op2, _vals[2]/op2, _vals[3]/op2);
}


inline Quatf& Quatf::operator*=(
	GLfloat op2)
{
	_vals[0] *= op2;
	_vals[1] *= op2;
	_vals[2] *= op2;
	_vals[3] *= op2;
	
	return *this;
}


inline Quatf& Quatf::operator/=(
	GLfloat op2)
{
	_vals[0] /= op2;
	_vals[1] /= op2;
	_vals[2] /= op2;
	_vals[3] /= op2;
	
	return *this;
}


//-----------------------------------------------------------------------------
// Binary operations
//-----------------------------------------------------------------------------

inline const Quatf Quatf::operator+(
	const Quatf& op2) const
{
	return Quatf(
		_vals[0]+op2._vals[0], _vals[1]+op2._vals[1],
		_vals[2]+op2._vals[2], _vals[3]+op2._vals[3]);
}


inline Quatf& Quatf::operator+=(
	const Quatf& op2)
{
	_vals[0] += op2._vals[0];
	_vals[1] += op2._vals[1];
	_vals[2] += op2._vals[2];
	_vals[3] += op2._vals[3];
	return *this;
}


inline const Quatf Quatf::operator-(
	const Quatf& op2) const
{
	return Quatf(
		_vals[0]-op2._vals[0], _vals[1]-op2._vals[1],
		_vals[2]-op2._vals[2], _vals[3]-op2._vals[3]);
}


inline Quatf& Quatf::operator-=(
	const Quatf& op2)
{
	_vals[0] -= op2._vals[0];
	_vals[1] -= op2._vals[1];
	_vals[2] -= op2._vals[2];
	_vals[3] -= op2._vals[3];
	return *this;
}


inline GLfloat Quatf::operator*(
	const Quatf& op2) const
{
	return _vals[0]*op2._vals[0] + _vals[0]*op2._vals[1] +
		_vals[2]*op2._vals[2] + _vals[3]*op2._vals[3];
}


/*
===============================================================================
*/


GLOW_NAMESPACE_END

