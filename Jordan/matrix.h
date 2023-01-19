#ifndef MATRIX_H
#define MATRIX_H

#endif // MATRIX_H



#define EPS 0.000001
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
using namespace std;

// оператор присваивания перегружается автоматически, когда я пишу конструктор копирования
struct Matrix{
    vector<vector<double>> mat;
    size_t m;
    size_t n;

    Matrix(size_t a, size_t b);
    Matrix(const vector<vector<double>> &mat);
    Matrix();

    Matrix Transpose() const;
    void Print() const;
    Matrix SubMatrix(size_t i1, size_t j1, size_t i2, size_t j2) const;
    double Norm() const; // вообще, нужна только для векторов, но пусть сделаем методом
    double How_Close() const; // считает невязку
    void SwapRows(size_t i, size_t j);
    void SwapColumns(size_t i, size_t j);
    void SubtractRows(size_t i, size_t j, double x); // из i-ой вычитаем j-ую, умноженную на x
    void SubtractColumns(size_t i, size_t j, double x);
    void MultRow(size_t i, double x); //умножаем строку на число
    void MultColumn(size_t i, double x);
    size_t MainElement_Col(size_t i); //   выбираем в i-ом столбце главный элемент (=наибольший по модулю)
    size_t MainElement_Row(size_t i, size_t t); // t - с какого места в строке искать главный
};

Matrix operator+(const Matrix &M1, const Matrix &M2);
Matrix operator-(const Matrix &M1, const Matrix &M2);
Matrix operator*(const Matrix &M1, const Matrix &M2);
Matrix operator*(double x, const Matrix &M);
Matrix operator/(const Matrix &M, double x);



Matrix::Matrix(size_t a, size_t b)
{
    m=a;
    n=b;
    mat=vector<vector<double>>(m,vector<double>(n));
}


Matrix::Matrix(vector<vector<double>> const &arr) // отсюда должен автоматом перестраиваться оператор присваивания для нашей структуры
{
    m=arr.size();
    n=arr[0].size();
    mat=arr;
}


Matrix::Matrix(){
    m=0;
    n=0;
}


Matrix Matrix::Transpose() const
{
    Matrix res(n, m);
    for(size_t i=0; i<n; i++)
    {
        for (size_t j=0; j<m; j++)
        {
            res.mat[i][j]=mat[j][i];
        }
    }
    return res;
}


double Matrix::Norm() const // вообще, пригодится только для векторов, но хохмы ради определим для любой матрицы
{
    double sum=0;
    for(size_t i=0; i<m; i++)
    {
        for(size_t j=0; j<n; j++)
        {
            sum=sum+mat[i][j]*mat[i][j];
        }
    }
    return sqrt(sum);
}

double Matrix::How_Close() const
{
    Matrix temp(*this);
    for(size_t i=0; i<m; i++)
    {
        temp.mat[i][i]=temp.mat[i][i]-1;
    }

    return temp.Norm();
}

size_t min(size_t a, size_t b)
{
    if(a<b) return a;
    else return b;
}

void Matrix::Print() const
{
    for(size_t i=0; i<min(m,5); i++)        // минимум берем т.к. прогу тестиовать будет на 3-4 значных m и n, на экране не поместятся
    {
        for(size_t j=0; j<min(n,5); j++)
        {
            cout<<fixed<<mat[i][j]<<" ";
        }
        cout<<endl;
    }
}

Matrix Matrix::SubMatrix(size_t i1, size_t j1, size_t i2, size_t j2) const //из нашей матрицы берем подматрицу с левым верхним
{                                                            //концом ячейке (i1,j1) и правым нижним в (i2,j2). Нумерация с 0.
    size_t raws=i2-i1+1;
    size_t col=j2-j1+1;
    Matrix res(raws, col);
    for(size_t i=0; i<raws; i++)
    {
        for(size_t j=0; j<col; j++)
        {
            res.mat[i][j]=mat[i1+i][j1+j];
        }
    }
    return res;
}

