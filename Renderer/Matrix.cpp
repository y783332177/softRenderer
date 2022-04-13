#include "Matrix.h"
#include<vector>
#include<iostream>
#include<istream>

void Matrix::initialize(float value)
{
	//初始化矩阵
	p = std::vector<std::vector<float>>(rowsNum, std::vector<float>(colsNum, 1.0f));
}

//声明一个全0矩阵
Matrix::Matrix(int rows, int cols) : rowsNum(rows), colsNum(cols)
{
	initialize(0.0f);
	for (int i = 0; i < rowsNum; i++)
	{
		for (int j = 0; j < colsNum; j++)
		{
			std::cout << p[i][j] << " ";
		}
		std::cout << "\n";
	}
}

Matrix::Matrix(int rows, int cols, float value) : rowsNum(rows), colsNum(cols)
{
	initialize(value);
}

Matrix::~Matrix() { }

Matrix& Matrix::operator = (const Matrix& m)
{
	if (this == &m) {
		return *this;
	}
	if (rowsNum != m.rowsNum || colsNum != m.colsNum)
	{
		rowsNum = m.rowsNum;
		colsNum = m.colsNum;
		initialize(0.0f);
	}
	for (int i = 0; i < rowsNum; i++)
	{
		for (int j = 0; j < colsNum; j++)
		{
			p[i][j] = m.p[i][j];
		}
	}
	return *this;
}

Matrix& Matrix::operator += (const Matrix& m)
{
	for (int i = 0; i < rowsNum; i++) {
		for (int j = 0; j < colsNum; j++) {
			p[i][j] += m.p[i][j];
		}
	}
	return *this;
}


Matrix& Matrix::operator -= (const Matrix& m)
{
	for (int i = 0; i < rowsNum; i++) {
		for (int j = 0; j < colsNum; j++) {
			p[i][j] -= m.p[i][j];
		}
	}
	return *this;
}


Matrix& Matrix::operator*=(const Matrix& m)
{
	//若C=AB,则矩阵C的行数等于矩阵A的行数，C的列数等于B的列数。
	Matrix temp(rowsNum, m.colsNum);
	for (int i = 0; i < temp.rowsNum; i++) {
		for (int j = 0; j < temp.colsNum; j++) {
			for (int k = 0; k < colsNum; k++) {
				temp.p[i][j] += (p[i][k] * m.p[k][j]);
			}
		}
	}
	*this = temp;
	return *this;
}


//实现矩阵的乘法
Matrix Matrix::operator*(const Matrix& m)const {
	Matrix ba_M(rowsNum, m.colsNum, 0.0f);
	for (int i = 0; i < rowsNum; i++) {
		for (int j = 0; j < m.colsNum; j++) {
			for (int k = 0; k < colsNum; k++) {
				ba_M.p[i][j] += (p[i][k] * m.p[k][j]);
			}
		}
	}
	return ba_M;
}


//矩阵显示
void Matrix::Show() const {
	//cout << rows_num <<" "<<cols_num<< endl;//显示矩阵的行数和列数
	for (int i = 0; i < rowsNum; i++) {
		for (int j = 0; j < colsNum; j++) {
			std::cout << p[i][j] << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}


//实现行变换
void Matrix::swapRows(int a, int b)
{
	a--;
	b--;
	std::vector<float>temp = p[a];
	p[a] = p[b];
	p[b] = temp;
}

// 返回矩阵第i行第j列的值
float Matrix::Point(int i, int j) const
{
	return this->p[i][j];
}

int Matrix::row() const 
{
	return rowsNum;
}

int Matrix::col() const
{
	return colsNum;
}

std::istream & operator >> (std::istream & is, Matrix & m)
{
	for (int i = 0; i < m.rowsNum; i++)
	{
		for (int j = 0; j < m.colsNum; j++)
		{
			is >> m.p[i][j];
		}
	}
	return is;
}