#pragma once

#ifndef __MATRIX_H__
#define __MATRIX_H__
#include<istream>
#include<vector>

class Matrix
{
private:
	int rowsNum, colsNum;
	std::vector<std::vector<float>> p;
	void initialize(float value);
public:
	Matrix(int rows, int cols);
	Matrix(int rows, int cols, float value);
	~Matrix();
	Matrix& operator = (const Matrix &m);
	Matrix& operator += (const Matrix &m);
	Matrix& operator -= (const Matrix &m);
	Matrix& operator *= (const Matrix &m);
	Matrix operator * (const Matrix &m) const;
	void Show() const;
	void swapRows(int a, int b);
	float Point(int i, int j) const;
	static Matrix inv(Matrix);
	int row() const;
	int col() const;
	static Matrix T(const Matrix &m);
	friend std::istream& operator>>(std::istream &is, Matrix &m);
};

#endif