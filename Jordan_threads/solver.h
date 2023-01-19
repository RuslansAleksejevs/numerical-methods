#ifndef SOLVER_H
#define SOLVER_H

#endif // SOLVER_H

// https://math.stackexchange.com/questions/2595425/is-it-allowed-to-swap-2-columns-in-gaussian-elimination
// https://learnc.info/c/pthreads_create_and_join.html топовое обьяснение

#define EPS 0.000000000000001
#include "matrix.h"
#include "synchronize.h"

/*// версия 1
struct pthread_arg
{
    size_t	p_current; //номер этого потока
    size_t p_total; // кол-во потоков
    size_t k; // номер шага алгоритма жордана (=номер столбца, который мы от всех отнимаем)
    Matrix *A; // поток будет преобразовывать эту матрицу
    Matrix *E; // и эту матрицу. Указатели, потому что мои потоки должны именно менять матрицу!!!
};

void *thread_program(void *arg) // то, что делает поток
{
    struct pthread_arg	*parg=(struct pthread_arg*)arg; // приводим аргумент к нужному типу
    size_t id=parg->p_current;
    size_t n=parg->A->n;
    size_t total=parg->p_total;
    size_t k=parg->k;


    for(size_t i=id*n/total; i<(id+1)*n/total; i++)
    {
        if(i!=k)
        {
            parg->E->SubtractColumns(i,k,parg->A->mat[k][i]); parg->A->SubtractColumns(i,k,parg->A->mat[k][i]);
        }
    }

    return 0; // хз, зачем вообще что-то возвращать
}


int Solve(Matrix &A, Matrix &E, size_t &amount)
{
    size_t n=A.m;

    pthread_t threads[amount];
    pthread_arg args[amount];
    int status;
    int status_addr;

    for(size_t i=0; i<amount; i++)
    {
        args[i].p_current=i;
        args[i].p_total=amount;
        args[i].A=&A;
        args[i].E=&E;
    }

    // ГЛАВНЫЙ ПО СТРОКЕ
    size_t ind;
    for(size_t k=0; k<n; k++)
    {
        ind=A.MainElement_Row(k,k);
        E.SwapColumns(k,ind); A.SwapColumns(k,ind);
        if(fabs(A.mat[k][k])<EPS)
        {
            cout<<endl<<endl<<"DIVING BY 0!!!!!!!!!! STOOOOOP!!!!"<<endl<<endl;
            return 0;
        }
        E.MultColumn(k,1/A.mat[k][k]); A.MultColumn(k,1/A.mat[k][k]);
        for(size_t i=0; i<amount; i++)
        {
            args[i].k=k;
            status=pthread_create(&threads[i],NULL,thread_program,(void*)&args[i]);
        }
        // потоки работают в разных местах, непересекающихся друг с другом. Осталось подождать, пока все не завершат работу, а дальше можно продолжить менять матрицу
        for(size_t i=0; i<amount; i++)
        {
            status=pthread_join(threads[i],(void**)&status_addr);
        } //чем этот цикл отличается от synchronize(amount)? В обоих случаях мы ждем, пока все потоки не закончат работу. Я понял так, что
        //синхронайз ждет пока все потоки не дойдут до этого места, а джойн ждет пока все потоки не сделают все, что у них было  планах. Это верно?
    } // A стала единичной, E стала A^(-1)
    return 0;
}
*/




// Версия 2
struct pthread_arg
{
    size_t	p_current; //номер этого потока
    size_t p_total; // кол-во потоков
    Matrix *A; // поток будет преобразовывать эту матрицу
    Matrix *E; // и эту матрицу. Указатели, потому что мои потоки должны именно менять матрицу!!!
};

void *thread_program(void *arg) // то, что делает поток
{
    struct pthread_arg	*parg=(struct pthread_arg*)arg; // приводим аргумент к нужному типу
    size_t id=parg->p_current;
    size_t n=parg->A->n;
    size_t total=parg->p_total;

    size_t ind;
    for(size_t k=0; k<n; k++)
    {
        if(parg->p_current==0) // начальfные действия пусть выполняет 0-ой поток
        {
            ind=parg->A->MainElement_Row(k,k);
            parg->E->SwapColumns(k,ind); parg->A->SwapColumns(k,ind);
            if(fabs(parg->A->mat[k][k])<EPS)
            {
                cout<<endl<<endl<<"DIVING BY 0!!!!!!!!!! STOOOOOP!!!!"<<endl<<endl;
                return 0;
            }
            parg->E->MultColumn(k,1/parg->A->mat[k][k]); parg->A->MultColumn(k,1/parg->A->mat[k][k]);
        }
        synchronize(parg->p_total);
        for(size_t i=id*n/total; i<(id+1)*n/total; i++)
        {
            if(i!=k)
            {
                parg->E->SubtractColumns(i,k,parg->A->mat[k][i]); parg->A->SubtractColumns(i,k,parg->A->mat[k][i]);
            }
        }
        synchronize(parg->p_total); // Все сделали свое дело на этой шаге, можно продолжить менять матрицу
    }

    return 0; // хз, зачем вообще что-то возвращать
}


int Solve(Matrix &A, Matrix &E, size_t &amount)
{
    pthread_t *threads; //pthread_t threads[amount];
    struct pthread_arg *args;
    int status;
    int status_addr;

    threads=(pthread_t*)malloc(sizeof(pthread_t) * amount);
    args=(struct pthread_arg*)malloc(sizeof(struct pthread_arg) * amount);

    for(size_t i=0; i<amount; i++)
    {
        args[i].p_current=i;
        args[i].p_total=amount;
        args[i].A=&A;
        args[i].E=&E;
    }

    for(size_t i=0; i<amount; i++)
    {
        status=pthread_create(&threads[i],NULL,thread_program,(void*)&args[i]);
        if(status!=0) cout<<endl<<endl<<"ERROR with threads!"<<endl;
    }

    for(size_t i=0; i<amount; i++) // Этот цикл вроде необязателен, т.к. у нас работа тредов кончается synchronize'ом
    {
        status=pthread_join(threads[i],(void**)&status_addr);
        if(status!=0) cout<<endl<<endl<<"ERROR with threads!"<<endl;
    }

    free(threads);
    free(args);
    return 0;
}


