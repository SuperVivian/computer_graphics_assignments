//
// originally implemented by Justin Legakis
//
#include"stdafx.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "matrix.h"
#include "vectors.h"



Matrix::Matrix (float m00, float m01, float m02, float m03,
	float m10, float m11, float m12, float m13,
	float m20, float m21, float m22, float m23,
	float m30, float m31, float m32, float m33)
{
	data[0][0] = m00;	data[0][1] = m01;	data[0][2] = m02;	data[0][3] = m03;
	data[1][0] = m10;	data[1][1] = m11;	data[1][2] = m12;	data[1][3] = m13;
	data[2][0] = m20;	data[2][1] = m21;	data[2][2] = m22;	data[2][3] = m23;
	data[3][0] = m30;	data[3][1] = m31;	data[3][2] = m32;	data[3][3] = m33;
}
// ===================================================================
// ====================================================================
// OVERLOADED OPERATORS

Matrix operator+(const Matrix& m1, const Matrix& m2) {
	Matrix answer;
	for (int y = 0; y<4; y++) {
		for (int x = 0; x<4; x++) {
			answer.data[y][x] = m1.data[y][x] + m2.data[y][x];
		}
	}
	return answer;
}

Matrix operator-(const Matrix& m1, const Matrix& m2) {
	Matrix answer;
	for (int y = 0; y<4; y++) {
		for (int x = 0; x<4; x++) {
			answer.data[y][x] = m1.data[y][x] - m2.data[y][x];
		}
	}
	return answer;
}

Matrix operator*(const Matrix& m1, const Matrix& m2) {
	Matrix answer;
	for (int y = 0; y<4; y++) {
		for (int x = 0; x<4; x++) {
			for (int i = 0; i<4; i++) {
				answer.data[y][x]
					+= m1.data[y][i] * m2.data[i][x];
			}
		}
	}
	return answer;
}

Matrix operator*(const Matrix& m, float f) {
	Matrix answer;
	for (int y = 0; y<4; y++) {
		for (int x = 0; x<4; x++) {
			answer.data[y][x] = m.data[y][x] * f;
		}
	}
	return answer;
}
float& Matrix::operator () (int i, int j)
{
	return data[i][j];
}
// ====================================================================
// ====================================================================
// TRANSFORMATIONS

void Matrix::Translate (float t_x, float t_y, float t_z) {
	Matrix t;
	t.SetToIdentity ();
	t.data[0][3] = t_x;
	t.data[1][3] = t_y;
	t.data[2][3] = t_z;
	t *= *this;
	*this = t;
}

void Matrix::Scale (float s_x, float s_y, float s_z) {
	Matrix s;
	s.SetToIdentity ();
	s.data[0][0] = s_x;
	s.data[1][1] = s_y;
	s.data[2][2] = s_z;
	s.data[3][3] = 1;
	s *= *this;
	*this = s;
}

void Matrix::XRotate (float theta) {
	Matrix rx;
	rx.SetToIdentity ();
	rx.data[1][1] = (float)cos ((float)theta);
	rx.data[1][2] = -(float)sin ((float)theta);
	rx.data[2][1] = (float)sin ((float)theta);
	rx.data[2][2] = (float)cos ((float)theta);
	rx *= *this;
	*this = rx;
}

void Matrix::YRotate (float theta) {
	Matrix ry;
	ry.SetToIdentity ();
	ry.data[0][0] = (float)cos ((float)theta);
	ry.data[0][2] = (float)sin ((float)theta);
	ry.data[2][0] = -(float)sin ((float)theta);
	ry.data[2][2] = (float)cos ((float)theta);
	ry *= *this;
	*this = ry;
}

