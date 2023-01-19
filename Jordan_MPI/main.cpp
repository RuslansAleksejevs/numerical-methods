#include "solver.h"
#include <fstream>
#include <sys/time.h>
#include <mpi.h>

double f_set(size_t i, size_t j, size_t N)
{
    return max(i,j)+1+N-N;
}

int main(int argc, char **argv)
{
    int rank, size, ind;
    ifstream f;
    size_t N; //размер матрицы
    double start_time, end_time;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if(rank==0) // ввод/вывод и прочую мишуру проведем через 0-ой процесс
    {
        cout<<"Size of the matrix:"<<endl;
        cin>>N;
    }
    MPI_Bcast(&N, 1, my_MPI_SIZE_T, 0, MPI_COMM_WORLD);

    Matrix A(N,N);

    if(rank==0)
    {
        // заполняем матрицу системы:
        cout<<endl<<"1 - from file, 2 - generate with function"<<endl;
        cin>>ind;
        cout<<endl<<endl;
        if(ind==1)
        {
            f.open("input.txt");
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
                        f>>A.mat[j][i];
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
                    A.mat[j][i]=f_set(i,j,N);
                }
            }
        }
        cout<<"Original matrix:"<<endl;
        A.Print();
        cout<<endl<<endl;
     }
    for(size_t i=0; i<N; i++)
        MPI_Bcast(&A.mat[i][0], N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

        Matrix B(A); // Она нам нужна просто, чтобы в конце посчитать невязку (т.е. убедиться, что то, что нашли, есть обратная матрица)
        Matrix E(N,N); //пока будет единичной, в конце станет A^(-1).
        if(rank==0)
        {
            for(size_t i=0; i<N; i++)
            {
                E.mat[i][i]=1;
            }
        }
        for(size_t i=0; i<N; i++)
            MPI_Bcast(&E.mat[i][0], N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // счет времени и выполнение алгоритма
    double time1, time2;
    start_time=MPI_Wtime();
    Solve(A,E,rank,size);
    end_time=MPI_Wtime();
    MPI_Reduce(&start_time, &time1, 1, MPI_DOUBLE, MPI_MIN, 0, MPI_COMM_WORLD);
    MPI_Reduce(&end_time, &time2, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    if(rank==0)
    {
        cout<<"Time of execution: "<<time2-time1<<endl<<endl;
        cout<<endl<<endl<<"Inverse matrix:"<<endl;
        E.Print();
        cout<<endl<<endl;
        cout<<scientific<<"Precision (невязка): "<<(B*E).How_Close()<<endl<<endl;
    }

    MPI_Finalize();
    return 0;
}

















