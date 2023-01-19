#ifndef MATRIX_H
#define MATRIX_H

#endif // MATRIX_H


#define EPS 0.0000001
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

    void Print() const;
    double Norm() const; // вообще, нужна только для векторов, но пусть сделаем методом
    double How_Close() const; // считает невязку
    void SwapColumns(size_t i, size_t j);
    void SubtractColumns(size_t i, size_t j, double x);
    void MultColumn(size_t i, double x);
    //size_t MainElement_Row(size_t i, size_t t); // t - с какого места в строке искать главный
};

Matrix operator*(const Matrix &M1, const Matrix &M2);

Matrix::Matrix(size_t a, size_t b)
{
    m=a;
    n=b;
    mat=vector<vector<double>>(n,vector<double>(m)); // на первом месте номер столбца. Делаю это осознанно, чтобы легче через MPI_Bcast() передавать сразу столбец целиком
}


Matrix::Matrix(vector<vector<double>> const &arr) // отсюда должен автоматом перестраиваться оператор присваивания для нашей структуры
{
    n=arr.size();
    m=arr[0].size();
    mat=arr;
}


Matrix::Matrix()
{
    m=0;
    n=0;
}




double Matrix::Norm() const // вообще, пригодится только для векторов, но хохмы ради определим для любой матрицы
{
    double sum=0;
    for(size_t i=0; i<m; i++)
    {
        for(size_t j=0; j<n; j++)
        {
            sum=sum+mat[j][i]*mat[j][i];
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
    for(size_t i=0; i<min(m,5); i++)        // минимум берем, чтобы на экране не поместилось
    {
        for(size_t j=0; j<min(n,5); j++)
        {
            cout<<fixed<<mat[j][i]<<" ";
        }
        cout<<endl;
    }
}

void Matrix::SwapColumns(size_t i, size_t j)
{
    vector<double> temp(m);
    for(size_t k=0; k<m; k++)
    {
        temp[k]=mat[i][k];
    }
    for(size_t k=0; k<m; k++)
    {
        mat[i][k]=mat[j][k];
    }
    for(size_t k=0; k<m; k++)
    {
        mat[j][k]=temp[k];
    }
}

void Matrix::SubtractColumns(size_t i, size_t j, double x)
{
    for(size_t k=0; k<m; k++)
    {
        mat[i][k]=mat[i][k]-x*mat[j][k];
    }
}

void Matrix::MultColumn(size_t i, double x)
{
    for(size_t k=0; k<n; k++)
    {
        mat[i][k]=x*mat[i][k];
    }
}

/*size_t Matrix::MainElement_Row(size_t i,  size_t t)
{
    size_t ind_max=t;
    for(size_t s=t+1; s<n; s++)
    {
        if(fabs(mat[s][i])>fabs(mat[ind_max][i])) ind_max=s;
    }
    return ind_max;
}*/

Matrix operator*(const Matrix &M1, const Matrix &M2)
{
    Matrix res(M1.m, M2.n);
    for(size_t i=0; i<M1.m; i++)
    {
        for(size_t j=0; j<M2.n; j++)
        {
            for(size_t k=0; k<M1.n; k++)
            {
            res.mat[j][i]=res.mat[j][i]+M1.mat[k][i]*M2.mat[j][k];;
            }
        }
    }
    return res;
}