void Matrix::ZRotate (float theta) {
	Matrix rz;
	rz.SetToIdentity ();
	rz.data[0][0] = (float)cos ((float)theta);
	rz.data[0][1] = -(float)sin ((float)theta);
	rz.data[1][0] = (float)sin ((float)theta);
	rz.data[1][1] = (float)cos ((float)theta);
	rz *= *this;
	*this = rz;
}
//static
 Matrix Matrix::MakeScale (Vec3f v) {
	 return Matrix
	 (
		 v.x(), 0, 0, 0,
		 0, v.y(), 0, 0,
		 0, 0, v.z(), 0,
		 0, 0, 0, 1
	 );
}
 Matrix Matrix::MakeTranslation (Vec3f v) {
	 return Matrix
	 (
		 1, 0, 0, v.x (),
		 0, 1, 0, v.y (),
		 0, 0, 1, v.z (),
		 0, 0, 0, 1
	 );
}
 Matrix Matrix::MakeXRotation (float radians) {
	 float c = cos (radians);
	 float s = sin (radians);

	 return Matrix
	 (
		 1, 0, 0, 0,
		 0, c, -s, 0,
		 0, s, c, 0,
		 0, 0, 0, 1
	 );
}
 Matrix Matrix::MakeYRotation (float radians) {
	 float c = cos (radians);
	 float s = sin (radians);

	 return Matrix
	 (
		 c, 0, s, 0,
		 0, 1, 0, 0,
		 -s, 0, c, 0,
		 0, 0, 0, 1
	 );
}
 Matrix Matrix::MakeZRotation (float radians) {
	 float c = cos (radians);
	 float s = sin (radians);

	 return Matrix
	 (
		 c, -s, 0, 0,
		 s, c, 0, 0,
		 0, 0, 1, 0,
		 0, 0, 0, 1
	 );
}
 Matrix Matrix::MakeAxisRotation (Vec3f rDirection, float radians) {
	 Vec3f normalizedDirection = rDirection.Normalized ();

	 float cosTheta = cos (radians);
	 float sinTheta = sin (radians);

	 float x = normalizedDirection.x ();
	 float y = normalizedDirection.y ();
	 float z = normalizedDirection.z ();

	 return Matrix
	 (
		 x * x * (1.0f - cosTheta) + cosTheta, y * x * (1.0f - cosTheta) - z * sinTheta, z * x * (1.0f - cosTheta) + y * sinTheta, 0.0f,
		 x * y * (1.0f - cosTheta) + z * sinTheta, y * y * (1.0f - cosTheta) + cosTheta, z * y * (1.0f - cosTheta) - x * sinTheta, 0.0f,
		 x * z * (1.0f - cosTheta) - y * sinTheta, y * z * (1.0f - cosTheta) + x * sinTheta, z * z * (1.0f - cosTheta) + cosTheta, 0.0f,
		 0.0f, 0.0f, 0.0f, 1.0f
	 );
}
// ====================================================================
// ====================================================================

