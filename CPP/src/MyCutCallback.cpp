#include "../include/MyCutCallback.h"

int contador = 0;

MyCutCallback::MyCutCallback(IloEnv env, const IloIntVarArray &x_ref, int cut_param) : IloCplex::UserCutCallbackI(env), x(x_ref), x_vars(env)
//MyCutCallback::MyCutCallback(IloEnv env, const IloIntVarArray &x_ref, int cut_param) : IloCplex::LazyConstraintCallbackI(env), x(x_ref), x_vars(env)
{
   //preenche x_vars
   /*
   for (int i = 0; i < Graph::getOrder(); i++)
   {
      for (int j = i + 1; j < Graph::getOrder(); j++)
      {
         this->x_vars.add(this->x[i][j]);
      }
   }
   */

   std::cout << "Inside Callback Constructor" << std::endl;

   for (int i = 0; i < x.getSize(); i++)
   {
      std::cout << i << std::endl;
      this->x_vars.add(this->x[i]);
   }

   std::cout << "Inside Callback Constructor" << std::endl;

   env_class = env;
}

MyCutCallback::MyCutCallback(IloEnv env, const IloArray<IloArray<IloBoolVarArray>> &x_ref, int cut_param) : IloCplex::UserCutCallbackI(env), y_class(x_ref), x_vars(env)
//MyCutCallback::MyCutCallback(IloEnv env, const IloArray<IloArray<IloBoolVarArray>> &x_ref, int cut_param) : IloCplex::LazyConstraintCallbackI(env), x(x_ref), x_vars(env)
{

   cut_class_param = cut_param;

   //preenche x_vars
   std::cout << "Inside Callback Constructor 2" << std::endl;

   for (int i = 0; i < y_class.getSize(); i++)
   {
      for (int k = 0; k < y_class[i].getSize(); k++)
      {
         for (int h = 0; h < y_class[i][k].getSize(); h++)
         {
            this->x_vars.add(this->y_class[i][k][h]);
         }
      }
   }

   std::cout << "Inside Callback Constructor 2" << std::endl;

   env_class = env;
}

//cplex exige a implementacao deste metodo. Ele apenas retorna uma copia do callback
IloCplex::CallbackI *MyCutCallback::duplicateCallback() const
{
   if (cut_class_param == 5)
   {
      return new (getEnv()) MyCutCallback(getEnv(), x, cut_class_param);
   }
   if (cut_class_param == 7)
   {
      return new (getEnv()) MyCutCallback(getEnv(), y_class, cut_class_param);
   }
}

std::vector<IloConstraint> *MyCutCallback::separate_case5()
{
   //pede todos os valores das variaveis x de uma vez só ao CPLEX
   //IloNumArray x_vals(getEnv(), (Graph::getOrder() * (Graph::getOrder() - 1)) / 2);
   IloNumArray x_vals(getEnv(), x_vars.getSize());
   getValues(x_vals, x_vars);

   //std::cout << "INSIDE CALLBACK SEPARATE METHOD" << std::endl;

   std::vector<IloConstraint> *cons = new std::vector<IloConstraint>();

   for (int i = 0; i < x_vars.getSize() - 1; i++)
   {
      if (x_vals[i] > x_vals[i + 1] + 0.001)
      {

         double viol = x_vals[i] - x_vals[i + 1];
         cons->push_back(x[i] <= x[i + 1]);
      }
   }

   return cons;
}

std::vector<IloConstraint> *MyCutCallback::separate_case7()
{
   //pede todos os valores das variaveis x de uma vez só ao CPLEX
   //IloNumArray x_vals(getEnv(), (Graph::getOrder() * (Graph::getOrder() - 1)) / 2);
   IloNumArray x_vals(getEnv(), x_vars.getSize());
   getValues(x_vals, x_vars);

   //std::cout << "INSIDE CALLBACK SEPARATE METHOD" << std::endl;

   std::vector<IloConstraint> *cons = new std::vector<IloConstraint>();

   for (int i = 0; i < y_class.getSize(); i++)
   {
      for (int k = 0; k < y_class[i].getSize(); i++)
      {
         for (int h = 0; h < y_class[i][k].getSize(); i++)
         {

            IloExpr somaE(env_class);
            int somaE_int;
            IloExpr somaD(env_class);
            int somaD_int;

            for (int h = 0; h < TMAX_class - k; h++)
            {
               somaE += y_class[i + 1][k][h];
            }

            for (int h = 0; h < TMAX_class - k; h++)
            {
               somaD += y_class[i][k][h];
            }

            //modelo.add(somaE <= somaD);
            IloExpr somaE_min_somaD(env_class);
            somaE_min_somaD = somaE - somaD;

            //if (somaE_min_somaD)
            if (somaE - somaD)
            {
               double viol = x_vals[i] - x_vals[i + 1];
               cons->push_back(somaE <= somaD);
            }
         }
      }
   }

   return cons;
}

//código do callback que é executado pelo cplex.
void MyCutCallback::main()
{
   if (cut_class_param == 5)
   {
      //contador = contador + 1;

      int n_p3 = x.getSize();

      std::vector<IloConstraint> *cons_p3 = separate_case5();

      //adiciona os cortes
      for (int i = 0; i < cons_p3->size(); i++)
      {
         add(cons_p3->at(i)).end(); //aqui tambem ha a possibilidade de se usar addLocal. Nesse caso, o corte so eh adicionado na subarvore enraizada pelo no atual
      }
      delete cons_p3;

      //std::cout << "contador: " << contador << std::endl;
   }
   if (cut_class_param == 7)
   {
      //contador = contador + 1;

      int n_p3 = y_class.getSize();

      std::vector<IloConstraint> *cons_p3 = separate_case7();

      //adiciona os cortes
      for (int i = 0; i < cons_p3->size(); i++)
      {
         add(cons_p3->at(i)).end(); //aqui tambem ha a possibilidade de se usar addLocal. Nesse caso, o corte so eh adicionado na subarvore enraizada pelo no atual
      }
      delete cons_p3;

      //std::cout << "contador: " << contador << std::endl;
   }
}
