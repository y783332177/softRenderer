#pragma once

#ifndef __MATRIX_H__
#define __MATRIX_H__
#include "VectorNT.h"

template<size_t ROW, size_t COL, typename T> struct Matrix {
	T m[ROW][COL];

	inline Matrix() {}

	inline Matrix(const Matrix<ROW, COL, T>& src) {
		for (size_t r = 0; r < ROW; r++) {
			for (size_t c = 0; c < COL; c++)
				m[r][c] = src.m[r][c];
		}
	}

	inline Matrix(const std::initializer_list<Vector<COL, T>>& u) {
		auto it = u.begin();
		for (size_t i = 0; i < ROW; i++) SetRow(i, *it++);
	}

	inline const T* operator [] (size_t row) const { assert(row < ROW); return m[row]; }
	inline T* operator [] (size_t row) { assert(row < ROW); return m[row]; }

	// 取一行
	inline Vector<COL, T> Row(size_t row) const {
		assert(row < ROW);
		Vector<COL, T> a;
		for (size_t i = 0; i < COL; i++) a[i] = m[row][i];
		return a;
	}

	// 取一列
	inline Vector<ROW, T> Col(size_t col) const {
		assert(col < COL);
		Vector<ROW, T> a;
		for (size_t i = 0; i < ROW; i++) a[i] = m[i][col];
		return a;
	}

	// 设置一行
	inline void SetRow(size_t row, const Vector<COL, T>& a) {
		assert(row < ROW);
		for (size_t i = 0; i < COL; i++) m[row][i] = a[i];
	}

	// 设置一列
	inline void SetCol(size_t col, const Vector<ROW, T>& a) {
		assert(col < COL);
		for (size_t i = 0; i < ROW; i++) m[i][col] = a[i];
	}

	// 取得删除某行和某列的子矩阵：子式
	inline Matrix<ROW - 1, COL - 1, T> GetMinor(size_t row, size_t col) const {
		Matrix<ROW - 1, COL - 1, T> ret;
		for (size_t r = 0; r < ROW - 1; r++) {
			for (size_t c = 0; c < COL - 1; c++) {
				ret.m[r][c] = m[r < row ? r : r + 1][c < col ? c : c + 1];
			}
		}
		return ret;
	}

	// 取得转置矩阵
	inline Matrix<COL, ROW, T> Transpose() const {
		Matrix<COL, ROW, T> ret;
		for (size_t r = 0; r < ROW; r++) {
			for (size_t c = 0; c < COL; c++)
				ret.m[c][r] = m[r][c];
		}
		return ret;
	}

	// 取得 0 矩阵
	inline static Matrix<ROW, COL, T> GetZero() {
		Matrix<ROW, COL, T> ret;
		for (size_t r = 0; r < ROW; r++) {
			for (size_t c = 0; c < COL; c++)
				ret.m[r][c] = 0;
		}
		return ret;
	}

	// 取得单位矩阵
	inline static Matrix<ROW, COL, T> GetIdentity() {
		Matrix<ROW, COL, T> ret;
		for (size_t r = 0; r < ROW; r++) {
			for (size_t c = 0; c < COL; c++)
				ret.m[r][c] = (r == c) ? 1 : 0;
		}
		return ret;
	}
};

/**************
数学库：矩阵运算
**************/
template<size_t ROW, size_t COL, typename T>
inline bool operator == (const Matrix<ROW, COL, T>& a, const Matrix<ROW, COL, T>& b) {
	for (size_t r = 0; r < ROW; r++) {
		for (size_t c = 0; c < COL; c++) {
			if (a.m[r][c] != b.m[r][c]) return false;
		}
	}
	return true;
}

template<size_t ROW, size_t COL, typename T>
inline bool operator != (const Matrix<ROW, COL, T>& a, const Matrix<ROW, COL, T>& b) {
	return !(a == b);
}

template<size_t ROW, size_t COL, typename T>
inline Matrix<ROW, COL, T> operator + (const Matrix<ROW, COL, T>& src) {
	return src;
}

template<size_t ROW, size_t COL, typename T>
inline Matrix<ROW, COL, T> operator - (const Matrix<ROW, COL, T>& src) {
	Matrix<ROW, COL, T> out;
	for (size_t j = 0; j < ROW; j++) {
		for (size_t i = 0; i < COL; i++)
			out.m[j][i] = -src.m[j][i];
	}
	return out;
}

template<size_t ROW, size_t COL, typename T>
inline Matrix<ROW, COL, T> operator + (const Matrix<ROW, COL, T>& a, const Matrix<ROW, COL, T>& b) {
	Matrix<ROW, COL, T> out;
	for (size_t j = 0; j < ROW; j++) {
		for (size_t i = 0; i < COL; i++)
			out.m[j][i] = a.m[j][i] + b.m[j][i];
	}
	return out;
}