void Matrix::Transform (Vec4f &v) {
	Vec4f answer;
	for (int y = 0; y<4; y++) {
		answer.data[y] = 0;
		for (int i = 0; i<4; i++) {
			answer.data[y] += data[y][i] * v[i];
		}
	}
	v = answer;
}
void changeZero (float& f) {
	if (f == -0)f = 0;
}
Matrix Matrix::inverse (bool* pbIsSingular, float epsilon) const
{
	float m00 = data[0][0];
	float m10 = data[1][0];
	float m20 = data[2][0];
	float m30 = data[3][0];

	float m01 = data[0][1];
	float m11 = data[1][1];
	float m21 = data[2][1];
	float m31 = data[3][1];

	float m02 = data[0][2];
	float m12 = data[1][2];
	float m22 = data[2][2];
	float m32 = data[3][2];

	float m03 = data[0][3];
	float m13 = data[1][3];
	float m23 = data[2][3];
	float m33 = data[3][3];

	float cofactor00 = Matrix3f::determinant3x3 (m11, m12, m13, m21, m22, m23, m31, m32, m33);
	float cofactor01 = -Matrix3f::determinant3x3 (m12, m13, m10, m22, m23, m20, m32, m33, m30);
	float cofactor02 = Matrix3f::determinant3x3 (m13, m10, m11, m23, m20, m21, m33, m30, m31);
	float cofactor03 = -Matrix3f::determinant3x3 (m10, m11, m12, m20, m21, m22, m30, m31, m32);
	changeZero (cofactor00);
	changeZero (cofactor00);
	changeZero (cofactor00);
	changeZero (cofactor00);

	float cofactor10 = -Matrix3f::determinant3x3 (m21, m22, m23, m31, m32, m33, m01, m02, m03);
	float cofactor11 = Matrix3f::determinant3x3 (m22, m23, m20, m32, m33, m30, m02, m03, m00);
	float cofactor12 = -Matrix3f::determinant3x3 (m23, m20, m21, m33, m30, m31, m03, m00, m01);
	float cofactor13 = Matrix3f::determinant3x3 (m20, m21, m22, m30, m31, m32, m00, m01, m02);
	changeZero (cofactor10);
	changeZero (cofactor11);
	changeZero (cofactor12);
	changeZero (cofactor13);
	float cofactor20 = Matrix3f::determinant3x3 (m31, m32, m33, m01, m02, m03, m11, m12, m13);
	float cofactor21 = -Matrix3f::determinant3x3 (m32, m33, m30, m02, m03, m00, m12, m13, m10);
	float cofactor22 = Matrix3f::determinant3x3 (m33, m30, m31, m03, m00, m01, m13, m10, m11);
	float cofactor23 = -Matrix3f::determinant3x3 (m30, m31, m32, m00, m01, m02, m10, m11, m12);
	changeZero (cofactor20);
	changeZero (cofactor21);
	changeZero (cofactor22);
	changeZero (cofactor23);
	float cofactor30 = -Matrix3f::determinant3x3 (m01, m02, m03, m11, m12, m13, m21, m22, m23);
	float cofactor31 = Matrix3f::determinant3x3 (m02, m03, m00, m12, m13, m10, m22, m23, m20);
	float cofactor32 = -Matrix3f::determinant3x3 (m03, m00, m01, m13, m10, m11, m23, m20, m21);
	float cofactor33 = Matrix3f::determinant3x3 (m00, m01, m02, m10, m11, m12, m20, m21, m22);
	changeZero (cofactor30);
	changeZero (cofactor31);
	changeZero (cofactor32);
	changeZero (cofactor33);
	float determinant = m00 * cofactor00 + m01 * cofactor01 + m02 * cofactor02 + m03 * cofactor03;

	bool isSingular = (fabs (determinant) < epsilon);
	if (isSingular)
	{
		if (pbIsSingular != NULL)
		{
			*pbIsSingular = true;
		}
		return Matrix ();
	}
	else
	{
		if (pbIsSingular != NULL)
		{
			*pbIsSingular = false;
		}

		float reciprocalDeterminant = 1.0f / determinant;
		changeZero (reciprocalDeterminant);
		return Matrix
		(
			cofactor00 * reciprocalDeterminant, cofactor10 * reciprocalDeterminant, cofactor20 * reciprocalDeterminant, cofactor30 * reciprocalDeterminant,
			cofactor01 * reciprocalDeterminant, cofactor11 * reciprocalDeterminant, cofactor21 * reciprocalDeterminant, cofactor31 * reciprocalDeterminant,
			cofactor02 * reciprocalDeterminant, cofactor12 * reciprocalDeterminant, cofactor22 * reciprocalDeterminant, cofactor32 * reciprocalDeterminant,
			cofactor03 * reciprocalDeterminant, cofactor13 * reciprocalDeterminant, cofactor23 * reciprocalDeterminant, cofactor33 * reciprocalDeterminant
		);
	}
}

void Matrix::transpose ()
{
	float temp;

	for (int i = 0; i < 3; ++i)
	{
		for (int j = i + 1; j < 4; ++j)
		{
			temp = (*this)(i, j);
			(*this)(i, j) = (*this)(j, i);
			(*this)(j, i) = temp;
		}
	}
}
Matrix Matrix::transposed ()
{
	Matrix out;
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			out (j, i) = (*this)(i, j);
		}
	}

	return out;
}
// ====================================================================
// ====================================================================