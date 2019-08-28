#ifndef MY_CUT_CALLBACK_H
#define MY_CUT_CALLBACK_H

#include <ilcplex/ilocplex.h>


/** Cut callback */ 
class MyCutCallback : public IloCplex::UserCutCallbackI 
{
   private:

      //quando da construcao do objeto desta classe, x é usado para colocar todas as variaveis no vetor x_vars.
      IloArray<IloBoolVarArray> x;
      //x_vars contem as variaveis x. Com esse vetor, peço todos os valores das variaveis x de uma vez só para o CPLEX. Isso é muito mais rápido que pedir cada valor
      //de uma vez.
      IloNumVarArray x_vars;

   public:

      //construtor da classe
      MyCutCallback(IloEnv env, const IloArray<IloBoolVarArray>&); 
      
      //metodo que retorna uma copia do calback. Cplex exige este método.
      IloCplex::CallbackI* duplicateCallback() const;

      //no método main do callback está o código que será executado pelo cplex.
      void main(); 
}; 

#endif