template<size_t ROW, size_t COL, typename T>
inline Matrix<ROW, COL, T> operator - (const Matrix<ROW, COL, T>& a, const Matrix<ROW, COL, T>& b) {
	Matrix<ROW, COL, T> out;
	for (size_t j = 0; j < ROW; j++) {
		for (size_t i = 0; i < COL; i++)
			out.m[j][i] = a.m[j][i] - b.m[j][i];
	}
	return out;
}

template<size_t ROW, size_t COL, size_t NEWCOL, typename T>
inline Matrix<ROW, NEWCOL, T> operator * (const Matrix<ROW, COL, T>& a, const Matrix<COL, NEWCOL, T>& b) {
	Matrix<ROW, NEWCOL, T> out;
	for (size_t j = 0; j < ROW; j++) {
		for (size_t i = 0; i < NEWCOL; i++) {
			out.m[j][i] = vector_dot(a.Row(j), b.Col(i));
		}
	}
	return out;
}

template<size_t ROW, size_t COL, typename T>
inline Matrix<ROW, COL, T> operator * (const Matrix<ROW, COL, T>& a, T x) {
	Matrix<ROW, COL, T> out;
	for (size_t j = 0; j < ROW; j++) {
		for (size_t i = 0; i < COL; i++) {
			out.m[j][i] = a.m[j][i] * x;
		}
	}
	return out;
}

template<size_t ROW, size_t COL, typename T>
inline Matrix<ROW, COL, T> operator / (const Matrix<ROW, COL, T>& a, T x) {
	Matrix<ROW, COL, T> out;
	for (size_t j = 0; j < ROW; j++) {
		for (size_t i = 0; i < COL; i++) {
			out.m[j][i] = a.m[j][i] / x;
		}
	}
	return out;
}

template<size_t ROW, size_t COL, typename T>
inline Matrix<ROW, COL, T> operator * (T x, const Matrix<ROW, COL, T>& a) {
	return (a * x);
}

template<size_t ROW, size_t COL, typename T>
inline Matrix<ROW, COL, T> operator / (T x, const Matrix<ROW, COL, T>& a) {
	Matrix<ROW, COL, T> out;
	for (size_t j = 0; j < ROW; j++) {
		for (size_t i = 0; i < COL; i++) {
			out.m[j][i] = x / a.m[j][i];
		}
	}
	return out;
}

template<size_t ROW, size_t COL, typename T>
inline Vector<COL, T> operator * (const Vector<ROW, T>& a, const Matrix<ROW, COL, T>& m) {
	Vector<COL, T> b;
	for (size_t i = 0; i < COL; i++)
		b[i] = vector_dot(a, m.Col(i));
	return b;
}

template<size_t ROW, size_t COL, typename T>
inline Vector<ROW, T> operator * (const Matrix<ROW, COL, T>& m, const Vector<COL, T>& a) {
	Vector<ROW, T> b;
	for (size_t i = 0; i < ROW; i++)
		b[i] = vector_dot(a, m.Row(i));
	return b;
}

//---------------------------------------------------------------------
// 数学库：行列式和逆矩阵等，光照计算有用
//---------------------------------------------------------------------

// 行列式求值：一阶
template<typename T>
inline T matrix_det(const Matrix<1, 1, T>& m) {
	return m[0][0];
}

// 行列式求值：二阶
template<typename T>
inline T matrix_det(const Matrix<2, 2, T>& m) {
	return m[0][0] * m[1][1] - m[0][1] * m[1][0];
}

// 行列式求值：多阶行列式，即第一行同他们的余子式相乘求和
template<size_t N, typename T>
inline T matrix_det(const Matrix<N, N, T>& m) {
	T sum = 0;
	for (size_t i = 0; i < N; i++) sum += m[0][i] * matrix_cofactor(m, 0, i);
	return sum;
}

// 余子式：一阶
template<typename T>
inline T matrix_cofactor(const Matrix<1, 1, T>& m, size_t row, size_t col) {
	return 0;
}

// 多阶余子式：即删除特定行列的子式的行列式值
template<size_t N, typename T>
inline T matrix_cofactor(const Matrix<N, N, T>& m, size_t row, size_t col) {
	return matrix_det(m.GetMinor(row, col)) * (((row + col) % 2) ? -1 : 1);
}

// 伴随矩阵：即余子式矩阵的转置
template<size_t N, typename T>
inline Matrix<N, N, T> matrix_adjoint(const Matrix<N, N, T>& m) {
	Matrix<N, N, T> ret;
	for (size_t j = 0; j < N; j++) {
		for (size_t i = 0; i < N; i++) ret[j][i] = matrix_cofactor(m, i, j);
	}
	return ret;
}

// 求逆矩阵：使用伴随矩阵除以行列式的值得到
template<size_t N, typename T>
inline Matrix<N, N, T> matrix_invert(const Matrix<N, N, T>& m) {
	Matrix<N, N, T> ret = matrix_adjoint(m);
	T det = vector_dot(m.Row(0), ret.Col(0));
	return ret / det;
}

// 输出到文本流
template<size_t ROW, size_t COL, typename T>
inline std::ostream& operator << (std::ostream& os, const Matrix<ROW, COL, T>& m) {
	for (size_t r = 0; r < ROW; r++) {
		Vector<COL, T> row = m.Row(r);
		os << row << std::endl;
	}
	return os;
}

