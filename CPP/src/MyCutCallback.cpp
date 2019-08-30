#include "../include/MyCutCallback.h"
#include "../include/NodeInfo.h"

MyCutCallback::MyCutCallback(IloEnv env, const IloArray<IloBoolVarArray> &x_ref) : IloCplex::UserCutCallbackI(env), x(x_ref), x_vars(env)
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
   for (int i = 0; i < x.getSize(); i)
   {
      this->x_vars.add(this->x[i]);
   }
}

//cplex exige a implementacao deste metodo. Ele apenas retorna uma copia do callback
IloCplex::CallbackI *MyCutCallback::duplicateCallback() const
{
   return new (getEnv()) MyCutCallback(getEnv(), x);
}

std::vector<IloConstraint> *MyCutCallback::separate_p3(int n_max_p3)
{
   //pede todos os valores das variaveis x de uma vez só ao CPLEX
   //IloNumArray x_vals(getEnv(), (Graph::getOrder() * (Graph::getOrder() - 1)) / 2);
   IloNumArray x_vals(getEnv(), x_vars.getSize());
   getValues(x_vals, x_vars);

   /* Codigo de Teobaldo
   if (true)
   {
      // Reconstruindo a matriz original necessaria para Teobaldo, nao eh necessario para o codigo
      //x_values é uma matriz com os valores das variaveis x
      std::vector<std::vector<float>> m(Graph::getOrder(), std::vector<float>(Graph::getOrder()));
      int l = 0;
      for (int i = 0; i < Graph::getOrder(); i++)
      {
         for (int j = i + 1; j < Graph::getOrder(); j++)
         {
            m[i][j] = x_vals[l++];
         }
      }

      //vetor com os p3s violados
      // Contem a restrição e o quanto absurdo eh essa violação
      std::vector<pair<IloConstraint, double>> p3s_cons_aux;

      for (int i = 0; i < Graph::getOrder(); i++)
      {
         for (int j = i + 1; j < Graph::getOrder(); j++)
         {
            for (int l = j + 1; l < Graph::getOrder(); l++)
            {

               if (1 + m[i][j] - m[i][l] - m[j][l] < -P3_EPS)
               {
                  double viol = abs(1 + m[i][j] - m[i][l] - m[j][l]);
                  p3s_cons_aux.push_back(make_pair(x[i][j] + 1 >= x[i][l] + x[j][l], viol));
               }

               if (1 + m[i][l] - m[i][j] - m[j][l] < -P3_EPS)
               {
                  double viol = abs(1 + m[i][l] - m[i][j] - m[j][l]);
                  p3s_cons_aux.push_back(make_pair(x[i][l] + 1 >= x[i][j] + x[j][l], viol));
               }

               if (1 + m[j][l] - m[i][j] - m[i][l] < -P3_EPS)
               {
                  double viol = abs(1 + m[j][l] - m[i][j] - m[i][l]);
                  p3s_cons_aux.push_back(make_pair(x[j][l] + 1 >= x[i][j] + x[i][l], viol));
               }
            }
         }
      }
   }
   */

   std::vector<std::pair<IloConstraint, double>> p3s_cons_aux;

   for (int i = 0; i < x_vars.getSize() - 1; i++)
   {
      if (x_vals[i] >= x_vals[i + 1])
      {
         // How to calculate the weight of each violation?
         double viol = 0.5;
         p3s_cons_aux.push_back(std::make_pair(x[i] <= x[i + 1], viol));
      }
   }

   if (n_max_p3 == -1)
   {
      n_max_p3 = p3s_cons_aux.size();
   }
   else
   {
      //std::sort(p3s_cons_aux.begin(), p3s_cons_aux.end(), compare_p3_viol);
      std::sort(p3s_cons_aux.begin(), p3s_cons_aux.end(), p3s_cons_aux);
   }

   std::vector<IloConstraint> *cons = new std::vector<IloConstraint>();
   for (int i = 0; i < p3s_cons_aux.size() and i < n_max_p3; i++)
      cons->push_back(p3s_cons_aux[i].first);

   return cons;
}

//código do callback que é executado pelo cplex.
void MyCutCallback::main()
{
   int n_p3 = x.getSize();

   std::vector<IloConstraint> *cons_p3 = separate_p3(n_p3);

   //adiciona os cortes
   for (int i = 0; i < cons_p3->size(); i++)
   {
      add(cons_p3->at(i)).end(); //aqui tambem ha a possibilidade de se usar addLocal. Nesse caso, o corte so eh adicionado na subarvore enraizada pelo no atual
   }
   delete cons_p3;
}
