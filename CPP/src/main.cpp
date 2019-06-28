
// TODO: Coisas A fazer
//// Coisas completamente erradoas
//? perguntas a serem solucionadas
//! possiveis erros de dar merda
// comentario normal
//* comentario mais claro


#include <ilcplex/ilocplex.h>
#include <iostream>
#include <math.h>
#include <string.h>
#include "read_write.h"


using namespace std;

void PSJP()
{
    // Criando o Ambiente
    IloEnv env;
    IloModel modelo(env);

    //* Dados

    // Numero de Simbolos
    int n = 14;

    // Conjunto de Simbolos X1 ... Xn
    int X[n];

    // Numero Maximo de copias do simbolo Xi E X
    int M[n];

    // Conjunto de indices das cópias xi E X,  Ki = {1, ..., Mi}
    // i  = {1,...N} 
    // Array de ponteiros
    int *K[n];
    
    for (int i = 0; i < n; i++) {
        int *aux = new int(M[i]);
        K[i] = aux;

        for (int j = 0; j < M[i]; j++) {
            K[i][j] = j;
        }
    }
    
    // Tamanho maximo das sequencias viaveis
    int TMAX = 4 * n;

    // Prioridades do símbolo Xi E X
    int c[n];  
    
    // Conjunto de posicoes que podem ser ocupadas pela k-ésima cópia do simbolo Xi E X
    // Hik = {k, ... ,TMAX}
    int **H[n];
    std::cout<<"Dados Criados, Agora, gerar as variáveis:"<<std::endl;

    for (int i = 0; i < n; i++) {
        H[i] = new int*[M[i]];

        for (int k = 0; k < M[i]; k++) {
            H[i][k] = new int(TMAX - k);
          
            int aux = 0;          
            for (int j = k; j < TMAX; j++) {
                H[i][k][aux] = j;
                aux++;              
            }
        }
    }

    //* Variaveis
    
    // y_ikh , variavel binaria
    // Dimensão n pois i vai até n
    IloArray <IloArray <IloBoolVarArray> > y(env, n);

    for (int i = 0; i < n; i++) {
        IloArray<IloBoolVarArray> vetorAux(env, M[i]); 
        y[i] = vetorAux;

        for (int k = 0; k < M[i]; k++) {
            IloBoolVarArray vetorAux2(env, TMAX - k);
            y[i][k] = vetorAux2;
        }
    }

    // Adicionando variavel no modelo    
    for (int i = 0; i < n; i++) {        
        for (int k = 0; k < M[i]; k++) {        
            for(int h = 0; h < TMAX - k; h++) {
                char var[100];
                sprintf(var, "y(%d,%d,%d)", i, k, H[i][k][h]);
                y[i][k][h].setName(var);
                modelo.add(y[i][k][h]);
            } 
        }
    }

    // d_i,k,k+1
    // Dimensao n, pois i vai ate n
    IloArray <IloIntVarArray> d(env, n);
    for(int i = 0; i < n; i++) {
        IloIntVarArray vetorAux(env, M[i], 0, IloInfinity); 
        d[i] = vetorAux;
    }

    // Adicionando variavel no modelo 
    for(int i = 0; i < n; i++) {        
        for(int k = 0; k < M[i]; k++) {
            char var[100];
            if(k == M[i] - 1 ) {
                sprintf(var, "d(%d, %d, %d)", i, k, 0);
            }
            else {
                sprintf(var, "d(%d, %d, %d)", i-1, k, k);
            }
            d[i][k].setName(var);
            modelo.add(d[i][k]);
        } 
    }

    // D_i
    // Maior Distancia entre duas copias consecutivas do simbolo x_i E X
    IloIntVarArray D(env, n, 0, IloInfinity);

    // Adicionando variavel no modelo    
    for(int i = 0; i < n; i++){
        char var[100];
        sprintf(var, "D(%d)", i);
        D[i].setName(var);
        modelo.add(D[i]);
    }

    // P
    // Maior produto D_i*c_i, para todo x_i E X
    //? Implementado como um valor unico
    IloIntVar P(env, 0, IloInfinity);

    // Adicionando variavel no modelo    
    char var[100];
    sprintf(var, "P");
    P.setName(var);
    modelo.add(P);

    // p_ik
    // Posicao da k-esima copia do simbolo x_i E X
    IloArray <IloIntVarArray>  p(env, n);
    for (int i = 0; i < n; i++) {
        IloIntVarArray vetorAux(env, M[i], 0, IloInfinity); 
        p[i] = vetorAux;
    }

    // Adicionando variavel no modelo 
    for(int i = 0; i < n; i++) {        
        for(int k = 0; k < M[i]; k++) {
            char var[100];
            sprintf(var, "p(%d,%d)", i, k);
            p[i][k].setName(var);
            modelo.add(p[i][k]);
        } 
    }
 
    // Criando a Função Objetivo (FO) 
    // min P
    IloObjective obj(env, P, IloObjective::Minimize);
    modelo.add(obj);
 
    // Constraints 

    // P >= D_i * c_i, para i = 1, ... , n (4.2) (3)
    for(int i = 0; i < n; i++){
        modelo.add( P >= D[i]*c[i] );
    }

    // D_i ≥ d_i,k,k+1 , para todo i = 1, . . . , n,  para todo k ∈ K_i \ {M_i }, (4.3) (4)
    for(int i = 0; i < n; i++){
        for(int k = 0; k < M[i]-1; k++){
            modelo.add(D[i] >= d[i][k]);
        }
    }

    // sum{i in X, k in K[i]} y_ikh ≤ 1, ∀h ∈ {1, . . . , T MAX}, (4.4) (5)    
    for(int h = 0; h < TMAX-1; h++){
        IloExpr soma(env);
        for (int i = 0; i < n; i++) {  
            for (int k = 0; k < M[i]; k++) { 
                if (h < TMAX - k)     
                    soma += y[i][k][h];            
            }
        }
        modelo.add(soma <= 1);
    }

    // sum{h in H[i][k]} y_ikh ≤ 1,, (4.4) (6)     
    for (int i = 0; i < n; i++) {  
        for (int k = 0; k < M[i]; k++) {      
            IloExpr soma(env);
            for (int h = 0; h < TMAX - k; h++){
                soma += y[i][k][h];            
            }
            modelo.add(soma <= 1);
        }
    }

    // sum{k in K[i], h in H[i][k]} y_ikh ≤ 1,  (4.4) (7)    
    for (int i = 0; i < n; i++) {  
        IloExpr soma(env);
        for (int k = 0; k < M[i]; k++) {      
            for(int h = 0; h < TMAX - k; h++){
                soma += y[i][k][h];            
            }
        }
        modelo.add(soma <= 1);
    }
    
    // (8)
    for(int i = 0; i < n; i++) {
        for (int k = 0; k < M[i]; k++){
            IloExpr soma(env);
            for (int h = 0; h < TMAX - k; h++) {
                soma += y[i][k][h]*H[i][k][h];
            }
            modelo.add(p[i][k] == soma);
        }
    }

    // (9)
    for(int i = 0; i < n; i++) {
        for (int k = 0; k < M[i] - 1; k++){
            IloExpr soma_esq(env);
            IloExpr soma_dir(env);
            for (int h = 0; h < TMAX - k; h++) {
                soma_esq += y[i][k][h];
                soma_dir += y[i][k+1][h];
            }
            modelo.add(soma_esq >= soma_dir);
        }
    }

    // (10)
    for(int i = 0; i < n; i++) {
        for (int k = 0; k < M[i] - 1; k++){
            IloExpr soma(env);
            for (int h = 0; h < TMAX - k; h++) {
                soma += y[i][k+1][h];
            }
            modelo.add(p[i][k+1] >= p[i][k] - (1 - soma)*TMAX);
        }
    }

    // (11)
    for(int i = 0; i < n; i++) {
        for (int k = 0; k < M[i] - 1; k++){
            for (int h = 0; h < TMAX - k; h++) {
                modelo.add(d[i][k] >= p[i][k+1] - p[i][k]);
            }
        }
    }


    /* 
    // y_ikh E {0, 1}, para todo i = 1, . . . , n, para todo k ∈ K_i , para todo h ∈ H_ik. (4.13)
    for (int i = 0; i < n; i++) {
        for (int k = 0; k < M[i]; k++) {
            for(int h = 0; h < H[i][k]; h++){
                IloRange y_ikh_restrition(env, 0, y[i][k][h], 1);
                modelo.add(y_ikh_restrition);
            }
        }
    }
    */


    // Creating a CPLEX solver
    IloCplex cplex(env);
    cplex.setParam(IloCplex::Param::Threads, 1);
    cplex.extract(modelo);
    
    cplex.out() << endl;
    cplex.solve();
    cplex.out() << endl << "solution status = " << cplex.getStatus() << endl;

    cplex.out() << "\e[32mcost\e[0m = " << cplex.getObjValue() << endl;

    env.out();
}

int main(int argc, char *argv[])
{
    std::cout << "Entrou na main()" << std::endl;
    try
    {
        std::cout<<"Entrou no try"<<std::endl;
        //PSJP();
        //write_res('v', 12, 7);
        read_instances("./data/csp_instances/ins_05_20_4.txt");
    }
    catch (IloException &e)
    {
        std::cout << "\n \n\e[31mError\e[0m" << endl
                  << e << std::endl;
        e.end();
    }

    return 0;
}