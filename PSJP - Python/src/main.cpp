/*********************************************
 * Author: lucasnn
 * Creation Date: Apr 10, 2019 at 11:00:21 PM
 *********************************************
*/

#include <iostream>
#include <ilcplex/ilocplex.h>
#include <math.h>

using namespace std;

void PSJP()
{
    // Creating a envirioment
    IloEnv env;

    // Constants

    // Numero de Simbolos
    int n = 14;

    // Conjunto de Simbolos
    int X[n];

    // Numero Maximo de copias do simbolo Xi E X
    int M[n];

    // Conjunto de indices
    int *K[n];

    for (int i = 0; i < n; i++)
    {
        K[i] = new int(M[i]);

        for (int j = 0; j < M[i]; j++)
            K[i][j] = j + 1;
    }

    /*
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < M[i]; j++)
            std::cout << *K[i] << std::endl;
    }
    // Tamanho maximo de uma sequencia viavel
    int TMAX = 4 * n;

    // Prioridades dos símbolos
    int c[n];

    // Conjunto de posicoes que podem ser ocupadas pela k-ésima cópia do simbolo Xi E X
    int **H[n];

    for (int i = 0; i < n; i++)
    {
        H[i] = new int(M[i]);

        for (int k = 0; k < M[i]; k++)
        {
            H[i][k] = new int(TMAX - (k + 1) + 1);

            int aux = k + 1;
            for (int j = 0; j < H[i][k]; j++)
            {
                H[i][k][j] = aux;
                aux++;
            }
        }
    }

    // Decision variables
    // y_ikh
    IloArray<IloArray<IloBoolVarArray>> y(env, n);

    /*
    IloNumVarArray players(env, nPlayers, 0, 1, ILOBOOL);
    //Creating objective function1
    IloExpr to_optimize(env);
    for (int i = 0; i < players.getSize(); ++i)
        to_optimize += defesa[i] * players[i];

    // Objective Function
    IloObjective obj(env, to_optimize, IloObjective::Maximize);

    // Constraints and ranges

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
    try
    {
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