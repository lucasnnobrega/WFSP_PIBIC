#ifndef MY_CUT_CALLBACK_H
#define MY_CUT_CALLBACK_H

#include <ilcplex/ilocplex.h>
#include <vector>
#include <utility>
#include <algorithm>
#include <iostream>

/** Cut callback */
class MyCutCallback : public IloCplex::UserCutCallbackI //LazyConstraintCallbackI //
{
private:
   //quando da construcao do objeto desta classe, x é usado para colocar todas as variaveis no vetor x_vars.
   //IloArray<IloBoolVarArray> x;
   IloIntVarArray x;

   IloArray<IloArray<IloBoolVarArray>> y_class;

   //x_vars contem as variaveis x. Com esse vetor, peço todos os valores das variaveis x de uma vez só para o CPLEX. Isso é muito mais rápido que pedir cada valor
   //de uma vez.
   IloNumVarArray x_vars;

   // Contem qual corte sera executado
   int cut_class_param;

   // Armazena do TMAX
   int TMAX_class;

   // Armazena o enviromnent
   IloEnv env_class;

public:
   //construtor da classe para o caso 1
   MyCutCallback(IloEnv env, const IloIntVarArray &x_ref, int cut_param, int TMAX);

   //construtor da classe para o caso 2
   MyCutCallback(IloEnv env, const IloArray<IloArray<IloBoolVarArray>> &x_ref, int cut_param, int TMAX);

   //metodo que retorna uma copia do calback. Cplex exige este método.
   IloCplex::CallbackI *duplicateCallback() const;

   //no método main do callback está o código que será executado pelo cplex.
   void main();

   //
   //std::vector<IloConstraint> *MyCutCallback::separate_p3(int n_max_p3)
   std::vector<IloConstraint> *separate_case5();

   std::vector<IloConstraint> *separate_case7();
};

#endif
