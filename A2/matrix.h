#ifndef MATRIX_H_
#define MATRIX_H_

//
// originally implemented by Justin Legakis
//

#include <math.h>
#include <assert.h>

#include "vectors.h"
#include"Matrix3f.h"
// ====================================================================
// ====================================================================

class Matrix {

public:

	// CONSTRUCTORS & DESTRUCTOR
	Matrix () { Clear (); }
	Matrix (float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23,
		float m30, float m31, float m32, float m33);
	Matrix (const Matrix& m) {
		for (int y = 0; y < 4; y++) {
			for (int x = 0; x < 4; x++) {
				data[y][x] = m.data[y][x];
			}
		}
	}
	~Matrix () {}


	

	// ACCESSOR
	float Get (int x, int y) const {
		assert (x >= 0 && x < 4);
		assert (y >= 0 && y < 4);
		return data[y][x];
	}

	// MODIFIERS
	void Set (int x, int y, float v) {
		assert (x >= 0 && x < 4);
		assert (y >= 0 && y < 4);
		data[y][x] = v;
	}
	void SetToIdentity () {
		for (int y = 0; y < 4; y++)
			for (int x = 0; x < 4; x++)
				data[y][x] = (x == y);
	}
	void Clear () {
		for (int y = 0; y < 4; y++)
			for (int x = 0; x < 4; x++)
				data[y][x] = 0;
	}
	Matrix inverse (bool* pbIsSingular = NULL, float epsilon = 0.f) const;//Äæ¾ØÕó
	void transpose ();//×ªÖÃ¾ØÕó
	Matrix transposed ();//×ªÖÃºóµÄ¾ØÕó
	// OVERLOADED OPERATORS
	Matrix& operator=(const Matrix& m) {
		for (int y = 0; y<4; y++) {
			for (int x = 0; x<4; x++) {
				data[y][x] = m.data[y][x];
			}
		}
		return (*this);
	}
	int operator==(const Matrix& m) const {
		for (int y = 0; y<4; y++) {
			for (int x = 0; x<4; x++) {
				if (this->data[y][x] != m.data[y][x]) {
					return 0;
				}
			}
		}
		return 1;
	}
	int operator!=(const Matrix &m) const { return !(*this == m); }

	friend Matrix operator+(const Matrix &m1, const Matrix &m2);
	friend Matrix operator-(const Matrix &m1, const Matrix &m2);
	friend Matrix operator*(const Matrix &m1, const Matrix &m2);
	friend Matrix operator*(const Matrix &m1, float f);
	friend Matrix operator*(float f, const Matrix &m) { return m * f; }
	float& operator () (int i, int j);
	Matrix& operator+=(const Matrix& m) {
		*this = *this + m;
		return *this;
	}
	Matrix& operator-=(const Matrix& m) {
		*this = *this - m;
		return *this;
	}
	Matrix& operator*=(const float d) {
		*this = *this * d;
		return *this;
	}
	Matrix& operator*=(const Matrix& m) {
		*this = *this * m;
		return *this;
	}
	Vec4f operator*(const Vec4f v) {
		Vec4f v4;
		float x = this->Get (0, 0)*v.x () + this->Get (0, 1)*v.y () + this->Get (0, 2)*v.z () + this->Get (0, 3)*v.w ();
		float y = this->Get (1, 0)*v.x () + this->Get (1, 1)*v.y () + this->Get (1, 2)*v.z () + this->Get (1, 3)*v.w ();
		float z = this->Get (2, 0)*v.x () + this->Get (2, 1)*v.y () + this->Get (2, 2)*v.z () + this->Get (2, 3)*v.w ();
		float w = this->Get (3, 0)*v.x () + this->Get (3, 1)*v.y () + this->Get (3, 2)*v.z () + this->Get (3, 3)*v.w ();
		v4 = Vec4f (x,y,z,w);
		return v4;
	}
	// TRANSFORMATIONS
	void Translate (float x, float y, float z);
	void Scale (float x, float y, float z);
	void Scale (float s) { Scale (s, s, s); }
	void XRotate (float theta);
	void YRotate (float theta);
	void ZRotate (float theta);
	void Transform (Vec4f &v);
	void Transform (Vec3f &v) {
		Vec4f v2 = Vec4f (v.x (), v.y (), v.z (), 1);
		Transform (v2);
		v.Set (v2.x (), v2.y (), v2.z ());
	}
	void Transform (Vec2f &v) {
		Vec4f v2 = Vec4f (v.x (), v.y (), 1, 1);
		Transform (v2);
		v.Set (v2.x (), v2.y ());
	}

	// INPUT / OUTPUT
	void Write (FILE *F = stdout) const {
		assert (F != NULL);
		for (int y = 0; y < 4; y++) {
			for (int x = 0; x < 4; x++) {
				fprintf (F, "%f ", data[y][x]);
			}
			fprintf (F, "\n");
		}
	}
	void Write3x3 (FILE *F = stdout) const {
		assert (F != NULL);
		for (int y = 0; y < 4; y++) {
			if (y == 2) continue;
			for (int x = 0; x < 4; x++) {
				if (x == 2) continue;
				fprintf (F, "%f ", data[y][x]);
			}
			fprintf (F, "\n");
		}
	}
	void Read (FILE *F) {
		assert (F != NULL);
		for (int y = 0; y < 4; y++) {
			for (int x = 0; x < 4; x++) {
				int scanned = fscanf_s (F, "%f", &data[y][x]);
				assert (scanned == 1);
			}
		}
	}
	void Read3x3 (FILE *F) {
		assert (F != NULL);
		Clear ();
		for (int y = 0; y < 4; y++) {
			if (y == 2) continue;
			for (int x = 0; x < 4; x++) {
				if (x == 2) continue;
				int scanned = fscanf_s (F, "%f", &data[y][x]);
				assert (scanned == 1);
			}
		}
	}
	//MAKE
	static Matrix MakeScale (Vec3f v);
	static Matrix MakeTranslation (Vec3f v);
	static Matrix MakeXRotation (float radians);
	static Matrix MakeYRotation (float radians);
	static Matrix MakeZRotation (float radians);
	static Matrix MakeAxisRotation (Vec3f v, float radians);
	//DETERMINANT
	static float Determinant (const Vec3f& v1, const Vec3f &v2, const Vec3f &v3) {
		float d = v1.x() *v2.y()*v3.z()
					+ v2.x()*v3.y()*v1.z()
					+ v3.x()*v1.y()*v2.z()
					- v3.x()*v2.y()*v1.z()
					- v2.x()*v1.y()*v3.z()
					- v1.x()*v3.y()*v2.z();
		return d;
	}   
private:

	// REPRESENTATION
	float	data[4][4];
};

// ====================================================================
// ====================================================================

#endif

