
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

void PSJP(int number_of_symbols, int m, int *priorities)
{
    // Criando o Ambiente
    IloEnv env;
    IloModel modelo(env);

    //* Dados

    // Numero de Simbolos
    std::cout << "Entrou no PSJP"  << std::endl;
    int n = number_of_symbols;
    std::cout << "Numero de Simbolos: " << number_of_symbols << " " << n << std::endl;

    // Conjunto de Simbolos X1 ... Xn
    int X[n];

    // Numero Maximo de copias do simbolo Xi E X
    int M[n];

    for(int i = 0; i < n; i++){
        M[i] = m;
    }


    std::cout << "breakpoint 2 L:38" << std::endl;

    // Conjunto de indices das cópias xi E X,  Ki = {1, ..., Mi}
    // i  = {1,...N} 
    // Array de ponteiros
    int *K[n];
    
    std::cout << "breakpoint 3 L:45" << std::endl;

    for (int i = 0; i < n; i++) {
        //std::cout << "breakpoint 3.1 " << i << std::endl;

        int *aux = new int(M[i]);
        K[i] = aux;

        //std::cout << "breakpoint 3.2 " << i << std::endl;

        for (int j = 0; j < M[i]; j++) {
            //std::cout << "breakpoint 3.2.1 " << j << std::endl;
            K[i][j] = j;
        }

        //std::cout << "breakpoint 3.3 " << i << std::endl;
    }
    
    std::cout << "breakpoint 4 L:60" << std::endl;

    // Tamanho maximo das sequencias viaveis
    int TMAX = 4 * n;

    // Prioridades do símbolo Xi E X
    int c[n];

    std::cout << std::endl;
    for(int i = 0; i < n; i++){
        c[i] = priorities[i];
        std::cout << "Prioridade: " << c[i] << " ";
    }
     std::cout << std::endl;

    
    // Conjunto de posicoes que podem ser ocupadas pela k-ésima cópia do simbolo Xi E X
    // Hik = {k, ... ,TMAX}
    int **H[n];
    std::cout << "Criando o array Hik:" << std::endl;

    for (int i = 0; i < n; i++) {
        //H[i] = new int*[M[i]];
        
        //std::cout << "breakpoint 5 i " << i << std::endl;
        
        H[i] = new int*[M[i]];

        //std::cout << "breakpoint 6 H[i] Criado" << std::endl;
        
        for (int k = 0; k < M[i]; k++) {
            int aux = 0;          
            //std::cout << "breakpoint 7 k:" << k << " TMAX " << TMAX <<" aux " << aux << std::endl;
          
            H[i][k] = new int[TMAX - k];
 
            //std::cout << "breakpoint 8 TMAX-k "<< TMAX - k << std::endl;
            for (int j = k; j < TMAX; j++) {
                //std::cout << "breakpoint 9 j " << j << " aux " << aux << std::endl;
                H[i][k][aux] = j;
                aux++;              
            }
        }
    }
    
    //* Conferindo dados de entrada
    std::cout << "#####################################" << std::endl;
    std::cout << "Conferindo dados de entrada: " << std::endl;
    std::cout << "Numero de símbolos: " << n << std::endl;
    std::cout << "TMAX: " << TMAX << std::endl;
    std::cout << "#####################################" << std::endl;
          

    std::cout<<"Dados Criados, Agora, gerar as variáveis:"<<std::endl;

    //* Variaveis
    
    // y_ikh , variavel binaria
    // Dimensão n pois i vai até n
    IloArray <IloArray <IloBoolVarArray> > y(env, n);

    std::cout << "breakpoint 10" << std::endl;

    for (int i = 0; i < n; i++) {
        IloArray<IloBoolVarArray> vetorAux(env, M[i]); 
        y[i] = vetorAux;

        for (int k = 0; k < M[i]; k++) {
            IloBoolVarArray vetorAux2(env, TMAX - k);
            y[i][k] = vetorAux2;
        }
    }

    std::cout << "breakpoint 11" << std::endl;

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

    std::cout << "breakpoint 12" << std::endl;

    // d_i,k,k+1
    // Dimensao n, pois i vai ate n
    IloArray <IloIntVarArray> d(env, n);
    for(int i = 0; i < n; i++) {
        IloIntVarArray vetorAux(env, M[i], 0, IloInfinity); 
        d[i] = vetorAux;
    }

    std::cout << "breakpoint 13" << std::endl;

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

    std::cout << "breakpoint 14" << std::endl;

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

    std::cout << "breakpoint 15" << std::endl;

    // P
    // Maior produto D_i*c_i, para todo x_i E X
    //? Implementado como um valor unico
    IloIntVar P(env, 0, IloInfinity);

    // Adicionando variavel no modelo    
    char var[100];
    sprintf(var, "P");
    P.setName(var);
    modelo.add(P);

    std::cout << "breakpoint 16" << std::endl;

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
 
    std::cout << "breakpoint 17" << std::endl;

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
    
    std::cout << "breakpoint 25" << std::endl;


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

    std::cout << "breakpoint 26" << std::endl;

    // (9)
    for(int i = 0; i < n; i++) {
        for (int k = 0; k < M[i] - 1; k++){
            std::cout << "  breakpoint 26.1 i:" << i << " k:" << k << std::endl;

            IloExpr soma_esq(env);
            IloExpr soma_dir(env);
            //?for (int h = 0; h < TMAX - k ; h++)
            for (int h = 0; h < TMAX - k - 1; h++) {
                std::cout << "    breakpoint 26.1.1 i:" << i << " k:" << k << " h:" << h << std::endl;
                soma_esq += y[i][k][h];
                soma_dir += y[i][k+1][h];
                std::cout << "    breakpoint 26.1.1 i:" << i << " k:" << k << " h:" << h << "end" <<  std::endl;
            }
            modelo.add(soma_esq >= soma_dir);
        }
    }

    std::cout << "breakpoint 27" << std::endl;


    // (10) Tese (4.9)
    for(int i = 0; i < n; i++) {
        for (int k = 0; k < M[i] - 1; k++){
            IloExpr soma(env);
            //?for (int h = 0; h < TMAX - k -1; h++) {
            for (int h = 0; h < TMAX - k -1; h++) {
                soma += y[i][k+1][h];
            }
            modelo.add(p[i][k+1] >= p[i][k] - (1 - soma)*TMAX);
        }
    }

    std::cout << "breakpoint 28" << std::endl;


    // (11) Tese (4.10)
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

    std::cout << "breakpoint 100" << std::endl;

    cplex.out() << y << endl;

    for (size_t i = 0; i < n; i++)
    {
        //cplex.out() << y[i][0][0] << endl;
        for (int j = 0; j < M[i]; j++)
        {
            for (int k = 0; k < TMAX-j; k++)
            {
                cplex.out() <<"i:" << i << "j:" << "k:" << k << " result:" << cplex.getValue(y[i][j][k]) << endl;
               
            }
            
        }
        
    }
    

    /*
    for (int i = 0; i < n; i++) {
        for (int k = 0; k < M[i]; k++) {
            for (int j = 0; j < k; j++){
                cplex.out() << y << endl;
            }
        }
    }
    */

    cplex.out() << "\e[32mcost\e[0m = " << cplex.getObjValue() << endl;

    env.out();
}

int main(int argc, char *argv[])
{
    std::cout << "Entrou na main()" << std::endl;
    try
    {
        File_content *aux = NULL;

        std::cout<<"Entrou no try"<<std::endl;
        
        aux = read_instances("./data/csp_instances/ins_05_20_4.txt");
        if (aux){
            std::cout<<"Entrou no try -> IF"<<std::endl;

            std::cout << "number of symbols: " << aux->number_of_symbols << std::endl;
            std::cout << "number m: " << aux->m  << std::endl;
            //PSJP(aux->number_of_symbols, aux->m, aux->priorities);
            
            write_res('v', 12, aux->number_of_symbols,
                12, 13, 
                aux->m,
                aux->priorities,
                aux->priorities,
                aux->priorities,
                aux->priorities,
                aux->priorities);    
            
            
            free(aux);
        }
        */
    }
    catch (IloException &e)
    {
        std::cout << "\n \n\e[31mError\e[0m" << endl
                  << e << std::endl;
        e.end();
    }

    return 0;
}