
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
#include <vector>
#include <algorithm>
#include "read_write.h"

using namespace std;

// Driver function to sort the 2D vector 
// on basis of a particular column 
bool sortcol( const vector<int>& v1, 
               const vector<int>& v2 , int col) { 
 return v1[col] < v2[col]; 
} 


void WFSP(int number_of_symbols, int m, int *priorities)
{
    // Criando o Ambiente
    IloEnv env;
    IloModel modelo(env);

    //* Dados

    // Numero de Simbolos
    std::cout << "Inside WFSP function"  << std::endl;
    int n = number_of_symbols;
    std::cout << "Number of symbols: " << number_of_symbols << " " << n << std::endl;

    // Conjunto de Simbolos X1 ... Xn
    int X[n];

    // Numero Maximo de copias do simbolo Xi E X
    int M[n];

     for(int i = 0; i < n; i++){
         M[i] = m;
    }


    

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
    int TMAX = m * n;

    // Prioridades do símbolo Xi E X
    int c[n];

    std::cout << std::endl;
    for(int i = 0; i < n; i++){
        c[i] = priorities[i];
        std::cout << "Priority: " << c[i] << " ";
    }
     std::cout << std::endl;

    
    // Conjunto de posicoes que podem ser ocupadas pela k-ésima cópia do simbolo Xi E X
    // Hik = {k, ... ,TMAX}
    int **H[n];
    std::cout << "Creating the array Hik:" << std::endl;

    for (int i = 0; i < n; i++) {
        //H[i] = new int*[M[i]];
        
        H[i] = new int*[M[i]];
        
        for (int k = 0; k < M[i]; k++) {
            int aux = 0;          
          
            H[i][k] = new int[TMAX - k];
 
            
            for (int j = k; j < TMAX; j++) {
                H[i][k][aux] = j;
                aux++;              
            }
        }
    }
    
    //* Verify input data
    std::cout << "#####################################" << std::endl;
    std::cout << "Verify input data: " << std::endl;
    std::cout << "Number of symbols: " << n << std::endl;
    std::cout << "TMAX: " << TMAX << std::endl;
    std::cout << "#####################################" << std::endl;
          

    std::cout<<"Dados Criados, Agora, gerar as variáveis: "<<std::endl;
    std::cout<<"Created data, now creating the variables: "<<std::endl;

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
    IloArray < IloIntVarArray > d(env, n);
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
            //?for (int h = 0; h < TMAX - k ; h++)
            for (int h = 0; h < TMAX - k - 1; h++) {
            
                soma_esq += y[i][k][h];
                soma_dir += y[i][k+1][h];

            }
            modelo.add(soma_esq >= soma_dir);
        }
    }



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


    //cplex.out() << y << endl;

    /*
    for (size_t i = 0; i < n; i++)
    {
        //cplex.out() << y[i][0][0] << endl;
        for (int k = 0; k < M[i]; k++)
        {
            for (int k = 0; k < TMAX-j; k++)
            {
                cplex.out() <<"i:" << i << "k:" << "k:" << k << " result:" << cplex.getValue(y[i][j][k]) << endl;
               
            }
            
        }
        
    }
    */



   vector< vector< int > > sequencia;

    for (int i = 0; i < n; i++) {   // 2 == n
        for (int k = 0; k < M[i]; k++) {
            for (int h = 0; h < TMAX - k; h++){
                //cplex.out() << " OUT = " << cplex.getValue(y[i][k][h]) << endl;
                if ( cplex.getValue(y[i][k][h])  == 0 ){
                    vector < int > aux;
                    aux.push_back(i);
                    aux.push_back(k);
                    aux.push_back(h); 

                    sequencia.push_back(aux);
                }                
            }
        }
    }

    for (int i = 0; i < sequencia.size(); i++) { 
        for (int j = 0; j < sequencia[i].size(); j++){ 
            std::cout << sequencia[i][j] << " ";
        }
        std::cout << "\n"; 
        std::cout << "símbolo: " << sequencia[i][0] << std::endl;
        std::cout << "k-ésima cópia: " << sequencia[i][1] << std::endl;
        std::cout << "posição h: " << sequencia[i][2] << std::endl;
        std::cout << "\n";
    } 



    cplex.out() << "\e[32mcost\e[0m = " << cplex.getObjValue() << endl;

    env.out();
}

int main(int argc, char *argv[])
{
    std::cout << "Inside the main function" << std::endl;
    try
    {
        std::cout<<"Inside the try"<<std::endl;

        
        File_content *aux = NULL;
        aux = read_instances("./data/csp_instances/ins_05_20_4.txt");

        if (aux){
            std::cout<<"Inside the try -> IF"<<std::endl;

            std::cout << "number of symbols: " << aux->number_of_symbols << std::endl;
            std::cout << "number m: " << aux->m  << std::endl;
            WFSP(aux->number_of_symbols, aux->m, aux->priorities);
            
            Write_content content;

            content.objective = 12;
            content.number_of_symbols = aux->number_of_symbols;
            content.occupied_positions = 12;
            content.total_positions = 13;
            content.avaliable_copies = aux->priorities;
            content.used_copies = aux->priorities;
            content.priorities = aux->priorities;
            content.Di = aux->priorities;
            content.Df = aux->priorities;
            content.Pi = aux->priorities;
            content.Pf = aux->priorities;
              
            write_res('v', content);        
            
            free(aux);
        }
    }
    catch (IloException &e)
    {
        std::cout << "\n \n\e[31mError IloException: \e[0m" << endl
                  << e << std::endl;
        e.end();
    }

    return 0;
}