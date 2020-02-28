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

// ====================================================================
// ====================================================================