void Matrix::SwapRows(size_t i, size_t j)
{
    vector<double> temp(n);
    for(size_t k=0; k<n; k++)
    {
        temp[k]=mat[i][k];
    }
    for(size_t k=0; k<n; k++)
    {
        mat[i][k]=mat[j][k];
    }
    for(size_t k=0; k<n; k++)
    {
        mat[j][k]=temp[k];
    }
}

void Matrix::SwapColumns(size_t i, size_t j)
{
    vector<double> temp(n);
    for(size_t k=0; k<n; k++)
    {
        temp[k]=mat[k][i];
    }
    for(size_t k=0; k<n; k++)
    {
        mat[k][i]=mat[k][j];
    }
    for(size_t k=0; k<n; k++)
    {
        mat[k][j]=temp[k];
    }
}

void Matrix::SubtractRows(size_t i, size_t j, double x)
{
    for(size_t k=0; k<n; k++)
    {
        mat[i][k]=mat[i][k]-x*mat[j][k];
    }
}

void Matrix::SubtractColumns(size_t i, size_t j, double x)
{
    for(size_t k=0; k<n; k++)
    {
        mat[k][i]=mat[k][i]-x*mat[k][j];
    }
}

void Matrix::MultRow(size_t i, double x)
{
    for(size_t k=0; k<n; k++)
    {
        mat[i][k]=x*mat[i][k];
    }
}

void Matrix::MultColumn(size_t i, double x)
{
    for(size_t k=0; k<n; k++)
    {
        mat[k][i]=x*mat[k][i];
    }
}

size_t Matrix::MainElement_Row(size_t i,  size_t t)
{
    size_t ind_max=t;
    for(size_t k=t+1; k<n; k++)
    {
        if(fabs(mat[i][k])>fabs(mat[i][ind_max])) ind_max=k;
    }
    return ind_max;
}

size_t Matrix::MainElement_Col(size_t i)
{
    size_t ind_max=0;
    for(size_t k=1; k<n; k++)
    {
        if(fabs(mat[k][i])>fabs(mat[ind_max][i])) ind_max=k;
    }
    return ind_max;
}

Matrix operator+(const Matrix &M1, const Matrix &M2)
{
    Matrix res(M1.m, M1.n);
    for(size_t i=0; i<M1.m; i++)
    {
        for(size_t j=0; j<M1.n; j++)
        {
            res.mat[i][j]=M1.mat[i][j]+M2.mat[i][j];
        }
    }
    return res;
}


Matrix operator-(const Matrix &M1, const Matrix &M2)
{
    Matrix res(M1.m, M1.n);
    for(size_t i=0; i<M1.m; i++)
    {
        for (size_t j=0; j<M1.n; j++)
        {
            res.mat[i][j]=M1.mat[i][j]-M2.mat[i][j];
        }
    }
    return res;
}

Matrix operator*(const Matrix &M1, const Matrix &M2)
{
    Matrix res(M1.m, M2.n);
    for(size_t i=0; i<M1.m; i++)
    {
        for(size_t j=0; j<M2.n; j++)
        {
            for(size_t k=0; k<M1.n; k++)
            {
            res.mat[i][j]=res.mat[i][j]+M1.mat[i][k]*M2.mat[k][j];;
            }
        }
    }
    return res;
}

Matrix operator*(double x, const Matrix &M)
{
    Matrix res(M.m, M.n);
    for(size_t i=0; i<M.m; i++)
    {
        for (size_t j=0; j<M.n; j++)
        {
            res.mat[i][j]=x*M.mat[i][j];
        }
    }
    return res;
}

Matrix operator/(const Matrix &M, double x)
{
    Matrix res(M.m, M.n);
    for(size_t i=0; i<M.m; i++)
    {
        for (size_t j=0; j<M.n; j++)
        {
            res.mat[i][j]=M.mat[i][j]/x;
        }
    }
    return res;
}
