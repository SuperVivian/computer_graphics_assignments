#ifndef MATRIX_H_
#define MATRIX_H_

//
// originally implemented by Justin Legakis
//

#include <math.h>
#include <assert.h>

#include "vectors.h"

// ====================================================================
// ====================================================================

class Matrix {

public:

	// CONSTRUCTORS & DESTRUCTOR
	Matrix () { Clear (); }
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

private:

	// REPRESENTATION
	float	data[4][4];

};

// ====================================================================
// ====================================================================

#endif

