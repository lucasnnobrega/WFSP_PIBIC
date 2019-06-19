
// TODO: Coisas A fazer
//// Coisas completamente erradoas
//? perguntas a serem solucionadas
//! possiveis erros de dar merda
// comentario normal
//* comentario mais claro


#include <ilcplex/ilocplex.h>
#include <iostream>
#include <math.h>


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
        //! conferir isso aqui
        int *aux = new int(M[i]);
        K[i] = aux;
        for (int j = 0; j < M[i]; j++) {
            K[i][j] = j + 1;
        }
    }
    
    // Tamanho maximo das sequencias viaveis
    int TMAX = 4 * n;

    // Prioridades do símbolo Xi E X
    int c[n];  
    
    // Conjunto de posicoes que podem ser ocupadas pela k-ésima cópia do simbolo Xi E X
    // Hik = {k, ... ,TMAX}
    int **H[n];

    for (int i = 0; i < n; i++) {
        *H[i] = new int(M[i]);

        for (int k = 0; k < M[i]; k++) {
            H[i][k] = new int(TMAX - (k + 1) + 1);

            int aux = k + 1;
            for (int j = 0; j < TMAX-(k+1)+1; j++) {
                H[i][k][j] = aux;
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
            IloBoolVarArray vetorAux2(env, TMAX-(k+1)+1);
            y[i][k] = vetorAux2;
        }
    }

    // Adicionando variavel no modelo    
    for (int i = 0; i < n; i++) {        
        for (int k = 0; k < M[i]; k++) {        
            for(int h = 0; h < TMAX-(k+1)+1; h++) {
                char var[100];
                sprintf(var, "y(%d,%d,%d)", i+1, k+1, h+1);
                y[i][k][h].setName(var);
                modelo.add(y[i][k][h]);
            } 
        }
    }

    // d_i,k,k+1
    // Dimensao n, pois i vai ate n
    IloArray <IloIntVarArray> d(env, n);
    for(int i = 0; i < n; i++) {
        IloIntVarArray vetorAux(env, M[i]); 
        d[i] = vetorAux;
    }

    // Adicionando variavel no modelo 
    //! verificar isso   
    for(int i = 0; i < n; i++) {        
        for(int k = 0; k < M[i]; k++) {
            char var[100];
            if(k == M[i] - 1 ) {
                sprintf(var, "d(%d, %d, %d)", i+1, k+1, 1);
            }
            else {
                sprintf(var, "d(%d, %d, %d)", i, k+1, k+1);
            }
            d[i][k].setName(var);
            modelo.add(d[i][k]);
        } 
    }

    // D_i
    // Maior Distancia entre duas copias consecutivas do simbolo x_i E X
    IloIntVarArray D(env, n);

    // Adicionando variavel no modelo    
    //! verificar isso
    for(int i = 0; i < n; i++){
        char var[100];
        sprintf(var, "D(%d)", i+1);
        D[i].setName(var);
        modelo.add(D[i]);
    }

    /*
    /////////////////////////////////////////////////////////////////

    // P
    // Maior produto D_i*c_i, para todo x_i E X
    //? Como assim ser um array? Num era para ser um valor unico contendo o maximo?
    //IloIntVarArray P(env, n);

    // Adicionando variavel no modelo    
    //! verificar isso
    //for(int i = 0; i < n; i++){
    //    char var[100];
    //    sprintf(var, "P(%d)", i+1);
    //    P[i].setName(var);
    //    modelo.add(P[i]);
    //}

    //////////////////////////////////////////////////////////////////
    */

    // P
    // Maior produto D_i*c_i, para todo x_i E X
    //? Implementado como um valor unico
    IloInt P(env, n);

    // Adicionando variavel no modelo    
    //! verificar isso
    char var[100];
    sprintf(var, "P");
    P.setName(var);
    modelo.add(P);
    


    // p_ik
    // Posicao da k-esima copia do simbolo x_i E X
    IloArray <IloIntVarArray>  p(env, n);
    for (int i = 0; i < n; i++) {
        IloIntVarArray vetorAux(env, M[i]); 
        p[i] = vetorAux;
    }

    // Adicionando variavel no modelo 
    //! verificar isso   
    for(int i = 0; i < n; i++) {        
        for(int k = 0; k < M[i]; k++) {
            char var[100];
            sprintf(var, "p(%d,%d)", i+1, k+1);
            p[i][k].setName(var);
            modelo.add(p[i][k]);
        } 
    }

    ///////////////////////////////////////////////////

    //// Criando a Função Objetivo (FO) 
    ////IloExpr sumY(env);
    ////for(int k = 0; k < data.getNItems(); k++)
    ////{   
    ////sumY += y[k];
    ////}
    //// Adicionando a FO
    ////modelo.add(IloMinimize(env, sumY));


    // Criando a Função Objetivo (FO) 
    // min P
    IloObjective obj(env, P, IloObjective::Minimize);
    
    modelo.add(obj)

    ///////////////////////////////////////////////////

    

    // Constraints and ranges

    // P >= D_i * c_i, para i = 1, ... , n (4.2)
    //! Conferir para o caso em que P for um array!!!
    for(int i = 0; i < n; i++){
        //* Um caso, P >= D_1*c_1
        IloRange P_restrition(env, (D[i]*c[i]), P[i], IloInfinity);
        // Adiciona a restricao do P no exemplo
        modelo.add(P_restrition);
    }

    // D_i ≥ d_i,k,k+1 , para todo i = 1, . . . , n,  para todo k ∈ K_i \ {M_i }, (4.3)

    for(int i = 0; i < n; i++){
        for(int k = 0; k < n; k++){
            IloRange D_restrition(env, d[])
        }
    }

    // somatorio1 somatorio2 y_ikh ≤ 1, ∀h ∈ {1, . . . , T MAX}, (4.4)    
    IloExpr somatorio_somatorio_yikh(env);

    for (int i = 0; i < n; i++) {
        for (int k = 0; k < M[i]; k++) {
            for(int h = 0; h < H[i][k]; h++){
                somatorio_somatorio_yikh += y[i][k][h];            
            }
        }
    }

    IloRange somatorio_somatorio_yikh_restrition(env, 0, somatorio_somatorio_yikh, 1)

    modelo.add(somatorio_somatorio_yikh_restrition);

    // somatorio_y_ikh ≤ 1, ∀i = 1, . . . , n, ∀k ∈ K i , (4.5)
    IloExpr somatorio_somatorio_yikh(env);

    for (int i = 0; i < n; i++) {
        for (int k = 0; k < M[i]; k++) {
            for(int h = 0; h < H[i][k]; h++){
                somatorio_somatorio_yikh += y[i][k][h];            
            }
        }
    }



    // y_ikh E {0, 1}, para todo i = 1, . . . , n, para todo k ∈ K_i , para todo h ∈ H_ik. (4.13)
    //! Conferir isso aqui
    for (int i = 0; i < n; i++) {
        for (int k = 0; k < M[i]; k++) {
            for(int h = 0; h < H[i][k]; h++){
                IloRange y_ikh_restrition(env, 0, y[i][k][h], 1);
                modelo.add(y_ikh_restrition);
            }
        }
    }




    /*
    // means
    IloExpr sum_assistencia(env);
    IloExpr sum_arremesso(env);
    IloExpr sum_rebote(env);
    IloExpr sum_defesa(env);

    IloExpr sum_positionA(env);
    IloExpr sum_positionC(env);
    IloExpr sum_positionD(env);

    IloExpr sum_all(env);

    for (int i = 0; i < players.getSize(); ++i)
    {
        sum_assistencia += assistencia[i] * players[i];
        sum_arremesso += arremesso[i] * players[i];
        sum_rebote += rebote[i] * players[i];
        sum_defesa += defesa[i] * players[i];

        sum_positionA += positionA[i] * players[i];
        sum_positionC += positionC[i] * players[i];
        sum_positionD += positionD[i] * players[i];

        sum_all += players[i];
    }

    IloRange mean_assistencia_restrition(env, 0, sum_assistencia, 14);
    IloRange mean_arremesso_restrition(env, 0, sum_arremesso, 14);
    IloRange mean_rebote_restrition(env, 0, sum_rebote, 14);
    IloRange mean_defesa_restrition(env, 0, sum_defesa, 14);
    IloRange max_players_restrition(env, 0, sum_all, 5);

    IloRange sum_positionA_restrition(env, 2, sum_positionA, IloInfinity);
    IloRange sum_positionC_restrition(env, 2, sum_positionC, IloInfinity);
    IloRange sum_positionD_restrition(env, 3, sum_positionD, IloInfinity);

    // Constrains

    // -3 < x3 + x5 <= 1
    IloRange r1(env, -3, players[2] + players[5], 1);

    // -INFINITO =< 2*x1 - x3 - x5 <= 0
    IloRange r2(env, -IloInfinity, 2 * players[0] - players[3] - players[4], 0);

    // 1 == x2 + x3 == 1
    IloRange r3(env, 1, players[1] + players[2], 1);


    // Creating a model
    IloModel model(env);
    // Adding obj function
    model.add(obj);

    // Add constrains and ranges to the model
    model.add(mean_assistencia_restrition);
    model.add(mean_arremesso_restrition);
    model.add(mean_rebote_restrition);
    model.add(mean_defesa_restrition);
    model.add(max_players_restrition);

    model.add(sum_positionA_restrition);
    model.add(sum_positionC_restrition);
    model.add(sum_positionD_restrition);

    model.add(r1);
    model.add(r2);
    model.add(r3);

    // Creating a CPLEX solver
    IloCplex cplex(env);
    cplex.setParam(IloCplex::Param::Threads, 1);
    cplex.extract(model);
    // Or
    //IloCplex cplex(model);

    cplex.out() << endl;
    cplex.solve();
    cplex.out() << endl;
    cplex.out() << "solution status = " << cplex.getStatus() << endl;

    cplex.out() << endl
                << "number of threads = " << cplex.getParam(IloCplex::Param::Threads) << endl;
    cplex.out() << endl
                << "number of players = " << players.getSize() << endl;

    cplex.out() << "\e[32mcost\e[0m = " << cplex.getObjValue() << endl;

    cplex.out() << "x"
                << " | "
                << "Ro"
                << " "
                << "Re" << endl;
    for (int i; i < players.getSize(); ++i)
    {
        //for(int i; i <= players.getSize(); ++i){
        cplex.out() << i
                    << " |  " << (round(cplex.getValue(players[i]))) << "  " << cplex.getValue(players[i]) << endl;
    }

    env.out();
*/
}

int main()
{

    std::cout<<"Entrou na main()"<<std::endl;
    try
    {
        std::cout<<"Entrou no try"<<std::endl;
        PSJP();
    }
    catch (IloException &e)
    {
        std::cout << "\n \n\e[31mError\e[0m" << endl
                  << e << std::endl;
        e.end();
    }

    return 0;
}