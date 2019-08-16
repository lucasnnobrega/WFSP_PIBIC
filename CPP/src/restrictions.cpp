

#include "../include/restrictions.h"

void restriction_3(IloModel modelo,
                   IloIntVar P,
                   int n,
                   IloIntVarArray D,
                   int c[])
{
    // P >= D_i * c_i, para i = 1, ... , n
    for (int i = 0; i < n; i++)
    {
        modelo.add(P >= D[i] * c[i]);
    }
}

void restriction_4(IloModel modelo,
                   int n,
                   int M[],
                   IloIntVarArray D,
                   IloArray<IloIntVarArray> d)
{
    // Di ≥ di,k,k+1, ∀i = 1, . . . , n, ∀k ∈ Ki \ {Mi }
    for (int i = 0; i < n; i++)
    {
        for (int k = 0; k < M[i] - 1; k++)
        {
            modelo.add(D[i] >= d[i][k]);
        }
    }
}

void restriction_5(IloModel modelo,
                   int n,
                   int M[],
                   IloArray<IloArray<IloBoolVarArray>> y,
                   int ***H,
                   int TMAX)
{

    IloEnv env = modelo.getEnv();

    for (int h = 0; h < TMAX - 1; h++)
    {
        IloExpr soma(env);
        for (int i = 0; i < n; i++)
        {
            for (int k = 0; k < M[i]; k++)
            {
                printf("h = %d\n", h);
                printf("H[%d][%d][0]:  %d \n", i, k, H[i][k][0]);
                if (h >= H[i][k][0])
                {
                    soma += y[i][k][h - k];
                }
            }
        }
        modelo.add(soma <= 1);
    }
}

void restriction_6(IloModel modelo,
                   int n,
                   int M[],
                   IloArray<IloArray<IloBoolVarArray>> y,
                   int TMAX)
{
    IloEnv env = modelo.getEnv();

    for (int i = 0; i < n; i++)
    {
        for (int k = 0; k < M[i]; k++)
        {
            IloExpr soma(env);
            for (int h = 0; h < TMAX - k; h++)
            {
                soma += y[i][k][h];
            }
            modelo.add(soma <= 1);
        }
    }
}

void restriction_7(IloModel modelo,
                   int n,
                   int M[],
                   IloArray<IloArray<IloBoolVarArray>> y,
                   int TMAX)
{
    IloEnv env = modelo.getEnv();

    for (int i = 0; i < n; i++)
    {
        IloExpr soma(env);
        for (int k = 0; k < M[i]; k++)
        {
            for (int h = 0; h < TMAX - k; h++)
            {
                soma += y[i][k][h];
            }
        }
        modelo.add(soma >= 1);
    }
}

void restriction_8(IloModel modelo,
                   int n,
                   int M[],
                   IloArray<IloArray<IloBoolVarArray>> y,
                   int TMAX,
                   int ***H,
                   IloArray<IloIntVarArray> &p)
{
    IloEnv env = modelo.getEnv();

    for (int i = 0; i < n; i++)
    {
        for (int k = 0; k < M[i]; k++)
        {
            IloExpr soma(env);
            for (int h = 0; h < TMAX - k; h++)
            {
                soma += y[i][k][h] * H[i][k][h];
            }
            modelo.add(p[i][k] == soma);
        }
    }
}

void restriction_9(IloModel modelo,
                   int n,
                   int M[],
                   IloArray<IloArray<IloBoolVarArray>> y,
                   int TMAX)
{
    IloEnv env = modelo.getEnv();

    for (int i = 0; i < n; i++)
    {
        for (int k = 0; k < M[i] - 1; k++)
        {

            IloExpr soma_esq(env);
            IloExpr soma_dir(env);
            for (int h = 0; h < TMAX - k - 1; h++)
            {

                soma_esq += y[i][k][h];
                soma_dir += y[i][k + 1][h];
            }
            modelo.add(soma_esq >= soma_dir);
        }
    }
}