template<size_t N, typename T>
inline Vector<N, T> TransformCoordinate(Vector<N, T> coord, Matrix<N, N, T> transMat)
{
	Vector<N, T> point;
	point = transMat * coord;
	return point;
}


typedef Matrix<4, 4, float> Mat4x4f;
typedef Matrix<3, 3, float> Mat3x3f;
typedef Matrix<4, 3, float> Mat4x3f;
typedef Matrix<3, 4, float> Mat3x4f;


inline Mat4x4f LookAtRH(const Vector3f& cameraPos, const Vector3f& cameraTar, const Vector3f& upVec)
{
	//Vector3f cameraDir = vector_normalize(cameraTar - cameraPos);
	//Vector3f cameraRight = vector_normalize(vector_cross(cameraDir, upVec));
	//Vector3f cameraUp = vector_cross(cameraRight, cameraDir);

	Vector3f cameraDir = vector_normalize(cameraTar - cameraPos);
	Vector3f cameraRight = vector_normalize(vector_cross(upVec, cameraDir));
	Vector3f cameraUp = vector_cross(cameraDir, cameraRight);

	Mat4x4f lookAt;
	lookAt.SetRow(0, Vector4f(cameraRight.x, cameraRight.y, cameraRight.z, -vector_dot(cameraPos, cameraRight)));
	lookAt.SetRow(1, Vector4f(cameraUp.x, cameraUp.y, cameraUp.z, -vector_dot(cameraPos, cameraUp)));
	lookAt.SetRow(2, Vector4f(cameraDir.x, cameraDir.y, cameraDir.z, -vector_dot(cameraPos, cameraDir)));
	lookAt.SetRow(3, Vector4f(0.0f, 0.0f, 0.0f, 1.0f));

	return lookAt;
}

inline Mat4x4f PerspectiveFovRH(const float fovy, const float Aspect, const float zn, const float zf)
{
	Mat4x4f projectionMatrix;
	projectionMatrix.SetRow(0, Vector4f(1. / tanf(fovy / 2), 0, 0, 0));
	projectionMatrix.SetRow(1, Vector4f(0, Aspect / tanf(fovy / 2), 0, 0));
	projectionMatrix.SetRow(2, Vector4f(0, 0, (zf + zn) / (zn - zf), 2 * zf * zn / (zf - zn)));
	projectionMatrix.SetRow(3, Vector4f(0, 0, 1, 0));
	return projectionMatrix;
}

inline Mat4x4f Translation(const Vector3f& transVec)
{
	Mat4x4f translationMatrix;
	translationMatrix.SetRow(0, Vector4f(1, 0, 0, transVec.x));
	translationMatrix.SetRow(1, Vector4f(0, 1, 0, transVec.y));
	translationMatrix.SetRow(2, Vector4f(0, 0, 1, transVec.z));
	translationMatrix.SetRow(3, Vector4f(0, 0, 0, 1));

	return translationMatrix;
}

inline Mat4x4f RotationYaw(const float yaw) 
{
	Mat4x4f roYaw;
	roYaw.SetRow(0, Vector4f(cosf(yaw), 0, sinf(yaw), 0));
	roYaw.SetRow(1, Vector4f(0, 1, 0, 0));
	roYaw.SetRow(2, Vector4f(-sinf(yaw), 0, cosf(yaw), 0));
	roYaw.SetRow(3, Vector4f(0, 0, 0, 1));
	return roYaw;
}

inline Mat4x4f RotationPitch(const float pitch)
{
	Mat4x4f roPitch;
	roPitch.SetRow(0, Vector4f(1, 0, 0, 0));
	roPitch.SetRow(1, Vector4f(0, cosf(pitch), -sinf(pitch), 0));
	roPitch.SetRow(2, Vector4f(0, sinf(pitch), cosf(pitch), 0));
	roPitch.SetRow(3, Vector4f(0, 0, 0, 1));
	return roPitch;
}
inline Mat4x4f RotationRoll(const float roll)
{
	Mat4x4f roRoll;
	roRoll.SetRow(0, Vector4f(cosf(roll), -sinf(roll), 0, 0));
	roRoll.SetRow(1, Vector4f(sinf(roll), cosf(roll), 0, 0));
	roRoll.SetRow(2, Vector4f(0, 0, 1, 0));
	roRoll.SetRow(3, Vector4f(0, 0, 0, 1));
	return roRoll;
}

inline Vector3f barycentric(const Vector3f& A, const Vector3f& B, const Vector3f& C, const Vector3f& P)
{
	Vector3f s[2];
	for (int i = 1; i >= 0; i--)
	{
		s[i][0] = C[i] - A[i];
		s[i][1] = B[i] - A[i];
		s[i][2] = A[i] - P[i];
	}
	Vector3f u = vector_cross(s[0], s[1]);
	if (u[2] > 0)
		return Vector3f(1.f - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z);
	return Vector3f(-1, 1, 1);
}

#endif