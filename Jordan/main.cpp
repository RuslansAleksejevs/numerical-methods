#include "solver.h"
#include <fstream>
#include <iostream>

double f_set(size_t i, size_t j, size_t N)
{
    return max(i,j)+1+N-N;
}

int main()
{
    ifstream f;
    size_t N; //размер матрицы
    cout<<"Size of the matrix:"<<endl;
    cin>>N;

    Matrix A(N,N); //матрица системы
    // заполняем матрицу систему:
    int ind;
    cout<<endl<<"1 - from file, 2 - generate with function"<<endl;
    cin>>ind;
    cout<<endl<<endl;
    if(ind==1)
    {
        f.open("C:\\Users\\Administrator.EliteBook-8760w\\Desktop\\my docs\\qt\\5 SEM\\Jordan\\input.txt");
        if(!f)
        {
            cout<<"File cannot be opened!";
            return 0;
        }
        else
        {
            for(size_t i=0; i<N; i++)
            {
                for(size_t j=0; j<N; j++)
                {
                    f>>A.mat[i][j];
                }
            }
        }
        f.close();
    }
    else
    {
        for(size_t i=0; i<N; i++)
        {
            for(size_t j=0; j<N; j++)
            {
                A.mat[i][j]=f_set(i,j,N);
            }
        }
    }

    A.Print();
    cout<<endl<<endl;
    //


    //А здесь мы считаем время
    //struct timeval	tv1, tv2, tv_diff;
    //gettimeofday(&tv1, NULL);

    Matrix B(A); // Она нам нужна просто, чтобы в конце проверить, что мы рили получили обратную
    Matrix E(N,N); //пока единичная, в конце станет A^(-1). Делаем это, чтобы не выделять память в подпрограмме
    for(size_t i=0; i<N; i++)
    {
        E.mat[i][i]=1;
    }

    Solve(A,E);

    //gettimeofday(&tv2, NULL);
    //timersub(&tv2, &tv1, &tv_diff);
    //cout<<endl<<endl<<"time:"<<endl;
   // printf("Time: %ld.%06ld\n", tv_diff.tv_sec, tv_diff.tv_usec);

    cout<<endl<<endl<<"Here comes the inverse:"<<endl;
    E.Print();
    cout<<endl<<endl<<"A*inv:"<<endl;
    (B*E).Print();
    cout<<endl<<endl<<"inv*A:"<<endl;
    (E*B).Print();
    cout<<endl<<endl;
    cout<<scientific<<(B*E).How_Close()<<endl<<endl;

    return 0;
}