void restriction_10(IloModel modelo,
                    int n,
                    int M[],
                    IloArray<IloArray<IloBoolVarArray>> y,
                    int TMAX,
                    IloArray<IloIntVarArray> p)
{
    IloEnv env = modelo.getEnv();

    for (int i = 0; i < n; i++)
    {
        for (int k = 0; k < M[i] - 1; k++)
        {
            IloExpr soma(env);
            for (int h = 0; h < TMAX - k - 1; h++)
            {
                soma += y[i][k + 1][h];
            }
            modelo.add(p[i][k + 1] >= p[i][k] - (1 - soma) * TMAX);
        }
    }
}

void restriction_11(IloModel modelo,
                    int n,
                    int M[],
                    IloArray<IloArray<IloBoolVarArray>> y,
                    int TMAX,
                    IloArray<IloIntVarArray> p,
                    IloArray<IloIntVarArray> d)
{
    for (int i = 0; i < n; i++)
    {
        for (int k = 0; k < M[i] - 1; k++)
        {
            for (int h = 0; h < TMAX - k; h++)
            {
                modelo.add(d[i][k] >= p[i][k + 1] - p[i][k]);
            }
        }
    }
}

// RESTRICTIONS PART 2

void restriction_15(IloModel modelo,
                    int n,
                    IloIntVarArray D,
                    IloIntVarArray W)
{
    for (int i = 0; i < n; i++)
    {
        modelo.add(D[i] >= W[i]);
    }
}

void restriction_16(IloModel modelo,
                    int n,
                    IloIntVarArray W,
                    IloIntVarArray U,
                    IloIntVarArray R)
{
    for (int i = 0; i < n; i++)
    {
        modelo.add(W[i] >= U[i] - R[i]);
    }
}

void restriction_17(IloModel modelo,
                    int n,
                    int M[],
                    IloArray<IloArray<IloBoolVarArray>> y,
                    int TMAX,
                    IloArray<IloIntVarArray> p,
                    IloIntVarArray U)
{
    IloEnv env = modelo.getEnv();

    for (int i = 0; i < n; i++)
    {
        IloExpr soma(env);
        for (int j = 0; j < n; j++)
        {
            for (int k = 0; k < M[j]; k++)
            {
                for (int h = 0; h < TMAX - k; h++)
                {
                    soma += y[j][k][h];
                }
            }
        }
        modelo.add(U[i] == soma + p[i][0]);
    }
}

void restriction_18(IloModel modelo,
                    int n,
                    int M[],
                    IloArray<IloIntVarArray> alpha,
                    IloIntVarArray R)
{
    IloEnv env = modelo.getEnv();

    for (int i = 0; i < n; i++)
    {
        IloExpr soma(env);
        for (int k = 0; k < M[i]; k++)
        {
            soma += alpha[i][k];
        }
        modelo.add(R[i] == soma);
    }
}

void restriction_19(IloModel modelo,
                    int n,
                    int M[],
                    IloArray<IloBoolVarArray> w)
{
    IloEnv env = modelo.getEnv();
    for (int i = 0; i < n; i++)
    {
        IloExpr soma(env);
        for (int k = 0; k < M[i]; k++)
        {
            soma += w[i][k];
        }
        modelo.add(soma == 1);
    }
}

void restriction_20(IloModel modelo,
                    int n,
                    int M[],
                    IloArray<IloIntVarArray> alpha,
                    IloArray<IloBoolVarArray> w,
                    IloArray<IloIntVarArray> p)
{
    for (int i = 0; i < n; i++)
    {
        for (int k = 0; k < M[i]; k++)
        {
            modelo.add(alpha[i][k] <= 1 - w[i][k] + p[i][k]);
        }
    }
}

void restriction_21(IloModel modelo,
                    int n,
                    int M[],
                    int TMAX,
                    IloArray<IloIntVarArray> alpha,
                    IloArray<IloBoolVarArray> w)
{
    for (int i = 0; i < n; i++)
    {
        for (int k = 0; k < M[i]; k++)
        {
            modelo.add(alpha[i][k] <= w[i][k] * TMAX);
        }
    }
}